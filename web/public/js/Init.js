/**
 * Developer: Stepan Burguchev
 * Date: 7/4/2014
 * Copyright: 2009-2013 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global require */

//noinspection ThisExpressionReferencesGlobalObjectJS
(function (global) {
    "use strict";

    //noinspection JSUnresolvedVariable
    require.config({
        paths: {
            underscore: 'lib/underscore/underscore',
            'underscore.string': 'lib/underscore.string/underscore.string',
            bluebird: 'lib/bluebird/bluebird',
            handlebars: 'lib/handlebars/handlebars',
            keypress: 'lib/keypress/keypress',

            backbone: 'lib/backbone/backbone',
            backbone_associations: 'lib/backbone-associations/backbone-associations',
            backbone_forms: 'lib/backbone-forms/backbone-forms',
            backbone_computedfields: 'lib/backbone-computedfields/backbone.computedfields',
            backbone_trackit: 'lib/backbone.trackit/backbone.trackit',
            'backbone.babysitter': 'lib/backbone.babysitter/backbone.babysitter',
            'backbone.wreqr': 'lib/backbone.wreqr/backbone.wreqr',
            'backbone.radio': 'lib/backbone.radio/backbone.radio',

            marionette: 'lib/marionette/backbone.marionette',

            jquery: 'lib/jquery/jquery',
            jquery_caret: 'lib/jquery.caret/jquery.caret.1.02.min',
            jquery_autosize: 'lib/jquery.autosize/jquery.autosize',
            jquery_mousewheel: 'lib/jquery-mousewheel/jquery.mousewheel',
            jquery_jstorage: 'lib/jstorage/jstorage',

            domReady: 'lib/require/domReady',
            text: 'lib/require/text',
            datetimePicker: 'lib/datetimepicker/bootstrap-datetimepicker',

            moment: 'lib/moment/moment',
            moment_en: 'lib/moment/locale/en-gb',
            moment_ru: 'lib/moment/locale/ru',
            moment_de: 'lib/moment/locale/de',

            'module/lib': 'lib/libApi',

            'commonApi': 'module/common/commonApi',
            'historyApi': 'module/history/historyApi',
            'caseCreationApi': 'module/caseCreation/caseCreationApi',

            localizationMap: global.compiled ?
                (global.langCode ? 'localizationMap.' + global.langCode : 'empty:') :
                (global.langCode ? 'compiled/localizationMap.' + global.langCode : 'empty:'),
            ajaxMap: global.compiled ? 'ajaxMap?noext' : 'compiled/ajaxMap?noext',

            'core': 'module/core'
        },
        shim: {
            marionette: {
                exports: 'Marionette'
            },

            'keypress': [],

            underscore: {
                exports: '_'
            },

            'underscore.string': {
                deps: [ 'underscore' ]
            },

            bluebird: {
                exports: 'Promise'
            },

            backbone: {
                deps: [ 'jquery', 'underscore' ],
                exports: 'Backbone'
            },

            datetimePicker: {
                deps: ['jquery']
            },

            backbone_associations: [ 'backbone' ],
            backbone_forms: [ 'backbone' ],
            backbone_computedfields: [ 'backbone' ],
            backbone_trackit: [ 'backbone', 'backbone_associations' ],

            jquery_caret: [ 'jquery' ],
            jquery_mousewheel: [ 'jquery' ],
            jquery_autosize: [ 'jquery' ],
            jquery_jstorage: [ 'jquery' ],

            moment_en: ['moment'],
            moment_ru: ['moment'],
            moment_de: ['moment']
        }
    });

    require([
        "text", // it's required to correctly build with r.js
        "Application",
        "AppRouter",
        "AppController",
        "commonApi"
    ], function (text, Application, AppRouter, AppController, commonApi) {
        Application.appRouter = new AppRouter({
            controller: new AppController()
        });
        //Promise.all(Application.navigation.load(), commonApi.services.ClassifierService.load()).then(function () {
        Application.navigation.load().then(function () {
            Application.start();
        });
    });
})(this);
