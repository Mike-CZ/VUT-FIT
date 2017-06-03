<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>

<div class="subcontent">

<?php

if (isset($user)) {
    if (isset($uzivatel)) { ?>

        <p style="color: white; font-size: 13pt; font-weight: bold;">Profil uživatele <?= $uzivatel[0]->login ?>:</p>
        <br />
        <table>
            <tbody>
                <tr>
                    <td>Jméno: </td>
                    <td><?= ($uzivatel[0]->jmeno != '' ? $uzivatel[0]->jmeno : '<span style=" font-style: italic">Nezadáno</span>') ?></td>
                </tr>
                <tr>
                    <td>Narozen: </td>
                    <td><?= ($uzivatel[0]->narozen != '0000-00-00' ? $uzivatel[0]->narozen : '<span style=" font-style: italic">Nezadáno</span>') ?></td>
                </tr>
                <tr>
                    <td>Rodné číslo: </td>
                    <td><?= ($uzivatel[0]->rodne_cislo != '' && $uzivatel[0]->rodne_cislo != '0' ? $uzivatel[0]->rodne_cislo : '<span style=" font-style: italic">Nezadáno</span>') ?></td>
                </tr>
                <tr>
                    <td>Registrován: </td>
                    <td><?= $uzivatel[0]->datum_registrace ?></td>
                </tr>
                <tr>
                    <td colspan="2" style="text-align: center;"><a href="/profil-edit?id=<?= $uzivatel[0]->id ?>">Upravit profil</a></td>
                </tr>
            </tbody>
        </table>
        <br />
        <p style="color: white; font-size: 13pt; font-weight: bold; text-align: center;">Moje letenky</p>
        <br />
        <?php if (count($moje_lety) > 0) : ?>
            <div class="result-table">
                <table id="flights-table">
                    <thead>
                    <tr>
                        <th>Číslo letenky</th>
                        <th>Datum</th>
                        <th>Čas odletu</th>
                        <th>Destinace</th>
                        <th>Třída</th>
                        <th>Cena</th>
                        <th>Detail</th>
                        <th>Zrušit</th>
                    </tr>
                    </thead>
                    <tbody>
                    <?php foreach ($moje_lety as $result){
                        $today_time = strtotime(date("Y-m-d"));
                        $flight_time = strtotime($result->odlet);

                        echo '<tr><td>' . $result->letenka_id . '</td><td>' . $result->odlet . '</td><td>' . $result->cas_odletu . '</td><td>' . $result->cil . '</td><td>' . $class[$result->trida] . '</td><td>' . $result->cena . ' Kč' . '</td><td><a href="/detail?detail=1&id=' . $result->id .'">Detail</a></td><td>' . ( ($today_time - $flight_time > 0) ? '<span style="color: black; font-style: italic;">Po terminu</span>' : '<a onclick="remove_record(' . $result->letenka_id . '); return false;" href="/">Zrušit</a></td>');
                    } ?>
                    </tbody>
                </table>
            </div>
        <?php else: ?>
            <p style="text-align: center;">Nemáte rezervován žádný let</p>
        <?php endif; ?>
    <?php }
    else {
        echo "<p>Pro zobrazení profilu se musíte <a href='/login'>přihlásit</a>.</p>";
    }
}
else {
    echo "<p>Pro zobrazení profilu se musíte <a href='/login'>přihlásit</a>.</p>";
}

?>
</div>

<script>
    function remove_record(id) {
        if (confirm('Opravdu chcete zrušit rezervaci?')) {
            window.location = "/admin-delete?id=" + id + "&section=letenky";
        }
    }
</script>