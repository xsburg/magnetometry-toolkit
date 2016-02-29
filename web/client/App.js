/**
 * Developer: Stepan Burguchev
 * Date: 7/4/2014
 * Copyright: 2009-2013 Stepan Burguchev®
 *       All Rights Reserved
 */

import React from 'react';
import _ from 'underscore';
import NavBar from './components/NavBar';
import SideBar from './components/SideBar';

import 'morris_css';
import 'bootstrap_css';
import 'bootstrap_theme_css';
import 'font_awesome_css';
import 'metismenu_css';
import './styles/index.scss';

export default class App extends React.Component {
    constructor() {
        super();
        this.state = {
            loaded: false
        };
        _.bindAll(this, 'onClick');
    }

    onClick() {
        this.setState({
            loaded: true
        });
    }

    render() {
        return (
            <div onClick={this.onClick}>
                <NavBar />
                <SideBar />
                <div className="js-application-content-region app-content"></div>
                {
                    this.state.loaded ? '' : <div className="js-startup-loading app-startup-loading-panel">
                        <div className="app-startup-loading-panel__fill"></div>
                        <div className="app-startup-loading-panel__text">
                            <div className="fa fa-cog fa-spin fa-2x"></div>
                        </div>
                    </div>
                }
                {/* bootstrap modal */}
                <div className="modal fade js-fatal-error-modal" tabIndex="-1" role="dialog" aria-labelledby="myModalLabel">
                    <div className="modal-dialog" role="document">
                        <div className="modal-content">
                            <div className="modal-header">
                                <h4 className="modal-title" id="myModalLabel">Fatal error</h4>
                            </div>
                            <div className="modal-body">
                                Something bad happened. Please reload the page.
                            </div>
                            <div className="modal-footer">
                                <button type="button" className="btn btn-default btn-primary" data-dismiss="modal">Reload</button>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}
