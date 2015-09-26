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
        'module/common/services/RoutingService'
    ], function (lib, core, RoutingService) {
        var App = new Marionette.Application();

        App.addRegions({
            loginRegion: ".js-application-login-region",
            navigationRegion: ".js-application-side-navigation-region",
            contentRegion: ".js-application-content-region",
            loadingRegion: ".js-application-loading-region",
            fadingRegion: ".js-application-fading-region",
            popupRegion: ".js-application-popup-region"
        });

        App.ui = {
            loginRegion: $(".js-application-login-region"),
            navigationRegion: $(".js-application-side-navigation-region"),
            contentRegion: $(".js-application-content-region"),
            loadingRegion: $(".js-application-loading-region"),
            fadingRegion: $(".js-application-fading-region"),
            popupRegion: $(".js-application-popup-region")
        };

        //App.navigation = new navigationApi.Controller();
        //App.login = new loginApi.Controller();

        function configure() {
            //noinspection JSUnresolvedVariable
            var langCode = window.langCode;
            //noinspection JSUnresolvedVariable
            lib.moment.locale(langCode);
        }

        App.addInitializer(function () {
            configure();

            RoutingService.initialize();

            // Removing startup loading element when everything is ready
            $('.js-startup-loading').remove();
        });

        // Loading view on application level
        App.addInitializer(function() {
            var loadingViewOptions = {
                text: 'Loading'
            };

            this.setLoading = function(visible) {
                if (_.isBoolean(visible)) {
                    if (visible) {
                        this.loadingRegion.show(new core.views.LoadingView(loadingViewOptions));
                    } else {
                        this.loadingRegion.reset();
                    }
                } else if (_.isObject(visible)) {
                    this.setLoading(true);

                    Promise.resolve(visible).bind(this).then(function() {
                        this.setLoading(false);
                    }, function() {
                        this.setLoading(false);
                    });
                } else {
                    core.helpers.throwError('Invalid argument format.', 'FormatError');
                }
            };
        });

        return App;
    });
})(this);
