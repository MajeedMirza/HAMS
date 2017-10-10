var express = require('express');
var router = express.Router();
var request = require('request');
var config = require('config/config.json');
var userService = require('services/user.service');

router.get('/', function (req, res) {
    // log user out
    delete req.session.token;

    // move success message into local variable so it only appears once (single read)
    var viewData = { success: req.session.success };
    delete req.session.success;
    if(req.query.id){
        req.session.company = req.query.id;
    }
    getDB(res);
});


router.post('/', function (req, res) {
    if(req.body.test){
        userService.writeDB(req.body).then(function(){
            return getDB(res);
        });
    } else{
        getDB(res);
    }
});


function getDB(res){
    return userService.getFromDB().then(function(result){
        result = JSON.stringify(result);
        var viewData = {'result':result}
        res.render('test', viewData);
    });
}

router.get('/write', function (req, res) {
    if(req.query.test){
        userService.writeDB(req.query).then(function(){
            return userService.getFromDB();
        }).then(function(result){
            result = result;
            var viewData = {'result':result}
            res.send(viewData);
        }).catch(function (err) {
            res.status(400).send(err);
        });
    } else {
        res.sendStatus(404);
    }
});

router.get('/delete', function (req, res) {
    userService.deleteAll().then(function(){
        res.redirect('back');
    })
});

module.exports = router;