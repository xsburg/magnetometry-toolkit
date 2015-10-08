/**
 * Developer: Stepan Burguchev
 * Date: 10/9/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'text!../templates/ebDeviceDiagnostics.html'
], function (lib, template) {
    'use strict';

    return Marionette.ItemView.extend({
        initialize: function () {
        },

        template: Handlebars.compile(template),

        className: 'panel panel-default'
    });
});
