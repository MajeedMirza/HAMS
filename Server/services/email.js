'use strict';
const nodemailer = require('nodemailer');

var mail = {}; 
mail.sendMail = sendMail;
module.exports = mail;

function sendMail(sendTo, subject, body){
    let transporter = nodemailer.createTransport({
        host: 'smtp.gmail.com',
        port: 465,
        secure: true, // secure:true for port 465, secure:false for port 587
        auth: {
            user: 'HAMSnotifcation@gmail.com',
            pass: 'HAMS1notifcation'
        }
    });

    // setup email data with unicode symbols
    let mailOptions = {
        from: 'HAMSnotifcation@gmail.com', // sender address
        to: sendTo, // list of receivers
        subject: subject, // Subject line
        text: body, // plain text body
        //html: '<b>Hello world ?</b>' // html body
    };

    // send mail with defined transport object
    transporter.sendMail(mailOptions, (error, info) => {
        if (error) {
            return console.log(error);
        }
        console.log("Email sent to: " + sendTo + " with subject: " + subject);
        console.log('Message %s sent: %s', info.messageId, info.response);
    });
}
// create reusable transporter object using the default SMTP transport
