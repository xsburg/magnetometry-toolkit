/**
 * Developer: Stepan Burguchev
 * Date: 3/22/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';

const GemPage = () => {
    return (
        <div className="container-fluid">
            <div className="row">
                <div className="col-lg-12">
                    <h2 className="page-header">GEM Systems Dashboard</h2>
                </div>
            </div>
            <div className="row">
                <div className="col-lg-6 pos__control-panel-block">
                </div>
                <div className="col-lg-6 pos__diagnostics-block">
                </div>
            </div>
        </div>
    );
};

export default GemPage;
