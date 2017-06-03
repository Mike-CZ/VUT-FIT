<?php
// get variables
if (isset($data['variables'])){
    foreach($data['variables'] as $index => $value){
        $$index = $value;
    }
}
?>

<header>
    <div class="header">
        <a href="/">
            <div class="logo">
                <img class="logo-image" src="../../images/graphics/logo.png" />
                <span>Otestuj své znalosti rostlin ČR!</span>
            </div>
        </a>
        <ul id="user-navigation">
            <?php if (isset($user)): ?>
                <li style="padding-right: 20px;" id="dropdown-list">
                    <a id="dropdown-toggle" href="#"><i style="padding-right: 5px;" class="fa fa-user"></i> <?= $user['login'] ?><i style="padding-left: 5px;" class="fa fa-caret-down"></i></a>
                    <ul id="dropdown">
                        <li><a href="#"><i class="fa fa-user-secret"></i> Profil</a></li>
                        <li><a href="/logout"><i class="fa fa-sign-out"></i> Odhlásit se</a></li>
                    </ul>
                </li>
            <?php else: ?>
                <li><a title="Přihlásit se" href="/login"><i class="fa fa-sign-in"></i> Přihlášení</a></li>
                <li><a title="Registrovat se" href="/register"><i class="fa fa-user-plus"></i> Registrace</a></li>
            <?php endif; ?>
        </ul>
    </div>
</header>
<div class="content">
            <section>
                <div class="subcontent">
                    <?php
                    // display subcontent
                    if (isset($data['content'])){
                        require "app/views/".$data['content'].".php";
                    }
                    ?>
                </div>
            </section>
            <nav>
                <div class="navigation">
                    <ul class="main-menu">
                        <li>
                            <div class="menu-wrapper">
                                <i class="fa fa-info-circle"></i> <a href="#">Informace</a>
                            </div>
                        </li>
                        <li>
                            <div class="menu-wrapper">
                                <i class="fa fa-list"></i> <a href="/score">Žebříček</a>
                            </div>
                        </li>
                        <li>
                            <div class="menu-wrapper">
                                <i class="fa fa-question-circle"></i> <a href="#">Nápověda</a>
                            </div>
                        </li>
                        <li>
                            &nbsp;
                        </li>
                        <li>
                            <div class="menu-wrapper">
                                <i class="fa fa-play"></i> <a href="/test-menu">Spustit test</a>
                            </div>
                        </li>
                    </ul>
                </div>
            </nav>
</div>
<footer>
    <a class="admin-link" href="/admin-login">Administrace</a>
</footer>

</body>
<script>
<?php if (isset($user)): ?>
    $(function() {

        var color_one = "#999";
        var color_two = "#fff";

        function toggle_color(elem){
            var color = elem.css("color");
            elem.css("color", color_one);
            if(color == elem.css("color")){
                elem.css("color", color_two);
            }
        }
// Dropdown toggle
        $('#dropdown-toggle').click(function(){
            $(this).next('#dropdown').toggle();
            toggle_color($(this));
        });

        $('body').on('click', function(e){
            var id = e.target.id;
            if (id != 'dropdown' && !$(e.target).parents().is('#dropdown-list')) {
               $('#dropdown').hide();
               $('#dropdown-toggle').css('color', '#fff');
            }
        });

    });
<?php endif; ?>
</script>