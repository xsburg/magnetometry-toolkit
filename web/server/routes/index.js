var express = require('express');
var cfg = require('../config');
var router = express.Router();

/* Single page application index is now a static file. */
/*router.get('/', function(req, res, next) {
    res.render('index', {
    });
});*/

module.exports = router;
