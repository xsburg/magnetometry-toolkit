/**
 * Developer: Stepan Burguchev
 * Date: 7/28/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

//noinspection ThisExpressionReferencesGlobalObjectJS
(function (global) {
    "use strict";

    define([
        'lib',
        'core',
        'module/common/commonApi',
        'module/navigation/navigationApi'
    ], function (lib, core, commonApi, navigationApi) {
        var App = new Marionette.Application();

        App.addRegions({
            loginRegion: ".js-application-login-region",
            navbarRegion: ".js-application-navbar-region",
            sidebarRegion: ".js-application-sidebar-region",
            contentRegion: ".js-application-content-region",
            loadingRegion: ".js-application-loading-region",
            fadingRegion: ".js-application-fading-region",
            popupRegion: ".js-application-popup-region"
        });

        App.ui = {
            loginRegion: $(".js-application-login-region"),
            navbarRegion: $(".js-application-navbar-region"),
            sidebarRegion: $(".js-application-sidebar-region"),
            contentRegion: $(".js-application-content-region"),
            loadingRegion: $(".js-application-loading-region"),
            fadingRegion: $(".js-application-fading-region"),
            popupRegion: $(".js-application-popup-region")
        };

        App.sidebarRegion.show(new navigationApi.views.SidebarView());
        App.navbarRegion.show(new navigationApi.views.NavbarView());

        function configure() {
            //noinspection JSUnresolvedVariable
            var langCode = window.langCode;
            //noinspection JSUnresolvedVariable
            lib.moment.locale(langCode);
        }

        App.addInitializer(function () {
            configure();

            commonApi.services.RoutingService.initialize();

            // Removing startup loading element when everything is ready
            $('.js-startup-loading').remove();
        });

        return App;
    });
})(this);
