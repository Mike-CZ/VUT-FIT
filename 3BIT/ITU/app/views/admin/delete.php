<div class="admin-delete">
    <?php if(isset($admin_active)): ?>
        <div class="back-button" style="text-align: left; padding-left: 10px;">
            <a class="admin-button" href="/admin">
                <div>
                    Zpět
                </div>
            </a>
        </div>
        <div class="delete">
        <?php if(!empty($items)) : ?>
            <?php foreach($items as $result) : ?>
                <div class="delete-box">
                    <div style="height: 155px;">
                        <div class="delete-box-image-box">
                            <div class="delete-box-image">
                                <img style="max-height: 150px; max-width:200px;" src="<?= 'images/upload/' . $result->obrazek ?>"/>
                            </div>
                        </div>
                    </div>
                    <div class="delete-box-question">
                        <div style="display: table-cell; vertical-align: middle; text-align: center;">
                            <?= $result->otazka ?>
                        </div>
                    </div>
                    <div class="delete-box-options">
                        <table class="delete-box-options-table">
                            <tbody>
                            <tr>
                                <td <?= ($result->spravna_odpoved == 1 ? 'class="bold"' : '' ) ?>>
                                    <?= $result->odpoved1 ?>
                                </td>
                                <td <?= ($result->spravna_odpoved == 2 ? 'class="bold"' : '' ) ?>>
                                    <?= $result->odpoved2 ?>
                                </td>
                            </tr>
                            <tr>
                                <td <?= ($result->spravna_odpoved == 3 ? 'class="bold"' : '' ) ?>>
                                    <?= $result->odpoved3 ?>
                                </td>
                                <td <?= ($result->spravna_odpoved == 4 ? 'class="bold"' : '' ) ?>>
                                    <?= $result->odpoved4 ?>
                                </td>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                    <div class="difficulty">
                        Obtížnost: <?= $obtiznost_select[$result->obtiznost] ?>
                    </div>
                    <div class="back-button">
                        <a class="admin-button" onclick="remove_record(<?= $result->id ?>); return false;" href="#">
                            <div style="width: 50%; height: 10px; line-height: 10px;">
                                Smazat
                            </div>
                        </a>
                    </div>
                </div>
            <?php endforeach; ?>
            </div>
            <div class="back-button" style="text-align: left; padding-left: 10px;">
                <a class="admin-button" href="/admin">
                    <div>
                        Zpět
                    </div>
                </a>
            </div>
        <?php else: ?>
            Žádné záznamy nebyly nalezeny
        <?php endif; ?>
    <?php else: ?>
        Chyba při zpracování požadavku
    <?php endif; ?>
</div>

<script>
    <?= (isset($delete_message) ? 'alert("' . $delete_message . '");' : '') ?>
    function remove_record(id) {
        if (confirm('Opravdu chcete smazat položku?')) {
            window.location.replace("/admin-delete?id=" + id);
        }
    }
</script>