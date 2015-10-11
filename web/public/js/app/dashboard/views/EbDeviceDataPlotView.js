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
    'text!../templates/ebDeviceDataPlot.html',
    './LogListView'
], function (lib, core, template, LogListView) {
    'use strict';

    var classes = {
        DATA_UPDATE_SPINNER_VISIBLE: 'eb-device__data-update-spinner_visible'
    };

    return Marionette.LayoutView.extend({
        initialize: function (options) {
            core.utils.helpers.ensureOption(options, 'model');
            core.utils.helpers.ensureOption(options, 'reqres');

            this.reqres = options.reqres;
        },

        template: Handlebars.compile(template),

        className: 'panel panel-default',

        ui: {
            dataUpdateSpinner: '.js-data-update-spinner',
            loadingPanel: '.js-loading-panel',
            dataPlot: '.js-data-plot'
        },

        regions: {
            logListRegion: '.js-log-list-region'
        },

        onShow: function () {
            var data = _.times(100, function (i) {
                return {
                    year: Number(new Date()) + i * 1000,//'' + (2000+i),
                    value: Math.floor(20 + 20 * Math.random())
                };
            });/*
            var data = [
                {year: '2008', value: 20},
                {year: '2009', value: 10},
                {year: '2010', value: 5},
                {year: '2011', value: 5},
                {year: '2012', value: 20}
            ];*/

            this.line = new Morris.Line({
                // ID of the element in which to draw the chart.
                element: this.ui.dataPlot[0],
                // Chart data records -- each entry in this array corresponds to a point on
                // the chart.
                data: data,
                // The name of the data record attribute that contains x-values.
                xkey: 'year',
                // A list of names of data record attributes that contain y-values.
                ykeys: ['value'],
                // Labels for the ykeys -- will be displayed when you hover over the
                // chart.
                labels: ['Field'],
                dateFormat: function (x) {
                    return lib.moment(x).format();
                },
                resize: true/*,
                hoverCallback: function (index, options, content, row) {
                    return "sin(" + row.x + ") = " + row.y;
                }*/
            });

        },

        setDataUpdating: function (isUpdating) {
            this.ui.dataUpdateSpinner.toggleClass(classes.DATA_UPDATE_SPINNER_VISIBLE, isUpdating);
        },

        displayData: function () {
            this.logListRegion.show(new LogListView({
                collection: this.model.get('logList')
            }));
        },

        setLoading: function (isLoading) {
            if (isLoading) {
                this.ui.loadingPanel.show();
            } else {
                this.ui.loadingPanel.hide();
            }
        }
    });
});
