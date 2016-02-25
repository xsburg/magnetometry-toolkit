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

require("babel-register");
var runTask = require("./runTask");

if (process.argv.length !== 2) {
    console.error('Invalid arguments');
    return;
}

const task = require(`./${process.argv[2]}.js`).default;
runTask(task).catch(err => console.error(err.stack));
