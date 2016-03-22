/**
 * Developer: Stepan Burguchev
 * Date: 3/11/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import {
    POS_UPDATE_FORM_DATA,
    POS_SEND_COMMAND_REQUEST, POS_LOAD_DATA_REQUEST, POS_LOAD_DATA_RESPONSE, POS_LOAD_DATA_FAILURE,
    POS_LOAD_STATUS_REQUEST, POS_LOAD_STATUS_RESPONSE, POS_LOAD_STATUS_FAILURE,
    POS_LOAD_LOG_REQUEST, POS_LOAD_LOG_RESPONSE, POS_LOAD_LOG_FAILURE
} from '../actions';
import { combineReducers } from 'redux';
import _ from 'lodash';

/*
* STATUS DATA EXAMPLE:
*
* {
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
* */

function config (state = {
    initialLoadingComplete: false,
    isLoading: false,
    hasUnhandledCommands: false,
    error: null,
    status: null,
    formData: null
}, action) {
    switch (action.type) {
    case POS_UPDATE_FORM_DATA:
        return {
            ...state,
            formData: _.merge({}, state.formData, action.patch)
        };
    case POS_LOAD_STATUS_REQUEST:
        return {
            ...state,
            isLoading: true,
            hasUnhandledCommandsSnapshot: state.hasUnhandledCommands
        };
    case POS_LOAD_STATUS_RESPONSE: {
        let newStatus = action.response;
        let newState = {
            ...state,
            status: newStatus,
            isLoading: false,
            initialLoadingComplete: true,
            error: null,
            // There was a new POS_SEND_COMMAND_REQUEST while we were waiting for the response so it's unclear whether the server has handled it or not.
            hasUnhandledCommands: !state.hasUnhandledCommandsSnapshot && state.hasUnhandledCommands,
            hasUnhandledCommandsSnapshot: null
        };
        if (!state.initialLoadingComplete) {
            // For the first time we load the status the form data must be filled with default values
            newState.formData = {
                samplingIntervalMs: newStatus.samplingIntervalMs,
                timeFixIntervalSeconds: newStatus.timeFixIntervalSeconds,
                mseedFileName: newStatus.mseedSettings.fileName,
                mseedNetwork: newStatus.mseedSettings.network,
                mseedStation: newStatus.mseedSettings.station,
                mseedLocation: newStatus.mseedSettings.location,
                mseedSamplesInRecord: newStatus.mseedSettings.samplesInRecord,
                fieldRangeCenter: null
            };
        }
        return newState;
    }
    case POS_LOAD_STATUS_FAILURE:
        return {
            ...state,
            isLoading: false,
            initialLoadingComplete: false,
            error: 'Failed to load the device status.',
            hasUnhandledCommandsSnapshot: null
        };
    case POS_SEND_COMMAND_REQUEST: {
        let status = state.status;
        if (action.request.command === 'set-device-stand-by') {
            let newStandBy = !state.status.standBy;
            status = {
                ...state.status,
                standBy: newStandBy
            };
        }
        return {
            ...state,
            status,
            hasUnhandledCommands: true
        };
    }
    default:
        return state;
    }
}

/*
* LOG ENTRY EXAMPLE
*
* {
     id: 1,
     logLevel: 'DEBUG',
     message: 'PosDevice is reading pending response messages...',
     time: '2016-03-02T14:56:19.721Z'
  }

* */

function diagnostics (state = {
    initialLoadingComplete: true,
    isLoading: false,
    logEntries: [],
    error: null
}, action) {
    switch (action.type) {
    case POS_LOAD_LOG_REQUEST:
        return {
            ...state,
            isLoading: true
        };
    case POS_LOAD_LOG_RESPONSE: {
        return {
            ...state,
            logEntries: action.response.messages,
            isLoading: false,
            initialLoadingComplete: true,
            error: null
        };
    }
    case POS_LOAD_STATUS_FAILURE:
        return {
            ...state,
            isLoading: false,
            initialLoadingComplete: false,
            error: 'Failed to load the device log.'
        };
    default:
        return state;
    }
}

function dataPlot (state = {
    initialLoadingComplete: true,
    isLoading: false,
    samples: []
}, action) {
    switch (action.type) {
    case POS_LOAD_DATA_REQUEST:
        return {
            ...state,
            isLoading: true
        };
    case POS_LOAD_DATA_RESPONSE: {
        let samples = action.response.samples.filter(x => x.state & 0x80).map(x => ({
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
