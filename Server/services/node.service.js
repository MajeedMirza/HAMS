var config = require('config/config.json');
var db = require('config/db.js');
var socket = require('config/socket');
var notifications = require("services/notification");

//Initialize collections for nodes and logs
const nodes = db.monk().get('nodes')
const logs = db.monk().get('logs')
 
//Object containing all service functions called externally 
var service = {};
service.create = create;
service.getAll = getAll;
service.getLogs = getLogs;
service.insertValues = insertValues;
service.garage = garage;
service.emergency = emergency;
service.clearAll = clearAll;

//export all methods needed externally 
module.exports = service;

//Creates a new node in the nodes collection
function create(newNode){
    //check if it already exists, if it does throw an error
    return getNode(newNode.id).then(function(nodeinfo){
        if (nodeinfo){
            throw new Error('Already exists');
        }
        var node = {};
        node.name = newNode.name;
        node.id = newNode.id;
        node.type = newNode.type;
        node.location = newNode.location;
        node.lastEmergency = null;
        node.values = {};
        //If one of these values are missing, node is not created
        if(!newNode.name || !newNode.id || !newNode.type || !newNode.location){
            throw new Error('Missing information');
        }
        return nodes.insert(node);
    });
}

//Inserts values into the logs collection
function insertValues(payload){
    if(!payload.id){
        throw new Error('No id received');
    }
    if(!payload.values){
        throw new Error('No values received');
    }
    //Check log for any other special cases before logging
    checkValues(payload);
    //log the log in the logs collection
    return logs.insert(payload).then(function(){
        //insert the latest values into the node in the nodes collection
        return nodes.update({id: payload.id}, {$set: { values: payload.values }}).then(function(){
            //broadcast the new values to listening clients
            return socket.emit('values', payload);
        })
    });
}

//Check any special cases and send a notification if needed
function checkValues(payload){
    if(payload.values.ultrasonic < config.specialCases.ultrasonic){
        //create an emergency for these values on the server side
        return emergency({id: payload.id, alarm: "Movement", time:payload.values.time});
    }
}

//Returns all nodes in the nodes collection
function getAll(){
    return nodes.find({});
}

//Gets a specific node in the nodes collection
function getNode(id){
    return nodes.findOne({id:id});
}

//Gets all logs from the logs collection
function getLogs(){
    return logs.find({});
}

//Sends a garage command to nodes listening to the topic
function garage(open){
    msg = {open:open}
    return socket.emit('garage', msg);
}

//Clears both the logs and nodes collection
function clearAll(){
    return logs.remove({}).then(function(){
        return nodes.remove({});
    })
}

//Updates the last time a node sent an emergency in the nodes collection
function updateEmergencyTime(id, time){
    return nodes.update({id: id}, {$set: { lastEmergency: time }});
}

//Sends a notification if the time between the last emergency and the current 
//emergency exceed a defined wait time
function emergency(message){
    //get all node information
    return getNode(message.id).then(function(nodeinfo){
        var lastEmergency = nodeinfo.lastEmergency;
        var newEmergency = moment(message.time);
        //compare times and record the difference
        var minutesDiff = newEmergency.diff(lastEmergency, 'minutes');
        //if the difference is greater than the set wait time, send a notification
        if (minutesDiff >= config.alarmWait || (!minutesDiff && minutesDiff != 0)){
            //create the emergency payload for Firebase
            var payload = {
                notification: {
                    title: "EMERGENCY",
                    body: message.id + ": " + message.alarm + " at " + message.time,
                    sound: "default"
                },
                data : message
            }
            //update the last emergency time before sending a notification
            return updateEmergencyTime(message.id, message.time).then(function(){
                //send the notification via Firebase to all listening clients
                return notifications.firebase.messaging().sendToTopic("emergency",payload).then(function(response){
                    console.log(response);
                    return response
                })
            });
        } else {
            //Remaining wait time in minutes before notification is sent
            minutesDiff = config.alarmWait - minutesDiff
            //If the alarm is to be ignored, log it on the console
            var emergStr = "Ignored for " + minutesDiff + " more minutes - " + message.alarm + " on " + message.id + " at " + message.time 
            console.log(emergStr);
            return emergStr;
        }
    });
}