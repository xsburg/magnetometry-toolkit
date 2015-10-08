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
    'text!../templates/dashboard.html',
    './EbDeviceView'
], function (lib, core, template, EbDeviceView) {
    'use strict';

    return Marionette.LayoutView.extend({
        initialize: function (options) {
            core.utils.helpers.ensureOption(options, 'model');
            core.utils.helpers.ensureOption(options, 'reqres');

            this.reqres = options.reqres;
        },

        template: Handlebars.compile(template),

        className: 'container-fluid',

        regions: {
            ebDeviceRegion: '.js-eb-device-region',
            ebDeviceDiagnosticsRegion: '.js-eb-device-diagnostics-region'
        },

        onRender: function () {
            var views = this.reqres.request('ebDeviceViews');
            this.ebDeviceRegion.show(views.view);
            this.ebDeviceDiagnosticsRegion.show(views.diagnosticsView);
        }
    });
});
