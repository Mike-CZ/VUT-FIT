<div class="login">
    <div class="register-form">
        <div class="register-text">
            <div class="circle">
                <i style="width: 29px;" class="fa fa-sign-in"></i>
            </div>
            <div class="reg-text">
                <p>Přihlášení uživatele</p>
            </div>
        </div>
        <div class="form" >
            <form action="/login" accept-charset="utf-8" method="post">
                <div class="input-box">
                    <i class="fa fa-user"></i>
                    <input type="text" placeholder="Uživatel" class="input-element" name="login-name" type="text">
                </div>
                <div class="input-box">
                    <i class="fa fa-lock"></i>
                    <input type="password" placeholder="Heslo" class="input-element" name="login-password" type="text">
                </div>
                <button class="register-button">Přihlásit se <i class="fa fa-chevron-right"></i></button>
            </form>
        </div>
        <?= (isset($loginError) ? '<p class="form-error">' . $loginError . '</p>' : '') ?>
    </div>
</div>