<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>
<div class="subcontent">
    <?php if (isset($flights) && count($flights)) : ?>
        <div class="result-table">
            <table id="flights-table">
                <thead>
                <tr>
                    <th>Datum</th>
                    <th>Čas odletu</th>
                    <th>Destinace</th>
                    <th>Doba letu</th>
                    <th>Gate</th>
                    <th>Letadlo</th>
                    <th>Zbývá míst</th>
                    <th>Objednat</th>
                </tr>
                </thead>
                <tbody>
                <?php foreach ($flights as $result){
                    echo '<tr><td>' . $result->odlet . '</td><td>' . $result->cas_odletu . '</td><td>' . $result->cil . '</td><td>' . $result->doba_letu . " min" . '</td>' .
                        '<td>' . $result->gate_nazev . '</td><td>' . $result->vyrobce . " " . $result->typ_nazev . '</td><td>' . ((int)$result->pocet_sedadel - ((int)$result->pocet_letenek)) . '</td><td><a href="/detail?id=' . $result->id .'">Zobrazit</a></td>';
                } ?>
                </tbody>
            </table>
        </div>
    <?php else: ?>
        <p style="text-align: center;">Žádné lety nebyly nalezeny</p>
    <?php endif; ?>
</div>

<script>
    $(document).ready(function() {
            $("#flights-table").tablesorter();
        }
    );
</script>