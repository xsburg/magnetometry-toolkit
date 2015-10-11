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
    'text!../templates/ebDeviceDiagnostics.html',
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
            logListRegion: '.js-log-list-region'
        },

        regions: {
            logListRegion: '.js-log-list-region'
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
