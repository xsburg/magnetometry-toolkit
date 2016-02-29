/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';

const SideBar = () => {
    return (
        <div className="app-sidebar sidebar">
            <ul id="side-menu" className="nav">
                <li>
                    <a className="active" href="index.html"><i className="fa fa-dashboard fa-fw" /> Dashboard</a>
                </li>
            </ul>
        </div>
    );
};

export default SideBar;
