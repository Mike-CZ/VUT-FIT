<?php
namespace config;

// uzivatel => heslo
$admin_config['users'] = array(
    'admin' => 'admin',
    'technik' => 'technik',
    'pracovnik' => 'pracovnik'
);

// uzivatel => prava
$admin_config['permissions'] = array(
    'admin' => '1',
    'technik' => '2',
    'pracovnik' => '3'
);

// pristup k polozkam v administraci na zaklade opravneni
$admin_config['admin_items'] = array(
    1 =>
        array(
            'users', 'flights', 'planes', 'plane_types', 'terminals', 'gates', 'setgates'
        ),
    2 => array(
            'planes', 'plane_types'
        ),
    3 => array(
            'flights', 'users'
        )
);

return $admin_config;