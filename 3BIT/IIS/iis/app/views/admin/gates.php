<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>

<div class="subcontent">
    <div id="gates-insert-box">
        <form id="gates-type" method="POST" accept-charset="utf-8" action="/admin-gates">
            <div class="form-content">
                <table>
                    <tbody>
                    <tr>
                        <td>
                            <label for="gate-name">Název: </label>
                        </td>
                        <td>
                            <input id="gate-name" name="nazev" value="<?= $nazev ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['nazev'])): ?>
                            <td><span class="error"><?= $errors['nazev'] ?></span></td>
                        <?php endif; ?>
                    <tr>
                        <td>
                            <label for="terminal-name">Terminál: </label>
                        </td>
                        <td>
                            <select id="terminal-name" name="terminal_id" >
                                <option value="0">(Vyberte)</option>
                                <?php foreach($terminals as $type){
                                    echo '<option value="' . $type->id . '" ' . ($terminal_id == $type->id ? 'selected="selected"' : '') .' >' . $type->nazev . '</option>';
                                } ?>
                            </select>
                        </td>
                        <?php if (isset($errors) && isset($errors['terminal_id'])): ?>
                            <td><span class="error"><?= $errors['terminal_id'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td colspan="2" style="text-align: center; padding: 10px;">
                            <button style="padding: 2px;">Vytvořit gate</button>
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
                    <td>Název</td>
                    <td>Terminál</td>
                    <td>Akce</td>
                </tr>
                </thead>
                <tbody>
                <?php foreach ($gate_result as $result){
                    echo '<tr><td>' . $result->gate_name . '</td><td>' . $result->terminal_name . '</td><td><a href="" onclick="remove_record(' . $result->id . '); return false;">Smazat</a></td>';
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
            $.ajax({url: '/admin-delete?section=gates&id=' + id,
                    success: function(output) {
                        if (output == "-1") {
                            alert('Chyba při zpracování požadavku');
                        }
                        else if(output == "-2") {
                            alert('Gate nelze odstranit, protože na tuto položku existují vazby!');
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