<?php
  $index = 1;
?>

<div class="top-score">

    <?php if(isset($spravne_odpovedi) && !empty($spravne_odpovedi)) : ?>
        <h2>Top 10 hráčů</h2>
        <table class="result-table">
            <thead>
                <tr>
                    <th>#</th>
                    <th>Login</th>
                    <th>Skóre</th>
                </tr>
            </thead>
            <tbody>
            <?php foreach($spravne_odpovedi as $res): ?>
                <tr>
                    <td><?= $index ?></td>
                    <td><?= $res->login ?></td>
                    <td><?= $res->score ?></td>
                </tr>
                <?php
                ++$index;
                ?>
            <?php endforeach; ?>
            </tbody>
        </table>
    <?php else: ?>

    <?php endif; ?>

</div>