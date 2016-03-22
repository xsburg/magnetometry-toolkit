/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';
import PosDiagnostics from '../containers/PosDiagnostics';
import PosDataPlot from '../containers/PosDataPlot';
import PosControlPanel from '../containers/PosControlPanel';

const PosPage = () => {
    return (
        <div className="container-fluid">
            <div className="row">
                <div className="col-lg-12">
                    <h2 className="page-header">POS-1 Dashboard</h2>
                </div>
            </div>
            <div className="row">
                <div className="col-lg-6 eb-device__control-panel-block">
                    <PosControlPanel />
                </div>
                <div className="col-lg-6 eb-device__diagnostics-block">
                    <PosDiagnostics />
                    <PosDataPlot />
                </div>
            </div>
        </div>
    );
};

export default PosPage;
