var config = require('config/config.json');
var express = require('express');
var router = express.Router();
var nodeService = require('services/node.service');

// routes
router.post('/register', registerNode);
router.post('/values', insertValues);
router.get('/getAll', getAll);


module.exports = router;

function registerNode(req, res) {
    console.log("Registering node with nodeId: " + req.body.id);
    nodeService.create(req.body)
        .then(function () {
            res.sendStatus(200);
        })
        .catch(function (err) {
            res.status(400).send(err);
        });
}

function insertValues(req, res) {
    console.log("Inserting values for nodeId: " + req.body.id);
    nodeService.insertValues(req.body)
        .then(function () {
            res.sendStatus(200);
        })
        .catch(function (err) {
            res.status(400).send(err);
        });
}

function getAll(req, res) {
    nodeService.getAll()
        .then(function (nodes) {
            res.send(nodes);
        })
        .catch(function (err) {
            res.status(400).send(err);
        });
}
