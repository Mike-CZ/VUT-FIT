<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>

<div class="subcontent">
    <div id="setgates-insert-box">
        <form id="insert-setgates" method="POST" accept-charset="utf-8" action="/admin-setgates">
            <div class="form-content">
                <table>
                    <tbody>
                    <tr>
                        <td>
                            <label for="gateID">Povolit z gate:</label>
                        </td>
                        <td>
                            <select id="gateID" name="gate_id" >
                                <option value="0">(Vyberte)</option>
                                <?php foreach($gates as $type){
                                    echo '<option value="' . $type->id . '" ' . ($type->id == $gate_id ? 'selected="selected"' : '') .' >' . $type->gate_nazev . ' - ' . $type->terminal_nazev . '</option>';
                                } ?>
                            </select>
                        </td>
                        <?php if (isset($errors) && isset($errors['gate_id'])): ?>
                            <td><span class="error"><?= $errors['gate_id'] ?></span></td>
                        <?php endif; ?>

                    </tr>
                    <tr>
                        <td>
                            <label for="typID">Typ letadla:</label>
                        </td>
                        <td>
                            <select id="typID" name="typ_letadla_id" >
                                <option value="0">(Vyberte)</option>
                                <?php foreach($types as $type){
                                    echo '<option value="' . $type->id . '" ' . ($type->id == $typ_letadla_id ? 'selected="selected"' : '') .' >' . $type->nazev . '</option>';
                                } ?>
                            </select>
                        </td>
                        <?php if (isset($errors) && isset($errors['typ_letadla_id'])): ?>
                            <td><span class="error"><?= $errors['typ_letadla_id'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td colspan="2" style="text-align: center; padding: 10px;">
                            <button style="padding: 2px;">Povolit odlety z gate!</button>
                        </td>
                    </tr>
                    </tbody>
                </table>
                <?= (isset($insertResult) ? '<p class="insertResult">' . $insertResult . '</p>' : '') ?>
            </div>
        </form>
    </div>
    <?php if (isset($gate_result)) : ?>
        <div class="result-table">
            <table>
                <thead>
                <tr>
                    <td>Gate</td>
                    <td>Typ letadla</td>
                    <td>Akce</td>
                </tr>
                </thead>
                <tbody>
                <?php foreach ($gate_result as $result){
                    echo '<tr><td>' . $result->gate_nazev . ' - ' . $result->terminal_nazev . '</td><td>' . $result->typ_nazev . '</td><td><a onclick="remove_record(' . $result->id . '); return false;" href="">Smazat</a></td>';
                } ?>
                </tbody>
            </table>
        </div>
    <?php endif; ?>
    <br />
    <a href="/admin">Zpět</a>
</div>

<script>
    function remove_record(id) {
        if (confirm('Opravdu chcete smazat položku?')) {
            $.ajax({url: '/admin-delete?section=setgates&id=' + id,
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
