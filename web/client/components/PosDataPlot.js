/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';
import moment from 'moment';
import Morris from 'morris';

export default class PosDataPlot extends React.Component {
    constructor (props) {
        super(props);
        this.state = {
            initialLoadingComplete: true,
            isDataUpdating: false,
            samples: [
                {
                    time: '2016-03-02T14:56:19.721Z',
                    field: 12345
                },
                {
                    time: '2016-03-02T14:56:20.721Z',
                    field: 2346
                },
                {
                    time: '2016-03-02T14:56:21.721Z',
                    field: 4345
                },
                {
                    time: '2016-03-02T14:56:22.721Z',
                    field: 6345
                },
                {
                    time: '2016-03-02T14:56:23.721Z',
                    field: 2345
                }
            ]
        };
    }

    componentDidMount () {
        this._updateData();
    }

    componentDidUpdate () {
        this._updateData();
    }

    _refresh = () => {

    };

    _updateData () {
        this._line.setData(this.state.samples, true);
    }

    _applyDataPlot = (el) => {
        this._line = new Morris.Line({
            element: el,
            data: [],
            xkey: 'time',
            ykeys: ['field'],
            labels: ['Field'],
            dateFormat: function (x) {
                return moment(x).format();
            },
            resize: true
        });
    };

    render () {
        return (
            <div className="panel panel-default">
                <div className="panel-heading">Data Plot</div>
                <div className="panel-body eb-device__panel-body">
                    <div className="eb-device__loading-panel"
                         style={this.state.isDataUpdating ? null : {display: 'none'}}>
                        <div className="eb-device__loading-panel__fill"></div>
                        <div className="eb-device__loading-panel__text">
                            <div className="fa fa-cog fa-spin fa-2x"></div>
                        </div>
                    </div>
                    <div style={this.state.initialLoadingComplete ? null : {display: 'none'}}>
                        <div className="alert alert-info alert-dismissible" role="alert">
                            <button type="button" className="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                            <strong>Note:</strong> Real time data is available via SeedLink protocol
                            <a href="#" className="alert-link">here</a>
                        </div>
                        <h4><small>
                            The data is updated manually <button type="button" className="btn btn-primary btn-xs"
                            onClick={this._refresh}>Refresh</button>
                        </small></h4>
                        <div style={this.state.samples.length > 0 ? { height: '350px' } : { display: 'none' }}
                             ref={this._applyDataPlot}></div>
                    </div>
                </div>
            </div>
        );
    }
}

PosDataPlot.propTypes = {
};

PosDataPlot.defaultProps = {
};
