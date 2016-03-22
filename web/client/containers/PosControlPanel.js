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
import $ from 'jquery';
import _ from 'lodash';
import moment from 'moment';
import dateTimeHelpers from '../utils/dateTimeHelpers';
import { posUpdateFormData, posSendCommand, posLoadStatus } from '../actions';
import LoadingPanel from '../components/LoadingPanel';

const statusUpdateInterval = 1500;

class PosControlPanel extends React.Component {
    static propTypes = {
        initialLoadingComplete: React.PropTypes.bool.isRequired,
        isLoading: React.PropTypes.bool.isRequired,
        hasUnhandledCommands: React.PropTypes.bool.isRequired,
        status: React.PropTypes.object,
        formData: React.PropTypes.object,
        error: React.PropTypes.string,
        posUpdateFormData: React.PropTypes.func.isRequired,
        posSendCommand: React.PropTypes.func.isRequired,
        posLoadStatus: React.PropTypes.func.isRequired
    };

    constructor (props) {
        super(props);
        this.state = {
            showMseedSettings: false
        };
    }

    componentDidMount () {
        this.props.posLoadStatus();
    }

    componentDidUpdate(prevProps) {
        if (!this.props.isLoading && prevProps.isLoading) {
            // Previous loading is complete and we can schedule a new one
            this.updateTimer = setTimeout(() => {
                this.props.posLoadStatus();
                this.updateTimer = null;
            }, statusUpdateInterval);
        }
    }

    componentWillUnmount () {
        if (this.updateTimer) {
            clearInterval(this.updateTimer);
        }
    }

    _createValueLink (key) {
        let keys = key.split('.');
        let value = this.props.formData;
        keys.forEach(k => {
            value = value[k];
        });
        function combineState(keyIndex, newValue) {
            return {
                [keys[keyIndex]]: keyIndex === keys.length - 1 ? newValue : combineState(keyIndex + 1, newValue)
            };
        }
        return {
            value: value,
            requestChange: newValue => {
                let patch = combineState(0, newValue);
                this.props.posUpdateFormData(patch);
            }
        };
    }

    _getSamplingIntervals = () => {
        let range60 = [ 1, 2, 3, 4, 5, 10, 15, 20, 25, 30, 40, 50 ];
        let range24 = [ 1, 2, 3, 4, 5, 6, 12, 24 ];
        let optionMs = function (hz, ms) {
            return {
                name: `${ms}ms (${hz}Hz)`,
                value: ms
            };
        };

        return [
            {
                groupName: '< 1 second',
                options: [
                    optionMs(5, 200),
                    optionMs(4, 250),
                    optionMs(3, 334),
                    optionMs(2, 500)
                ]
            },
            {
                groupName: 'Seconds',
                options: range60.map(sec => ({
                    name: `${sec} second(s)`,
                    value: sec * 1000
                }))
            },
            {
                groupName: 'Minutes',
                options: range60.map(min => ({
                    name: `${min} minute(s)`,
                    value: min * 60 * 1000
                }))
            },
            {
                groupName: 'Hours',
                options: range24.map(hour => ({
                    name: `${hour} hour(s)`,
                    value: hour * 60 * 60 * 1000
                }))
            }
        ];
    };

    _showMseedSettings = () => {
        this.setState({
            showMseedSettings: true
        });
    };

    _applyMseedSettings = () => {
        let props = {
            fileName: this.props.formData.mseedFileName,
            network: this.props.formData.mseedNetwork,
            station: this.props.formData.mseedStation,
            location: this.props.formData.mseedLocation,
            samplesInRecord: this.props.formData.mseedSamplesInRecord
        };
        this.props.posSendCommand({
            command: 'apply-mseed-settings',
            ...props
        });
        this.setState({
            showMseedSettings: false
        });
    };

    _cancelMseedSettings = () => {
        this.setState({
            showMseedSettings: false
        });
    };

    _setRange = () => {
        this.props.posSendCommand({
            command: 'set-device-range',
            range: this.props.formData.fieldRangeCenter
        });
    };

    _fixDeviceTime = () => {
        this.props.posSendCommand({
            command: 'set-device-time',
            time: Math.floor(Number(new Date()) / 1000)
        });
    };

    _toggleStandBy = () => {
        this.props.posSendCommand({
            command: 'set-device-stand-by',
            standBy: !this.props.status.standBy
        });
    };

    _runDiagnostics = () => {
        this.props.posSendCommand({
            command: 'run-diagnostics'
        });
    };

    _runAutoTest = () => {
        this.props.posSendCommand({
            command: 'run-mode-auto-test'
        });
    };

