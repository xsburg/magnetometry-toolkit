/**
 * Developer: Stepan Burguchev
 * Date: 2/24/2016
 * Copyright: 2009-2016 Comindware®
 *       All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Comindware
 *       The copyright notice above does not evidence any
 *       actual or intended publication of such source code.
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
        debugger;
        var d = 2;
        var f = 2;
    },

    render: function() {
        return (
            <div className="commentBox loading-panel" onClick={this.onClick}>
                Hello, world! I am a CommentBox.2
            </div>
        );
    }
});
