
/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    '../utils/helpers'
], function (lib, helpers) {
    'use strict';

    return {
        get: function (url) {
            return this.request('GET', url, null);
        },
        
        post: function (url, data) {
            return this.request('POST', url, data);
        },
        
        request: function (method, url, data) {
            helpers.assertArgumentNotFalsy(method, 'method');
            helpers.assertArgumentNotFalsy(url, 'url');
            return Promise.resolve($.ajax({
                method: method,
                url: url,
                data: data != null ? JSON.stringify(data || {}) : null,
                dataType: 'json',
                contentType: 'application/json'
            }));
        }
    };
});
