﻿require('rootpath')();
var express = require('express');
var app = express();
var session = require('express-session');
var bodyParser = require('body-parser');
var expressJwt = require('express-jwt');
var config = require('config/config.json');
var https = require('https');
var http = require('http');
var fs = require('fs');
var path = require('path');
var request = require('request');
var cors = require('cors');
var io = require('config/socket')

config.port = "3001";
//config.Ip = "172.17.82.126";
config.Ip = "localhost"
config.portUrl = "http://" + config.Ip + ":" + config.port;
config.apiUrl = config.portUrl + "/api"

app.set('view engine', 'ejs');
app.set('views', __dirname + '/views');
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
app.use(session({ secret: config.secret, resave: false, saveUninitialized: true }));
app.use(cors());
// use JWT auth to secure the api
//app.use('/api', expressJwt({ secret: config.secret }).unless({ path: ['/api/users/authenticate'] }));

// routes
app.use('/test', require('./controllers/test.controller'));
app.use('/login', require('./controllers/login.controller'));
app.use('/app', require('./controllers/app.controller'));
app.use('/api/users', require('./controllers/api/users.controller'));
app.use('/api/nodes', require('./controllers/api/nodes.controller'));

// make '/app' default route
app.get('/', function (req, res) {
    return res.redirect('/app');
});


var server = app.listen(config.port, config.Ip, function () {
    io.connect(server);
    io.io().on('connection', function(socket){
        console.log('a user connected');
        socket.on('disconnect', function(){
            console.log('user disconnected');
        });
    });
    console.log('Server listening at http://' + server.address().address + ':' + server.address().port);
    var tests = require('tests/runTests.js')
});


   