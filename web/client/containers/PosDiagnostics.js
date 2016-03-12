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

class PosDiagnostics extends React.Component {
    static propTypes = {
        initialLoadingComplete: React.PropTypes.bool.isRequired,
        isLoading: React.PropTypes.bool.isRequired,
        logEntries: React.PropTypes.arrayOf(React.PropTypes.object).isRequired
    };

    componentDidMount () {
        this._scrollToBottom();
    }

    componentDidUpdate () {
        //this._scrollToBottom();
    }

    _scrollToBottom () {
        this._pre.scrollTop = this._pre.scrollHeight - this._pre.clientHeight;
    }

    render () {
        let dataUpdatingLoaderClass = this.props.isLoading ? 'eb-device__data-update-spinner_visible' : '';

        let logEntries = this.props.logEntries.map(entry => <div key={entry.id}>
            <span className="eb-device__diagnostics-log-list-item__time" title={moment(entry.time).format('LLL')}>
                {moment(entry.time).format('LTS')}</span> [{entry.logLevel}] {entry.message}
        </div>);

        return (
            <div className="panel panel-default">
                <div className="panel-heading">POS-1 Diagnostic Log</div>
                <div className="panel-body eb-device__panel-body">
                    <div className="eb-device__loading-panel"
                         style={this.props.initialLoadingComplete ? {display: 'none'} : null}>
                        <div className="eb-device__loading-panel__fill"></div>
                        <div className="eb-device__loading-panel__text">
                            <div className="fa fa-cog fa-spin fa-2x"></div>
                        </div>
                    </div>
                    <h4><small>
                        <span>The service log is updated in real time </span>
                        <span className={`fa fa-circle-o-notch fa-spin eb-device__data-update-spinner ${dataUpdatingLoaderClass}`} />
                    </small></h4>
                    <pre className="eb-device__diagnostics-log-list" ref={el => this._pre = el}>
                        {logEntries}
                    </pre>
                </div>
            </div>
        );
    }
}

function mapStateToProps(state) {
    return state.pos.diagnostics;
}

export default connect(mapStateToProps)(PosDiagnostics);
