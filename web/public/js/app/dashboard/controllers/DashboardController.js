/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    '../views/DashboardView',
    './EbDeviceController'
], function (DashboardView, EbDeviceController) {
    'use strict';

    return Marionette.Controller.extend({
        initialize: function (options) {
            this.bindReqres();
            this.createModel();
            this.createView();
        },

        bindReqres: function () {
            this.reqres = new Backbone.Wreqr.RequestResponse();
            this.reqres.setHandler('ebDeviceView', this.__createEbDeviceView, this);
        },

        __createEbDeviceView: function () {
            if (this.ebDeviceController) {
                this.ebDeviceController.destroy();
            }
            this.ebDeviceController = new EbDeviceController();
            return this.ebDeviceController.view;
        },

        createModel: function () {
            this.model = new Backbone.Model({
            });
        },

        createView: function () {
            this.view = new DashboardView({
                model: this.model,
                reqres: this.reqres
            });
        }
    });
});
