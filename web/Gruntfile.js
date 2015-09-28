/* global require, define, module, __dirname */

(function ()
{
    'use strict';

    module.exports = function (grunt) {
        var _ = require('underscore');
        var autoprefixer = require('autoprefixer');
        var cssnano = require('cssnano');

        grunt.loadNpmTasks('grunt-bower-task');
        grunt.loadNpmTasks('grunt-contrib-sass');
        grunt.loadNpmTasks('grunt-contrib-watch');
        grunt.loadNpmTasks('grunt-contrib-requirejs');
        grunt.loadNpmTasks('grunt-contrib-cssmin');
        grunt.loadNpmTasks('grunt-shell');
        grunt.loadNpmTasks('grunt-postcss');

        // SASS requirements:
        // http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-2.2.2.exe
        // gem install sass

        //noinspection JSUnresolvedFunction
        grunt.initConfig({
            bower: {
                install: {
                    options: {
                        targetDir: './public/js/lib',
                        layout: 'byComponent',
                        verbose: true
                    }
                }
            },
            sass: {
                dist: {
                    files: {
                        './public/styles/generated.css' : './sass/main.scss'
                    }
                }
            },
            postcss: {
                options: {
                    map: true,
                    processors: [
                        autoprefixer({
                            browsers: ['last 2 versions']
                        }),
                        cssnano()
                    ]
                },
                dist: {
                    src: 'public/styles/generated.css'
                }
            },
            watch: {
                css: {
                    files: './sass/**/*.scss',
                    tasks: [ 'sass', 'postcss' ]
                }
            },
            requirejs: {
                compile: {
                    options: {
                        baseUrl: "public/js",
                        mainConfigFile: "public/js/Init.js",
                        name: 'Init',
                        out: 'public/js/compiled/Init.js',
                        wrapShim: true,
                        optimize: "none"
                        //optimize: "uglify2"
                    }
                }
            }
        });

        grunt.registerTask('update', [ 'bower' ]);
        grunt.registerTask('development', [ 'sass', 'postcss' ]);
        grunt.registerTask('production', [ 'requirejs', 'sass', 'postcss' ]);

        grunt.registerTask('default', [ 'watch' ]);
    };
}());
