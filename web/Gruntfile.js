/* global require, define, module, __dirname */

(function ()
{
    'use strict';

    module.exports = function (grunt) {
        var _ = require('underscore');

        grunt.loadNpmTasks('grunt-bower-task');
        grunt.loadNpmTasks('grunt-contrib-sass');
        grunt.loadNpmTasks('grunt-contrib-watch');
        grunt.loadNpmTasks('grunt-contrib-requirejs');
        grunt.loadNpmTasks('grunt-contrib-cssmin');
        grunt.loadNpmTasks('grunt-shell');

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
            watch: {
                css: {
                    files: './sass/**/*.scss',
                    tasks: [ 'sass' ]
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
            },
            cssmin: {
                options: {
                },
                target: {
                    files: {
                        './public/styles/generated.min.css': './public/styles/generated.css'
                    }
                }
            }
        });

        grunt.registerTask('update', [ 'bower' ]);
        grunt.registerTask('development', [ 'sass' ]);
        grunt.registerTask('production', [ 'sass', 'requirejs', 'cssmin' ]);

        grunt.registerTask('default', [ 'watch' ]);
    };
}());
