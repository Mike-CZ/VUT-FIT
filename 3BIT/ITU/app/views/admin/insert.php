<?php
if (isset($db_cols)){
    foreach ($db_cols as $col => $value){
        $$col = $value;
    }
}
?>
<div class="admin-insert">
    <?php if(isset($admin_active)): ?>
        <div class="insert-form">
            <form action="/admin-insert" accept-charset="utf-8" method="post" enctype="multipart/form-data">
                <div style="text-align: left; display: inline-block;">
                    <div class="insert-box">
                        <label for="otazka">Otázka</label>
                        <input type="text" id="otazka" name="otazka" value="<?= $otazka ?>" />
                    </div>
                    <div class="insert-box">
                        <label for="odpoved1">Odpověď 1</label>
                        <input type="text" id="odpoved1" name="odpoved1" value="<?= $odpoved1 ?>" />
                    </div>
                    <div class="insert-box">
                        <label for="odpoved2">Odpověď 2</label>
                        <input type="text" id="odpoved2" name="odpoved2" value="<?= $odpoved2 ?>" />
                    </div>
                    <div class="insert-box">
                        <label for="odpoved3">Odpověď 3</label>
                        <input type="text" id="odpoved3" name="odpoved3" value="<?= $odpoved3 ?>" />
                    </div>
                    <div class="insert-box">
                        <label for="odpoved4">Odpověď 4</label>
                        <input type="text" id="odpoved4" name="odpoved4" value="<?= $odpoved4 ?>" />
                    </div>
                    <div class="insert-box">
                        <label for="obtiznost">Obtížnost</label>
                        <select id="obtiznost" name="obtiznost">
                            <?php foreach($obtiznost_select as $index => $value) {
                              echo '<option ' . ($index == $obtiznost ? 'selected="selected"' : '') . ' value="' . $index . '">' . $value . '</option>';
                            }
                            ?>
                        </select>
                    </div>
                    <div class="insert-box">
                        <label for="spravna_odpoved">Řešení</label>
                        <select id="spravna_odpoved" name="spravna_odpoved">
                            <option value="1" <?= ($spravna_odpoved == 1? 'selected="selected"' : '') ?>>Odpověď 1</option>
                            <option value="2" <?= ($spravna_odpoved == 2? 'selected="selected"' : '') ?>>Odpověď 2</option>
                            <option value="3" <?= ($spravna_odpoved == 3? 'selected="selected"' : '') ?>>Odpověď 3</option>
                            <option value="4" <?= ($spravna_odpoved == 4? 'selected="selected"' : '') ?>>Odpověď 4</option>
                        </select>
                    </div>
                    <div class="insert-box">
                        <label for="image">Obrázek</label>
                        <input type="file" name="image" id="image">
                    </div>
                    <div style="text-align: center;">
                        <div class="insert-box">
                            <button style="width: 250px; height: 40px;" class="register-button">Vlož otázku</button>
                        </div>
                        <?= (isset($uploadErr) ? '<p style="margin-top: 20px;" class="form-error">' . $uploadErr . '</p>' : '') ?>
                        <?= (isset($uploadSuccess) ? '<p style="margin-top: 20px;" class="form-success">' . $uploadSuccess . '</p>' : '') ?>
                    </div>
                    
                </div>
            </form>
        </div>
        <div class="back-button" style="text-align: left; padding-left: 10px;">
            <a class="admin-button" href="/admin">
                <div>
                    Zpět
                </div>
            </a>
        </div>
    <?php else: ?>
        Chyba při zpracování požadavku
    <?php endif; ?>
</div>