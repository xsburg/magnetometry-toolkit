/**
 * Developer: Stepan Burguchev
 * Date: 3/22/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';

const LoadingPanel = () => {
    return (
        <div className="eb-device__loading-panel">
            <div className="eb-device__loading-panel__fill"></div>
            <div className="eb-device__loading-panel__text">
                <div className="fa fa-cog fa-spin fa-2x"></div>
            </div>
        </div>
    );
};

export default LoadingPanel;
