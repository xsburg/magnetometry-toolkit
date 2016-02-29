/**
 * Developer: Stepan Burguchev
 * Date: 5/21/2015
 * Copyright: 2009-2016 Comindware®
 *       All Rights Reserved
 * Published under the MIT license
 */

'use strict';

const webpack = require('webpack');
const path = require('path');
const autoprefixer = require('autoprefixer');
const ExtractTextPlugin = require('extract-text-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const _ = require('underscore');

const pathResolver = {
    client: function () {
        //noinspection Eslint
        return path.resolve.apply(path.resolve, [__dirname, 'client/assets'].concat(_.toArray(arguments)));
    },
    source: function () {
        //noinspection Eslint
        return path.resolve.apply(path.resolve, [__dirname, 'client'].concat(_.toArray(arguments)));
    }
};

module.exports = {
    build: function (options) {
        const DEVELOPMENT = options.env === 'development';
        const PRODUCTION = options.env === 'production';
        const TEST = options.env === 'test';

        const FONT_LIMIT = PRODUCTION ? 10000 : 1000000;
        const GRAPHICS_LIMIT = PRODUCTION ? 10000 : 1000000;

        //noinspection JSUnresolvedFunction
        let webpackConfig = {
            cache: true,
            entry: {
                app: ['./client/main'],
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
                filename: '[name].js',
                sourceMapFilename: '[file].map'
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
                                'transform-es2015-modules-commonjs',
                                'transform-runtime'
                            ]
                        }
                    },
                    (PRODUCTION ? {
                        test: /\.scss$/,
                        loader: ExtractTextPlugin.extract('style-loader', [
                            'css-loader',
                            'postcss-loader',
                            `sass-loader?includePaths[]=${pathResolver.client()}`
                        ].join('!'))
                    } : {
                        test: /\.scss$/,
                        loaders: ['style', 'css?sourceMap', 'postcss?sourceMap', `sass?sourceMap&includePaths[]=${pathResolver.client()}`]
                    }),
                    (PRODUCTION ? {
                        test: /\.css$/,
                        loader: ExtractTextPlugin.extract('style-loader', [
                            'css-loader',
                            'postcss-loader'
                        ].join('!'))
                    } : {
                        test: /\.css$/,
                        loaders: ['style', 'css?sourceMap', 'postcss?sourceMap']
                    }),
                    {
                        test: /\.json$/,
                        loader: 'json'
                    },
                    {
                        test: /\.woff(\?.*)?$/,
                        loader: `url?prefix=fonts/&name=[path][name].[ext]&limit=${FONT_LIMIT}&mimetype=application/font-woff`
                    },
                    {
                        test: /\.woff2(\?.*)?$/,
                        loader: `url?prefix=fonts/&name=[path][name].[ext]&limit=${FONT_LIMIT}&mimetype=application/font-woff2`
                    },
                    {
                        test: /\.otf(\?.*)?$/,
                        loader: `file?prefix=fonts/&name=[path][name].[ext]&limit=${FONT_LIMIT}&mimetype=font/opentype`
                    },
                    {
                        test: /\.ttf(\?.*)?$/,
                        loader: `url?prefix=fonts/&name=[path][name].[ext]&limit=${FONT_LIMIT}&mimetype=application/octet-stream`
                    },
                    {
                        test: /\.eot(\?.*)?$/,
                        loader: 'file?prefix=fonts/&name=[path][name].[ext]'
                    },
                    {
                        test: /\.svg(\?.*)?$/,
                        loader: `url?prefix=fonts/&name=[path][name].[ext]&limit=${GRAPHICS_LIMIT}&mimetype=image/svg+xml`
                    },
                    {
                        test: /\.(png|jpg)$/,
                        loader: `url?limit=${GRAPHICS_LIMIT}`
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
                    hash: PRODUCTION,
                    favicon: pathResolver.source('favicon.ico'),
                    filename: 'index.html',
                    inject: 'body',
                    chunks: ['vendor', 'app'],
                    minify: {
                        collapseWhitespace: false
                    }
                }),
                new webpack.optimize.CommonsChunkPlugin({
                    name: 'vendor',
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
                    bootstrap_css: `${__dirname}/node_modules/bootstrap/dist/css/bootstrap.css`,
                    bootstrap_theme_css: `${__dirname}/node_modules/bootstrap/dist/css/bootstrap-theme.css`,
                    morris_css: `${__dirname}/node_modules/morris.js/morris.css`,
                    font_awesome_css: `${__dirname}/node_modules/font-awesome/css/font-awesome.css`,
                    metismenu_css: `${__dirname}/node_modules/metismenu/dist/metisMenu.css`
                }
            }
        };

        if (PRODUCTION) {
            webpackConfig.cache = false;
            webpackConfig.debug = false;
            webpackConfig.devtool = false;

            let babelLoader = _.findWhere(webpackConfig.module.loaders, { loader: 'babel-loader' });
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
        let babelLoader = _.findWhere(webpackConfig.module.loaders, { loader: 'babel-loader' });
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
