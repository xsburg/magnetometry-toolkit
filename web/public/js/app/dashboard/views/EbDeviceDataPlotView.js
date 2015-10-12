/**
 * Developer: Stepan Burguchev
 * Date: 10/9/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'core',
    'text!../templates/ebDeviceDataPlot.html'
], function (lib, core, template) {
    'use strict';

    return Marionette.ItemView.extend({
        initialize: function (options) {
            core.utils.helpers.ensureOption(options, 'model');
            core.utils.helpers.ensureOption(options, 'reqres');

            this.reqres = options.reqres;
        },

        template: Handlebars.compile(template),

        className: 'panel panel-default',

        ui: {
            loadingPanel: '.js-loading-panel',
            dataPlot: '.js-data-plot',
            refreshButton: '.js-refresh-button',
            dataPanel: '.js-data-panel'
        },

        events: {
            'click @ui.refreshButton': '__refresh'
        },

        onRender: function () {
            this.ui.dataPanel.hide();
        },

        onShow: function () {
            this.setLoading(true);
            this.reqres.request('data:reload').then(function (data) {
                this.ui.dataPanel.show();
                this.line = new Morris.Line({
                    element: this.ui.dataPlot[0],
                    data: [],
                    xkey: 'time',
                    ykeys: ['field'],
                    labels: ['Field'],
                    dateFormat: function (x) {
                        return lib.moment(x).format();
                    },
                    resize: true/*,
                     hoverCallback: function (index, options, content, row) {
                     return "sin(" + row.x + ") = " + row.y;
                     }*/
                });
                this.__updateData(data);
                this.setLoading(false);
            }.bind(this));
        },

        __updateData: function (data) {
            if (data.samples.length === 0) {
                this.ui.dataPlot.hide();
            } else {
                this.ui.dataPlot.show();
                this.line.setData(data.samples, true);
            }
        },

        setDataUpdating: function (isUpdating) {
            this.ui.dataUpdateSpinner.toggleClass(classes.DATA_UPDATE_SPINNER_VISIBLE, isUpdating);
        },

        setLoading: function (isLoading) {
            if (isLoading) {
                this.ui.loadingPanel.show();
            } else {
                this.ui.loadingPanel.hide();
            }
        },

        __refresh: function () {
            this.setLoading(true);
            this.reqres.request('data:reload').then(function (data) {
                this.__updateData(data);
                this.setLoading(false);
            }.bind(this));
        }
    });
});
