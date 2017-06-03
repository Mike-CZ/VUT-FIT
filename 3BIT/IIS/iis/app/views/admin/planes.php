<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}?>

<div class="subcontent">
    <div id="plane-insert-box">
        <form id="insert-plane" method="POST" accept-charset="utf-8" action="/admin-planes">
            <div class="form-content">
                <table>
                    <tbody>
                    <tr>
                        <td>
                            <label for="vyrobce">Výrobce: </label>
                        </td>
                        <td>
                            <input id="vyrobce" name="vyrobce" value="<?= $vyrobce ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['vyrobce'])): ?>
                            <td><span class="error"><?= $errors['vyrobce'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td>
                            <label for="typ_id">Typ letadla: </label>
                        </td>
                        <td>
                            <select id="typ_id" name="typ_id" >
                                <option value="0">(Vyberte)</option>
                                <?php foreach($planeTypes as $type){
                                    echo '<option value="' . $type->id . '" ' . ($typ_id == $type->id ? 'selected="selected"' : '') .' >' . $type->nazev . '</option>';
                                } ?>
                            </select>
                        </td>
                        <?php if (isset($errors) && isset($errors['typ_id'])): ?>
                            <td><span class="error"><?= $errors['typ_id'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td>
                            <label for="pocet_clenu">Počet členů: </label>
                        </td>
                        <td>
                            <input id="pocet_clenu" name="pocet_clenu" value="<?= $pocet_clenu ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['pocet_clenu'])): ?>
                            <td><span class="error"><?= $errors['pocet_clenu'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td>
                            <label for="vyroba">Datum výroby: </label>
                        </td>
                        <td>
                            <input id="vyroba" name="vyroba" value="<?= $vyroba ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['vyroba'])): ?>
                            <td><span class="error"><?= $errors['vyroba'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td>
                            <label for="revize">Poslední revize: </label>
                        </td>
                        <td>
                            <input id="revize" name="revize" value="<?= $revize ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['revize'])): ?>
                            <td><span class="error"><?= $errors['revize'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td colspan="2" style="text-align: center; padding: 10px;">
                            <button style="padding: 2px;">Vytvořit letadlo</button>
                        </td>
                    </tr>
                    </tbody>
                </table>
                <?= (isset($insertResult) ? '<p class="insertResult">' . $insertResult . '</p>' : '') ?>
            </div>
        </form>
    </div>
    <?php if (isset($planes_result)) : ?>
        <div class="result-table">
            <table>
                <thead>
                <tr>
                    <td>Výrobce</td>
                    <td>Typ letadla</td>
                    <td>Počet členů</td>
                    <td>Datum výroby</td>
                    <td>Datum poslední revize</td>
                    <td>Akce</td>
                </tr>
                </thead>
                <tbody>
                <?php foreach ($planes_result as $result){
                    echo '<tr><td>' . $result->vyrobce . '</td><td>' . $result->nazev . '</td><td>' . $result->pocet_clenu . '</td><td>' . $result->vyroba . '</td><td>' . $result->revize . '</td><td><a onclick="remove_record(' . $result->id . '); return false;" href="">Smazat</a></td>';
                } ?>
                </tbody>
            </table>
        </div>
    <?php endif; ?>
    <br />
    <a href="/admin">Zpět</a>
</div>

<script>
    $(function() {
        $( "#vyroba" ).datepicker( $.datepicker.regional[ "cs" ] );
        $( "#vyroba" ).datepicker( "option", "showAnim", "drop");
        $( "#vyroba" ).datepicker( "option", "dateFormat", "yy-mm-dd");
        <?= ((isset($vyroba) && strtotime($vyroba))? '$( "#vyroba" ).datepicker( "setDate", "'.$vyroba.'" );' : '') ?>

    });
    $(function() {
        $( "#revize" ).datepicker( $.datepicker.regional[ "cs" ] );
        $( "#revize" ).datepicker( "option", "showAnim", "drop");
        $( "#revize" ).datepicker( "option", "dateFormat", "yy-mm-dd");
        <?= ((isset($revize) && strtotime($revize))? '$( "#revize" ).datepicker( "setDate", "'.$revize.'" );' : '') ?>

    });

    function remove_record(id) {
        if (confirm('Opravdu chcete smazat Letadlo?')) {
            $.ajax({url: '/admin-delete?section=planes&id=' + id,
                    success: function(output) {
                        if (output == "-1") {
                            alert('Chyba při zpracování požadavku');
                        }
                        else if (output == "-2") {
                            alert('Letadlo nelze odstranit, protože letadlo má přiřazen let!!');
                        }
                        else{
                            if (output.length == 0) {
                                $('.result-table').css('display', 'none');
                            }
                            else {
                                $('.result-table tbody').html(output);
                            }
                        }
                    },
                    error: function () {
                        alert("Nepodařilo se odstranit záznam");
                    }}
            );
            $('.insertResult').text('');
        }
    }
</script>