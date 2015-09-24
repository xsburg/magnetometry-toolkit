/**
 * Developer: Stepan Burguchev
 * Date: 8/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'module/common/services/RoutingService',
    'module/common/services/UrlService',
    'module/common/services/ClassifierService',
    'module/common/header/views/HeaderView',
    'module/common/meta'
], function (
    RoutingService,
    UrlService,
    ClassifierService,
    HeaderView,
    meta
) {
    'use strict';

    return {
        services: {
            RoutingService: RoutingService,
            UrlService: UrlService,
            ClassifierService: ClassifierService
        },
        header: {
            View: HeaderView
        },
        meta: meta
    };
});
