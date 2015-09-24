/**
 * Developer: Stepan Burguchev
 * Date: 8/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'module/lib',
    'core',
    'commonApi',
    'Application',
    './app/dashboard/controllers/DashboardController',
    './app/case/controllers/CaseController',
    './app/caseList/controllers/CaseListController'
], function (lib, core, common, Application, DashboardController, CaseController, CaseListController) {
    'use strict';

    return Marionette.Object.extend({
        index: function () {
            common.services.RoutingService.navigateToUrl(common.services.UrlService.getDashboardUrl(), { replace: true });
            $.get('api/SystemContextApi').then(function () { console.log(arguments); });
        },

        showDashboard: function () {
            var dashboardController = new DashboardController();
            Application.contentRegion.show(dashboardController.view);
        }
    });
});
