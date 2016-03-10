/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';
import { Link } from 'react-router';

const SideBar = () => {
    return (
        <div className="app-sidebar sidebar">
            <ul id="side-menu" className="nav">
                <li>
                    <Link to="/Pos" className="active"><i className="fa fa-dashboard fa-fw" /> POS-1</Link>
                </li>
            </ul>
        </div>
    );
};

export default SideBar;
