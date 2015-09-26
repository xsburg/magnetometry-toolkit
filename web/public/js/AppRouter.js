/**
 * Developer: Stepan Burguchev
 * Date: 8/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib'
], function () {
    'use strict';

    return Marionette.AppRouter.extend({
        appRoutes: {
            "": "index",
            "Dashboard": "showDashboard"
        }
    });
});
