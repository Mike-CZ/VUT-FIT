<?php

namespace App\Http\Controllers;

use App\Helpers\FileUploader;
use App\Models\Category;
use App\Models\Video;
use Illuminate\Http\Request;
use Intervention\Image\Image;

class AdminController extends BaseController {

    /**
     * AdminController constructor.
     */
    public function __construct()
    {
        $this->middleware('admin');
        $this->setTitle('Administrace');

        // get navigation items
        view()->composer('admin.components.navigation', function ($view) {
            $view->with('navigation', config('admin.navigation'));
        });
    }

    /**
     * Show index page
     *
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function index()
    {

        //$test = \App::make('FileUploader');


        return $this->render('admin.content.dashboard');
    }

    /**
     * Show categories page
     *
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function categories()
    {
        $this->setTitle('Přehled kategorií');
        $this->pushBreadcrumb('Přehled kategorií', route('admin.categories'));
        $categories = Category::all();
        return $this->render('admin.content.category.list', compact('categories'));
    }

    /**
     * Show create category page
     *
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function createCategory()
    {
        $this->setTitle('Vytvořit kategorii');
        $this->pushBreadcrumb('Přehled kategorií', route('admin.categories'));
        $this->pushBreadcrumb('Vytvořit kategorii');
        return $this->render('admin.content.category.create');
    }

    /**
     * Store new category into database
     *
     * @param $request Request
     * @return \Illuminate\Http\RedirectResponse
     */
    public function storeCategory(Request $request)
    {
        $data = $this->validateCategoryRequest($request);
        Category::create($data);
        flash('Kategorie byla úspěšně vytvořena.')->important()->success();
        return redirect()->route('admin.categories');
    }

    /**
     * Show edit category page
     *
     * @param $category Category
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function editCategory(Category $category)
    {
        $this->setTitle('Upravit kategorii');
        $this->pushBreadcrumb('Přehled kategorií', route('admin.categories'));
        $this->pushBreadcrumb('Upravit kategorii');
        return $this->render('admin.content.category.create', compact('category'));
    }

    /**
     * Update category
     *
     * @param $category Category
     * @param $request Request
     * @return \Illuminate\Http\RedirectResponse
     */
    public function updateCategory(Category $category, Request $request)
    {
        $data = $this->validateCategoryRequest($request, $category);
        $category->update($data);
        flash('Kategorie byla úspěšně upravena.')->important()->success();
        return redirect()->route('admin.categories');
    }

    /**
     * Delete category
     *
     * @param $category Category
     * @return \Illuminate\Http\RedirectResponse
     */
    public function deleteCategory(Category $category)
    {
        $category->delete();
        flash('Kategorie byla úspěšně smazána.')->important()->success();
        return redirect()->route('admin.categories');
    }

    /**
     * Show videos page
     *
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function videos()
    {
        $this->setTitle('Přehled videozáznamů');
        $this->pushBreadcrumb('Přehled videozáznamů', route('admin.videos'));
        $videos = Video::with('category')->get();
        return $this->render('admin.content.video.list', compact('videos'));
    }

    /**
     * Show create video page
     *
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function createVideo()
    {
        $this->setTitle('Vložit videozáznam');
        $this->pushBreadcrumb('Přehled videozáznamů', route('admin.videos'));
        $this->pushBreadcrumb('Vložit videozáznam');
        $categories = Category::all();
        return $this->render('admin.content.video.create', compact('categories'));
    }

    /**
     * Store new video into database
     *
     * @param $request Request
     * @return \Illuminate\Http\RedirectResponse
     */
    public function storeVideo(Request $request)
    {
        $data = $this->validateVideoRequest($request);
        $previewImage = $this->handlePreviewUpload($data['file_preview']);

        \DB::transaction(function () use($data, $previewImage) {
            $video = Video::create($data);
            $video->category()->increment('videos_count');

            // fill video data
            $video->file_path = $data['file_name'];
            $video->file_format = $data['file_extension'];
            $video->file_size = $data['file_size'];
            $video->file_duration = $data['file_duration'];
            $video->file_preview = $previewImage;
            $video->save();
        }, 5);

        if (is_null($previewImage)) {
            flash('Videozáznam byl vložen, ale nepodařilo se vytvořit náhlededový obrázek. Pro vytvoření náhledového obrázku zkuste použít novější prohlížeč.')->important()->warning();
        }
        else {
            flash('Videozáznam byl úspěšně vložen.')->important()->success();
        }
        return redirect()->route('admin.videos');
    }

