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
            _.bindAll(this, 'bar');

            this.bindReqres();
            this.createNestedControllers();
            this.createModel();
            this.createView();
            this.load();
        },

        bindReqres: function () {
            this.reqres = new Backbone.Wreqr.RequestResponse();

            this.reqres.setHandler('foo', this.bar);
        },

        createNestedControllers: function () {
            this.ebDeviceController = new EbDeviceController();
        },

        createModel: function () {
            this.model = new Backbone.Model({
                ebDevice: this.ebDeviceController.model
            });
        },

        createView: function () {
            this.view = new DashboardView({
                model: this.model,
                reqres: this.reqres
            });
        },

        load: function () {
            return this.ebDeviceController.updateStatus();
        },

        bar: function () {
            // TODO
        }
    });
});
