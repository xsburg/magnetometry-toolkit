/**
 * Developer: Stepan Burguchev
 * Date: 10/4/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    './views/SidebarView',
    './views/NavbarView'
], function (SidebarView, NavbarView) {
    'use strict';

    return {
        views: {
            SidebarView: SidebarView,
            NavbarView: NavbarView
        }
    };
});
