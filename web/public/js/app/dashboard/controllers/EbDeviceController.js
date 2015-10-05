/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    '../models/EbDeviceStatusModel',
    '../views/EbDeviceView'
], function (lib, EbDeviceStatusModel, EbDeviceView) {
    'use strict';

    var config = {
        UPDATER_INTERVAL: 1000
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
            this.model = new Backbone.Model({
                status: new EbDeviceStatusModel(),
                loading: true
            });
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
            this.reqres.setHandler('status:update', this.__updateStatus, this);
        },

        onDestroy: function () {
            this.isDestroyed = true;
        },

        __startBackgroundUpdater: function () {
            setTimeout(function () {
                this.__updateStatus().finally(function () {
                    if (!this.isDestroyed) {
                        this.__startBackgroundUpdater();
                    }
                }.bind(this));
            }.bind(this), config.UPDATER_INTERVAL);
        },

        __updateStatus: function () {
            return new Promise(function (resolve) {
                setTimeout(function () {
                    resolve();
                }.bind(this), 1000);
            }.bind(this));
            //return Promise.resolve(this.model.fetch());
        },

        __startLogging: function (samplingIntervalMs) {
            return new Promise(function (resolve) {
                setTimeout(function () {
                    resolve();
                }.bind(this), 1000);
            }.bind(this));
        },

        __stopLogging: function () {
            return new Promise(function (resolve) {
                setTimeout(function () {
                    resolve();
                }.bind(this), 1000);
            }.bind(this));
        },

        __setStandBy: function (enabled) {
            return new Promise(function (resolve) {
                setTimeout(function () {
                    resolve();
                }.bind(this), 1000);
            }.bind(this));
        },

        __fixDeviceTime: function () {
            return new Promise(function (resolve) {
                setTimeout(function () {
                    resolve();
                }.bind(this), 1000);
            }.bind(this));
        }
    });
});
