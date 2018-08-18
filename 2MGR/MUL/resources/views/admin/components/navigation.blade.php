<nav class="navbar navbar-expand-lg navbar-dark bg-dark fixed-top" id="mainNav">
    <a class="navbar-brand" href="{{ route('admin.index') }}"><i class="fa fa-video-camera" aria-hidden="true"></i> Databáze videozáznamů</a>
    <button class="navbar-toggler navbar-toggler-right" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarResponsive">
        <ul class="navbar-nav navbar-sidenav" id="exampleAccordion">
            @foreach($navigation as $item)
                <li class="nav-item" data-toggle="tooltip" data-placement="right" title="{{ $item['title'] }}">
                    @if(isset($item['childs']) && count($item['childs']) > 0)
                        <a class="nav-link nav-link-collapse collapsed" data-toggle="collapse" href="#collapse{{ $collapseID = preg_replace('/\W+/','',strtolower(strip_tags($item['title']))) }}" data-parent="#exampleAccordion">
                            <i class="fa fa-fw {{ $item['icon'] }}"></i>
                            <span class="nav-link-text">{{ $item['title'] }}</span>
                        </a>
                        <ul class="sidenav-second-level collapse" id="collapse{{ $collapseID }}">
                        @foreach($item['childs'] as $child)
                                <li>
                                    <a href="{{ route($child['route']) }}">{{ $child['title'] }}</a>
                                </li>
                        @endforeach
                        </ul>
                    @else
                    <a class="nav-link" href="{{ route($item['route']) }}">
                        <i class="fa fa-fw {{ $item['icon'] }}"></i>
                        <span class="nav-link-text">{{ $item['title'] }}</span>
                    </a>
                    @endif
                </li>
            @endforeach
        </ul>
        <ul class="navbar-nav sidenav-toggler">
            <li class="nav-item">
                <a class="nav-link text-center" id="sidenavToggler">
                    <i class="fa fa-fw fa-angle-left"></i>
                </a>
            </li>
        </ul>
        @include('admin.components.top-bar')
    </div>
</nav>