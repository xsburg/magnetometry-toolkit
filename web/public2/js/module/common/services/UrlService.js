/**
 * Developer: Stepan Burguchev
 * Date: 8/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'core'
], function (core) {
    'use strict';

    var format = core.utils.helpers.format.bind(core.utils.helpers);

    return {
        getDashboardUrl: function () {
            return '#/Dashboard';
        }
    };
});
