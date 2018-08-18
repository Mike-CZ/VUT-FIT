@extends('web.layouts.master')

@section('body')
    @include('web.components.header')
    @include('web.components.menu')
    <div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main">
        <div class="main-grids">
            <div class="top-grids">
                @yield('content')
                <div class="clearfix"> </div>
            </div>
        </div>
    </div>
@endsection