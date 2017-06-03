<?php
?>

<div class="subcontent">
    <div id="admin-login">
        <form action="/admin-login" method="post">
            <label for="admin-name">Login: </label>
            <input id="admin-name" name="admin-name" />
            <label for="admin-password">Heslo: </label>
            <input type="password" id="admin-password" name="admin-password"/>
            <button>Přihlásit se</button>
            <?= isset($admin_message) ? '<p>'.$admin_message.'</p>' : ''; ?>
        </form>
    </div>
</div>