<!DOCTYPE HTML>
<html>
<head>
    <title>{{ $title }} - databáze videozáznamů</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link rel="icon" type="image/png" sizes="16x16" href="/icon.png">
    <!-- Style-->
    {!! Html::style(mix('css/style.css')) !!}
    <!-- fonts -->
    <link href='//fonts.googleapis.com/css?family=Open+Sans:300italic,400italic,600italic,700italic,800italic,400,300,600,700,800' rel='stylesheet' type='text/css'>
    <link href='//fonts.googleapis.com/css?family=Poiret+One' rel='stylesheet' type='text/css'>
    <!-- //fonts -->
</head>
<body>
@yield('body')
<!-- JavaScript-->
{!! Html::script(mix('js/app.js')) !!}
@stack('script')
</body>
</html>