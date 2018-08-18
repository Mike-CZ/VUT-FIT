<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Builder;

class Video extends Model
{
    const PREVIEW_WIDTH = 620;
    const PREVIEW_HEIGHT = 350;

    /**
     * The attributes that are mass assignable.
     *
     * @var array
     */
    protected $fillable = [
        'title', 'description', 'url', 'category_id'
    ];

    /**
     * The table associated with the model.
     *
     * @var string
     */
    protected $table = 'videos';

    /**
     * The attributes that should be mutated to dates.
     *
     * @var array
     */
    protected $dates = ['created_at'];

    /**
     * Indicates if the model should be timestamped.
     *
     * @var bool
     */
    public $timestamps = false;

    /**
     * Get route key nae
     *
     * @return string
     */
    public function getRouteKeyName()
    {
        return 'url';
    }

    /**
     * Get category
     *
     * @return \Illuminate\Database\Eloquent\Relations\BelongsTo
     */
    public function category()
    {
        return $this->belongsTo(Category::class, 'category_id', 'id');
    }

    /**
     * Get full url
     *
     * @return string
     */
    function getFullUrlAttribute()
    {
        return route('video', [$this->category->url, $this->url]);
    }

    /**
     * Get video
     *
     * @return string
     */
    public function getVideoUrlAttribute()
    {
        return asset("/upload/video/{$this->file_path}");
    }

    /**
     * Get preview
     *
     * @return string
     */
    function getPreviewUrlAttribute()
    {
        if(!is_null($this->file_preview)){
            return static::getPreviewUrl($this->file_preview);
        }
        return 'http://placehold.it/' . static::PREVIEW_WIDTH . 'x' . static::PREVIEW_HEIGHT . '.png&text=video';
    }

    /**
     * Get formatted file size
     *
     * @return string
     */
    function getFileSizeFormattedAttribute()
    {
        $units = ['B', 'KB', 'MB', 'GB', 'TB'];
        $bytes = max($this->file_size, 0);
        $pow = floor(($bytes ? log($bytes) : 0) / log(1024));
        $pow = min($pow, count($units) - 1);
        $bytes /= (1 << (10 * $pow));
        return round($bytes, 2) . ' ' . $units[$pow];
    }

    /**
     * Get video directory
     *
     * @return string
     */
    public static function getVideoDir()
    {
        return public_path() . '/upload/video/';
    }

    /**
     * Get preview directory
     *
     * @return string
     */
    public static function getPreviewDir()
    {
        return public_path() . '/upload/preview/';
    }

    /**
     * Get video directory path
     *
     * @param $path string
     * @return string
     */
    public static function getVideoUrl(string $path)
    {
        return asset("/upload/video/$path");
    }

    /**
     * Get preview directory path
     *
     * @param $path string
     * @return string
     */
    public static function getPreviewUrl(string $path)
    {
        return asset("/upload/preview/$path");
    }

    /*
     * Boot video
     *
     * @return void
     */
    public static function boot()
    {
        parent::boot();

        // init video instance
        static::creating(function($video){
            $video->views_count = 0;
            $video->created_at = \Carbon\Carbon::now();
        });

        // get sorted videos by title
        static::addGlobalScope('sort', function (Builder $builder) {
            $builder->orderBy('videos.title', 'ASC');
        });

        // remove files on delete
        static::deleting(function($video){
            if (\File::exists(Video::getVideoDir() . $video->file_path)) {
                \File::delete(Video::getVideoDir() . $video->file_path);
            }
            if (!is_null($video->file_preview) && \File::exists(Video::getPreviewDir() . $video->file_preview)) {
                \File::delete(Video::getPreviewDir() . $video->file_preview);
            }
        });
    }
}
