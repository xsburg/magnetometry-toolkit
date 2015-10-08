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
    '../models/EbDeviceStatusModel',
    '../views/EbDeviceView'
], function (lib, core, EbDeviceStatusModel, EbDeviceView) {
    'use strict';

    var config = {
        UPDATER_INTERVAL: 1500
    };

    return Marionette.Controller.extend({
        initialize: function (options) {
            this.bindReqres();
            this.createModel();
            this.createView();
            this.__updateStatus().finally(function () {
                this.view.displayData();
                this.view.setLoading(false);
                this.__startBackgroundUpdater();
            }.bind(this));
        },

        createModel: function () {
            this.model = new EbDeviceStatusModel();
        },

        createView: function () {
            this.view = new EbDeviceView({
                model: this.model,
                reqres: this.reqres
            });
        },

        bindReqres: function () {
            this.reqres = new Backbone.Wreqr.RequestResponse();

            this.reqres.setHandler('logging:start', this.__startLogging, this);
            this.reqres.setHandler('logging:stop', this.__stopLogging, this);
            this.reqres.setHandler('standBy', this.__setStandBy, this);
            this.reqres.setHandler('deviceTime:fix', this.__fixDeviceTime, this);
            this.reqres.setHandler('device:update', this.__forceDeviceUpdate, this);
            this.reqres.setHandler('range:set', this.__setRange, this);
        },

        onDestroy: function () {
            this.isDestroyed = true;
        },

        __startBackgroundUpdater: function () {
            setTimeout(function () {
                this.view.setDataUpdating(true);
                this.__updateStatus().finally(function () {
                    if (!this.isDestroyed) {
                        this.view.setDataUpdating(false);
                        this.__startBackgroundUpdater();
                    }
                }.bind(this));
            }.bind(this), config.UPDATER_INTERVAL);
        },

        __updateStatus: function () {
            return Promise.resolve(this.model.fetch());
        },

        __startLogging: function (samplingIntervalMs) {
            return Promise.resolve($.post('api/dashboard/eb-device/command', {
                command: 'run',
                intervalMilliseconds: samplingIntervalMs
            }));
        },

        __stopLogging: function () {
            return Promise.resolve($.post('api/dashboard/eb-device/command', {
                command: 'stop'
            }));
        },

        __setStandBy: function (enabled) {
            return core.services.AjaxService;
            return Promise.resolve($.ajax({
                method: 'POST',
                url: 'api/dashboard/eb-device/command',
                data: JSON.stringify({
                    command: 'set-device-stand-by',
                    standBy: enabled
                }),
                dataType: 'json',
                contentType: 'application/json'
            }));
        },

        __fixDeviceTime: function () {
            return Promise.resolve($.post('api/dashboard/eb-device/command', {
                command: 'set-device-time',
                time: Number(new Date())
            }));
        },

        __setRange: function (center) {
            return Promise.resolve($.post('api/dashboard/eb-device/command', {
                command: 'set-device-range',
                range: center
            }));
        },

        __forceDeviceUpdate: function () {
            return Promise.resolve($.post('api/dashboard/eb-device/command', {
                command: 'update-status'
            }));
        }
    });
});
