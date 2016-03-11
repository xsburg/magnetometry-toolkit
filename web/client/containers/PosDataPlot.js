/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';
import { connect } from 'react-redux';
import moment from 'moment';
import Morris from 'morris';

class PosDataPlot extends React.Component {
    static propTypes = {
        initialLoadingComplete: React.PropTypes.bool.isRequired,
        isLoading: React.PropTypes.bool.isRequired,
        samples: React.PropTypes.arrayOf(React.PropTypes.object).isRequired
    };

    componentDidMount () {
        this._updateData();
    }

    componentDidUpdate () {
        //this._updateData();
    }

    _refresh = () => {

    };

    _updateData () {
        this._line.setData(this.props.samples, true);
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
            resize: false
        });
    };

    render () {
        return (
            <div className="panel panel-default">
                <div className="panel-heading">Data Plot</div>
                <div className="panel-body eb-device__panel-body">
                    <div className="eb-device__loading-panel"
                         style={this.props.isLoading ? null : {display: 'none'}}>
                        <div className="eb-device__loading-panel__fill"></div>
                        <div className="eb-device__loading-panel__text">
                            <div className="fa fa-cog fa-spin fa-2x"></div>
                        </div>
                    </div>
                    <div style={this.props.initialLoadingComplete ? null : {display: 'none'}}>
                        <div className="alert alert-info alert-dismissible" role="alert">
                            <button type="button" className="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                            <strong>Note:</strong> Real time data is available via SeedLink protocol
                            <a href="#" className="alert-link">here</a>
                        </div>
                        <h4><small>
                            The data is updated manually <button type="button" className="btn btn-primary btn-xs"
                            onClick={this._refresh}>Refresh</button>
                        </small></h4>
                        <div style={this.props.samples.length > 0 ? { height: '350px' } : { display: 'none' }}
                             ref={this._applyDataPlot}></div>
                    </div>
                </div>
            </div>
        );
    }
}

function mapStateToProps(state) {
    return state.pos.dataPlot;
}

export default connect(mapStateToProps)(PosDataPlot);
