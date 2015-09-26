/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'module/core/utils/helpers',
    'module/core/utils/keyCode'
], function (lib, helpers, keyCode) {
    'use strict';

    return {
        utils: {
            helpers: helpers,
            keyCode: keyCode
        }
    };
});
