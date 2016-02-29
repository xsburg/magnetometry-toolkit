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
            navbarRegion: ".js-application-navbar-region",
            sidebarRegion: ".js-application-sidebar-region",
            contentRegion: ".js-application-content-region"
        });

        App.ui = {
            navbarRegion: $(".js-application-navbar-region"),
            sidebarRegion: $(".js-application-sidebar-region"),
            contentRegion: $(".js-application-content-region"),
            fatalErrorModal: $(".js-fatal-error-modal")
        };

        App.sidebarRegion.show(new navigationApi.views.SidebarView());
        App.navbarRegion.show(new navigationApi.views.NavbarView());

        App.ui.fatalErrorModal.modal({
            keyboard: false,
            backdrop: "static",
            show: false
        });
        App.ui.fatalErrorModal.on('hide.bs.modal', function () {
            document.location.reload(true);
        });

        function configure() {
            //noinspection JSUnresolvedVariable
            var langCode = window.langCode;
            //noinspection JSUnresolvedVariable
            lib.moment.locale(langCode);

            Promise.onPossiblyUnhandledRejection(function(error, promise) {
                var reason = promise.reason();
                var isAuthenticationError = (reason && reason.status === 401);
                if (!isAuthenticationError) {
                    App.ui.fatalErrorModal.modal("show");
                }
                throw error;
            });
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
