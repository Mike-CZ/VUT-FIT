<?php
?>
<div class="subcontent">
    <div class="admin-menu">
        <?php if(isset($admin_content)){
            foreach ($admin_content as $item) {
                switch ($item){
                    case 'users':
                        echo '<p><a href="/admin-users">Zákazníci</a></p>';
                        break;
                    case 'flights':
                        echo '<p><a href="/admin-flights">Lety</a></p>';
                        break;
                    case 'planes':
                        echo '<p><a href="/admin-planes">Letadla</a></p>';
                        break;
                    case 'plane_types':
                        echo '<p><a href="/admin-ptypes">Typy letadel</a></p>';
                        break;
                    case 'terminals':
                        echo '<p><a href="/admin-terminals">Terminály</a></p>';
                        break;
                    case 'gates':
                        echo '<p><a href="/admin-gates">Gate</a></p>';
                        break;
                    case 'setgates':
                        echo '<p><a href="/admin-setgates">Povolení odletů z gate</a></p>';
                        break;
                }
            }
        } ?>
        <p style="margin-top: 20px;"><a href="/admin-logout">Odhlasit se</a></p>
    </div>
</div>