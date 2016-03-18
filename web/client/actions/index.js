/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import 'isomorphic-fetch';

function ajaxApi ({ types, url, method = 'GET', data }) {
    let [ requestType, responseType, failureType ] = types;

    return (dispatch, getState) => {
        dispatch({
            type: requestType,
            request: data
        });

        let promise;
        let mt = method.toLowerCase();
        switch (mt) {
        case 'get':
            promise = fetch(url);
            break;
        case 'post':
        case 'put':
            promise = fetch(url, {
                method: mt,
                headers: {
                    Accept: 'application/json',
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(data)
            });
            break;
        default:
            throw new Error('Invalid method.');
        }

        promise.
            then(response => response.json()).
            then(response => {debugger;return dispatch({
                type: responseType,
                request: data,
                response
            });}).
            catch(error => dispatch({
                type: failureType,
                request: data,
                error
            }));
    };
}

/* POS ACTIONS */

export const POS_LOAD_STATUS_REQUEST = 'POS_LOAD_STATUS_REQUEST';
export const POS_LOAD_STATUS_RESPONSE = 'POS_LOAD_STATUS_RESPONSE';
export const POS_LOAD_STATUS_FAILURE = 'POS_LOAD_STATUS_FAILURE';
export function posLoadStatus () {
    return ajaxApi({
        types: [ POS_LOAD_STATUS_REQUEST, POS_LOAD_STATUS_RESPONSE, POS_LOAD_STATUS_FAILURE ],
        url: '/api/devices/pos/status'
    });
}

export const POS_LOAD_LOG_REQUEST = 'POS_LOAD_LOG_REQUEST';
export const POS_LOAD_LOG_RESPONSE = 'POS_LOAD_LOG_RESPONSE';
export const POS_LOAD_LOG_FAILURE = 'POS_LOAD_LOG_FAILURE';
export function posLoadLog () {
    return ajaxApi({
        types: [ POS_LOAD_LOG_REQUEST, POS_LOAD_LOG_RESPONSE, POS_LOAD_LOG_FAILURE ],
        url: '/api/devices/pos/log'
    });
}

export const POS_LOAD_DATA_REQUEST = 'POS_LOAD_DATA_REQUEST';
export const POS_LOAD_DATA_RESPONSE = 'POS_LOAD_DATA_RESPONSE';
export const POS_LOAD_DATA_FAILURE = 'POS_LOAD_DATA_FAILURE';
export function posLoadData () {
    return ajaxApi({
        types: [ POS_LOAD_DATA_REQUEST, POS_LOAD_DATA_RESPONSE, POS_LOAD_DATA_FAILURE ],
        url: '/api/devices/pos/data'
    });
}

export const POS_SEND_COMMAND_REQUEST = 'POS_SEND_COMMAND_REQUEST';
export const POS_SEND_COMMAND_RESPONSE = 'POS_SEND_COMMAND_RESPONSE';
export const POS_SEND_COMMAND_FAILURE = 'POS_SEND_COMMAND_FAILURE';
export function posSendCommand (data) {
    return ajaxApi({
        types: [ POS_SEND_COMMAND_REQUEST, POS_SEND_COMMAND_RESPONSE, POS_SEND_COMMAND_FAILURE ],
        url: '/api/devices/pos/command',
        method: 'POST',
        data: data
    });
}

export const POS_UPDATE_CONFIG_DATA = 'POS_UPDATE_CONFIG_DATA';
export function posUpdateConfigData (patch) {
    return {
        type: POS_UPDATE_CONFIG_DATA,
        patch: patch
    };
}
