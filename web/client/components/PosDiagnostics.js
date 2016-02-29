/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';

const PosDiagnostics = () => {
    return (
        <div className="panel panel-default">
            <div className="panel-heading">MagState Diagnostics</div>
            <div className="panel-body eb-device__panel-body">
                <div className="eb-device__loading-panel js-loading-panel">
                    <div className="eb-device__loading-panel__fill"></div>
                    <div className="eb-device__loading-panel__text">
                        <div className="fa fa-cog fa-spin fa-2x"></div>
                    </div>
                </div>
                <h4><small>
                    The service log is updated in real time <span className="fa fa-circle-o-notch fa-spin js-data-update-spinner eb-device__data-update-spinner"></span>
                </small></h4>
                <div className="js-log-list-region"></div>
            </div>
        </div>
    );
};

export default PosDiagnostics;
