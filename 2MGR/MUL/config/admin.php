<?php

return [

    /*
     * Navigation items
     */
    'navigation' => [
        [
            'title' => 'Dashboard',
            'route' => 'admin.index',
            'icon' => 'fa-dashboard',
            'childs' => []
        ],
        [
            'title' => 'Kategorie',
            'route' => 'admin.categories',
            'icon' => 'fa-list',
            'childs' => []
        ],
        [
            'title' => 'Videozáznamy',
            'route' => 'admin.videos',
            'icon' => 'fa-camera',
            'childs' => []
        ]
    ]
];
