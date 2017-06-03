<?php
$index = 1;
?>

<div class="test">
    <?php if (isset($questions) && !empty($questions)): ?>
        <div class="test-countdown">
            <div id="countdown-box">

            </div>
        </div>
        <div id="start-button" style="padding-top: 150px;">
            <h2>Váš test je připraven</h2>
            <div class="test-button" style="margin-top: 10px;">
                <a class="test-main-button" onclick="startTest(); return false;" href="#">
                    <div>
                        Zahájit test
                    </div>
                </a>
            </div>
        </div>
        <div class="test-box">
            <form id="test-hidden-form" accept-charset="utf-8" method="post" action="/result">
        <?php foreach($questions as $result): ?>
            <div id="box-<?= $index ?>" style="display: none;">
                <div class="test-image-box">
                    <img class="test-image" src="<?= 'images/upload/' . $result->obrazek ?>">
                </div>
                <div class="test-question">
                    <?= $result->otazka ?>
                </div>
                <div class="test-answers">
                    <table class="test-box-options-table">
                        <tbody>
                        <tr>
                            <td>
                                <div class="test-button" style="text-align: left; padding-left: 10px;">
                                    <a class="test-main-button" onclick="answer_click(<?= $result->id ?>, 1); return false;" href="#">
                                        <div>
                                            <?= $result->odpoved1 ?>
                                        </div>
                                    </a>
                                </div>
                            </td>
                            <td>
                                <div class="test-button" style="text-align: left; padding-left: 10px;">
                                    <a class="test-main-button" onclick="answer_click(<?= $result->id ?>, 2); return false;" href="#">
                                        <div>
                                            <?= $result->odpoved2 ?>
                                        </div>
                                    </a>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div class="test-button" style="text-align: left; padding-left: 10px;">
                                    <a class="test-main-button" onclick="answer_click(<?= $result->id ?>, 3); return false;" href="#">
                                        <div>
                                            <?= $result->odpoved3 ?>
                                        </div>
                                    </a>
                                </div>
                            </td>
                            <td>
                                <div class="test-button" style="text-align: left; padding-left: 10px;">
                                    <a class="test-main-button" onclick="answer_click(<?= $result->id ?>, 4); return false;" href="#">
                                        <div>
                                            <?= $result->odpoved4 ?>
                                        </div>
                                    </a>
                                </div>
                            </td>
                        </tr>
                        </tbody>
                    </table>
                </div>

                <input type="hidden" id="question_<?= $result->id ?>" name="question_<?= $result->id ?>" value="0">
                <?php
                    ++$index;
                ?>
            </div>
        <?php endforeach; ?>
                <input type="hidden" id="time" name="time" value="0">
            </form>
        </div>
    <?php else: ?>
        Chyba při generování otázek.
    <?php endif; ?>
</div>

<?php if (isset($questions) && !empty($questions)): ?>
<script>
    $('#countdown-box').countdown({until: 60, format: 'S', labels: ['', '', '', '', '', '', ''], labels1: ['', '', '', '', '', '', ''],
    compact: true, onExpiry: timeOut});
    $('#countdown-box').countdown('pause');
    var questions_count = <?= count($questions); ?>;
    var index = 1;
    var startTime = 0;

    function answer_click(id, answer) {
        $('#question_'+id).val(answer);
        $('#box-'+index).fadeOut( "fast", function() {
            index += 1;
            if (index <= questions_count) {
                $('#box-'+index).fadeIn( "fast", function() {
                    // Animation complete
                });
            }
            else{
                var currentTime =  new Date() / 1000 | 0;
                $('#time').val( 60 - (currentTime - startTime));
                $('#test-hidden-form').submit();
            }
        });
    }

    function timeOut() {
        $('#test-hidden-form').submit();
    }

    function startTest() {
        $('#start-button').fadeOut( "fast", function() {
            $('#box-'+index).fadeIn( "fast", function() {
                // Animation complete
                $('#countdown-box').countdown('resume');
            });
        });
        startTime = new Date() / 1000 | 0;
    }
</script>

<?php endif; ?>