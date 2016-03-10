/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';
import $ from 'jquery';
import moment from 'moment';
import dateTimeHelpers from '../utils/dateTimeHelpers';

function createValueLink(component, key) {
    let keys = key.split('.');
    let value = component.state;
    function combineState(obj, keyIndex, newValue) {
        return {
            ...obj,
            [keys[keyIndex]]: keyIndex === keys.length - 1 ? newValue : combineState(obj[keys[keyIndex]], keyIndex + 1, newValue)
        };
    }
    keys.forEach(k => {
        value = value[k];
    });

    return {
        value: value,
        requestChange: function(newValue) {
            component.setState(combineState(component.state, 0, newValue));
        }
    };
}

export default class PosControlPanel extends React.Component {
    static defaultProps = {
    };

    static propTypes = {
        initialCount: React.PropTypes.number
    };

    constructor (props) {
        super(props);
        this.state = {
            initialLoadingComplete: true,
            enabled: true,
            isDataUpdating: false,
            showMseedSettings: false,
            data: {
                about: 'MagState version 3.03\nQuantum Magnetometry Laboratory\nUrals State Technical University\nCopyright 2013.',
                commandQueueSize: 0,
                enq: 'MagState version 3.03',
                isRunning: false,
                mseedSettings: {
                    fileName: 'data.mseed',
                    location: 'SK',
                    network: 'RU',
                    samplesInRecord: 2,
                    station: 'IFZ'
                },
                range: {
                    maxField: 50400,
                    minField: 48100,
                    center: null
                },
                samplingIntervalMs: 1000,
                standBy: false,
                time: '1997-02-03T23:40:03.000Z',
                timeFixIntervalSeconds: 0,
                timeUpdated: '2016-03-02T14:56:23.325Z',
                updated: '2016-03-02T14:56:23.325Z'
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

    };

    _applyMseedSettings = () => {

    };

    _cancelMseedSettings = () => {

    };

    _setRange = () => {

    };

    _fixDeviceTime = () => {

    };

    _toggleStandBy = () => {

    };

    _runDiagnostics = () => {

    };

    _runAutoTest = () => {

    };

    _onStartLogging = () => {

    };

    _onStopLogging = () => {

    };

    _forceUpdate = () => {

    };

    _applySelectPicker (el) {
        if (el) {
            $(el).selectpicker();
        }
    }

    render () {
        let intervalOptions = this._getSamplingIntervals().map(group =>
            <optgroup key={group.groupName} label={group.groupName}>{group.options.map(option =>
                <option key={option.value} value={option.value}>{option.name}</option>)}</optgroup>);

        let timeFixInterval = dateTimeHelpers.durationToReadableString(moment.duration({
            seconds: this.state.data.timeFixIntervalSeconds
        }));

        let dataUpdatingLoaderClass = this.state.isDataUpdating ? 'eb-device__data-update-spinner_visible' : '';

        let about = this.state.data.about;
        about = about.split(/[\r\n]+/g).map((line, i) => <span key={i}>{line}<br /></span>);

        return (
            <div className="panel panel-default">
                <div className="panel-heading">MagState Control Panel</div>
                <div className="panel-body eb-device__panel-body">
                    <div className="eb-device__loading-panel"
                         style={this.state.initialLoadingComplete ? {display: 'none'} : null}>
                        <div className="eb-device__loading-panel__fill"></div>
                        <div className="eb-device__loading-panel__text">
                            <div className="fa fa-cog fa-spin fa-2x"></div>
                        </div>
                    </div>
                    <table className="table" style={this.state.initialLoadingComplete ? null : {display: 'none'}}>
                        <caption>
                            <span>The values here are updated in real time </span>
                            <span className={`fa fa-circle-o-notch fa-spin eb-device__data-update-spinner ${dataUpdatingLoaderClass}`} />
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
                                <span>{moment(this.state.data.updated).format('lll')} </span>
                                <button type="button" className="btn btn-primary btn-xs"
                                        disabled={!this.state.enabled}
                                        onClick={this._forceUpdate}>Force device update</button>
                            </td>
                        </tr>
                        <tr>
                            <td>Commands in queue</td>
                            <td>{this.state.data.commandQueueSize}</td>
                        </tr>
                        <tr>
                            <td>Registration Status</td>
                            <td>
                                <div style={this.state.data.isRunning ? null : {display: 'none'}}>
                                    <span>Running </span>
                                    <button type="button" className="btn btn-danger btn-xs"
                                            disabled={!this.state.enabled}
                                            onClick={this._onStopLogging}>Stop</button>
                                </div>
                                <div style={this.state.data.isRunning ? {display: 'none'} : null}>
                                    Stopped
                                    <div className="form-group eb-device__start-logging-container">
                                        <label htmlFor="samplingIntervalSelect" className="form-label">Sampling interval</label>
                                        <div>
                                            <select id="samplingIntervalSelect" className="form-control"
                                                    ref={this._applySelectPicker}
                                                    disabled={!this.state.enabled}
                                                    valueLink={createValueLink(this, 'data.samplingIntervalMs')}>
                                                {intervalOptions}
                                            </select>
                                        </div>
                                        <label htmlFor="deviceTimeCorrectionSelect" className="form-label">Device time correction</label>
                                        <div>
                                            <select id="deviceTimeCorrectionSelect" className="form-control"
                                                    ref={this._applySelectPicker}
                                                    disabled={!this.state.enabled}
                                                    valueLink={createValueLink(this, 'data.timeFixIntervalSeconds')}>
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
                                                disabled={!this.state.enabled}
                                                onClick={this._onStartLogging}>Start</button>
                                    </div>
                                </div>
                            </td>
                        </tr>
                        <tr style={this.state.data.isRunning ? null : {display: 'none'}}>
                            <td>Sampling Interval (ms)</td>
                            <td>{this.state.data.samplingIntervalMs}</td>
                        </tr>
                        <tr style={this.state.data.isRunning ? null : {display: 'none'}}>
                            <td>Device time correction interval</td>
                            <td>{timeFixInterval}</td>
                        </tr>
                        <tr>
                            <td>Diagnostics</td>
                            <td>
                                <button type="button" className="btn btn-primary btn-xs"
                                        disabled={!this.state.enabled}
                                        onClick={this._runDiagnostics}>General</button>
                                <span> </span>
                                <button type="button" className="btn btn-primary btn-xs"
                                        disabled={!this.state.enabled}
                                        onClick={this._runAutoTest}>Sampling test</button>
                            </td>
                        </tr>
                        <tr>
                            <td>MiniSEED settings</td>
                            <td>
                                <button type="button" className="btn btn-primary btn-xs"
                                        style={this.state.showMseedSettings ? null : {display: 'none'}}
                                        disabled={!this.state.enabled}
                                        onClick={this._showMseedSettings}>Edit</button>
                                <div style={this.state.showMseedSettings ? { display: 'none' } : null}>
                                    <label htmlFor="mseedSettingsFileName" className="form-label">File Name</label>
                                    <input id="mseedSettingsFileName" type="text" className="form-control"
                                           disabled={!this.state.enabled}
                                           valueLink={createValueLink(this, 'data.mseedSettings.fileName')} placeholder="data.mseed" />
                                    <label htmlFor="mseedSettingsNetwork" className="form-label">Network</label>
                                    <input id="mseedSettingsNetwork" type="text" className="form-control"
                                           disabled={!this.state.enabled}
                                           valueLink={createValueLink(this, 'data.mseedSettings.network')} placeholder="RU" />
                                    <label htmlFor="mseedSettingsStation" className="form-label">Station</label>
                                    <input id="mseedSettingsStation" type="text" className="form-control"
                                           disabled={!this.state.enabled}
                                           valueLink={createValueLink(this, 'data.mseedSettings.station')} placeholder="IFZ" />
                                    <label htmlFor="mseedSettingsLocation" className="form-label">location</label>
                                    <input id="mseedSettingsLocation" type="text" className="form-control"
                                           disabled={!this.state.enabled}
                                           valueLink={createValueLink(this, 'data.mseedSettings.location')} placeholder="SK" />
                                    <label htmlFor="mseedSettingsSamplesInRecord" className="form-label">Samples in MiniSEED record</label>
                                    <input id="mseedSettingsSamplesInRecord" type="number" className="form-control"
                                           disabled={!this.state.enabled}
                                           valueLink={createValueLink(this, 'data.mseedSettings.samplesInRecord')} placeholder="2" />
                                    <div className="eb-device__mseed-settings-buttons">
                                        <button type="button" className="btn btn-primary btn-sm"
                                                disabled={!this.state.enabled}
                                                onClick={this._applyMseedSettings}>Apply</button>
                                        <button type="button" className="btn btn-link btn-sm"
                                                disabled={!this.state.enabled}
                                                onClick={this._cancelMseedSettings}>Cancel</button>
                                    </div>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>Stand By</td>
                            <td>
                                <span>{this.state.data.standBy ? 'On ' : 'Off '}</span>
                                <button type="button" className="btn btn-primary btn-xs"
                                        disabled={!this.state.enabled}
                                        onClick={this._toggleStandBy}>{this.state.data.standBy ? 'Turn off ' : 'Turn on '}</button>
                            </td>
                        </tr>
                        <tr>
                            <td>Device Time</td>
                            <td>
                                <span>{moment(this.state.data.time).format('lll')} </span>
                                <button type="button" className="btn btn-primary btn-xs"
                                        onClick={this._fixDeviceTime}
                                        disabled={!this.state.enabled}>Fix</button>
                            </td>
                        </tr>
                        <tr>
                            <td>Range (pT)</td>
                            <td>
                                [ {this.state.data.range.minField}, {this.state.data.range.maxField} ]
                                <div className="form-group eb-device__update-range-container">
                                    <input id="newCenterRange" type="number" min="1" max="10000000"
                                           className="form-control"
                                           valueLink={createValueLink(this, 'data.range.center')}
                                           disabled={!this.state.enabled}
                                           placeholder="Center range (pT)" />
                                    <button type="button" className="btn btn-primary btn-sm eb-device__update-range-btn"
                                            onClick={this._setRange}
                                            disabled={!this.state.enabled}>Apply</button>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>ENQ</td>
                            <td>{this.state.data.enq}</td>
                        </tr>
                        <tr>
                            <td>About</td>
                            <td>{about}</td>
                        </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        );
    }
}
