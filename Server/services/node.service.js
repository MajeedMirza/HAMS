var config = require('config/config.json');
var jwt = require('jsonwebtoken');
var bcrypt = require('bcryptjs');
var db = require('config/db.js');
var encryptor = require('simple-encryptor')(config.secret);
var shortid = require('shortid');
const email = require('services/email');
var socket = require('config/socket');
var notifications = require("services/notification");
var moment = require('moment')

const nodes = db.monk().get('nodes')
const logs = db.monk().get('logs')
 
var service = {};
service.create = create;
service.getAll = getAll;
service.getLogs = getLogs;
service.insertValues = insertValues;
service.garage = garage;
service.emergency = emergency;
service.clearAll = clearAll;

module.exports = service;

function create(newNode){
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
        if(!newNode.name || !newNode.id || !newNode.type || !newNode.location){
            throw new Error('Missing information');
        }
        return nodes.insert(node);
    });
}

function insertValues(payload){
   if(!payload.id){
       throw new Error('No id received');
   }
   if(!payload.values){
       throw new Error('No values received');
   }
    return logs.insert(payload).then(function(){
        return nodes.update({id: payload.id}, {$set: { values: payload.values }});
    });
}

function getAll(){
    return nodes.find({});
}

function getNode(id){
    return nodes.findOne({id:id});
}

function getLogs(){
    return logs.find({});
}

function garage(open){
    msg = {open:open}
    return socket.emit('garage', msg);
}

function clearAll(){
    return logs.remove({}).then(function(){
        return nodes.remove({});
    })
}

function updateEmergencyTime(id, time){
    return nodes.update({id: id}, {$set: { lastEmergency: time }});
}


function emergency(message){
    return getNode(message.id).then(function(nodeinfo){
        var payload = {
            notification: {
                title: "EMERGENCY",
                body: message.id + ": " + message.alarm + " at " + message.time,
                sound: "default"
            },
            data : message
        }
        var lastEmergency = nodeinfo.lastEmergency;
        var newEmergency = moment(message.time);
        var minutesDiff = newEmergency.diff(lastEmergency, 'minutes');
        if (minutesDiff >= 2 || (!minutesDiff && minutesDiff != 0)){
            return updateEmergencyTime(message.id, message.time).then(function(){
                return notifications.firebase.messaging().sendToTopic("emergency",payload).then(function(response){
                    console.log(response);
                    return response
                })
            });
        } else {
            minutesDiff = 2 - minutesDiff
            var emergStr2 = "Ignored for " + minutesDiff + " more minutes - " + message.alarm + " on " + message.id + " at " + message.time 
            console.log(emergStr2);
            return emergStr2;
        }
    });
}