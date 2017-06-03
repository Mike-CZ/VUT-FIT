<!DOCTYPE html>
<html lang="<?php echo LANGUAGE_CODE; ?>">
<head>

	<!-- Site meta -->
	<meta charset="utf-8">
    <link href='https://fonts.googleapis.com/css?family=Source+Sans+Pro:700,400&subset=latin,latin-ext' rel='stylesheet' type='text/css'>
	<title><?php echo (isset($data['title']) ? $data['title'] . ' - ' . SITETITLE : SITETITLE) //SITETITLE defined in app/core/config.php ?></title>

	<!-- CSS -->
	<?php
		helpers\assets::css(array(
            helpers\url::template_path() . 'css/font-awesome.min.css',
            helpers\url::template_path() . 'css/jquery.countdown.css',
            helpers\url::template_path() . 'css/style.css'
		))
	?>
    <link rel="stylesheet" href="//code.jquery.com/ui/1.11.4/themes/smoothness/jquery-ui.css">
    <!-- JS -->
    <?php
    helpers\assets::js(array(
        helpers\url::template_path() . 'js/jquery.js',
        helpers\url::template_path() . 'js/jquery.plugin.min.js',
        helpers\url::template_path() . 'js/jquery.countdown.min.js'
    ))
    ?>
    <script src="//code.jquery.com/ui/1.11.4/jquery-ui.js"></script>
    <?php
    helpers\assets::js(array(
        helpers\url::template_path() . 'js/script.js'
    ))
    ?>

</head>
<body>