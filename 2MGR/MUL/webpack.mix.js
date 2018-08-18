let mix = require('laravel-mix');

/*
 |--------------------------------------------------------------------------
 | Mix Asset Management
 |--------------------------------------------------------------------------
 |
 | Mix provides a clean, fluent API for defining some Webpack build steps
 | for your Laravel application. By default, we are compiling the Sass
 | file for the application as well as bundling up all the JS files.
 |
 */

// mix admin assets
mix.autoload({
    jquery: ['$', 'window.jQuery', 'jQuery'],
    'popper.js': ['Popper']
}).js('resources/assets/js/admin/app.js', 'public/js/admin.js')
    .sass('resources/assets/sass/admin/style.scss', 'public/css/admin.css')
    .version();

// mix web assets
mix.js('resources/assets/js/web/app.js', 'public/js')
    .sass('resources/assets/sass/web/style.scss', 'public/css')
    .options({
        processCssUrls: false
    })
    .version();

// copy libraries
mix.copy('resources/assets/libs/fileuploader/js/jquery.fileuploader.js', 'public/js/libs');

// copy fonts
mix.copy('node_modules/font-awesome/fonts', 'public/fonts/font-awesome');
mix.copy('node_modules/bootstrap-sass/assets/fonts/bootstrap', 'public/fonts/bootstrap');