/**
 * Developer: Stepan Burguchev
 * Date: 3/10/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import React, { Component, PropTypes } from 'react';
import { connect } from 'react-redux';
import { resetErrorMessage } from '../actions';
import NavBar from '../components/NavBar';
import SideBar from '../components/SideBar';
import 'bootstrap';
import 'bootstrap-select';

import 'morris_css';
import 'bootstrap_css';
import 'bootstrap_select_css';
import 'font_awesome_css';
import 'metismenu_css';
import '../styles/index.scss';

class App extends Component {
    static propTypes = {
        // Injected by React Redux
        errorMessage: PropTypes.string,
        resetErrorMessage: PropTypes.func.isRequired,
        // Injected by React Router
        children: PropTypes.node
    };

    constructor(props) {
        super(props);
    }

    handleDismissClick = (e) => {
        this.props.resetErrorMessage();
        e.preventDefault();
    };

    renderErrorMessage() {
        const { errorMessage } = this.props;
        if (!errorMessage) {
            return null;
        }

        return (
            <p style={{ backgroundColor: '#e99', padding: 10 }}>
                <b>{errorMessage}</b>
                {' '}
                (<a href="#"
                    onClick={this.handleDismissClick}>
                    Dismiss
                </a>)
            </p>
        );
    }

    render() {
        const { children } = this.props;
        return (
            <div onClick={this.onClick}>
                <NavBar />
                <SideBar />
                <div className="app-content">
                    {this.renderErrorMessage()}
                    {children}
                </div>
                {
                    /*this.state.loaded ? '' : <div className="js-startup-loading app-startup-loading-panel">
                        <div className="app-startup-loading-panel__fill"></div>
                        <div className="app-startup-loading-panel__text">
                            <div className="fa fa-cog fa-spin fa-2x"></div>
                        </div>
                    </div>*/
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

function mapStateToProps(state, ownProps) {
    return {
        errorMessage: state.errorMessage
    };
}

export default connect(mapStateToProps, {
    resetErrorMessage
})(App);
