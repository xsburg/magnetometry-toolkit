/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

var request = require("request");
var express = require('express');
var router = express.Router();

router.get('/eb-device/status', function(req, res) {

    request({
        url: 'http://simba.adm:8000/api/status',
        json: true
    }, function (error, response, body) {
        if (!error && response.statusCode === 200) {
            console.log(body) // Print the json response
            res.json(body);
        }
    });

    /*request('http://www.google.com', function (error, response, body) {
     if (!error && response.statusCode == 200) {
     console.log(body) // Show the HTML for the Google homepage.
     }
     })*/

return;
    res.json({
        isRunning: false,
        updated: new Date(),
        standBy: true,
        samplingIntervalMs: 1000,
        range: {
            minField: 47500,
            maxField: 55000
        },
        enq: 'MagState version 3.03',
        about: 'MagState version 3.03\r\n\
            Quantum Magnetometry Laboratory\r\n\
        Urals State Technical University\r\n\
        Copyright 2013.',
        time: new Date(),
        commandQueueSize: 2
    });
});

router.post('/eb-device/command', function (req, res) {
    var data = req.body;
    switch (data.command) {
    case 'standBy':
        res.json({

        });
        break;
    }
    res.json({

    });
});

module.exports = router;
