/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    '../models/EbDeviceModel'
], function (lib, EbDeviceModel) {
    'use strict';

    return Marionette.Controller.extend({
        initialize: function (options) {
            _.bindAll(this, 'bar');

            this.bindReqres();
            this.createModel();
        },

        createModel: function () {
            this.model = new EbDeviceModel();
        },

        updateStatus: function () {
            return Promise.resolve(this.model.fetch());
        },

        bindReqres: function () {
            this.reqres = new Backbone.Wreqr.RequestResponse();

            this.reqres.setHandler('foo', this.bar);
        },

        bar: function () {
            // TODO
        }
    });
});
