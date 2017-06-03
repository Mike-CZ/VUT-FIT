<?php
// get variables
if (isset($data['variables'])){
    foreach($data['variables'] as $index => $value){
        $$index = $value;
    }
}
?>

<div class="main-wrapper">
    <div class="content">
        <div class="page-header">
            <div id="logo" class="inline">
                <a href="/"><img src=/images/logo.png alt="Informační systém" /></a>
            </div>
            <div class="float-right inline navigation">
                <?php if (isset($user)): ?>
                    <span><?= $user['login'] ?></span>
                    <a title="Zobrazit profil" href="/profil"><i class="fa fa-user"></i> Profil</a>
                    <a title="Odhlásit se" href="/logout"><i class="fa fa-sign-out"></i> Odhlášení</a>
                <?php else: ?>
                    <a title="Přihlásit se" href="/login"><i class="fa fa-sign-in"></i> Přihlásit se</a>
                    <a href="/register"><i class="fa fa-user-plus"></i> Registrovat se</a>
                <?php endif; ?>
            </div>
        </div>
        <div class="main-menu">
            <ul>
                <li><a href="/" <?= isset($home_active) ? 'style="color: #436CF2;"' : '' ?>><i class="fa fa-home" style="color: white;"></i> Hlavní stránka</a></li>
                <li><a href="/reservation" <?= isset($reservation_active) ? 'style="color: #436CF2;"' : '' ?>><i style="color: white;" class="fa fa-plane"></i> Rezervace letenek</a></li>
                <li><a href="/contact" <?= isset($contact_active) ? 'style="color: #436CF2;"' : '' ?>><i style="color: white;" class="fa fa-envelope-o"></i> Kontakt</a></li>
                <li class="float-right"><a href="/admin-login" <?= isset($admin_active) ? 'style="color: #436CF2;"' : '' ?>>Administrace <i style="color: white;" class="fa fa-cogs"></i></a></li>
            </ul>
        </div>
    <?php
    // display subcontent
    if (isset($data['content'])){
        require "app/views/".$data['content'].".php";
    }
    ?>
    </div>


</div>


