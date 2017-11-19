var newman = require('newman'); // require newman in your project
var nodeService = require('services/node.service');
var config = require('config/config.json');
var db = require('config/db.js');

if(config.TEST === true){
    test()
}

function test() {
    nodeService.clearAll().then(function(){
        // call newman.run to pass `options` object and wait for callback
        newman.run({
            collection: require('tests/HAMSTestCollection.postman_collection.json'),
            environment: require('tests/HAMS.postman_environment.json'),
            reporters: 'cli'
        }, function (err) {
            if (err) { throw err; }
            console.log('collection run complete!');
        });
    })
}

