/**
 * Developer: Stepan Burguchev
 * Date: 5/21/2015
 * Copyright: 2009-2016 Comindware®
 *       All Rights Reserved
 * Published under the MIT license
 */

"use strict";

var webpack = require('webpack');
var path = require('path');
var autoprefixer = require('autoprefixer');
var ExtractTextPlugin = require('extract-text-webpack-plugin');
var HtmlWebpackPlugin = require('html-webpack-plugin');
var _ = require('underscore');

const pathResolver = {
    client: function () {
        return path.resolve.apply(path.resolve, [__dirname, 'public/assets'].concat(_.toArray(arguments)));
    },
    source: function () {
        return path.resolve.apply(path.resolve, [__dirname, 'public'].concat(_.toArray(arguments)));
    }
};

module.exports = {
    build: function (options) {
        const DEVELOPMENT = options.env === 'development';
        const PRODUCTION = options.env === 'production';
        const TEST = options.env === 'test';

        //noinspection JSUnresolvedFunction
        var webpackConfig = {
            cache: true,
            entry: {
                app: [ './public/main' ],
                vendor: [
                    'react',
                    'react-redux',
                    'react-router',
                    'react-router-redux',
                    'redux'
                ]
            },
            devtool: 'source-map',
            debug: true,
            output: {
                path: pathResolver.client(),
                publicPath: '/',
                filename: "[name].js",
                sourceMapFilename: "[file].map"
            },
            module: {
                loaders: [
                    {
                        test: /\.jsx?$/,
                        exclude: /(node_modules|bower_components)/,
                        loader: 'babel-loader',
                        query: {
                            cacheDirectory: true,
                            plugins: [
                                "transform-es2015-modules-commonjs",
                                "transform-runtime"
                            ]
                        }
                    },
                    (PRODUCTION ? {
                        test: /\.scss$/,
                        loader: ExtractTextPlugin.extract('style-loader', [
                            'css-loader',
                            'postcss-loader',
                            'sass-loader?includePaths[]=' + pathResolver.client()
                        ].join('!'))
                    } : {
                        test: /\.scss$/,
                        loaders: ['style', 'css?sourceMap', 'postcss?sourceMap', 'sass?sourceMap&includePaths[]=' + pathResolver.client()]
                    }),
                    {
                        test: /\.json$/,
                        loader: 'json'
                    },
                    {
                        test: /\.woff(\?.*)?$/,
                        loader: 'url?prefix=fonts/&name=[path][name].[ext]&limit=10000&mimetype=application/font-woff'
                    },
                    {
                        test: /\.woff2(\?.*)?$/,
                        loader: 'url?prefix=fonts/&name=[path][name].[ext]&limit=10000&mimetype=application/font-woff2'
                    },
                    {
                        test: /\.otf(\?.*)?$/,
                        loader: 'file?prefix=fonts/&name=[path][name].[ext]&limit=10000&mimetype=font/opentype'
                    },
                    {
                        test: /\.ttf(\?.*)?$/,
                        loader: 'url?prefix=fonts/&name=[path][name].[ext]&limit=10000&mimetype=application/octet-stream'
                    },
                    {test: /\.eot(\?.*)?$/, loader: 'file?prefix=fonts/&name=[path][name].[ext]'},
                    {
                        test: /\.svg(\?.*)?$/,
                        loader: 'url?prefix=fonts/&name=[path][name].[ext]&limit=10000&mimetype=image/svg+xml'
                    },
                    {
                        test: /\.(png|jpg)$/,
                        loader: 'url?limit=8192'
                    },
                    {
                        test: /bluebird/,
                        loader: 'expose?Promise'
                    },
                    {
                        test: /underscore\.js/,
                        loader: 'expose?_'
                    }
                ]
            },
            plugins: [
                new HtmlWebpackPlugin({
                    template: pathResolver.source('index.html'),
                    hash: false,
                    favicon: pathResolver.source('favicon.ico'),
                    filename: 'index.html',
                    inject: 'body',
                    chunks: ['vendor','app'],
                    minify: {
                        collapseWhitespace: false
                    }
                }),
                new webpack.optimize.CommonsChunkPlugin({
                    name: "vendor",
                    minChunks: Infinity
                })
            ],
            postcss: [
                autoprefixer({
                    browsers: ['last 2 versions']
                })
            ],
            resolve: {
                root: [
                    pathResolver.source()
                ],
                alias: {
                }
            }
        };

        if (PRODUCTION) {
            webpackConfig.cache = false;
            webpackConfig.debug = false;
            webpackConfig.devtool = false;
            webpackConfig.output.filename = "[name].[hash].js";

            var babelLoader = _.findWhere(webpackConfig.module.loaders, { loader: 'babel-loader' });
            babelLoader.query.plugins.push(
                'transform-react-remove-prop-types',
                'transform-react-constant-elements'
            );

            //noinspection JSUnresolvedFunction
            webpackConfig.plugins.push(
                new webpack.DefinePlugin({
                    'process.env.NODE_ENV': PRODUCTION ? '"production"' : '"development"',
                    __DEV__: DEVELOPMENT
                }),
                new ExtractTextPlugin('[name].css'),
                new webpack.optimize.OccurrenceOrderPlugin(),
                new webpack.optimize.DedupePlugin(),
                new webpack.optimize.UglifyJsPlugin({
                    compress: {
                        unused: true,
                        dead_code: true,
                        warnings: false
                    }
                })
            );
        }

        if (options.hmr) {
            this.applyHmr(webpackConfig);
        }

        return webpackConfig;
    },

    applyHmr: function (webpackConfig) {
        // Entry points
        _.each(webpackConfig.entry, function (v) {
            v.push('webpack-hot-middleware/client');
        });
        // React transform
        var babelLoader = _.findWhere(webpackConfig.module.loaders, { loader: 'babel-loader' });
        babelLoader.query.plugins.push([
            'react-transform', {
                transforms: [
                    {
                        transform: 'react-transform-hmr',
                        imports: ['react'],
                        locals: ['module']
                    }, {
                        transform: 'react-transform-catch-errors',
                        imports: ['react', 'redbox-react']
                    }
                ]
            }
        ]);
        // Plugins
        //noinspection JSUnresolvedFunction
        webpackConfig.plugins.push(
            new webpack.HotModuleReplacementPlugin(),
            new webpack.NoErrorsPlugin()
        );
        return webpackConfig;
    }
};
