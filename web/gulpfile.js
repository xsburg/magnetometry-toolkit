/**
 * Developer: Stepan Burguchev
 * Date: 2/2/2016
 * Copyright: 2009-2016 Comindware®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

const gulp = require('gulp');
const gutil = require('gulp-util');
const path = require('path');
const webpack = require('webpack');
const webpackConfigFactory = require('./webpack.config.js');
const browserSync = require('browser-sync').create();
const webpackDevMiddleware = require('webpack-dev-middleware');
const webpackHotMiddleware = require('webpack-hot-middleware');
const startServer = require('./startServer');
const del = require('del');
const mkdirp = require('mkdirp');

//noinspection JSUnresolvedVariable
const config = {
    hmr: process.env.HMR || process.env.HMR === undefined,
    server: 'http://localhost:3000',
    serverPath: path.resolve(`${__dirname}/server/bin/www`),
    proxyPort: 8080,
    assetsDir: path.resolve(`${__dirname}/client/assets`)
};

gulp.task('clear', function () {
    del.sync([`${config.assetsDir}/**`]);
    mkdirp.sync(config.assetsDir);
});

gulp.task('start', function () {
    let webpackConfig = webpackConfigFactory.build({
        env: 'development',
        hmr: config.hmr
    });
    let compiler = webpack(webpackConfig);

    const middleware = [];
    middleware.push(webpackDevMiddleware(compiler, {
        noInfo: true,
        publicPath: webpackConfig.output.publicPath,
        stats: {
            colors: true
        }
    }));
    if (config.hmr) {
        middleware.push(webpackHotMiddleware(compiler));
    }

    startServer('node', [config.serverPath], function () {
        browserSync.init({
            proxy: {
                target: config.server,
                middleware: middleware
            },
            port: config.proxyPort,
            open: false
        });
    });
});

gulp.task('deploy', ['clear'], function (callback) {
    const webpackConfig = webpackConfigFactory.build({
        env: 'production',
        hmr: false
    });
    webpack(webpackConfig, function (err, stats) {
        if (err) {
            throw new gutil.PluginError('deploy', err);
        }
        gutil.log('[deploy]', stats.toString({
            colors: true
        }));
        callback();
    });
});

gulp.task('default', ['start']);
