<!DOCTYPE html>
<html lang="cs">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="Databaze videozaznamu - administrace">
    <meta name="author" content="Lukas Pelanek <xpelan03@stud.fit.vutbr.cz>">
    <link rel="icon" type="image/png" sizes="16x16" href="/icon.png">
    <title>{{ $title }} - administrace</title>
    <!-- Style-->
    {!! Html::style(mix('css/admin.css')) !!}
</head>
<body class="fixed-nav sticky-footer bg-dark" id="page-top">
@yield('body')
<!-- JavaScript-->
{!! Html::script(mix('js/admin.js')) !!}
@stack('script')
</body>
</html>
