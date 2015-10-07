/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    './htmlHelpers'
], function (lib, htmlHelpers) {
    'use strict';

    var console = window.console;

    Handlebars.registerHelper("debug", function(optionalValue) {
        console.log("Current Context");
        console.log("====================");
        console.log(this);

        if (optionalValue) {
            console.log("Value");
            console.log("====================");
            console.log(optionalValue);
        }
    });

    Handlebars.registerHelper("renderFullDate", function(date) {
        return lib.moment(date).format('ll');
    });

    Handlebars.registerHelper("renderFullDateTime", function(date) {
        return lib.moment(date).format('LLL');
    });

    Handlebars.registerHelper("renderAsHtml", function(text) {
        return htmlHelpers.textToHtml(text);
    });
});
