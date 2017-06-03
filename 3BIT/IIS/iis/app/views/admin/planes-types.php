<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>
<div class="subcontent">
    <div id="plane-type-insert-box">
        <form id="insert-plane-type" method="POST" accept-charset="utf-8" action="/admin-ptypes">
            <div class="form-content">
                <table>
                    <tbody>
                        <tr>
                            <td>
                                <label for="ptype-name">Název: </label>
                            </td>
                            <td>
                                <input id="ptype-name" name="nazev" value="<?= $nazev ?>" />
                            </td>
                            <?php if (isset($errors) && isset($errors['nazev'])): ?>
                                <td><span class="error"><?= $errors['nazev'] ?></span></td>
                            <?php endif; ?>
                        </tr>
                        <tr>
                            <td>
                                <label for="pseatCount">Počet sedadel: </label>
                            </td>
                            <td>
                                <input id="pseatCount" name="pocet_sedadel" value="<?= $pocet_sedadel ?>" />
                            </td>
                            <?php if (isset($errors) && isset($errors['pocet_sedadel'])): ?>
                                <td><span class="error"><?= $errors['pocet_sedadel'] ?></span></td>
                            <?php endif; ?>
                        </tr>
                        <tr>
                            <td>
                                <label for="pseatType">Rozmístění sedadel: </label>
                            </td>
                            <td>
                                <select id="pseatType" name="typ_rozmisteni" >
                                    <option value="0">(Vyberte)</option>
                                    <?php foreach($seatTypes as $index => $type){
                                        echo '<option value="' . $index . '" ' . ($typ_rozmisteni == $index ? 'selected="selected"' : '') .' >' . $type . '</option>';
                                    } ?>
                                </select>
                            </td>
                                <?php if (isset($errors) && isset($errors['typ_rozmisteni'])): ?>
                            <td><span class="error"><?= $errors['typ_rozmisteni'] ?></span></td>
                                <?php endif; ?>

                        </tr>
                        <tr>
                            <td colspan="2" style="text-align: center; padding: 10px;">
                                <button style="padding: 2px;">Vytvorit typ letadla</button>
                            </td>
                        </tr>
                    </tbody>
                </table>
                <?= (isset($insertResult) ? '<p class="insertResult">' . $insertResult . '</p>' : '') ?>
            </div>
        </form>
    </div>
    <?php if (isset($types_result)) : ?>
    <div class="result-table">
        <table>
            <thead>
            <tr>
                <td>Název</td>
                <td>Počet sedadel</td>
                <td>Rozmístění sedadel</td>
                <td>Akce</td>
            </tr>
            </thead>
            <tbody>
            <?php foreach ($types_result as $result){
                echo '<tr><td>' . $result->nazev . '</td><td>' . $result->pocet_sedadel . '</td><td>' . $seatTypes[$result->typ_rozmisteni] . '</td><td><a href="" onclick="remove_record(' . $result->id . '); return false;">Smazat</a></td>';
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
            $.ajax({url: '/admin-delete?section=ptypes&id=' + id,
                    success: function(output) {
                        if (output == "-1") {
                            alert('Chyba při zpracování požadavku');
                        }
                        else if (output == "-2") {
                            alert('Typ letadla nelze odstranit, protože na tuto položku existují vazby!');
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