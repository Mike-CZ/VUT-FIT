<?php

use Illuminate\Database\Seeder;
use App\Models\Category;

class CategoriesTableSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run() {

        Category::create([
            'title' => 'Sport',
            'url' => 'sport',
            'sort' => 1
        ]);

        Category::create([
            'title' => 'Ostatní',
            'url' => 'ostatni',
            'sort' => 2
        ]);
    }
}
