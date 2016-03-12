/**
 * Developer: Stepan Burguchev
 * Date: 3/11/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import { POS_UPDATE_CONFIG_DATA } from '../actions';
import { combineReducers } from 'redux';
import _ from 'lodash';

function config (state = {
    initialLoadingComplete: true,
    isLoading: false,
    readonly: false,
    data: {
        about: 'MagState version 3.03\nQuantum Magnetometry Laboratory\nUrals State Technical University\nCopyright 2013.',
        commandQueueSize: 0,
        enq: 'MagState version 3.03',
        isRunning: false,
        mseedSettings: {
            fileName: 'data.mseed',
            location: 'SK',
            network: 'RU',
            samplesInRecord: 2,
            station: 'IFZ'
        },
        range: {
            maxField: 50400,
            minField: 48100,
            center: null
        },
        samplingIntervalMs: 1000,
        standBy: false,
        time: '1997-02-03T23:40:03.000Z',
        timeFixIntervalSeconds: 0,
        timeUpdated: '2016-03-02T14:56:23.325Z',
        updated: '2016-03-02T14:56:23.325Z'
    }
}, action) {
    switch (action.type) {
    case POS_UPDATE_CONFIG_DATA:
        return {
            ...state,
            data: _.merge({}, state.data, action.patch)
        };
    default:
        return state;
    }
}

function diagnostics (state, action) {
    return {
        initialLoadingComplete: true,
        isLoading: false,
        logEntries: [
            {
                id: 1,
                logLevel: 'DEBUG',
                message: 'PosDevice is reading pending response messages...',
                time: '2016-03-02T14:56:19.721Z'
            }, {
                id: 2,
                logLevel: 'DEBUG',
                message: 'PosDevice got response message, raw size = 22, decoded size = 22...',
                time: '2016-03-02T14:56:19.721Z'
            }, {
                id: 3,
                logLevel: 'DEBUG',
                message: 'PosDevice response messages: 1 messages read in total.',
                time: '2016-03-02T14:56:19.721Z'
            }, {
                id: 4,
                logLevel: 'DEBUG',
                message: 'PosDevice is reading pending response messages...',
                time: '2016-03-02T14:56:19.822Z'
            }, {
                id: 5,
                logLevel: 'DEBUG',
                message: 'PosDevice read data suspicious behavior: `waitForReadyRead timeout exceeded`. Error string: No such process.',
                time: '2016-03-02T14:56:20.823Z'
            }, {
                id: 6,
                logLevel: 'DEBUG',
                message: 'PosDevice is reading pending response messages...',
                time: '2016-03-02T14:56:21.323Z'
            }, {
                id: 7,
                logLevel: 'DEBUG',
                message: 'PosDevice got response message, raw size = 16, decoded size = 16...',
                time: '2016-03-02T14:56:21.324Z'
            }, {
                id: 8,
                logLevel: 'DEBUG',
                message: 'PosDevice response messages: 1 messages read in total.',
                time: '2016-03-02T14:56:21.324Z'
            }, {
                id: 9,
                logLevel: 'DEBUG',
                message: 'PosDevice got text response message: "set binary mode"',
                time: '2016-03-02T14:56:21.324Z'
            }, {
                id: 10,
                logLevel: 'INFO',
                message: 'Executing command UPDATE-STATUS...',
                time: '2016-03-02T14:56:21.324Z'
            }, {
                id: 11,
                logLevel: 'DEBUG',
                message: 'PosDevice is reading pending response messages...',
                time: '2016-03-02T14:56:21.824Z'
            }, {
                id: 12,
                logLevel: 'DEBUG',
                message: 'PosDevice got response message, raw size = 22, decoded size = 22...',
                time: '2016-03-02T14:56:21.824Z'
            }, {
                id: 13,
                logLevel: 'DEBUG',
                message: 'PosDevice response messages: 1 messages read in total.',
                time: '2016-03-02T14:56:21.824Z'
            }, {
                id: 14,
                logLevel: 'DEBUG',
                message: 'PosDevice got text response message: "MagState version 3.03"',
                time: '2016-03-02T14:56:21.824Z'
            }, {
                id: 15,
                logLevel: 'DEBUG',
                message: 'PosDevice is reading pending response messages...',
                time: '2016-03-02T14:56:22.324Z'
            }, {
                id: 16,
                logLevel: 'DEBUG',
                message: 'PosDevice got response message, raw size = 103, decoded size = 103...',
                time: '2016-03-02T14:56:22.324Z'
            }, {
                id: 17,
                logLevel: 'DEBUG',
                message: 'PosDevice response messages: 1 messages read in total.',
                time: '2016-03-02T14:56:22.324Z'
            }, {
                id: 18,
                logLevel: 'DEBUG',
                message: 'PosDevice got text response message: "MagState version 3.03\n' +
                'Quantum Magnetometry Laboratory\nUrals State Technical University\nCopyright 2013."',
                time: '2016-03-02T14:56:22.325Z'
            }, {
                id: 19,
                logLevel: 'DEBUG',
                message: 'PosDevice is reading pending response messages...',
                time: '2016-03-02T14:56:22.825Z'
            }, {
                id: 20,
                logLevel: 'DEBUG',
                message: 'PosDevice got response message, raw size = 13, decoded size = 9...',
                time: '2016-03-02T14:56:22.825Z'
            }, {
                id: 21,
                logLevel: 'DEBUG',
                message: 'PosDevice response messages: 1 messages read in total.',
                time: '2016-03-02T14:56:22.825Z'
            }, {
                id: 22,
                logLevel: 'DEBUG',
                message: 'PosDevice is reading pending response messages...',
                time: '2016-03-02T14:56:23.325Z'
            }, {
                id: 23,
                logLevel: 'DEBUG',
                message: 'PosDevice got response message, raw size = 5, decoded size = 5...',
                time: '2016-03-02T14:56:23.325Z'
            }, {
                id: 24,
                logLevel: 'DEBUG',
                message: 'PosDevice response messages: 1 messages read in total.',
                time: '2016-03-02T14:56:23.325Z'
            }
        ]
    };
}

function dataPlot (state, action) {
    return {
        initialLoadingComplete: true,
        isLoading: false,
        samples: [
            {
                time: '2016-03-02T14:56:19.721Z',
                field: 12345
            },
            {
                time: '2016-03-02T14:56:20.721Z',
                field: 2346
            },
            {
                time: '2016-03-02T14:56:21.721Z',
                field: 4345
            },
            {
                time: '2016-03-02T14:56:22.721Z',
                field: 6345
            },
            {
                time: '2016-03-02T14:56:23.721Z',
                field: 2345
            }
        ]
    };
}

const posReducer = combineReducers({
    config,
    diagnostics,
    dataPlot
});

export default posReducer;
