var config = require('config/config.json');
var jwt = require('jsonwebtoken');
var bcrypt = require('bcryptjs');
const db = require('monk')(config.connectionString);
var encryptor = require('simple-encryptor')(config.secret);
var shortid = require('shortid');
const email = require('services/email');
var socket = require('config/socket');

const nodes = db.get('nodes')
const logs = db.get('logs')
 
var service = {};
service.create = create;
service.getAll = getAll;
service.insertValues = insertValues;
service.garage = garage;

module.exports = service;

function create(newNode){
    var node = {};
    node.name = newNode.name;
    node.id = newNode.id;
    node.type = newNode.type;
    node.location = newNode.location;
    node.values = {};
    return nodes.insert(node);
}

function insertValues(payload){
   return logs.insert(payload).then(function(){
        return nodes.update({id: payload.id}, {$set: { values: payload.values }});
    })
}

function getAll(){
    return nodes.find({});
}

function garage(open){
    msg = {open:open}
    return socket.emit('garage', msg);
}