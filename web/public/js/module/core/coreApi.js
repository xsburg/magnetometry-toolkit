/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'module/core/services/AjaxService',
    'module/core/utils/helpers',
    'module/core/utils/htmlHelpers',
    'module/core/utils/dateTimeHelpers',
    'module/core/utils/handlebarsHelpers',
    'module/core/utils/keyCode'
], function (lib, AjaxService, helpers, htmlHelpers, dateTimeHelpers, handlebarsHelpers, keyCode) {
    'use strict';

    return {
        services: {
            AjaxService: AjaxService
        },
        utils: {
            helpers: helpers,
            htmlHelpers: htmlHelpers,
            dateTimeHelpers: dateTimeHelpers,
            keyCode: keyCode
        }
    };
});
