<?php

use Illuminate\Database\Seeder;
use App\Models\User;

class UserTableSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run() {

        // create admin user
        $user = User::create([
            'login' => 'admin',
            'email' => 'xpelan03@stud.fit.vutbr.cz',
            'password' => bcrypt('123456'),
        ]);

        // create admin role
        $role = \Spatie\Permission\Models\Role::create(['name' => 'admin']);

        // attach the role
        $user->assignRole($role);
    }
}
