/**
 * Developer: Stepan Burguchev
 * Date: 7/4/2014
 * Copyright: 2009-2013 Stepan Burguchev®
 *       All Rights Reserved
 */

import React from 'react';
import ReactDOM from 'react-dom';

export default React.createClass({
    foo: function () {

    },

    onClick: function () {
        var b = arguments;
        var a = 1;
        var e = 2;
        var c = 2;
        var d = 2;
        var f = 2;
        debugger;
    },

    render: function() {
        return (
            <div className="commentBox" onClick={this.onClick}>
                Hello, world! I am a CommentBox.2
            </div>
        );
    }
});
