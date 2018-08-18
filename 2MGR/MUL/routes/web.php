<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

// admin routes
require('_admin.php');

Route::get('/', [
    'as' => 'home',
    'uses' => 'MainController@index'
]);

Route::get('/kategorie', [
    'as' => 'category.all',
    'uses' => 'MainController@showAll'
]);

Route::get('/kategorie/{category}', [
    'as' => 'category.list',
    'uses' => 'MainController@showCategory'
]);

Route::get('/kategorie/{category}/videozaznam/{video}', [
    'as' => 'video',
    'uses' => 'MainController@showVideo'
]);

Route::get('/stahnout/videozaznam/{video}', [
    'as' => 'video.download',
    'uses' => 'MainController@downloadVideo'
]);