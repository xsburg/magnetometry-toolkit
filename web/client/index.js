/**
 * Developer: Stepan Burguchev
 * Date: 7/4/2014
 * Copyright: 2009-2013 Stepan Burguchev®
 *       All Rights Reserved
 */

import 'babel-polyfill';
import React from 'react';
import { render } from 'react-dom';
import ReactRouter, { browserHistory } from 'react-router';
import { syncHistoryWithStore } from 'react-router-redux';
import Root from './containers/Root';
import configureStore from './store/configureStore';

const store = configureStore();
const history = syncHistoryWithStore(browserHistory, store);

render(
    <Root store={store} history={history} />,
    document.getElementById('root')
);

/*

function run() {
    render(
        <Root store={store} history={history} />,
        document.getElementById('root')
    );
}

if (process.env.NODE_ENV === 'production') {
    run();
} else {
    // Inline CSS which is extracted into a separate file on production needs some time to load
    setTimeout(run, 1000);
}
*/

//-----------------------------------------------------

//import createBrowserHistory from 'history/lib/createBrowserHistory'
//import { useRouterHistory } from 'react-router'
//import { syncHistoryWithStore } from 'react-router-redux'
//import makeRoutes from './routes'
//import Root from './containers/Root'
//import configureStore from './redux/configureStore'

// Create redux store and sync with react-router-redux. We have installed the
// react-router-redux reducer under the key "router" in src/routes/index.js,
// so we need to provide a custom `selectLocationState` to inform
// react-router-redux of its location.
/*const initialState = window.__INITIAL_STATE__
const store = configureStore(initialState);

// Configure history for react-router
const browserHistory = useRouterHistory(createBrowserHistory)({
    basename: __BASENAME__
})
const history = syncHistoryWithStore(browserHistory, store, {
    selectLocationState: (state) => state.router
})

// Now that we have the Redux store, we can create our routes. We provide
// the store to the route definitions so that routes have access to it for
// hooks such as `onEnter`.
const routes = makeRoutes(store)*/
