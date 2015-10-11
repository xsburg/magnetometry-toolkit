/**
 * Developer: Stepan Burguchev
 * Date: 10/11/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'text!../templates/logListItem.html'
], function (lib, template) {
    'use strict';

    return Marionette.ItemView.extend({
        initialize: function () {
        },

        template: Handlebars.compile(template),

        templateHelpers: function () {
            var time = lib.moment(this.model.get('time'));
            return {
                timeShortStr: time.format('LTS'),
                timeFullStr: time.format('LLL')
            }
        },

        tagName: 'div'
    });
});
