var express = require('express');
var router = express.Router();
var request = require('request');
var config = require('config/config.json');
var userService = require('services/user.service');

router.get('/', function (req, res) {
    res.render('HAMS', {});
});

module.exports = router;