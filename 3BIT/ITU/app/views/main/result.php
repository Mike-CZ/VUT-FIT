<div class="result">
<?php if(isset($spravne_odpovedi)) : ?>
    <div class="score-box">
        <div class="correct-answers">
            Počet správných odpovědí: <?= $spravne_odpovedi ?>/<?= $otazek ?>
        </div>
        <div class="score" >
            Skóre: <?= $skore ?>
        </div>
        <div class="test-button" style="font-size: 16px; margin-top: 10px;">
            <a class="test-main-button" href="/test-menu">
                <div>
                    Zkusit znovu
                </div>
            </a>
        </div>
    </div>

<?php else: ?>
    Chyba při zpracování požadavku
<?php endif; ?>
</div>