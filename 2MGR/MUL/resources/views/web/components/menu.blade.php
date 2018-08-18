<div class="col-sm-3 col-md-2 sidebar">
    <div class="top-navigation">
        <div class="t-menu">MENU</div>
        <div class="t-img">
            <img src="images/lines.png" alt="" />
        </div>
        <div class="clearfix"> </div>
    </div>
    <div class="drop-navigation drop-navigation">
        <ul class="nav nav-sidebar">
            <li @if(Route::is('home')) class="active" @endif ><a href="{{ route('home') }}" class="home-icon"><span class="glyphicon glyphicon-home" aria-hidden="true"></span>Úvodní strana</a></li>
            <li><a href="#" class="menu1"><span class="glyphicon glyphicon-film" aria-hidden="true"></span>Kategorie<span class="glyphicon glyphicon-menu-down" aria-hidden="true"></span></a></li>
            <ul class="cl-effect-2" @if(Route::is('category.*')) style="display: block;" @endif>
                <li @if(Route::is('category.all')) class="active" @endif><a href="{{ route('category.all') }}">Vše&nbsp;({{ (int)$categories->reduce(function ($carry, $category) { return $carry + $category->videos_count; }) }})</a></li>
                @foreach($categories as $category)
                    <li @if(Route::is('category.list') && optional(Request::route('category'))->url == $category->url) class="active" @endif><a href="{{ route('category.list', $category->url) }}">{{ $category->title }}&nbsp;({{ $category->videos_count }})</a></li>
                @endforeach
            </ul>
        </ul>
        <div class="side-bottom">
            <div class="copyright">
                <p>© 2018 Lukáš Pelánek | Design by <a href="http://w3layouts.com/">W3layouts</a></p>
            </div>
        </div>
    </div>
</div>