/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

var express = require('express');
var router = express.Router();

router.get('/eb-device/status', function(req, res, next) {
    res.json({ isRunning: true, errorText: 'testtesttest' });
});

module.exports = router;
