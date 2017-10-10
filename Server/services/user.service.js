var config = require('config/config.json');
var jwt = require('jsonwebtoken');
var bcrypt = require('bcryptjs');
const db = require('monk')(config.connectionString);
var encryptor = require('simple-encryptor')(config.secret);
var shortid = require('shortid');
const email = require('services/email');

const users = db.get('users')
const logs = db.get('logs')
 
var service = {};
service.writeDB = writeDB;
service.getFromDB = getFromDB;
service.deleteAll = deleteAll;
service.create = create;

module.exports = service;

function writeDB(test){
    //If the message contains an @ symbol, will attempt to send an email
    if(test.test.indexOf("@") != -1){
        email.sendMail(test.test,"Hello", "Hello there " + test.test);
    }
    return users.insert(test);
}

function getFromDB(){
    return users.find({});
}

function deleteAll(){
    return users.remove({});
}

function create(newUser){
    var user = {};
    user.name = newUser.name;
    user.hash = newUser.hash;
    user.email = newUser.email;
    user.phone = newUser.phone;
    user.address = newUser.address;
    return users.insert(user);
}
 