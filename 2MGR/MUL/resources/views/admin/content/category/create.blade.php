@extends('admin.pages.main')

@section('content')
    <div class="mb-5 ml-3 mt-3">
        <h4 class="mb-4">@if(Route::is('admin.categories_edit')) Upravit @else Vytvořit @endif kategorii</h4>
        {!! Form::model($category ?? null, ['url' => URL::current(), 'class' => 'form-prevent-multiple-submits', 'method' => Route::is('admin.categories_edit') ? 'patch' : 'post']) !!}
            <div class="form-group">
                {!! Form::label($name = 'title', 'Název', ['class' => 'label-required']) !!}
                {!! Form::text($name, null, ['class' => 'form-control', 'placeholder' => 'Zadejte název']) !!}
                @include('admin.vendor.field_error_single')
            </div>
            <div class="form-group">
                {!! Form::label($name = 'url', 'Url', ['class' => 'label-required']) !!}
                {!! Form::text($name, null, ['class' => 'form-control', 'placeholder' => 'Zadejte url']) !!}
                @include('admin.vendor.field_error_single')
            </div>
            <div class="form-group">
                {!! Form::label($name = 'sort', 'Pořadí', ['class' => 'label-required']) !!}
                {!! Form::number($name, null, ['class' => 'form-control', 'placeholder' => 'Zadejte pořadí', 'min' => '0']) !!}
                @include('admin.vendor.field_error_single')
            </div>
            {!! Form::submit((Route::is('admin.categories_edit') ? 'Upravit' : 'Vytvořit') . ' kategorii', ['class' => 'btn btn-primary btn-sm mt-2 button-prevent-multiple-submits']) !!}
        {!! Form::close() !!}
    </div>
@endsection