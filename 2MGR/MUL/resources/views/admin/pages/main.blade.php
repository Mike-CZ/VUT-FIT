@extends('admin.layouts.master')

@section('body')
    @include('admin.components.navigation')
    <div class="content-wrapper">
        <div class="container-fluid">
            {{ Breadcrumbs::render('breadcrumb') }}
            <div class="row">
                <div class="col-12">
                    @yield('content')
                </div>
            </div>
        </div>
        @include('admin.components.footer')
        @include('admin.components.logout-message')
    </div>
@endsection