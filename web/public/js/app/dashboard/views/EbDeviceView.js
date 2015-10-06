/**
 * Developer: Stepan Burguchev
 * Date: 10/5/2015
 * Copyright: 2009-2015 Comindware®
 *       All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Comindware
 *       The copyright notice above does not evidence any
 *       actual or intended publication of such source code.
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'core',
    'text!../templates/ebDevice.html'
], function (lib, core, template) {
    'use strict';

    return Marionette.ItemView.extend({
        initialize: function (options) {
            core.utils.helpers.ensureOption(options, 'model');
            core.utils.helpers.ensureOption(options, 'reqres');

            this.reqres = options.reqres;
        },

        modelEvents: {
        },

        template: Handlebars.compile(template),

        className: 'panel panel-default',

        ui: {
            loadingPanel: '.js-loading-panel',
            dataTable: '.js-data-table',
            statusUpdated: '.js-status-updated',
            condStopped: '.js-cond-stopped',
            condRunning: '.js-cond-running',
            condSamplingIntervalRow: '.js-cond-sampling-interval-row',
            statusSamplingInterval: '.js-status-sampling-interval',
            statusStandBy: '.js-status-stand-by',
            statusStandByButton: '.js-status-stand-by-button',
            statusDeviceTime: '.js-status-device-time',
            statusRangeMin: '.js-status-range-min',
            statusRangeMax: '.js-status-range-max',
            statusCenterRangeInput: '.js-status-center-range-input',
            setRangeButton: '.js-set-range-button',
            statusEnq: '.js-status-enq',
            statusAbout: '.js-status-about'
        },

        onRender: function () {
            this.ui.dataTable.hide();
        },

        displayData: function () {
            this.ui.dataTable.show();
            this.__updateData();
        },

        setLoading: function (isLoading) {
            if (isLoading) {
                this.ui.loadingPanel.show();
            } else {
                this.ui.loadingPanel.hide();
            }
        },

        __updateData: function () {
            var data = this.model.toJSON();
            debugger;
        }
    });
});
