/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

var request = require("request");
var _ = require("underscore");
var express = require('express');
var appConfig = require('../../config');
var router = express.Router();

router.get('/eb-device/status', function(req, res) {
    request.get({
        url: appConfig.ebDeviceUrl + '/api/status',
        json: true,
        timeout: 50000
    }, function (error, response, body) {
        if (!error && response.statusCode === 200) {
            res.json(body);
        } else {
            res.status(500);
            res.send('Device has failed to process the data.');
        }
    });
});

router.get('/eb-device/log', function(req, res) {
    request.get({
        url: appConfig.ebDeviceUrl + '/api/log',
        json: true,
        timeout: 50000
    }, function (error, response, body) {
        if (!error && response.statusCode === 200) {
            res.json(body);
        } else {
            res.status(500);
            res.send('Device has failed to process the data.');
        }
    });
});

router.get('/eb-device/data', function(req, res) {
    request.get({
        url: appConfig.ebDeviceUrl + '/api/data',
        json: true,
        timeout: 50000
    }, function (error, response, body) {
        if (!error && response.statusCode === 200) {
            res.json(body);
        } else {
            res.status(500);
            res.send('Device has failed to process the data.');
        }
    });
});

router.post('/eb-device/command', function (req, res) {
    request.post({
        url: appConfig.ebDeviceUrl + '/api/command',
        json: true,
        body: req.body,
        timeout: 50000
    }, function (error, response, body) {
        if (!error && response.statusCode === 200) {
            res.json(body);
        } else {
            res.status(500);
            res.send('Device has failed to process the data.');
        }
    });
});

module.exports = router;
