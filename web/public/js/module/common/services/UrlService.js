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

    var assertNotFalsy = function (propertyValue, propertyName) {
        if (!propertyValue) {
            core.utils.throwArgumentError(format('Property {0} is falsy.', propertyName));
        }
    };

    return {
        getDashboardUrl: function () {
            return '#/Dashboard';
        }
    };
});
