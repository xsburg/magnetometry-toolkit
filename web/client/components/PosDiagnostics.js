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

export default class PosDiagnostics extends React.Component {
    constructor (props) {
        super(props);
        this.state = {
            initialLoadingComplete: true,
            isDataUpdating: false,
            messages: [
                {
                    id: 1,
                    logLevel: 'DEBUG',
                    message: 'PosDevice is reading pending response messages...',
                    time: '2016-03-02T14:56:19.721Z'
                }, {
                    id: 2,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got response message, raw size = 22, decoded size = 22...',
                    time: '2016-03-02T14:56:19.721Z'
                }, {
                    id: 3,
                    logLevel: 'DEBUG',
                    message: 'PosDevice response messages: 1 messages read in total.',
                    time: '2016-03-02T14:56:19.721Z'
                }, {
                    id: 4,
                    logLevel: 'DEBUG',
                    message: 'PosDevice is reading pending response messages...',
                    time: '2016-03-02T14:56:19.822Z'
                }, {
                    id: 5,
                    logLevel: 'DEBUG',
                    message: 'PosDevice read data suspicious behavior: `waitForReadyRead timeout exceeded`. Error string: No such process.',
                    time: '2016-03-02T14:56:20.823Z'
                }, {
                    id: 6,
                    logLevel: 'DEBUG',
                    message: 'PosDevice is reading pending response messages...',
                    time: '2016-03-02T14:56:21.323Z'
                }, {
                    id: 7,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got response message, raw size = 16, decoded size = 16...',
                    time: '2016-03-02T14:56:21.324Z'
                }, {
                    id: 8,
                    logLevel: 'DEBUG',
                    message: 'PosDevice response messages: 1 messages read in total.',
                    time: '2016-03-02T14:56:21.324Z'
                }, {
                    id: 9,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got text response message: "set binary mode"',
                    time: '2016-03-02T14:56:21.324Z'
                }, {
                    id: 10,
                    logLevel: 'INFO',
                    message: 'Executing command UPDATE-STATUS...',
                    time: '2016-03-02T14:56:21.324Z'
                }, {
                    id: 11,
                    logLevel: 'DEBUG',
                    message: 'PosDevice is reading pending response messages...',
                    time: '2016-03-02T14:56:21.824Z'
                }, {
                    id: 12,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got response message, raw size = 22, decoded size = 22...',
                    time: '2016-03-02T14:56:21.824Z'
                }, {
                    id: 13,
                    logLevel: 'DEBUG',
                    message: 'PosDevice response messages: 1 messages read in total.',
                    time: '2016-03-02T14:56:21.824Z'
                }, {
                    id: 14,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got text response message: "MagState version 3.03"',
                    time: '2016-03-02T14:56:21.824Z'
                }, {
                    id: 15,
                    logLevel: 'DEBUG',
                    message: 'PosDevice is reading pending response messages...',
                    time: '2016-03-02T14:56:22.324Z'
                }, {
                    id: 16,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got response message, raw size = 103, decoded size = 103...',
                    time: '2016-03-02T14:56:22.324Z'
                }, {
                    id: 17,
                    logLevel: 'DEBUG',
                    message: 'PosDevice response messages: 1 messages read in total.',
                    time: '2016-03-02T14:56:22.324Z'
                }, {
                    id: 18,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got text response message: "MagState version 3.03\n' +
                    'Quantum Magnetometry Laboratory\nUrals State Technical University\nCopyright 2013."',
                    time: '2016-03-02T14:56:22.325Z'
                }, {
                    id: 19,
                    logLevel: 'DEBUG',
                    message: 'PosDevice is reading pending response messages...',
                    time: '2016-03-02T14:56:22.825Z'
                }, {
                    id: 20,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got response message, raw size = 13, decoded size = 9...',
                    time: '2016-03-02T14:56:22.825Z'
                }, {
                    id: 21,
                    logLevel: 'DEBUG',
                    message: 'PosDevice response messages: 1 messages read in total.',
                    time: '2016-03-02T14:56:22.825Z'
                }, {
                    id: 22,
                    logLevel: 'DEBUG',
                    message: 'PosDevice is reading pending response messages...',
                    time: '2016-03-02T14:56:23.325Z'
                }, {
                    id: 23,
                    logLevel: 'DEBUG',
                    message: 'PosDevice got response message, raw size = 5, decoded size = 5...',
                    time: '2016-03-02T14:56:23.325Z'
                }, {
                    id: 24,
                    logLevel: 'DEBUG',
                    message: 'PosDevice response messages: 1 messages read in total.',
                    time: '2016-03-02T14:56:23.325Z'
                }
            ]
        };
    }

    componentDidMount () {
        this._scrollToBottom();
    }

    componentDidUpdate () {
        this._scrollToBottom();
    }

    _scrollToBottom () {
        this._pre.scrollTop = this._pre.scrollHeight - this._pre.clientHeight;
    }

    render () {
        let dataUpdatingLoaderClass = this.state.isDataUpdating ? 'eb-device__data-update-spinner_visible' : '';

        let logEntries = this.state.messages.map(entry => <div key={entry.id}>
            <span className="eb-device__diagnostics-log-list-item__time" title={moment(entry.time).format('LLL')}>
                {moment(entry.time).format('LTS')}</span> [{entry.logLevel}] {entry.message}
        </div>);

        return (
            <div className="panel panel-default">
                <div className="panel-heading">MagState Diagnostics</div>
                <div className="panel-body eb-device__panel-body">
                    <div className="eb-device__loading-panel"
                         style={this.state.initialLoadingComplete ? {display: 'none'} : null}>
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

PosDiagnostics.propTypes = {
};

PosDiagnostics.defaultProps = {
};
