<!DOCTYPE html>
<html lang="<?php echo LANGUAGE_CODE; ?>">
<head>

	<!-- Site meta -->
	<meta charset="utf-8">
    <link href='http://fonts.googleapis.com/css?family=Quattrocento+Sans:400,700' rel='stylesheet' type='text/css'>
	<title><?php echo (isset($data['title']) ? $data['title'] . ' - ' . SITETITLE : SITETITLE) //SITETITLE defined in app/core/config.php ?></title>

	<!-- CSS -->
	<?php
		helpers\assets::css(array(
            helpers\url::template_path() . 'css/font-awesome.min.css',
			helpers\url::template_path() . 'css/style.css',
            helpers\url::template_path() . 'css/jquery.timepicker.css'
		))
	?>
    <link rel="stylesheet" href="//code.jquery.com/ui/1.11.4/themes/smoothness/jquery-ui.css">
    <!-- JS -->
    <?php
    helpers\assets::js(array(
        helpers\url::template_path() . 'js/jquery.js',
        helpers\url::template_path() . 'js/jquery.timepicker.min.js',
        helpers\url::template_path() . 'js/jquery.tablesorter.min.js'
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