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
import { posLoadData } from '../actions';
import LoadingPanel from '../components/LoadingPanel';

class PosDataPlot extends React.Component {
    static propTypes = {
        initialLoadingComplete: React.PropTypes.bool.isRequired,
        isLoading: React.PropTypes.bool.isRequired,
        samples: React.PropTypes.arrayOf(React.PropTypes.object).isRequired,
        posLoadData: React.PropTypes.func.isRequired
    };

    componentDidMount () {
        this._refresh();
    }

    componentDidUpdate (prevProps) {
        if (prevProps.samples !== this.props.samples) {
            this._updateData();
        }
    }

    _refresh = () => {
        this.props.posLoadData();
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
        let loadingPanel = null;
        if (this.props.isLoading) {
            loadingPanel = <LoadingPanel />;
        }

        return (
            <div className="panel panel-default">
                <div className="panel-heading">Data Plot</div>
                <div className="panel-body eb-device__panel-body">
                    {loadingPanel}
                    <div style={this.props.initialLoadingComplete ? null : {display: 'none'}}>
                        <div className="alert alert-info alert-dismissible" role="alert">
                            <button type="button" className="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                            <strong>Note:</strong> Real time data is available via SeedLink protocol{' '}
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

export default connect(mapStateToProps, {
    posLoadData
})(PosDataPlot);