    _onStartLogging = () => {
        this.props.posSendCommand({
            command: 'run',
            intervalMilliseconds: this.props.status.samplingIntervalMs,
            timeFixIntervalSeconds: this.props.status.timeFixIntervalSeconds
        });
    };

    _onStopLogging = () => {
        this.props.posSendCommand({
            command: 'stop'
        });
    };

    _forceUpdate = () => {
        this.props.posSendCommand({
            command: 'update-status'
        });
    };

    _applySelectPicker (el) {
        if (el) {
            $(el).selectpicker();
        }
    }

    render () {
        let loadingPanel = null;
        if (!this.props.initialLoadingComplete) {
            loadingPanel = <LoadingPanel />;
        }

        let content;
        if (this.props.initialLoadingComplete) {
            let dataUpdatingLoaderClass = this.props.isLoading ? 'eb-device__data-update-spinner_visible' : '';

            let intervalOptions = this._getSamplingIntervals().map(group =>
                <optgroup key={group.groupName} label={group.groupName}>{group.options.map(option =>
                    <option key={option.value} value={option.value}>{option.name}</option>)}</optgroup>);

            let timeFixInterval = dateTimeHelpers.durationToReadableString(moment.duration({
                seconds: this.props.status.timeFixIntervalSeconds
            }));

            let about = this.props.status.about;
            about = about.split(/[\r\n]+/g).map((line, i) => <span key={i}>{line}<br /></span>);

            let readonly = this.props.hasUnhandledCommands || this.props.status.commandQueueSize > 0;

            content = (<table className="table" style={this.props.initialLoadingComplete ? null : {display: 'none'}}>
                <caption>
                    <span>The values here are updated in real time </span>
                <span
                    className={`fa fa-circle-o-notch fa-spin eb-device__data-update-spinner ${dataUpdatingLoaderClass}`} />
                </caption>
                <thead>
                <tr>
                    <th>Property Name</th>
                    <th>Property Value</th>
                </tr>
                </thead>
                <tbody>
                <tr>
                    <td>Device Status Updated</td>
                    <td>
                        <span>{moment(this.props.status.updated).format('lll')} </span>
                        <button type="button" className="btn btn-primary btn-xs"
                                disabled={readonly}
                                onClick={this._forceUpdate}>Force device update
                        </button>
                    </td>
                </tr>
                <tr>
                    <td>Commands in queue</td>
                    <td>{this.props.status.commandQueueSize}</td>
                </tr>
                <tr>
                    <td>Registration Status</td>
                    <td>
                        <div style={this.props.status.isRunning ? null : {display: 'none'}}>
                            <span>Running </span>
                            <button type="button" className="btn btn-danger btn-xs"
                                    disabled={readonly}
                                    onClick={this._onStopLogging}>Stop
                            </button>
                        </div>
                        <div style={this.props.status.isRunning ? {display: 'none'} : null}>
                            Stopped
                            <div className="form-group eb-device__start-logging-container">
                                <label htmlFor="samplingIntervalSelect" className="form-label">Sampling interval</label>
                                <div>
                                    <select id="samplingIntervalSelect" className="form-control"
                                            ref={this._applySelectPicker}
                                            disabled={readonly}
                                            valueLink={this._createValueLink('samplingIntervalMs')}>
                                        {intervalOptions}
                                    </select>
                                </div>
                                <label htmlFor="deviceTimeCorrectionSelect" className="form-label">Device time
                                    correction</label>
                                <div>
                                    <select id="deviceTimeCorrectionSelect" className="form-control"
                                            ref={this._applySelectPicker}
                                            disabled={readonly}
                                            valueLink={this._createValueLink('timeFixIntervalSeconds')}>
                                        <option value="0">Never</option>
                                        <option value="10">10 Seconds</option>
                                        <option value="600">10 Minutes</option>
                                        <option value="3600">1 Hour</option>
                                        <option value="21600">6 Hours</option>
                                        <option value="86400">24 Hours</option>
                                        <option value="604800">1 Week</option>
                                    </select>
                                </div>
                                <button type="button" className="btn btn-success btn-sm eb-device__start-logging-btn"
                                        disabled={readonly}
                                        onClick={this._onStartLogging}>Start
                                </button>
                            </div>
                        </div>
                    </td>
                </tr>
                <tr style={this.props.status.isRunning ? null : {display: 'none'}}>
                    <td>Sampling Interval (ms)</td>
                    <td>{this.props.status.samplingIntervalMs}</td>
                </tr>
                <tr style={this.props.status.isRunning ? null : {display: 'none'}}>
                    <td>Device time correction interval</td>
                    <td>{timeFixInterval}</td>
                </tr>
                <tr>
                    <td>Diagnostics</td>
                    <td>
                        <button type="button" className="btn btn-primary btn-xs"
                                disabled={readonly}
                                onClick={this._runDiagnostics}>General
                        </button>
                        <span> </span>
                        <button type="button" className="btn btn-primary btn-xs"
                                disabled={readonly}
                                onClick={this._runAutoTest}>Sampling test
                        </button>
                    </td>
                </tr>
                <tr>
                    <td>MiniSEED settings</td>
                    <td>
                        <button type="button" className="btn btn-primary btn-xs"
                                style={this.state.showMseedSettings ? {display: 'none'} : null}
                                disabled={readonly}
                                onClick={this._showMseedSettings}>Edit
                        </button>
                        <div style={this.state.showMseedSettings ? null : {display: 'none'}}>
                            <label htmlFor="mseedSettingsFileName" className="form-label">File Name</label>
                            <input id="mseedSettingsFileName" type="text" className="form-control"
                                   disabled={readonly}
                                   valueLink={this._createValueLink('mseedFileName')} placeholder="data.mseed" />
                            <label htmlFor="mseedSettingsNetwork" className="form-label">Network</label>
                            <input id="mseedSettingsNetwork" type="text" className="form-control"
                                   disabled={readonly}
                                   valueLink={this._createValueLink('mseedNetwork')} placeholder="RU" />
                            <label htmlFor="mseedSettingsStation" className="form-label">Station</label>
                            <input id="mseedSettingsStation" type="text" className="form-control"
                                   disabled={readonly}
                                   valueLink={this._createValueLink('mseedStation')} placeholder="IFZ" />
                            <label htmlFor="mseedSettingsLocation" className="form-label">Location</label>
                            <input id="mseedSettingsLocation" type="text" className="form-control"
                                   disabled={readonly}
                                   valueLink={this._createValueLink('mseedLocation')} placeholder="SK" />
                            <label htmlFor="mseedSettingsSamplesInRecord" className="form-label">Samples in MiniSEED
                                record</label>
                            <input id="mseedSettingsSamplesInRecord" type="number" className="form-control"
                                   disabled={readonly}
                                   valueLink={this._createValueLink('mseedSamplesInRecord')} placeholder="2" />
                            <div className="eb-device__mseed-settings-buttons">
                                <button type="button" className="btn btn-primary btn-sm"
                                        disabled={readonly}
                                        onClick={this._applyMseedSettings}>Apply
                                </button>
                                <button type="button" className="btn btn-link btn-sm"
                                        disabled={readonly}
                                        onClick={this._cancelMseedSettings}>Cancel
                                </button>
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td>Stand By</td>
                    <td>
                        <span>{this.props.status.standBy ? 'On ' : 'Off '}</span>
                        <button type="button" className="btn btn-primary btn-xs"
                                disabled={readonly}
                                onClick={this._toggleStandBy}>{this.props.status.standBy ? 'Turn off ' : 'Turn on '}</button>
                    </td>
                </tr>
                <tr>
                    <td>Device Time</td>
                    <td>
                        <span>{moment(this.props.status.time).format('lll')} </span>
                        <button type="button" className="btn btn-primary btn-xs"
                                onClick={this._fixDeviceTime}
                                disabled={readonly}>Fix
                        </button>
                    </td>
                </tr>
                <tr>
                    <td>Range (pT)</td>
                    <td>
                        [ {this.props.status.range.minField}, {this.props.status.range.maxField} ]
                        <div className="form-group eb-device__update-range-container">
                            <input id="newCenterRange" type="number" min="1" max="10000000"
                                   className="form-control"
                                   valueLink={this._createValueLink('fieldRangeCenter')}
                                   disabled={readonly}
                                   placeholder="Center range (pT)" />
                            <button type="button" className="btn btn-primary btn-sm eb-device__update-range-btn"
                                    onClick={this._setRange}
                                    disabled={readonly}>Apply
                            </button>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td>ENQ</td>
                    <td>{this.props.status.enq}</td>
                </tr>
                <tr>
                    <td>About</td>
                    <td>{about}</td>
                </tr>
                </tbody>
            </table>);
        } else if (this.props.error) {
            content = <div className="pos__control-panel__error">{this.props.error}</div>;
        } else {
            content = null;
        }

        return (
            <div className="panel panel-default">
                <div className="panel-heading">MagState Control Panel</div>
                <div className="panel-body eb-device__panel-body">
                    {loadingPanel}
                    {content}
                </div>
            </div>
        );
    }
}

function mapStateToProps(state) {
    return state.pos.config;
}

export default connect(mapStateToProps, {
    posUpdateFormData,
    posSendCommand,
    posLoadStatus
})(PosControlPanel);
