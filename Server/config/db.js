var config = require('config/config.json');
var db

exports.connect = function(dbConnection) {
    db = require('monk')(dbConnection);
}

exports.monk = function() {
    if(!db){
        if(config.TEST){
            console.log("TESTING")
            db = require('monk')(config.connectionStringTEST);
        } else {
            console.log("PRODUCTION")
            db = require('monk')(config.connectionString);
        }
    }
    return db
}
