@extends('web.pages.main')

@section('content')
    <div class="song">
        <div class="latest-videos">
            <h3>{{ $video->title }}</h3>
        </div>
        <div class="video-grid" style="height: auto; max-width: 620px;">
            <video
                    id="my-player"
                    class="video-js vjs-default-skin vjs-16-9 vjs-big-play-centered"
                    controls
                    preload="auto"
                    poster="{{ $video->preview_url }}"
                    data-setup='{}'>
                <source src="{{ $video->video_url }}" type="video/{{ $video->file_format }}"></source>
                <p class="vjs-no-js">
                    Pro zobrazení videa aktivujte javascript a zvažte používání prohlížeče, který
                    <a href="http://videojs.com/html5-video-support/" target="_blank">
                        podporuje HTML5 video
                    </a>.
                </p>
            </video>
        </div>
    </div>
    <div class="clearfix"> </div>
    <div class="published">
        <div class="load_more">
            <ul id="myList">
                <li>
                    <h4>Nahráno {{ $video->created_at->format('d.m.Y H:i') }}</h4>
                    <p>{!! $video->description !!}</p>
                </li>
                <li>
                    <h4><strong>Informace o videozáznamu:</strong></h4>
                </li>
                <li><h4>Formát: {{ $video->file_format }}</h4></li>
                <li><h4>Doba trvání: {{ $video->file_duration }}</h4></li>
                <li><h4>Velikost souboru: {{ $video->file_size_formatted }}</h4></li>
                <li><h4>Počet shlédnutí: {{ $video->views_count }}</h4></li>
                <li><p><a href="{{ route('video.download', $video->url) }}" class="btn btn-primary">Stáhnout videozáznam</a></p></li>
            </ul>
        </div>
    </div>
    <div id="fb-root"></div>
    <div class="fb-comments" data-href="{{ URL::current() }}" data-width="620" data-numposts="5"></div>
@endsection

@push('script')
    <script>
        (function(d, s, id) {
            var js, fjs = d.getElementsByTagName(s)[0];
            if (d.getElementById(id)) return;
            js = d.createElement(s); js.id = id;
            js.src = 'https://connect.facebook.net/cs_CZ/sdk.js#xfbml=1&version=v2.12';
            fjs.parentNode.insertBefore(js, fjs);
        }(document, 'script', 'facebook-jssdk'));
    </script>
@endpush