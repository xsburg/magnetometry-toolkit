/**
 * Developer: Stepan Burguchev
 * Date: 7/4/2014
 * Copyright: 2009-2013 Stepan Burguchev®
 *       All Rights Reserved
 */

import React from 'react';
import ReactDOM from 'react-dom';
import injectTapEventPlugin from 'react-tap-event-plugin';
import App from './App';
import $ from 'jquery';

injectTapEventPlugin();

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

$('.js-startup-loading').remove();

ReactDOM.render(
    <App />,
    document.getElementById('app')
);