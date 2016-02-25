/**
 * Developer: Stepan Burguchev
 * Date: 2/2/2016
 * Copyright: 2009-2016 Comindware®
 *       All Rights Reserved
 * Published under the MIT license
 */

"use strict";

const gulp = require('gulp');
const gutil = require('gulp-util');
const webpack = require('webpack');
const webpackConfigFactory = require('./webpack.config.js');
const browserSync = require('browser-sync').create();
const webpackDevMiddleware = require('webpack-dev-middleware');
const webpackHotMiddleware = require('webpack-hot-middleware');

//noinspection JSUnresolvedVariable
const config = {
    hmr: process.env.HMR || process.env.HMR === undefined,
    server: 'http://localhost:3000',
    proxyPort: 8080
};

gulp.task("start", function () {
    var webpackConfig = webpackConfigFactory.build({
        env: 'development',
        hmr: config.hmr
    });
    var compiler = webpack(webpackConfig);

    let middleware = [];
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

    browserSync.init({
        proxy: {
            target: config.server,
            middleware: middleware,
            port: config.proxyPort
        }
    });
});

gulp.task('deploy', function (callback) {
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
