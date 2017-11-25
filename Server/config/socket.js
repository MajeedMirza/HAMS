var io;

exports.connect = function(server) {
    return io = require('socket.io').listen(server);
}

exports.io = function() {
    return io
}

exports.emit = function(topic, message) {
     return new Promise( ( resolve, reject ) => {
         if (io.emit(topic, message)){
             resolve(true)
         }
         else{
             return reject("failed socket io emit");
         }
    } );
}
