/**
 * Developer: Stepan Burguchev
 * Date: 2/29/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

import React from 'react';
import _ from 'underscore';

export default class PosControlPanel extends React.Component {
    static defaultProps = {
    };

    static propTypes = {
        initialCount: React.PropTypes.number
    };

    constructor (props) {
        super(props);
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

        return {
            samplingIntervalOptions: [
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
            ]
        };
    };

    render () {
        let intervalOptions = this._getSamplingIntervals().map(group =>
            <optgroup label={group.groupName}>{group.options.map(option =>
                <option value={option.value}>{option.name}</option>)}</optgroup>);

        return (
            <div className="panel panel-default">
                <div className="panel-heading">MagState Control Panel</div>
                <div className="panel-body eb-device__panel-body">
                    <div className="eb-device__loading-panel js-loading-panel">
                        <div className="eb-device__loading-panel__fill"></div>
                        <div className="eb-device__loading-panel__text">
                            <div className="fa fa-cog fa-spin fa-2x"></div>
                        </div>
                    </div>
                    <table className="table js-data-table">
                        <caption>The values here are updated in real time <span className="fa fa-circle-o-notch fa-spin js-data-update-spinner eb-device__data-update-spinner"></span></caption>
                        <thead>
                        <tr>
                            <th>Property Name</th>
                            <th>Property Value</th>
                        </tr>
                        </thead>
                        <tbody>
                        <tr>
                            <td>Device Status Updated</td>
                            <td><span className="js-status-updated"></span> <button type="button" className="btn btn-primary btn-xs js-force-update-button">Force device update</button></td>
                        </tr>
                        <tr>
                            <td>Commands in queue</td>
                            <td><span className="js-status-command-queue"></span></td>
                        </tr>
                        <tr>
                            <td>Registration Status</td>
                            <td>
                                <div className="js-cond-stopped">Running <button type="button" className="btn btn-danger btn-xs js-stop-logging-button">Stop</button></div>
                                <div className="js-cond-running">
                                    Stopped
                                    <div className="form-group eb-device__start-logging-container">
                                        <label htmlFor="samplingIntervalSelect" className="form-label">Sampling interval</label>
                                        <div>
                                            <select id="samplingIntervalSelect" className="form-control js-status-sampling-interval-input">
                                                {intervalOptions}
                                            </select>
                                        </div>
                                        <label htmlFor="deviceTimeCorrectionSelect" className="form-label">Device time correction</label>
                                        <div>
                                            <select id="deviceTimeCorrectionSelect" className="form-control js-time-fix-interval-input">
                                                <option value="0">Never</option>
                                                <option value="10">10 Seconds</option>
                                                <option value="600">10 Minutes</option>
                                                <option value="3600">1 Hour</option>
                                                <option value="21600">6 Hours</option>
                                                <option value="86400">24 Hours</option>
                                                <option value="604800">1 Week</option>
                                            </select>
                                        </div>
                                        <button type="button" className="btn btn-success btn-sm eb-device__start-logging-btn js-start-logging-button">Start</button>
                                    </div>
                                </div>
                            </td>
                        </tr>
                        <tr className="js-cond-sampling-interval-row">
                            <td>Sampling Interval (ms)</td>
                            <td className="js-status-sampling-interval"></td>
                        </tr>
                        <tr className="js-cond-sampling-interval-row">
                            <td>Device time correction interval</td>
                            <td className="js-status-time-fix-interval"></td>
                        </tr>
                        <tr>
                            <td>Diagnostics</td>
                            <td>
                                <button type="button" className="btn btn-primary btn-xs js-run-diagnostics-button">General</button>
                                <button type="button" className="btn btn-primary btn-xs js-run-auto-test-button">Sampling test</button>
                            </td>
                        </tr>
                        <tr>
                            <td>MiniSEED settings</td>
                            <td>
                                <button type="button" className="btn btn-primary btn-xs js-edit-mseed-settings-button">Edit</button>
                                <div className="js-mseed-settings-panel">
                                    <label htmlFor="mseedSettingsFileName" className="form-label">File Name</label>
                                    <input id="mseedSettingsFileName" type="text" className="form-control js-mseed-settings-fileName-input" placeholder="data.mseed" />
                                    <label htmlFor="mseedSettingsNetwork" className="form-label">Network</label>
                                    <input id="mseedSettingsNetwork" type="text" className="form-control js-mseed-settings-network-input" placeholder="RU" />
                                    <label htmlFor="mseedSettingsStation" className="form-label">Station</label>
                                    <input id="mseedSettingsStation" type="text" className="form-control js-mseed-settings-station-input" placeholder="IFZ" />
                                    <label htmlFor="mseedSettingsLocation" className="form-label">location</label>
                                    <input id="mseedSettingsLocation" type="text" className="form-control js-mseed-settings-location-input" placeholder="SK" />
                                    <label htmlFor="mseedSettingsSamplesInRecord" className="form-label">Samples in MiniSEED record</label>
                                    <input id="mseedSettingsSamplesInRecord" type="number" className="form-control js-mseed-settings-samplesInRecord-input" placeholder="2" />
                                    <div className="eb-device__mseed-settings-buttons">
                                        <button type="button" className="btn btn-primary btn-sm js-apply-mseed-settings-button">Apply</button>
                                        <button type="button" className="btn btn-link btn-sm js-cancel-mseed-settings-button">Cancel</button>
                                    </div>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>Stand By</td>
                            <td><span className="js-status-stand-by"></span> <button type="button" className="btn btn-primary btn-xs js-status-stand-by-button"></button></td>
                        </tr>
                        <tr>
                            <td>Device Time</td>
                            <td>
                                <span className="js-status-device-time"></span> <button type="button" className="btn btn-primary btn-xs js-fix-device-time-button">Fix</button>
                            </td>
                        </tr>
                        <tr>
                            <td>Range (pT)</td>
                            <td>
                                [ <span className="js-status-range-min"></span>, <span className="js-status-range-max"></span> ]
                                <div className="form-group eb-device__update-range-container">
                                    <input id="newCenterRange" type="number" min="1" max="10000000" className="form-control js-status-center-range-input" placeholder="Center range (pT)" />
                                    <button type="button" className="btn btn-primary btn-sm eb-device__update-range-btn js-set-range-button">Apply</button>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>ENQ</td>
                            <td className="js-status-enq"></td>
                        </tr>
                        <tr>
                            <td>About</td>
                            <td className="js-status-about"></td>
                        </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        );
    }
}
