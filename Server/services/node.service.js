var config = require('config/config.json');
var jwt = require('jsonwebtoken');
var bcrypt = require('bcryptjs');
var db = require('config/db.js');
var encryptor = require('simple-encryptor')(config.secret);
var shortid = require('shortid');
const email = require('services/email');
var socket = require('config/socket');
var notifications = require("services/notification");

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
    var node = {};
    node.name = newNode.name;
    node.id = newNode.id;
    node.type = newNode.type;
    node.location = newNode.location;
    node.values = {};
    if(!newNode.name || !newNode.id || !newNode.type || !newNode.location){
        throw new Error('Missing information');
    }
    return nodes.insert(node);
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


function emergency(message){
    var payload = {
        notification: {
            title: "EMERGENCY",
            body: "This is a HAMS notification",
            sound: "default"
        },
        data : message
    }

    return notifications.firebase.messaging().sendToTopic("emergency",payload).then(function(response){
        console.log(response);
        //return insert(message.node_id, message.time, message.status, message.water, 
                    //message.gas, message.temperature, message.humidity)
        return response
    })
}