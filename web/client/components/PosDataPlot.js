/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';

const PosDataPlot = () => {
    return (
        <div className="panel panel-default">
            <div className="panel-heading">Data Plot</div>
            <div className="panel-body eb-device__panel-body">
                <div className="eb-device__loading-panel js-loading-panel">
                    <div className="eb-device__loading-panel__fill"></div>
                    <div className="eb-device__loading-panel__text">
                        <div className="fa fa-cog fa-spin fa-2x"></div>
                    </div>
                </div>
                <div className="js-data-panel">
                    <div className="alert alert-info alert-dismissible" role="alert">
                        <button type="button" className="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                        <strong>Note:</strong> Real time data is available via SeedLink protocol
                        <a href="#" className="alert-link">here</a>
                    </div>
                    <h4><small>
                        The data is updated manually <button type="button" className="btn btn-primary btn-xs js-refresh-button">Refresh</button>
                    </small></h4>
                    <div className="js-data-plot" style={{ height: '350px' }}></div>
                </div>
            </div>
        </div>
    );
};

export default PosDataPlot;
