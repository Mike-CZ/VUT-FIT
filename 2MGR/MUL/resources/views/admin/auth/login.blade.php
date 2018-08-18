@extends('admin.pages.empty')

@section('content')
    <div class="container">
        <div class="card card-login mx-auto mt-5">
            <div class="card-header"><i class="fa fa-video-camera" aria-hidden="true"></i> Administrace</div>
            <div class="card-body">
                {!! Form::open([
                'url' => route('admin.login-submit'),
                'enctype' => 'multipart/form-data',
                'autocomplete' =>'off'
                ])
            !!}
                <div class="form-group">
                    {{ Form::label($name = 'login', 'Login') }}
                    {!! Form::text($name, old($name), [
                        'class' => 'form-control',
                        'placeholder' => 'Zadejte login'
                        ])
                     !!}
                    @include('admin.vendor.field_error_single')
                </div>
                <div class="form-group">
                    {{ Form::label($name = 'password', 'Heslo') }}
                    {!! Form::password($name, [
                        'class' => 'form-control',
                        'placeholder' => 'Zadejte heslo'
                        ])
                     !!}
                    @include('admin.vendor.field_error_single')
                </div>
                <div class="form-group">
                    <div class="form-check">
                        {!! Form::label($name = 'remember',
                            Form::checkbox($name, 1, null, ['class' => 'form-check-input', 'id' => 'remember']) . ' Zapamatovat heslo',
                            ['class' => 'form-check-label'],
                            false
                        ) !!}
                    </div>
                </div>
                {!! Form::button('Přihlásit se', [
                'class' => 'btn btn-primary btn-block',
                'type' => 'submit'
                ]) !!}
                {!! Form::close() !!}
            </div>
        </div>
    </div>
@endsection