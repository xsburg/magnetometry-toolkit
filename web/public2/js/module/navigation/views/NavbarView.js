/**
 * Developer: Stepan Burguchev
 * Date: 10/4/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'text!../templates/navbar.html'
], function (lib, template) {
    'use strict';
    return Marionette.ItemView.extend({
        initialize: function () {
        },

        template: Handlebars.compile(template),

        tagName: 'nav',

        className: 'navbar navbar-default navbar-static-top app-nav__navbar',

        attributes: {
            role: 'navigation'
        }
    });
});
