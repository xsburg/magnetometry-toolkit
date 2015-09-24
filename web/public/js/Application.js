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
        'module/lib',
        'core',
        'module/navigation/navigationApi',
        'module/login/loginApi',
        'module/common/services/RoutingService'
    ], function (lib, core, navigationApi, loginApi, RoutingService) {
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

        App.navigation = new navigationApi.Controller();
        App.login = new loginApi.Controller();

        function configure() {
            var langCode = window.langCode;

            // MomentJS
            //noinspection JSUnresolvedVariable
            lib.moment.locale(langCode);

            // DateTimePicker
            if (!$.fn.datetimepicker.dates[langCode]) {
                $.fn.datetimepicker.dates[langCode] = {
                    days: Localizer.get('CORE.FORMATS.DATETIME.DAYSFULL').split(','), //["Sunday", "Monday", ... ]
                    daysShort: Localizer.get('CORE.FORMATS.DATETIME.DAYSSHORT').split(','), //["Sun", "Mon", ... ],
                    daysMin: Localizer.get('CORE.FORMATS.DATETIME.DAYSSHORT').split(','),
                    months: Localizer.get('CORE.FORMATS.DATETIME.MONTHS').split(','), //["January", "February", ... ]
                    monthsShort: Localizer.get('CORE.FORMATS.DATETIME.MONTHSSHORT').split(','), //["Jan", "Feb", ... ]
                    today: Localizer.get('CORE.FORMATS.DATETIME.TODAY'),
                    meridiem: Localizer.get('CORE.FORMATS.DATETIME.MERIDIEM').split(',')
                };
            }
        }

        App.addInitializer(function () {
            configure();

            // Window service provides global dialog and other stuff
            core.services.WindowService.initialize({
                fadingRegion: App.fadingRegion,
                popupRegion: App.popupRegion,
                ui: App.ui
            });

            // After that we show left navigation and login screen components
            App.navigationRegion.show(App.navigation.view);
            App.loginRegion.show(App.login.view);

            RoutingService.initialize();

            // And remove startup loading element
            $('.js-startup-loading').remove();
        });

        App.addInitializer(function() { // Application's loading view
            var loadingViewOptions = {
                text: Localizer.get('CORE.VIEWS.BEHAVIORS.LOADING.DEFAULTLOADINGSMALL')
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
