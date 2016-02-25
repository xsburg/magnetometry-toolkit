/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib'
], function (lib) {
    'use strict';

    return {
        textToHtml: function (text) {
            if (!text) {
                return '';
            }
            var lines = text.split(/[\r\n]+/g);
            var result = [];
            _.each(lines, function (line) {
                result.push(Handlebars.Utils.escapeExpression(line));
            });
            return result.join('<br>');
        }
    };
});
