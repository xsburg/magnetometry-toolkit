/**
 * Developer: Stepan Burguchev
 * Date: 7/4/2014
 * Copyright: 2009-2013 Stepan Burguchev®
 *       All Rights Reserved
 */

import React from 'react';
import AppBar from 'material-ui/lib/app-bar';
import RaisedButton from 'material-ui/lib/raised-button';
import MenuItem from 'material-ui/lib/menu/menu-item';
import LeftNav from 'material-ui/lib/left-nav';

export default class App {
    render() {
        return (
            <div>
                <AppBar title="My AppBar" />
                <LeftNav open={false}>
                    <AppBar title="My AppBar" />
                    <MenuItem>Menu Item</MenuItem>
                    <MenuItem>Menu Item 2</MenuItem>
                </LeftNav>
                <RaisedButton label="My Button" primary={true} />
            </div>
        );
    }
}
