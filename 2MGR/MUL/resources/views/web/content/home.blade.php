@extends('web.pages.main')

@section('content')
    <div class="latest-videos" style="margin-bottom: 20px;">
        <h3>Nejnovější videozáznamy</h3>
    </div>
    @forelse ($latestVideos as $video)
        <div class="col-md-3 resent-grid recommended-grid">
            <div class="resent-grid-img recommended-grid-img">
                <a href="{{ $video->full_url }}"><img src="{{ $video->preview_url }}" alt="{{ $video->title }}" /></a>
                <div class="time small-time">
                    <p style="white-space: nowrap;overflow: hidden;"><span class="glyphicon glyphicon-time" aria-hidden="true"></span> {{ $video->file_duration }}</p>
                </div>
            </div>
            <div class="resent-grid-info recommended-grid-info video-info-grid">
                <h5><a href="{{ $video->full_url }}" class="title">{{ $video->title }}</a></h5>
                <ul>
                    <li><p class="author author-info"><span class="glyphicon glyphicon-align-justify" aria-hidden="true"></span><a href="{{ route('category.list', $video->category->url) }}" class="author">{{ $video->category->title }}</a></p></li>
                    <li class="right-list"><p class="views views-info">{{ $video->views_count }} shlédnutí</p></li>
                </ul>
            </div>
        </div>
    @empty
        <h5>Nebyly vloženy žádné videozáznamy.</h5>
    @endforelse
@endsection