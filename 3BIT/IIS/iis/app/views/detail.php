<div class="subcontent">

    <?php if (isset($let) && !empty($let)) :
        $tmp_cena = $let[0]->cena;
        $id = $let[0]->id;
        $pocet_mist =  $let[0]->pocet_sedadel;
        $prodanych_vstupenek = $pocet_letenek[0]->pocet;
        $today_time = strtotime(date("Y-m-d"));
        $flight_time = strtotime($let[0]->odlet);
        ?>
        <table>
            <tbody>
        <?php
        foreach ($let as $col) : ?>
            <tr>
                <td>Číslo letu:</td>
                <td><?= $col->id ?></td>
            </tr>
            <tr>
                <td>Destinace:</td>
                <td><?= $col->cil ?></td>
            </tr>
            <tr>
                <td>Datum:</td>
                <td><?= $col->odlet ?></td>
            </tr>
            <tr>
                <td>Čas:</td>
                <td><?= $col->cas_odletu ?></td>
            </tr>
            <tr>
                <td>Doba letu:</td>
                <td><?= $col->doba_letu . ' minut'?></td>
            </tr>
            <tr>
                <td>Gate:</td>
                <td><?= $col->gate_nazev ?></td>
            </tr>
            <tr>
                <td>Terminál:</td>
                <td><?= $col->terminal_nazev ?></td>
            </tr>
            <?php if ($zobraz_detail != '1') : ?>
            <tr>
                <td>Základní cena:</td>
                <td><?= $col->cena . ' Kč' ?></td>
            </tr>
            <?php endif; ?>
            <tr>
                <td>Letadlo:</td>
                <td><?= $col->vyrobce . ' ' . $col->typ_nazev ?></td>
            </tr>
        <?php endforeach; ?>
        </tbody>
    </table>
        <?php if ($zobraz_detail != '1') : ?>
        <br />
        <p>Volných míst: <?= $pocet_mist - $prodanych_vstupenek ?></p>
        <br />
        <?php if (isset($user)) :
            if ( ! ($today_time - $flight_time > 0) && ! (($pocet_mist - $prodanych_vstupenek) <= 0)) {
                if (!isset($success)) : ?>
                    <form id="insert-plane" method="POST" accept-charset="utf-8" target="/detail">
                        <table>
                            <tr>
                                <td>Třída</td>
                                <td>
                                    <select id="class" name="trida">
                                        <option value="0">(Vyberte)</option>
                                        <?php foreach ($class as $index => $type) {
                                            echo '<option value="' . $index . '">' . $type . '</option>';
                                        } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td style="padding-top: 5px;">Cena:</td>
                                <td style="padding-top: 5px;" id="cena"> <?= $tmp_cena . ' Kč' ?> </td>
                            </tr>
                            <tr>
                                <td style="text-align: center; padding-top: 5px;" colspan="2">
                                    <button>Rezervuj!</button>
                                </td>
                            </tr>
                        </table>
                        <input name="let_id" type="hidden" value="<?= $id ?>"/>
                        <input id="cena_final" name="cena_final" type="hidden" value="<?= $tmp_cena ?>"/>
                    </form>
                    <?php if (isset($err)) {
                        echo '<br />';
                        echo '<p style="color: white;">' . $err . '</p>';
                    } ?>
                <?php else: ?>
                    <p style="color: #330066;"><?= $success ?></p>
                <?php endif;
            }
            else if (($pocet_mist - $prodanych_vstupenek) <= 0) { ?>
                <p style="color: #fff;">Letenku nelze zarezervovat, všechna místa jsou již obsazena!</p>
            <?php }
            else { ?>
                <p style="color: #fff;">Letenku nelze zarezervovat, protože let již proběhl!</p>
            <?php }?>
        <?php else: ?>
            <p>Pro zakoupení letenky musíte být přihlášen!</p>
        <?php endif; ?>

        <br />
        <p><a href="/reservation">Zpět na rezervaci letenek</a></p>

        <?php else: ?>
        <br />
        <p><a href="/profil">Zpět na profil</a></p>
        <?php endif; ?>
    <?php else: ?>
        <p>Let nebyl nalezen</p>
        <br />
        <p><a href="/reservation">Zpět na rezervaci letenek</a></p>
    <?php endif; ?>
</div>


<script>
    var cena = <?= $tmp_cena ?>;
    $('#class').on('change', function(){
       switch (this.value) {
           case "1":
               $('#cena').html(cena + 2500 + " Kč");
               $('#cena_final').val(cena + 2500);
               break;
           case "2":
               $('#cena').html(cena + 1200 + " Kč");
               $('#cena_final').val(cena + 1200);
               break;

           case "3":
               $('#cena').html(cena + " Kč");
               $('#cena_final').val(cena);
               break;
           default:
               $('#cena').html(cena + " Kč");
               $('#cena_final').val(cena);
               break;
       }
    });

</script>