/**
 * Developer: Stepan Burguchev
 * Date: 3/10/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

import React, { Component, PropTypes } from 'react';
import { connect } from 'react-redux';
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
        // Injected by React Router
        children: PropTypes.node
    };

    constructor(props) {
        super(props);
    }

    render() {
        const { children } = this.props;
        return (
            <div>
                <NavBar />
                <SideBar />
                <div className="app-content">
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
})(App);
