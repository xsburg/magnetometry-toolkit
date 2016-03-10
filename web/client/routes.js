/**
 * Developer: Stepan Burguchev
 * Date: 3/10/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import React from 'react';
import { Route, IndexRedirect } from 'react-router';
import App from './containers/App';
import PosPage from './components/PosPage';

export default (
    <Route path="/" component={App}>
        <IndexRedirect to="/Pos" />
        <Route path="/Pos" component={PosPage} />
    </Route>
);
