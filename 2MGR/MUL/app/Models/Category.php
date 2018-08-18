<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Builder;

class Category extends Model
{

    /**
     * The attributes that are mass assignable.
     *
     * @var array
     */
    protected $fillable = ['title', 'url', 'sort'];

    /**
     * The table associated with the model.
     *
     * @var string
     */
    protected $table = 'categories';

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
     * Get videos
     *
     * @return \Illuminate\Database\Eloquent\Relations\HasMany
     */
    public function videos()
    {
        return $this->hasMany(Video::class, 'category_id', 'id');
    }

    /*
     * Boot category
     *
     * @return void
     */
    public static function boot()
    {
        parent::boot();

        // init category instance
        static::creating(function($category){
            $category->videos_count = 0;
            $category->created_at = \Carbon\Carbon::now();
        });

        // get sorted categories
        static::addGlobalScope('sort', function (Builder $builder) {
            $builder->orderBy('categories.sort', 'ASC');
        });

        // delete related videos when deleting category
        static::deleting(function($category) {
            \DB::transaction(function() use($category) {
                foreach ($category->videos as $video) {
                    $video->delete();
                }
            });
        });
    }
}
