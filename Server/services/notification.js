var admin = require("firebase-admin");
var serviceAccount = require("config/hamsKey.json");

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://hams-2e8df.firebaseio.com"
});

module.exports.firebase = admin;