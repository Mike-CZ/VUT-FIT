

<div class="subcontent">
<?php if(!isset($user)): ?>
    <form id="loginForm" action="/login" method="post">

        <table>
            <tbody>
            <tr>
                <td>
                    <label for="login-form">Login* </label>
                </td>
                <td>
                    <input id="login-form" name="login" value="<?= $login ?>" />
                </td>
            </tr>
            <tr>
                <td>
                    <label for="password-form">Heslo*</label>
                </td>
                <td>
                    <input type="password" id="password-form" name="password" value="<?= $password ?>" />
                </td>
            </tr>
            <tr>
                <td colspan="2" style="text-align: center; padding: 10px 0 0 0;">
                    <button>Přihlásit se</button>
                </td>
            </tr>
            </tbody>
        </table>
    </form>
    <?= isset($loginError)? '<p>'.$loginError.'</p>' : '' ?>

<?php else: ?>
    Byl jste úspěšně přihlášen
    <script>
        var delay = 2000;
        setTimeout(function(){ window.location = "/"; }, delay);
    </script>
<?php endif; ?>
</div>
<script>
    $('#loginForm > button').on('click', function(event){
        if ($('#login-form').val().length == 0){
            alert('Musíte vyplnit login!');
            event.preventDefault();
        }
        else if($('#password-form').val().length == 0){
            alert('Musíte vyplnit heslo!');
            event.preventDefault();
        }
    })
</script>