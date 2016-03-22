/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import 'babel-polyfill';
import React from 'react';
import { render } from 'react-dom';
import { browserHistory } from 'react-router';
import { syncHistoryWithStore } from 'react-router-redux';
import Root from './containers/Root';
import configureStore from './store/configureStore';
import moment from 'moment';

const store = configureStore();
const history = syncHistoryWithStore(browserHistory, store);

moment.locale('en');

function run() {
    render(
        <Root store={store} history={history} />,
        document.getElementById('root')
    );
}

if (process.env.NODE_ENV === 'production') {
    run();
} else {
    // Inline CSS which is extracted into a separate file in production needs some time to load
    setTimeout(run, 1000);
}
