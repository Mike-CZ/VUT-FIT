<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreateVideosTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('videos', function (Blueprint $table) {
            $table->increments('id');
            $table->string('title');
            $table->integer('category_id');
            $table->text('description')->nullable();
            $table->string('url');
            $table->string('file_path')->nullable();
            $table->char('file_format', 20)->nullable();
            $table->string('file_duration')->nullable();
            $table->integer('file_size')->default(0);
            $table->string('file_preview')->nullable();
            $table->integer('views_count')->default(0);
            $table->dateTime('created_at');
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('videos');
    }
}
