/**
 * Developer: Stepan Burguchev
 * Date: 10/11/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Handlebars, Backbone, Marionette, $, _, Localizer */

define([
    'lib',
    'core',
    './LogListItemView'
], function (lib, core, LogListItemView) {
    'use strict';

    return Marionette.CollectionView.extend({
        initialize: function (options) {
            core.utils.helpers.ensureOption(options, 'collection');
        },

        collectionEvents: {
            'add remove reset': '__onCollectionChange'
        },

        childView: LogListItemView,

        className: 'eb-device__diagnostics-log-list',

        tagName: 'pre',

        onShow: function () {
            this.__scrollToBottom();
        },

        __onCollectionChange: function () {
            // If scrolled to bottom, keep it
            var isScrolledToBottom = this.el.scrollHeight - this.el.clientHeight <= this.el.scrollTop + 1;
            // Defer is required since CollectionView update is bound to the same event
            _.defer(function () {
                if (isScrolledToBottom) {
                    this.__scrollToBottom();
                }
            }.bind(this));
        },

        __scrollToBottom: function () {
            this.el.scrollTop = this.el.scrollHeight - this.el.clientHeight;
        }
    });
});
