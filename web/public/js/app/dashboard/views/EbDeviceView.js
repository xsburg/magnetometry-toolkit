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

    var classes = {
        DATA_UPDATE_SPINNER_VISIBLE: 'eb-device__data-update-spinner_visible'
    };

    return Marionette.ItemView.extend({
        initialize: function (options) {
            core.utils.helpers.ensureOption(options, 'model');
            core.utils.helpers.ensureOption(options, 'reqres');

            this.reqres = options.reqres;
        },

        modelEvents: {
            'change': '__onDataChange'
        },

        template: Handlebars.compile(template),

        templateHelpers: function () {
            var range60 = [ 1, 2, 3, 4, 5, 10, 15, 20, 25, 30, 40, 50 ];
            var range24 = [ 1, 2, 3, 4, 5, 6, 12, 24 ];
            var optionMs = function (hz, ms) {
                return {
                    name: core.utils.helpers.format('{1}ms ({0}Hz)', hz, ms),
                    value: ms
                };
            };

            var samplingIntervalOptions = [
                optionMs(5, 200),
                optionMs(4, 250),
                optionMs(3, 334),
                optionMs(2, 500)
            ].concat(_.map(range60, function (sec) {
                return {
                    name: core.utils.helpers.format('{0} second(s)', sec),
                    value: sec * 1000
                };
            })).concat(_.map(range60, function (min) {
                return {
                    name: core.utils.helpers.format('{0} minute(s)', min),
                    value: min * 60 * 1000
                };
            })).concat(_.map(range24, function (hour) {
                return {
                    name: core.utils.helpers.format('{0} hour(s)', hour),
                    value: hour * 60 * 60 * 1000
                };
            }));

            return {
                samplingIntervalOptions: samplingIntervalOptions
            };
        },

        className: 'panel panel-default',

        ui: {
            dataUpdateSpinner: '.js-data-update-spinner',
            loadingPanel: '.js-loading-panel',
            dataTable: '.js-data-table',
            statusUpdated: '.js-status-updated',
            condStopped: '.js-cond-stopped',
            condRunning: '.js-cond-running',
            fixDeviceTimeButton: '.js-fix-device-time-button',
            condSamplingIntervalRow: '.js-cond-sampling-interval-row',
            statusSamplingIntervalInput: '.js-status-sampling-interval-input',
            statusSamplingInterval: '.js-status-sampling-interval',
            statusStandBy: '.js-status-stand-by',
            statusStandByButton: '.js-status-stand-by-button',
            statusDeviceTime: '.js-status-device-time',
            statusRangeMin: '.js-status-range-min',
            statusRangeMax: '.js-status-range-max',
            statusCenterRangeInput: '.js-status-center-range-input',
            setRangeButton: '.js-set-range-button',
            statusEnq: '.js-status-enq',
            statusAbout: '.js-status-about',
            startLoggingButton: '.js-start-logging-button',
            stopLoggingButton: '.js-stop-logging-button',
            forceUpdateButton: '.js-force-update-button'
        },

        events: {
            'click @ui.statusStandByButton': '__onToggleStandBy',
            'click @ui.fixDeviceTimeButton': '__onFixDeviceTime',
            'click @ui.setRangeButton': '__onSetRange',
            'click @ui.startLoggingButton': '__onStartLogging',
            'click @ui.stopLoggingButton': '__onStopLogging',
            'click @ui.forceUpdateButton': '__onForceUpdate'
        },

        onRender: function () {
            this.ui.dataTable.hide();
            this.ui.statusSamplingIntervalInput.val(1000);
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
            
            this.ui.statusSamplingInterval.text(data.samplingIntervalMs);
            if (!this.ui.statusSamplingIntervalInput.val()) {
                this.ui.statusSamplingIntervalInput.text(data.samplingIntervalMs);
            }
            if (data.standBy) {
                this.ui.statusStandBy.text('On');
                this.ui.statusStandByButton.text('Turn off');
            } else {
                this.ui.statusStandBy.text('Off');
                this.ui.statusStandByButton.text('Turn on');
            }
            this.ui.statusDeviceTime.text(moment(data.time).format('lll'));
            this.ui.statusRangeMin.text(data.range.minField);
            this.ui.statusRangeMax.text(data.range.maxField);
            this.ui.statusEnq.text(data.enq);
            this.ui.statusAbout.html(core.utils.htmlHelpers.textToHtml(data.about));
            if (data.isRunning) {
                this.ui.condStopped.show();
                this.ui.condRunning.hide();
                this.ui.condSamplingIntervalRow.show();
            } else {
                this.ui.condStopped.hide();
                this.ui.condRunning.show();
                this.ui.condSamplingIntervalRow.hide();
            }
            this.ui.statusUpdated.text(moment(data.updated).format('lll'));
        },

        __onDataChange: function () {
            this.__updateData();
        },

        setDataUpdating: function (isUpdating) {
            this.ui.dataUpdateSpinner.toggleClass(classes.DATA_UPDATE_SPINNER_VISIBLE, isUpdating);
        },

        __onToggleStandBy: function () {
            var newValue = !this.model.get('standBy');
            debugger;
        },

        __onFixDeviceTime: function () {
            debugger;
        },

        __onSetRange: function () {
            var centerValue = this.ui.statusCenterRangeInput.val();
            debugger;
        },

        __onStartLogging: function () {
            var centerValue = this.ui.statusSamplingIntervalInput.val();
            debugger;
        },

        __onStopLogging: function () {
            debugger;
        },

        __onForceUpdate: function () {
            debugger;
        }
    });
});
