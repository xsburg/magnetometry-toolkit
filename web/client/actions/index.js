/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

export const RESET_ERROR_MESSAGE = 'RESET_ERROR_MESSAGE';

// Resets the currently visible error message.
export function resetErrorMessage () {
    return {
        type: RESET_ERROR_MESSAGE
    };
}

//import { CALL_API, Schemas } from '../middleware/api';
/*
export const STARGAZERS_REQUEST = 'STARGAZERS_REQUEST';
export const STARGAZERS_SUCCESS = 'STARGAZERS_SUCCESS';
export const STARGAZERS_FAILURE = 'STARGAZERS_FAILURE';

// Fetches a page of stargazers for a particular repo.
// Relies on the custom API middleware defined in ../middleware/api.js.
function fetchStargazers (fullName, nextPageUrl) {
    return {
        fullName,
        [CALL_API]: {
            types: [STARGAZERS_REQUEST, STARGAZERS_SUCCESS, STARGAZERS_FAILURE],
            endpoint: nextPageUrl,
            schema: Schemas.USER_ARRAY
        }
    };
}

// Fetches a page of stargazers for a particular repo.
// Bails out if page is cached and user didn’t specifically request next page.
// Relies on Redux Thunk middleware.
export function loadStargazers (fullName, nextPage) {
    return (dispatch, getState) => {
        const {
            nextPageUrl = `repos/${fullName}/stargazers`,
            pageCount = 0
            } = getState().pagination.stargazersByRepo[fullName] || {};

        if (pageCount > 0 && !nextPage) {
            return null;
        }

        return dispatch(fetchStargazers(fullName, nextPageUrl));
    };
}*/
