/**
 * Developer: Stepan Burguchev
 * Date: 3/11/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import {
    POS_UPDATE_CONFIG_DATA,
    POS_SEND_COMMAND_REQUEST, POS_LOAD_DATA_REQUEST, POS_LOAD_DATA_RESPONSE, POS_LOAD_DATA_FAILURE,
    POS_LOAD_STATUS_REQUEST, POS_LOAD_STATUS_RESPONSE, POS_LOAD_STATUS_FAILURE
} from '../actions';
import { combineReducers } from 'redux';
import _ from 'lodash';

function config (state = {
    initialLoadingComplete: false,
    isLoading: false,
    hasUnhandledCommands: false,
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
    case POS_LOAD_STATUS_REQUEST:
        return {
            ...state,
            isLoading: true,
            hasUnhandledCommandsSnapshot: state.hasUnhandledCommands
        };
    case POS_LOAD_STATUS_RESPONSE:
    case POS_LOAD_STATUS_FAILURE:
        // TODO
        return {
            ...state,
            isLoading: false,
            initialLoadingComplete: true,
            // There was a new POS_SEND_COMMAND_REQUEST while we were waiting for the response so it's unclear whether the server has handled it or not.
            hasUnhandledCommands: !state.hasUnhandledCommandsSnapshot && state.hasUnhandledCommands,
            hasUnhandledCommandsSnapshot: null
        };
    case POS_SEND_COMMAND_REQUEST: {
        let data = state.data;
        if (action.request.command === 'set-device-stand-by') {
            let newStandBy = !state.data.standBy;
            data = {
                ...state.data,
                standBy: newStandBy
            };
        }
        return {
            ...state,
            data,
            hasUnhandledCommands: true
        };
    }
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

function dataPlot (state = {
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
}, action) {
    switch (action.type) {
    case POS_LOAD_DATA_REQUEST:
        return {
            ...state,
            isLoading: true
        };
    case POS_LOAD_DATA_RESPONSE: {
        // TODO
        let samples = action.response.filter(x => x.state & 0x80).map(x => ({
            ...x,
            time: Number(new Date(x.time))
        }));
        return {
            ...state,
            samples,
            isLoading: false
        };
    }
    case POS_LOAD_DATA_FAILURE:
        return {
            ...state,
            isLoading: false
        };
    default:
        return state;
    }
}

const posReducer = combineReducers({
    config,
    diagnostics,
    dataPlot
});

export default posReducer;
