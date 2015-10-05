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

        },

        onRender: function () {
            this.ui.dataTable.hide();
        },

        displayData: function () {
            this.ui.dataTable.show();
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
