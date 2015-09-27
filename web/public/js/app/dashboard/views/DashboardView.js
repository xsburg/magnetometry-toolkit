/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'core',
    'text!../templates/dashboard.html'
], function (lib, core, template) {
    'use strict';

    return Marionette.LayoutView.extend({
        initialize: function (options) {
            core.utils.helpers.ensureOption(options, 'model');
            core.utils.helpers.ensureOption(options, 'reqres');

            _.bindAll(this, '__updateEbDeviceStatus');

            this.listenTo(this.model.get('ebDevice'), 'change', this.__updateEbDeviceStatus)
        },

        template: Handlebars.compile(template),

        ui: {
            isRunning: '.js-is-running'
        },

        regions: {},

        onRender: function () {
            // show regions
        },

        __updateEbDeviceStatus: function () {
            this.ui.isRunning.text(this.model.get('ebDevice').get('isRunning'));
        }
    });
});
