/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Backbone, Marionette, $, _, Localizer */

define([
    'lib'
], function (lib) {
    'use strict';

    return Backbone.Model.extend({
        initialize: function () {
        },

        defaults: {},

        urlRoot: 'api/dashboard/eb-device/status'
    });
});
