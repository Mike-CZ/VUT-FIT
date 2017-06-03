<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>

<div class="subcontent">
<?php if (isset($err)) {
    echo '<p style="text-align: center;">' . $err . '</p>';
}
else{
    if (isset($user_edit) && count($user_edit) > 0) { ?>
        <form id="edit-form" action="/profil-edit?id=<?= $user_id ?>" method="post">
        <table>
            <tbody>
            <tr>
                <td>
                    <label for="name-form">Jméno a příjmení</label>
                </td>
                <td>
                    <input id="name-form" name="jmeno" value="<?= $user_edit[0]->jmeno ?>" />
                </td>
            </tr>
            <tr>
                <td>
                    <label for="number-form">Rodné číslo</label>
                </td>
                <td>
                    <input id="number-form" name="rodne_cislo" value="<?= ($user_edit[0]->rodne_cislo != 0 ?  $user_edit[0]->rodne_cislo : '') ?>" />
                </td>
            </tr>
            <tr>
                <td>
                    <label for="birth-form">Datum narození</label>
                </td>
                <td>
                    <input id="birth-form" name="narozen" value="<?= ($user_edit[0]->narozen != '0000-00-00' ? $user_edit[0]->narozen : '') ?>" />
                </td>
            </tr>
            <tr>
                <td colspan="2" style="text-align: center; padding: 10px 0 0 0;">
                    <button>Upravit</button>
                </td>
            </tr>
            </tbody>
        </table>
    </form>
        <?= isset($editMessage)? '<br /><p style="padding: 5px;">'.$editMessage.'</p>' : '' ?>
        <br />
        <a href="<?= (isset($user)? '/profil' : '/admin-users') ?>">Zpět</a>
    <?php }
    else {
        echo '<p style="text-align: center;">Nastala chyba při editaci uživatele</p>';
    }
} ?>

</div>

<script>
    $(function() {
        $( "#birth-form" ).datepicker( $.datepicker.regional[ "cs" ] );
        $( "#birth-form" ).datepicker( "option", "showAnim", "drop");
        $( "#birth-form" ).datepicker( "option", "dateFormat", "yy-mm-dd");
        $( "#birth-form" ).datepicker( "option", "changeYear", "true");
        $( "#birth-form" ).datepicker( "option", "changeMonth", "true");
        $( "#birth-form" ).datepicker( "option", "yearRange", "1900:2015");
        <?= ((isset($user_edit[0]->narozen) && strtotime($user_edit[0]->narozen))? '$( "#birth-form" ).datepicker( "setDate", "'.($user_edit[0]->narozen != '0000-00-00' ? $user_edit[0]->narozen : '').'" );' : '') ?>

    });
</script>