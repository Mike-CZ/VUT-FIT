<?php

/*
|--------------------------------------------------------------------------
| Admin Routes
|--------------------------------------------------------------------------
*/

Route::group(['prefix' => 'admin', 'as' => 'admin.'], function () {

    // DASHBOARD
    Route::get('/', [
        'as' => 'index',
        'uses' => 'AdminController@index'
    ]);

    // CATEGORIES ROUTES START
    Route::get('kategorie', [
        'as' => 'categories',
        'uses' => 'AdminController@categories'
    ]);
    Route::get('kategorie/vytvorit', [
        'as' => 'categories_create',
        'uses' => 'AdminController@createCategory'
    ]);
    Route::post('kategorie/vytvorit', [
        'as' => 'categories_store',
        'uses' => 'AdminController@storeCategory'
    ]);
    Route::get('kategorie/upravit/{category}', [
        'as' => 'categories_edit',
        'uses' => 'AdminController@editCategory'
    ]);
    Route::patch('kategorie/upravit/{category}', [
        'as' => 'categories_update',
        'uses' => 'AdminController@updateCategory'
    ]);
    Route::get('kategorie/smazat/{category}', [
        'as' => 'categories_delete',
        'uses' => 'AdminController@deleteCategory'
    ]);
    // CATEGORIES ROUTES END

    // VIDEOS ROUTES START
    Route::get('videozaznamy', [
        'as' => 'videos',
        'uses' => 'AdminController@videos'
    ]);
    Route::get('videozaznamy/vlozit', [
        'as' => 'videos_create',
        'uses' => 'AdminController@createVideo'
    ]);
    Route::post('videozaznamy/vlozit', [
        'as' => 'videos_store',
        'uses' => 'AdminController@storeVideo'
    ]);
    Route::get('videozaznamy/upravit/{video}', [
        'as' => 'videos_edit',
        'uses' => 'AdminController@editVideo'
    ]);
    Route::patch('videozaznamy/upravit/{video}', [
        'as' => 'videos_update',
        'uses' => 'AdminController@updateVideo'
    ]);
    Route::get('videozaznamy/smazat/{video}', [
        'as' => 'videos_delete',
        'uses' => 'AdminController@deleteVideo'
    ]);


    Route::post('videozaznamy/video-upload', [
        'as' => 'video-upload',
        'uses' => 'AdminController@videoUpload'
    ]);
    Route::post('videozaznamy/video-delete', [
        'as' => 'video-delete',
        'uses' => 'AdminController@videoDelete'
    ]);
    // VIDEO ROUTES END


    // AUTH ROUTES START
    Route::get('/login', [
        'as' => 'login',
        'uses' => 'Auth\Admin\LoginController@showLoginForm'
    ]);
    Route::post('/login', [
        'as' => 'login-submit',
        'uses' => 'Auth\Admin\LoginController@login'
    ]);
    Route::get('/logout', [
        'as' => 'logout',
        'uses' => 'Auth\Admin\LoginController@logout'
    ]);
    // AUTH ROUTES END
});


