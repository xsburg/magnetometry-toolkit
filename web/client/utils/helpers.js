/**
 * Developer: Stepan Burguchev
 * Date: 3/10/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import _ from 'lodash';

let helpers = {
    ensureOption: function (options, optionName) {
        if (!options) {
            helpers.throwError('The options object is required.', 'MissingOptionError');
        }
        if (optionName.indexOf('.') !== -1) {
            let selector = optionName.split('.');
            for (let i = 0, len = selector.length; i < len; i++) {
                optionName = selector[i];
                if (options[optionName] === undefined) {
                    optionName = _.take(selector, i + 1).join('.');
                    helpers.throwError(`The option '${optionName}' is required.`, 'MissingOptionError');
                }
                options = options[optionName];
            }
        } else {
            if (options[optionName] === undefined) {
                helpers.throwError(`The option '${optionName}' is required.`, 'MissingOptionError');
            }
        }
    },

    ensureProperty: function (object, propertyName) {
        if (!object) {
            helpers.throwError('The object is null.', 'NullObjectError');
        }
        if (propertyName.indexOf('.') !== -1) {
            let selector = propertyName.split('.');
            for (let i = 0, len = selector.length; i < len; i++) {
                propertyName = selector[i];
                if (object[propertyName] === undefined) {
                    propertyName = _.take(selector, i + 1).join('.');
                    helpers.throwError(`The property '${propertyName}' is required.`, 'MissingPropertyError');
                }
                object = object[propertyName];
            }
        } else {
            if (object[propertyName] === undefined) {
                helpers.throwError(`The property '${propertyName}' is required.`, 'MissingPropertyError');
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
            this.throwError(`Argument '${argumentName}' is falsy.`, 'ArgumentFalsyError');
        }
    },

    throwError: function (message, name) {
        let error = new Error(message);
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

export default helpers;
