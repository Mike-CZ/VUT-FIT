<?php

// breadcrumb init
Breadcrumbs::register('breadcrumb', function ($breadcrumbs) {
    $breadcrumbs->push('<i class="fa fa-home"></i> Dashboard', route('admin.index'));
});