<?php

namespace App\Http\Controllers;


use App\Models\Category;
use App\Models\Video;

class MainController extends BaseController {

    /**
     * MainController constructor.
     */
    public function __construct()
    {
        $this->setTitle('Databáze videozáznamů');

        // get menu categories
        view()->composer('web.components.menu', function ($view) {
            $view->with('categories', Category::all());
        });
    }

    /**
     * Show index page
     *
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function index()
    {
        $latestVideos = Video::orderByDesc('id')
            ->limit(4)
            ->get();
        return $this->render('web.content.home', compact('latestVideos'));
    }

    /**
     * Show all videos
     *
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function showAll()
    {
        $this->setTitle('Vše - Databáze videozáznamů');
        $videos = Video::with('category')
            ->orderByDesc('id')
            ->get();
        return $this->render('web.content.list', compact('videos'));
    }

    /**
     * Show category page
     *
     * @param $category Category
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function showCategory(Category $category)
    {
        $this->setTitle($category->title . ' - Databáze videozáznamů');
        $videos = Video::with('category')
            ->where('category_id', '=', $category->id)
            ->orderByDesc('id')
            ->get();
        return $this->render('web.content.list', compact('videos', 'category'));
    }

    /**
     * Show video page
     *
     * @param $category Category
     * @param $video Video
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function showVideo(Category $category, Video $video)
    {
        $this->setTitle($video->title . ' - Databáze videozáznamů');
        $video->views_count += 1;
        $video->save();
        return $this->render('web.content.video', compact('category', 'video'));
    }

    /**
     * Show index page
     *
     * @param $video Video
     * @return \Symfony\Component\HttpFoundation\BinaryFileResponse
     */
    public function downloadVideo(Video $video)
    {
        return response()->download(Video::getVideoDir() . '/' . $video->file_path);
    }
}
