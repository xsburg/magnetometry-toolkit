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
    'module/common/meta'
], function (
    RoutingService,
    UrlService,
    meta
) {
    'use strict';

    return {
        services: {
            RoutingService: RoutingService,
            UrlService: UrlService
        },
        meta: meta
    };
});
