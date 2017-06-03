<div class="register">
    <div class="register-form">
        <div class="register-text">
            <div class="circle">
                <i style="width: 29px;" class="fa fa-user-plus"></i>
            </div>
            <div class="reg-text">
                <p>Registrace uživatele</p>
            </div>
        </div>
        <div class="form" >
            <form action="/register" accept-charset="utf-8" method="post">
                <div class="input-box">
                    <i class="fa fa-user"></i>
                    <input type="text" placeholder="Uživatel" class="input-element" name="register-name" type="text">
                </div>
                <div class="input-box">
                    <i class="fa fa-lock"></i>
                    <input type="password" placeholder="Heslo" class="input-element" name="register-password" type="text">
                </div>
                <button class="register-button">Registrovat <i class="fa fa-chevron-right"></i></button>
            </form>
        </div>
        <?= (isset($registerMessage) ? '<p class="form-error">' . $registerMessage . '</p>' : '') ?>
        <?= (isset($registerSuccess) ? '<p class="form-success">' . $registerSuccess . '</p>' : '') ?>
    </div>
</div>