    /**
     * Show edit video page
     *
     * @param $video Video
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    public function editVideo(Video $video)
    {
        $this->setTitle('Upravit videozáznam');
        $this->pushBreadcrumb('Přehled videozáznamů', route('admin.videos'));
        $this->pushBreadcrumb('Upravit videozáznam');
        $categories = Category::all();
        return $this->render('admin.content.video.create', compact('video', 'categories'));
    }

    /**
     * Update video
     *
     * @param $video Video
     * @param $request Request
     * @return \Illuminate\Http\RedirectResponse
     */
    public function updateVideo(Video $video, Request $request)
    {
        $data = $this->validateVideoRequest($request, $video);
        $video->update($data);
        flash('Videozáznam byl úspěšně upraven.')->important()->success();
        return redirect()->route('admin.videos');
    }

    /**
     * Delete video
     *
     * @param $video Video
     * @return \Illuminate\Http\RedirectResponse
     */
    public function deleteVideo(Video $video)
    {
        \DB::transaction(function () use($video) {
            $video->category()->where('videos_count', '>', 0)->decrement('videos_count');
            $video->delete();
        }, 5);
        flash('Videozáznam byl úspěšně smazán.')->important()->success();
        return redirect()->route('admin.videos');
    }

    /**
     * Handle the video upload - Ajax call
     *
     * @return \Illuminate\Http\JsonResponse
     */
    public function videoUpload()
    {
        $fileUploader = new FileUploader('file', array(
            'limit' => null,
            'maxSize' => null,
            'fileMaxSize' => null,
            'extensions' => ['ogv', 'mp4', 'webm'],
            'required' => false,
            'uploadDir' => Video::getVideoDir(),
            'title' => 'auto',
            'replace' => false,
            'listInput' => true,
            'files' => null
        ));

        $data = $fileUploader->upload();

        return response()->json($data);
    }

    /**
     * Delete video
     *
     * @return void
     */
    public function videoDelete(Request $request) {
        if (!is_null($file = $request->post('file', null))) {
            if (\File::exists(Video::getVideoDir() . $file)) {
                \File::delete(Video::getVideoDir() . $file);
            }
        }
    }


    /**
     * Validate category request
     *
     * @param $request Request
     * @param $category Category|null
     * @return array
     */
    private function validateCategoryRequest(Request $request, Category $category = null)
    {
        $rules = [
            'title' => 'required|max:255',
            'url' => 'required|max:20|unique:categories,url',
            'sort' => 'required|integer'
        ];

        if (!is_null($category)) {
            $rules['url'] = 'required|max:20|unique:categories,url,' . $category->id;
        }

        return $this->validate($request, $rules);
    }

    /**
     * Validate category request
     *
     * @param $request Request
     * @param $video Video|null
     * @return array
     */
    private function validateVideoRequest(Request $request, Video $video = null)
    {
        $rules = [
            'title' => 'required|max:255',
            'url' => 'required|max:20|unique:videos,url',
            'category_id' => 'required|exists:categories,id',
            'description' => 'nullable'
        ];

        if (is_null($video)){
            $rules['file_info'] = 'required';
        }

        if (!is_null($video)) {
            $rules['url'] = 'required|max:20|unique:videos,url,' . $video->id;
        }

        // validate
        $result = $this->validate($request, $rules);

        // add additional data
        if (!is_null($jsonFileInfo = $request->input('file_info', null))) {
            $data = json_decode($jsonFileInfo);
            $result['file_type'] = $data->type;
            $result['file_path'] = $data->file;
            $result['file_size'] = $data->size;
            $result['file_name'] = $data->name;
            $result['file_extension'] = $data->extension;
            $result['file_preview'] = $data->preview ?? null;
            $result['file_duration'] = $data->video_duration ?? null;
            unset($result['file_info']);
        }
        return $result;
    }

    /**
     * Handle preview image upload
     *
     * @param $base64Image string
     * @return string|null
     */
    private function handlePreviewUpload(string $base64Image = null)
    {
        if (!is_null($base64Image)) {
            $newName = time() . '.png';

            // create folder if doesn't exist
            if (!\File::isDirectory(Video::getPreviewDir())) {
                \File::makeDirectory(Video::getPreviewDir(), 0777, true, true);
            }

            // resize and store image
            \Image::make(file_get_contents($base64Image))->fit(Video::PREVIEW_WIDTH,Video::PREVIEW_HEIGHT,
                function($constraint){
                    $constraint->aspectRatio();
                    $constraint->upsize();
                })->save(Video::getPreviewDir() . $newName);

            return $newName;
        }
        return null;
    }
}
