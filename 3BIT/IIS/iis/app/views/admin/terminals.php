<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}?>

<div class="subcontent">
    <div id="plane-type-insert-box">
        <form id="insert-terminals" method="POST" accept-charset="utf-8" action="/admin-terminals">
            <div class="form-content">
                <table>
                    <tbody>
                    <tr>
                        <td>
                            <label for="terminal-name">Název: </label>
                        </td>
                        <td>
                            <input id="terminal-name" name="nazev" value="<?= $nazev ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['nazev'])): ?>
                            <td><span class="error"><?= $errors['nazev'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td>
                            <label for="cap">Kapacita: </label>
                        </td>
                        <td>
                            <input id="cap" name="kapacita" value="<?= $kapacita ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['kapacita'])): ?>
                            <td><span class="error"><?= $errors['kapacita'] ?></span></td>
                        <?php endif; ?>
                    </tr>
                    <tr>
                        <td>
                            <label for="parkingSlots">Počet parkovacích míst: </label>
                        </td>
                        <td>
                            <input id="parkingSlots" name="parkovacich_mist" value="<?= $parkovacich_mist ?>" />
                        </td>
                        <?php if (isset($errors) && isset($errors['parkovacich_mist'])): ?>
                            <td><span class="error"><?= $errors['parkovacich_mist'] ?></span></td>
                        <?php endif; ?>

                    </tr>
                    <tr>
                        <td colspan="2" style="text-align: center; padding: 10px;">
                            <button style="padding: 2px;">Vytvořit terminál</button>
                        </td>
                    </tr>
                    </tbody>
                </table>
                <?= (isset($insertResult) ? '<p class="insertResult">' . $insertResult . '</p>' : '') ?>
            </div>
        </form>
    </div>
    <?php if (isset($terminal_result) && count($terminal_result)) : ?>
        <div class="result-table">
            <table>
                <thead>
                <tr>
                    <td>Název</td>
                    <td>Kapacita</td>
                    <td>Parkovacích míst</td>
                    <td>Akce</td>
                </tr>
                </thead>
                <tbody>
                <?php foreach ($terminal_result as $result){
                    echo '<tr><td>' . $result->nazev . '</td><td>' . $result->kapacita . '</td><td>' . $result->parkovacich_mist . '</td><td><a href="" onclick="remove_record(' . $result->id . '); return false;">Smazat</a></td>';
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
            $.ajax({url: '/admin-delete?section=terminals&id=' + id,
                    success: function(output) {
                        if (output == "-1") {
                            alert('Chyba při zpracování požadavku');
                        }
                        else if (output == "-2") {
                            alert('Terminál nelze odstranit, protože k tomuto terminálu je přiřazena Gate!');
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
