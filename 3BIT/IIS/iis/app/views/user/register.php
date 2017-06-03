<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>

<div class="subcontent">
    <form id="register-form" action="/register" method="post">
        <table>
            <tbody>
            <tr>
                <td>
                    <label for="login-form">Login*</label>
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
                <td>
                    <label for="name-form">Jméno a příjmení</label>
                </td>
                <td>
                    <input id="name-form" name="jmeno" value="<?= $jmeno ?>" />
                </td>
            </tr>
            <tr>
                <td>
                    <label for="number-form">Rodné číslo</label>
                </td>
                <td>
                    <input id="number-form" name="rodne_cislo" value="<?= $rodne_cislo ?>" />
                </td>
            </tr>
            <tr>
                <td>
                    <label for="birth-form">Datum narození</label>
                </td>
                <td>
                    <input id="birth-form" name="narozen" value="<?= $narozen ?>" />
                </td>
            </tr>
            <tr>
                <td colspan="2" style="text-align: center; padding: 10px 0 0 0;">
                    <button>Registrovat</button>
                </td>
            </tr>
            </tbody>
        </table>
    </form>
    <?= isset($registerMessage)? '<p style="padding: 5px;">'.$registerMessage.'</p>' : '' ?>
</div>
<script>
    $(function() {
        $( "#birth-form" ).datepicker( $.datepicker.regional[ "cs" ] );
        $( "#birth-form" ).datepicker( "option", "showAnim", "drop");
        $( "#birth-form" ).datepicker( "option", "dateFormat", "yy-mm-dd");
        $( "#birth-form" ).datepicker( "option", "changeYear", "true");
        $( "#birth-form" ).datepicker( "option", "changeMonth", "true");
        $( "#birth-form" ).datepicker( "option", "yearRange", "1900:2015");
        <?= ((isset($narozen) && strtotime($narozen))? '$( "#birth-form" ).datepicker( "setDate", "'.$narozen.'" );' : '') ?>

    });
    $('#register-form > button').on('click', function(event){
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