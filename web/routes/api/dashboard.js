/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

var request = require("request");

var express = require('express');
var router = express.Router();

router.get('/eb-device/status', function(req, res, next) {

    request({
        url: 'http://myUrl.com/api/status',
        json: true
    }, function (error, response, body) {
        if (!error && response.statusCode === 200) {
            console.log(body) // Print the json response
        }
    });


    res.json({ isRunning: true, errorText: 'testtesttest' });
});

module.exports = router;
