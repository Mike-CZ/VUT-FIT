<div class="subcontent">

<?php if (isset($admin_users) && count($admin_users) > 0) : ?>

    <div class="result-table">
        <table>
            <thead>
            <tr>
                <td>Id</td>
                <td>Login</td>
                <td>Jméno</td>
                <td>Rodné číslo</td>
                <td>Narozen</td>
                <td>Datum registrace</td>
                <td>Upravit</td>
                <td>Smazat</td>
            </tr>
            </thead>
            <tbody>
            <?php foreach ($admin_users as $result){
                echo '<tr><td>' . $result->id . '</td><td>' . $result->login . '</td><td>' . ($result->jmeno == '' ? 'Neuvedeno' : $result->jmeno) . '</td><td>' . (($result->rodne_cislo == '0' || $result->rodne_cislo == '') ? 'Neuvedeno' : $result->rodne_cislo) . '</td><td>' . ($result->narozen == '0000-00-00' ? 'Neuvedeno' : $result->narozen) . '</td><td>' . $result->datum_registrace . '</td><td><a href="/profil-edit?id='. $result->id . '">Upravit</a></td><td><a href="" onclick="remove_record(' . $result->id . '); return false;">Smazat</a></td>';
            } ?>
            </tbody>
        </table>
    </div>

<?php else: ?>
<p style="text-align: center">Žádní uživatelé nebyli nalezeni</p>
<?php endif; ?>

<br />
<a href="/admin">Zpět</a>
</div>


<script>
    function remove_record(id) {
        if (confirm('Opravdu chcete smazat uživatele? Všechny letenky zakoupené tímto uživatelem budou smazány')) {
            $.ajax({url: '/admin-delete?section=users&id=' + id,
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