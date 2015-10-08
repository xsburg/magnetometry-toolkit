/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

var request = require("request");
var _ = require("underscore");
var express = require('express');
var router = express.Router();

router.get('/eb-device/status', function(req, res) {
    request({
        url: 'http://simba.adm:8000/api/status',
        json: true
    }, function (error, response, body) {
        if (!error && response.statusCode === 200) {
            res.json(body);
        }
    });
});

router.post('/eb-device/command', function (req, res) {
    var data = req.body;
    var availableCommands = [
        'run',
        'stop',
        'update-status',
        'set-device-time',
        'set-device-range',
        'set-device-stand-by'
    ];
    if (!_.contains(availableCommands, data.command)) {
        res.status(500);
        res.send('Invalid command');
        return;
    }
    request({
        url: 'http://simba.adm:8000/api/command',
        json: true
    }, function (error, response, body) {
        if (!error && response.statusCode === 200) {
            res.json(body);
        }
    });
});

module.exports = router;
