/**
 * Developer: Stepan Burguchev
 * Date: 8/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'core',
    'module/common/commonApi',
    'Application',
    'app/dashboard/dashboardApi'
], function (lib, core, common, Application, dashboardApi) {
    'use strict';

    return Marionette.Object.extend({
        index: function () {
            common.services.RoutingService.navigateToUrl(common.services.UrlService.getDashboardUrl(), { replace: true });
        },

        showDashboard: function () {
            var dashboardController = new dashboardApi.Controller();
            Application.contentRegion.show(dashboardController.view);
        }
    });
});
