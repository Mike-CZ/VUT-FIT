
/**
 * First we will load all of this project's JavaScript dependencies which
 * includes Vue and other libraries. It is a great starting point when
 * building robust, powerful web applications using Vue and Laravel.
 */

require('../bootstrap');

require('bootstrap-sass');

window.HELP_IMPROVE_VIDEOJS = false;
require('video.js');

// custom scripts
$( "li a.menu1" ).click(function() {
    $( "ul.cl-effect-2" ).slideToggle( 300, function() {
        // Animation complete.
    });
});

$( "li a.menu" ).click(function() {
    $( "ul.cl-effect-1" ).slideToggle( 300, function() {
        // Animation complete.
    });
});

$( ".top-navigation" ).click(function() {
    $( ".drop-navigation" ).slideToggle( 300, function() {
        // Animation complete.
    });
});