/**
 * Developer: Stepan Burguchev
 * Date: 9/27/2015
 * Copyright: 2009-2015 Stepan Burguchev®
 *       All Rights Reserved
 */

/* global define, require, Backbone, Marionette, $, _, Localizer */

define([
    'lib'
], function (lib) {
    'use strict';

    return Backbone.Model.extend({
        initialize: function () {
        },

        defaults: {
            isRunning: false,
            updated: new Date(),
            standBy: false,
            samplingIntervalMs: 1000,
            range: {
                minField: 47500,
                maxField: 55000
            },
            enq: 'MagState version 3.03',
            about: 'MagState version 3.03\r\n\
            Quantum Magnetometry Laboratory\r\n\
        Urals State Technical University\r\n\
        Copyright 2013.',
            time: new Date(),
            commandQueueSize: 2
        },

        urlRoot: 'api/dashboard/eb-device/status'
    });
});
