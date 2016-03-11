import * as ActionTypes from '../actions';
import merge from 'lodash/merge';
import { routerReducer as routing } from 'react-router-redux';
import { combineReducers } from 'redux';
import pos from './pos';

// Updates an entity cache in response to any action with response.entities.
function entities (state = {users: {}, repos: {}}, action) {
    if (action.response && action.response.entities) {
        return merge({}, state, action.response.entities);
    }

    return state;
}

// Updates error message to notify about the failed fetches.
function errorMessage (state = null, action) {
    const { type, error } = action;

    if (type === ActionTypes.RESET_ERROR_MESSAGE) {
        return null;
    } else if (error) {
        return action.error;
    }

    return state;
}

const rootReducer = combineReducers({
    pos,
    errorMessage,
    routing
});

export default rootReducer;
