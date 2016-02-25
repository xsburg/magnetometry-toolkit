/**
 * Developer: Stepan Burguchev
 * Date: 7/28/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */


//noinspection ThisExpressionReferencesGlobalObjectJS
(function (global) {
    "use strict";

    define([
        'keypress',
        'moment',
        'handlebars',
        'bluebird',
        'underscore',
        'underscore.string',
        'backbone',
        'backbone_associations',
        'backbone_forms',
        'backbone_trackit',
        'marionette',
        'jquery',
        'jquery_caret',
        'jquery_autosize',
        'jquery_mousewheel',
        'jquery_jstorage',
        'datetimePicker',
        'moment_en',
        'moment_ru',
        'moment_de',
        'bootstrap',
        'bootstrapSelect',
        'raphael',
        'morris'
    ], function (keypress, moment, Handlebars, Promise, _, underscoreString) {
        'use strict';

        global.Handlebars = Handlebars;
        global.Promise = Promise;
        global.moment = moment;
        _.string = underscoreString;

        return {
            initialize: function (apiMixins) {
                _.extend(this, apiMixins);
            },
            keypress: keypress,
            moment: moment
        };
    });
})(this);
