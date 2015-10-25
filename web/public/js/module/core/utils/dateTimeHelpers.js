/**
 * Developer: Stepan Burguchev
 * Date: 10/25/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    './helpers'
], function (lib, helpers) {
    'use strict';

    return {
        durationToReadableString: function (duration) {
            var result;
            var days = duration.days();
            if (days) {
                result += days + ' days ';
            }
            var hours = duration.hours();
            if (hours) {
                result += hours + ' hours ';
            }
            var minutes = duration.minutes();
            if (minutes) {
                result += minutes + ' minutes ';
            }
            var seconds = duration.seconds();
            if (seconds) {
                result += seconds + ' seconds ';
            }
            return result;
        }
    }
});
