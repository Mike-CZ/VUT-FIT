@extends('admin.pages.main')

@section('content')
    <div class="mb-5 ml-3 mt-3">
        <h4 class="mb-4">@if(Route::is('admin.videos_edit')) Upravit @else Vložit @endif videozáznam</h4>
        {!! Form::model($video ?? null, ['id' => 'video-form', 'url' => URL::current(), 'class' => 'form-prevent-multiple-submits', 'method' => Route::is('admin.videos_edit') ? 'patch' : 'post']) !!}
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
                {!! Form::label($name = 'category_id', 'Kategorie', ['class' => 'label-required']) !!}
                {!! Form::select($name, ['' => 'Vyberte kategorii'] + $categories->pluck('title', 'id')->toArray(), null, ['class' => 'form-control']) !!}
                @include('admin.vendor.field_error_single')
            </div>
            <div class="form-group">
                {!! Form::label('file', 'Videozáznam (mp4, ogv, webm)', ['class' => 'label-required']) !!}
                @if(Route::is('admin.videos_create'))
                    {!! Form::hidden($name = 'file_info', null) !!}
                    {!! Form::file('file', ['accept' => 'video/mp4,video/x-m4v,video/*']) !!}
                    <div id="image-error-message">
                        @include('admin.vendor.field_error_single')
                    </div>
                @else
                    <img class="form-text" src="{{ $video->preview_url }}">
                    <small class="form-text text-muted">Videozáznam nelze editovat. Pro vložení videozáznamu vytvořte novou položku.</small>
                @endif
            </div>
            <div class="form-group">
                {!! Form::label($name = 'description', 'Popis') !!}
                {!! Form::hidden($name, null, ['id' => $name, 'class' => 'form-control']) !!}
                <div id="editor-container" style="min-height: 150px; max-height: 400px; overflow-y: scroll;">
                    {!! old('description') ?? (isset($video) ? $video->description : '') !!}
                </div>
                @include('admin.vendor.field_error_single')
            </div>
            {!! Form::submit((Route::is('admin.videos_edit') ? 'Upravit' : 'Vložit') . ' videozáznam', ['class' => 'btn btn-primary btn-sm mt-2 button-prevent-multiple-submits']) !!}
        {!! Form::close() !!}
    </div>
@endsection

@push('script')
    {!! Html::script('js/libs/jquery.fileuploader.js') !!}
    <script>
        (function(){
            var quill = new Quill('#editor-container', {
                modules: {
                    toolbar: [
                        ['bold', 'italic'],
                        ['link', 'blockquote', 'code-block'],
                        [{ list: 'ordered' }, { list: 'bullet' }]
                    ]
                },
                placeholder: 'Zadejte popis',
                theme: 'snow'
            });

            // set description input data
            var form = document.querySelector('form');
            form.onsubmit = function() {
                var description = document.querySelector('input[name="description"]');
                description.value = quill.container.firstChild.innerHTML;
            };
        })();

        @if(Route::is('admin.videos_create'))
        // check if file is uploaded
        $('#video-form').on('submit', function(e){
            if (! $('input[name="file_info"]').val()) {
                e.preventDefault();
                $('#video-form .button-prevent-multiple-submits').prop('disabled', false);
                $('#image-error-message').html('<span class="help-block text-danger">Položka videozáznam je povinná.</span>');
            }
            else {
                $('#image-error-message').html('');
            }
        });

        $('input[name="file"]').fileuploader({
            limit: 1,
            extensions: ['ogv', 'mp4', 'webm'],
            files: [
                    @if(!is_null($oldFileEncoded = old('file_info', null)))
                    {
                        name: '{{ ($oldFile = json_decode($oldFileEncoded))->old_name }}',
                        type: '{{ $oldFile->type }}',
                        size: '{{ $oldFile->size }}',
                        file: '{{ \App\Models\Video::getVideoUrl($oldFile->name) }}',

                    }
                    @endif
            ],
            upload: {
                url: '{{ route('admin.video-upload') }}',
                data: null,
                type: 'POST',
                enctype: 'multipart/form-data',
                start: true,
                synchron: true,
                chunk: 0.4,
                onSuccess: function(result, item) {
                    data = result.files[0];
                    data.video_duration = item.reader.duration2;
                    data.preview = item.reader.frame;
                    $('#image-error-message').html('');
                    $('input[name="file_info"]').val(JSON.stringify(data));
                },
                onError: function(item) {
                    var progressBar = item.html.find('.progress-bar2');

                    if(progressBar.length > 0) {
                        progressBar.find('span').html(0 + "%");
                        progressBar.find('.fileuploader-progressbar .bar').width(0 + "%");
                        item.html.find('.progress-bar2').fadeOut(400);
                    }
                    item.upload.status != 'cancelled' && item.html.find('.fileuploader-action-retry').length == 0 ? item.html.find('.column-actions').prepend(
                        '<a class="fileuploader-action fileuploader-action-retry" title="Opakovat"><i></i></a>'
                    ) : null;
                },
                onProgress: function(data, item) {
                    var progressBar = item.html.find('.progress-bar2');

                    if(progressBar.length > 0) {
                        progressBar.show();
                        progressBar.find('span').html(data.percentage + "%");
                        progressBar.find('.fileuploader-progressbar .bar').width(data.percentage + "%");
                    }
                },
                onComplete: null,
            },
            onRemove: function(item) {
                $.post('{{ route('admin.video-delete') }}', {
                    file: JSON.parse($('input[name="file_info"]').val()).name
                });
                $('input[name="file_info"]').val('');
            }
        });
        @endif
    </script>
@endpush