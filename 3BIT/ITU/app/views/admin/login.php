<div class="admin-login">
    <div class="register-form">
        <div class="register-text">
            <div class="circle">
                <i style="width: 29px;" class="fa fa-wrench"></i>
            </div>
            <div class="reg-text">
                <p>Přihlášení do administrace</p>
            </div>
        </div>
        <div class="form" >
            <form action="/admin-login" accept-charset="utf-8" method="post">
                <div class="input-box">
                    <i class="fa fa-user"></i>
                    <input type="text" placeholder="Admin" class="input-element" name="admin-name" type="text">
                </div>
                <div class="input-box">
                    <i class="fa fa-lock"></i>
                    <input type="password" placeholder="Heslo" class="input-element" name="admin-password" type="text">
                </div>
                <button class="register-button">Přihlásit se <i class="fa fa-chevron-right"></i></button>
            </form>
        </div>
        <?= (isset($admin_message) ? '<p class="form-error">' . $admin_message . '</p>' : '') ?>
    </div>
</div>