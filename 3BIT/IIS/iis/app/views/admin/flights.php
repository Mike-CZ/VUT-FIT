<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>

<div class="subcontent">
    <div id="flight-insert-box">
        <form id="insert-flight" method="POST" accept-charset="utf-8" action="/admin-flights">
            <div class="form-content">
                <table>
                    <tbody>
                    <tr>
                        <td>
                            <label for="flight-to">Destinace:</label>
                        </td>
                        <td>
                            <input id="flight-to" name="cil" value="<?= $cil ?>" />

                        </td>
                        <?php if (isset($errors) && isset($errors['cil'])): ?>
                            <td><span class="error"><?= $errors['cil'] ?></span></td>
                        <?php endif; ?>

                    <tr>
                    <tr>
                        <td>
                            <label for="flight-date">Datum odletu:</label>
                        </td>
                        <td>
                            <input id="flight-date" name="odlet" value="<?= $odlet ?>" />

                        </td>
                        <?php if (isset($errors) && isset($errors['odlet'])): ?>
                            <td><span class="error"><?= $errors['odlet'] ?></span></td>
                        <?php endif; ?>

                    <tr>
                    <tr>
                        <td>
                            <label for="time">Čas:</label>
                        </td>
                        <td>
                            <input id="time" name="cas_odletu" value="<?= $cas_odletu ?>" />

                        </td>
                        <?php if (isset($errors) && isset($errors['cas_odletu'])): ?>
                            <td><span class="error"><?= $errors['cas_odletu'] ?></span></td>
                        <?php endif; ?>

                    <tr>
                    <tr>
                        <td>
                            <label for="price">Cena (Kč):</label>
                        </td>
                        <td>
                            <input id="price" name="cena" value="<?= $cena ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['cena'])): ?>
                            <td><span class="error"><?= $errors['cena'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td>
                            <label for="flight-time">Doba letu (min):</label>
                        </td>
                        <td>
                            <input id="flight-time" name="doba_letu" value="<?= $doba_letu ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['doba_letu'])): ?>
                            <td><span class="error"><?= $errors['doba_letu'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td>
                            <label for="gateID">Gate:</label>
                        </td>
                        <td>
                            <select id="gateID" name="gate_id" >
                                <option value="0">(Vyberte)</option>
                                <?php foreach($gates as $type){
                                    echo '<option value="' . $type->id . '" ' . ($type->id == $gate_id ? 'selected="selected"' : '') .' >' .  $type->gate_nazev . ' - ' . $type->terminal_nazev . '</option>';
                                } ?>
                            </select>
                        </td>
                        <?php if (isset($errors) && isset($errors['gate_id'])): ?>
                            <td><span class="error"><?= $errors['gate_id'] ?></span></td>
                        <?php endif; ?>

                    </tr>
                    <tr>
                        <td>
                            <label for="plane_id">Letadlo:</label>
                        </td>
                        <td>
                            <select id="plane_id" name="letadlo_id" >
                                <option value="0">(Vyberte)</option>
                                <?php foreach($planes as $type){
                                    echo '<option value="' . $type->id . '" ' . ($type->id == $letadlo_id? 'selected="selected"' : '') .' >' . $type->id . " - "  . $type->vyrobce . " " . $type->nazev . '</option>';
                                } ?>
                            </select>
                        </td>
                        <?php if (isset($errors) && isset($errors['letadlo_id'])): ?>
                            <td><span class="error"><?= $errors['letadlo_id'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td colspan="2" style="text-align: center; padding: 10px;">
                            <button style="padding: 2px;">Vytvořit let!</button>
                        </td>
                    </tr>

                    </tbody>
                </table>
                <?= (isset($insertResult) ? '<p class="insertResult">' . $insertResult . '</p>' : '') ?>
            </div>
        </form>
    </div>
    <?php if (isset($flight_result) && count($flight_result)) : ?>
        <div class="result-table">
            <table>
                <thead>
                <tr>
                    <td>Id</td>
                    <td>Cíl</td>
                    <td>Datum</td>
                    <td>Čas</td>
                    <td>Cena</td>
                    <td>Doba letu</td>
                    <td>Gate</td>
                    <td>Letadlo</td>
                    <td>Zbývá míst</td>
                    <td>Akce</td>
                </tr>
                </thead>
                <tbody>
                <?php foreach ($flight_result as $result){
                    echo '<tr><td>' . $result->id . '</td><td>' . $result->cil . '</td><td>' . $result->odlet . '</td><td>' . $result->cas_odletu . '</td><td>' . $result->cena . ' Kč' . '</td><td>' . $result->doba_letu . " min" . '</td>' .
                        '<td>' . $result->gate_nazev . '</td><td>' . $result->vyrobce . " " . $result->typ_nazev . '</td><td>' . ((int)$result->pocet_sedadel - ((int)$result->vydanych_vstupenek - 1)) . '</td><td><a onclick="remove_record(' . $result->id . '); return false;" href="">Smazat</a></td>';
                } ?>
                </tbody>
            </table>
        </div>
    <?php endif; ?>
    <br />
    <a href="/admin">Zpět</a>
</div>

<script>
    $('#time').timepicker({'disableTextInput' : 'true', 'timeFormat': 'H:i'});
    $(function() {
        $( "#flight-date" ).datepicker( $.datepicker.regional[ "cs" ] );
        $( "#flight-date" ).datepicker( "option", "showAnim", "drop");
        $( "#flight-date" ).datepicker( "option", "dateFormat", "yy-mm-dd");
        <?= ((isset($odlet) && strtotime($odlet))? '$( "#flight-date" ).datepicker( "setDate", "'.$odlet.'" );' : '') ?>
    });

    function remove_record(id) {
        if (confirm('Opravdu chcete smazat let? Všechny letenky na tento let budou smazány!')) {
            $.ajax({url: '/admin-delete?section=let&id=' + id,
                    success: function(output) {
                        if (output == "-1") {
                            alert('Chyba při zpracování požadavku');
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