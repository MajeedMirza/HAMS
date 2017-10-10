var config = require('config/config.json');
var express = require('express');
var router = express.Router();
var userService = require('services/user.service');

// routes
router.post('/authenticate', authenticateUser);
router.post('/register', registerUser);



module.exports = router;

function authenticateUser(req, res) {
}

function registerUser(req, res) {
    userService.create(req.body)
        .then(function () {
            res.sendStatus(200);
        })
        .catch(function (err) {
            res.status(400).send(err);
        });
}