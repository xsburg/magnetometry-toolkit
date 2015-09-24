/**
 * Developer: Stepan Burguchev
 * Date: 7/28/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

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
    'backbone_computedfields',
    'backbone_trackit',
    'backbone.radio',
    'marionette',
    'jquery',
    'jquery_caret',
    'jquery_autosize',
    'jquery_mousewheel',
    'jquery_jstorage',
    'datetimePicker',
    'moment_en',
    'moment_ru',
    'moment_de'
], function (keypress, moment, Handlebars, Promise, _, underscoreString) {
    'use strict';

    var root = typeof global !== 'undefined' ? global : window; // jshint ignore:line
    root.Handlebars = Handlebars;
    root.Promise = Promise;
    root.moment = moment;
    _.string = underscoreString;

    return {
        initialize: function (apiMixins) {
            _.extend(this, apiMixins);
        },
        keypress: keypress,
        moment: moment
    };
});
