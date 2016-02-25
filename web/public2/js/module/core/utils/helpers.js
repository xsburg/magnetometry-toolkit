/**
 * Developer: Stepan Burguchev
 * Date: 8/21/2014
 * Copyright: 2009-2014 Comindware®
 *       All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Comindware
 *       The copyright notice above does not evidence any
 *       actual or intended publication of such source code.
 */

/* global define, require, _, $ */

define(['lib' ],
    function (lib) {
        'use strict';

        var timeoutCache = {};

        var queueCache = {};

        var helpers = {
            comparatorFor: function (comparatorFn, propertyName) {
                if (comparatorFn.length === 1) {
                    return function (a) {
                        return comparatorFn(a.get(propertyName));
                    };
                } else if (comparatorFn.length === 2) {
                    return function (a, b) {
                        return comparatorFn(a.get(propertyName), b.get(propertyName));
                    };
                } else {
                    throw new Error('Invalid arguments count in comparator function.');
                }
            },

            /*
            * Javascript version of string.Format in .NET
            * */
            format: function(text) {
                if (!_.isString(text)) {
                    return '';
                }
                for (var i = 1; i < arguments.length; i++) {
                    var regexp = new RegExp('\\{'+(i-1)+'\\}', 'gi');
                    text = text.replace(regexp, arguments[i]);
                }
                return text;
            },

            applyBehavior: function (target) {
                var behaviors = _.rest(arguments, 1);
                _.each(behaviors, function (Behavior) {
                    _.extend(target, new Behavior(target));
                });
            },
            
            ensureOption: function (options, optionName) {
                if (!options) {
                    helpers.throwError('The options object is required.', 'MissingOptionError');
                }
                if (optionName.indexOf('.') !== -1) {
                    var selector = optionName.split('.');
                    for (var i = 0, len = selector.length; i < len; i++) {
                        optionName = selector[i];
                        if (options[optionName] === undefined) {
                            optionName = _.take(selector, i + 1).join('.');
                            helpers.throwError('The option `' + optionName + '` is required.', 'MissingOptionError');
                        }
                        options = options[optionName];
                    }
                } else {
                    if (options[optionName] === undefined) {
                        helpers.throwError('The option `' + optionName + '` is required.', 'MissingOptionError');
                    }
                }
            },

            ensureProperty: function (object, propertyName) {
                if (!object) {
                    helpers.throwError('The object is null.', 'NullObjectError');
                }
                if (propertyName.indexOf('.') !== -1) {
                    var selector = propertyName.split('.');
                    for (var i = 0, len = selector.length; i < len; i++) {
                        propertyName = selector[i];
                        if (object[propertyName] === undefined) {
                            propertyName = _.take(selector, i + 1).join('.');
                            helpers.throwError('The property `' + propertyName + '` is required.', 'MissingPropertyError');
                        }
                        object = object[propertyName];
                    }
                } else {
                    if (object[propertyName] === undefined) {
                        helpers.throwError('The property `' + propertyName + '` is required.', 'MissingPropertyError');
                    }
                }
            },

            getPropertyOrDefault: function (propertyPath, obj) {
                return [obj].concat(propertyPath.split('.')).reduce(function(prev, curr) {
                    return prev === undefined ? undefined : prev[curr];
                });
            },

            assertArgumentNotFalsy: function (argumentValue, argumentName) {
                if (!argumentValue) {
                    this.throwError('Argument `' + argumentName + '` is falsy.', 'ArgumentFalsyError');
                }
            },

            throwError: function (message, name) {
                var error = new Error(message);
                error.name = name || 'Error';
                throw error;
            },

            throwInvalidOperationError: function (message) {
                helpers.throwError(message || 'Invalid operation', 'InvalidOperationError');
            },

            throwFormatError: function (message) {
                helpers.throwError(message || 'Invalid format', 'FormatError');
            },

            throwArgumentError: function (message) {
                helpers.throwError(message || 'Invalid argument', 'ArgumentError');
            },

            throwNotSupportedError: function (message) {
                helpers.throwError(message || 'The operation is not supported', 'NotSupportedError');
            },

            throwNotImplementedError: function (message) {
                helpers.throwError(message || 'The operation is not implemented', 'NotImplementedError');
            },

            throwNotFoundError: function (message) {
                helpers.throwError(message || 'Object not found', 'NotFoundError');
            }
        };

        return helpers;
    });
