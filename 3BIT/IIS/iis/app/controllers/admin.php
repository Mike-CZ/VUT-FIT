<?php
namespace controllers;
use core\view;
use helpers\data;
use helpers\Database;
use helpers\session;
use helpers\url;
use models\user;

class Admin extends \controllers\page {

    protected $adminConfig;
    protected $config;

    public function __construct(){
        $this->adminConfig = $this->loadAdminConfig();
        $this->config = $this->loadConfig();
        // pokud je prihlasen uzivatel a snazi se dostat do administrace -> odhlasime jej
        if ($this->user){
            if (Session::get('permissions')){
                $this->user->logout();
            }
        }
        parent::__construct();
    }

    // AJAX CALL
    public function admin_delete () {
        $section = $_GET['section'];
        $id = $_GET['id'];

        if (empty($section) || empty($id) || !is_numeric($id)) {
            echo '-1';
            return;
        }

        // mazani polozek z databaze a kontrola vazeb
        switch ($section) {
            case "let" :
                try {
                    $this->db->delete('let', array('id' => $id));
                    $this->db->delete('letenka', array('let' => $id));
                    $items = $this->db->select('SELECT t4.id, t4.cena, t4.cas_odletu, t4.cil, t4.odlet, t4.doba_letu, t3.nazev as gate_nazev, t1.vyrobce, t2.nazev as typ_nazev, t4.vydanych_vstupenek, t2.pocet_sedadel FROM letadlo t1, letadlo_typ t2, gate t3, let t4
                                     WHERE t1.typ_id=t2.id AND t3.id=t4.gate_id AND t1.id=t4.letadlo_id');
                    foreach ($items as $result){
                        echo '<tr><td>' . $result->id . '</td><td>' . $result->start . '</td><td>' . $result->cil . '</td><td>' . $result->odlet . '</td><td>' . $result->cena . ' Kč' . '</td><td>' . $result->doba_letu . " min" . '</td>' .
                            '<td>' . $result->gate_nazev . '</td><td>' . $result->vyrobce . " " . $result->typ_nazev . '</td><td>' . ((int)$result->pocet_sedadel - ((int)$result->vydanych_vstupenek - 1)) . '</td><td><a onclick="remove_record(' . $result->id . '); return false;" href="">Smazat</a></td>';
                    }
                }
                catch ( \Exception $e) {
                    echo '-1';
                }
                break;

            case "setgates" :
                try {
                    $this->db->delete('typ_gate', array('id' => $id));
                    $items = $this->db->select('SELECT t1.id, t3.nazev AS gate_nazev, t2.nazev AS typ_nazev FROM typ_gate t1, letadlo_typ t2, gate t3 WHERE t1.gate_id=t3.id AND t1.typ_letadla_id=t2.id ORDER BY t3.nazev');
                    foreach ($items as $result) {
                        echo '<tr><td>' . $result->gate_nazev . '</td><td>' . $result->typ_nazev . '</td><td><a onclick="remove_record(' . $result->id . '); return false;" href="">Smazat</a></td>';
                    }
                }
                catch ( \Exception $e) {
                    echo '-1';
                }
                break;

            case "gates" :
                $check = $this->db->select('SELECT 1 FROM typ_gate WHERE gate_id=' . $id . ' LIMIT 1');
                if (empty($check)) {
                    $check = $this->db->select('SELECT 1 FROM let WHERE gate_id=' . $id . ' LIMIT 1');
                    if (empty($check)) {
                        try {
                            $this->db->delete('gate', array('id' => $id));
                            $items = $this->db->select('SELECT t1.id, t1.nazev as gate_name, t2.nazev as terminal_name, t1.terminal_id FROM gate t1, terminal t2 WHERE t2.id=t1.terminal_id ORDER BY t1.nazev');
                            foreach ($items as $result) {
                                echo '<tr><td>' . $result->gate_name . '</td><td>' . $result->terminal_name . '</td><td><a href="" onclick="remove_record(' . $result->id . '); return false;">Smazat</a></td>';
                            }
                        }
                        catch ( \Exception $e) {
                            echo '-1';
                        }
                    }
                    else {
                        echo '-2';
                    }
                }
                else {
                    echo '-2';
                }
                break;

            case "terminals" :
                $check = $this->db->select('SELECT 1 FROM typ_gate WHERE gate_id=' . $id . ' LIMIT 1');
                if (empty($check)) {
                    try {
                        $this->db->delete('terminal', array('id' => $id));
                        $items = $this->db->select('SELECT * FROM terminal ORDER BY nazev');
                        foreach ($items as $result) {
                            echo '<tr><td>' . $result->nazev . '</td><td>' . $result->kapacita . '</td><td>' . $result->parkovacich_mist . '</td><td><a href="" onclick="remove_record(' . $result->id . '); return false;">Smazat</a></td>';
                        }
                    }
                    catch ( \Exception $e) {
                        echo '-1';
                    }
                }
                else {
                    echo '-2';
                }

                break;

            case "ptypes" :
                $check = $this->db->select('SELECT 1 FROM typ_gate WHERE typ_letadla_id=' . $id . ' LIMIT 1');
                if (empty($check)) {
                    $check = $this->db->select('SELECT 1 FROM letadlo WHERE typ_id=' . $id . ' LIMIT 1');
                    if (empty($check)) {
                        try {
                            $this->db->delete('letadlo_typ', array('id' => $id));
                            $items = $this->db->select('SELECT * FROM letadlo_typ ORDER BY nazev');
                            foreach ($items as $result) {
                                echo '<tr><td>' . $result->nazev . '</td><td>' . $result->pocet_sedadel . '</td><td>' . $this->config['seat_types'][$result->typ_rozmisteni] . '</td><td><a href="" onclick="remove_record(' . $result->id . '); return false;">Smazat</a></td>';
                            }
                        }
                        catch ( \Exception $e) {
                            echo '-1';
                        }
                    }
                    else {
                        echo '-2';
                    }
                }
                else {
                    echo '-2';
                }
                break;
            case "users" :
                try {
                    $this->db->delete('user', array('id' => $id));
                    $this->db->delete('letenka', array('cestujici' => $id));
                    $items = $this->db->select('SELECT * FROM user');
                    foreach ($items as $result) {
                        echo '<tr><td>' . $result->id . '</td><td>' . $result->login . '</td><td>' . $result->jmeno . '</td><td>' . $result->rodne_cislo . '</td><td>' . $result->narozen . '</td><td>' . $result->datum_registrace . '</td><td><a href="/profil-edit?id='. $result->id . '">Upravit</a></td><td><a href="" onclick="remove_record(' . $result->id . '); return false;">Smazat</a></td>';
                    }
                }
                catch ( \Exception $e) {
                    echo '-1';
                }
                break;
            case "planes" :
                $check = $this->db->select('SELECT 1 FROM let WHERE letadlo_id=' . $id . ' LIMIT 1');
                if (empty($check)) {
                    try {
                        $this->db->delete('letadlo', array('id' => $id));
                        $items = $this->db->select('SELECT t1.id, t1.vyrobce, t2.nazev, t1.pocet_clenu, t2.nazev, t1.revize, t1.vyroba  FROM letadlo t1, letadlo_typ t2 WHERE t1.typ_id=t2.id ORDER BY t1.vyrobce');
                        foreach ($items as $result) {
                            echo '<tr><td>' . $result->vyrobce . '</td><td>' . $result->nazev . '</td><td>' . $result->pocet_clenu . '</td><td>' . $result->vyroba . '</td><td>' . $result->revize . '</td><td><a onclick="remove_record(' . $result->id . '); return false;" href="">Smazat</a></td>';
                        }
                    }
                    catch ( \Exception $e) {
                        echo '-1';
                    }
                }
                else {
                    echo '-2';
                }

                break;
            case "letenky" :
                $this->db->delete('letenka', array('id' => $id));
                url::redirect('/profil');
                break;
        }
    }

    public function edit_users() {
        $this->checkLogin();

        if ( ! in_array("users", $this->adminConfig['admin_items'][Session::get('permissions')])){
            url::redirect('/admin');
        }

        $result = $this->db->select('SELECT * FROM user');
        $this->setVariable('admin_users', $result);



        $this->data['title'] = 'Administrace - Uživatelé';
        $this->data['content'] = 'admin/users';

        $this->render();
    }

    public function edit_flights() {
        $this->checkLogin();

        if ( ! in_array("flights", $this->adminConfig['admin_items'][Session::get('permissions')])){
            url::redirect('/admin');
        }

        $db_cols = array(
            'cil' => '', 'odlet' => '', 'cas_odletu' => '', 'doba_letu' => '', 'gate_id' => '', 'letadlo_id' => '', 'vydanych_vstupenek' => '1'
        );

        if ($_POST){
            $errors = array();

            // kontrola zda jsou vsechny hodnoty formulare vyplneny
            foreach ($_POST as $key => $value){
                if (empty($value) || ctype_space($value)){
                    $errors[$key] = 'Vyplňte pole!';
                }
                else{
                    $db_cols[$key] = $value;
                }
            }
            // osetreni zadanych hodnot
            if ($_POST){

                if ($_POST['gate_id'] == "0"){
                    $errors['gate_id'] = 'Vyberte gate!';
                }
                if ($_POST['letadlo_id'] == "0"){
                    $errors['letadlo_id'] = 'Vyberte letadlo!';
                }
                if ( ! is_numeric($_POST['doba_letu'])) {
                    $errors['doba_letu'] = 'Zadejte číselnou hodnotu!';
                }
                if ( ! preg_match('/([01]?[0-9]|2[0-3]):[0-5][0-9]$/', $_POST['cas_odletu'])) {
                    $errors['cas_odletu'] = 'Zadejte čas ve formátu HH:mm';
                }
                if ( ! is_numeric($_POST['cena'])) {
                    $errors['cena'] = 'Zadejte číselnou hodnotu!';
                }
                else if ($_POST['cena'] < 0) {
                    $errors['cena'] = 'Cena musí být větší než 0!';
                }
                $today_time = strtotime(date("Y-m-d"));
                $flight_time = strtotime($_POST['odlet']);
                if ($flight_time < $today_time) {
                    $errors['odlet'] = 'Zadejte budoucí čas!';
                }

                // pokud jsou vsechny hodnoty validni -> vlozime do databaze, jinak predame chyby
                if (empty($errors)){

                    // check if plane can depart from gate
                    $check = $this->db->select('SELECT 1 FROM typ_gate WHERE gate_id=' . $_POST['gate_id'] . ' AND typ_letadla_id IN (SELECT t2.id FROM letadlo_typ t2, letadlo t3 WHERE t2.id=t3.typ_id AND t3.id=' . $_POST['letadlo_id']  .')');
                    if (empty($check)) {
                        $this->setVariable('insertResult', '<span style="color: #f00">Letadlo nemůže odletět z tohoto gate!</span>');
                    }
                    else {
                        if (!empty($result)){
                            $this->setVariable('insertResult', 'Toto pravidlo již existuje!');
                            $db_cols = array_fill_keys(array_keys($db_cols), '');
                        }
                        else {
                            $data = array();

                            foreach ($db_cols as $key => $value){
                                $data[$key] = $_POST[$key];
                            }
                            // set to 1
                            $data['vydanych_vstupenek'] = 1;
                            if ($this->db->insert('let', $data)){
                                $this->setVariable('insertResult', 'Let vložen do databáze!');
                                $db_cols = array_fill_keys(array_keys($db_cols), '');
                            }
                            else{
                                $this->setVariable('insertResult', 'Let se nepodařil vložit do databáze!');
                            }
                        }
                    }
                }
                else{
                    $this->setVariable('errors', $errors);
                }
            }

        }

        // vyber aktualnich polozek z db
        $result = $this->db->select('SELECT t4.id, t4.cena, t4.cas_odletu, t4.cil, t4.odlet, t4.doba_letu, t3.nazev as gate_nazev, t1.vyrobce, t2.nazev as typ_nazev, t4.vydanych_vstupenek, t2.pocet_sedadel FROM letadlo t1, letadlo_typ t2, gate t3, let t4
                                     WHERE t1.typ_id=t2.id AND t3.id=t4.gate_id AND t1.id=t4.letadlo_id');
        $this->setVariable('flight_result', $result);
        $this->setVariable('admin_active', true);

        $this->setVariable('gates', $this->db->select('SELECT t1.id, t1. nazev as gate_nazev, t2.nazev as terminal_nazev FROM gate t1, terminal t2 WHERE t2.id=t1.terminal_id ORDER BY t1.nazev, t2.nazev'));
        $this->setVariable('planes', $this->db->select('SELECT t1.id, t1.vyrobce, t2.nazev FROM letadlo t1, letadlo_typ t2 WHERE t2.id=t1.typ_id ORDER BY t1.id'));

        $this->setVariable('db_cols', $db_cols);

        $this->data['title'] = 'Administrace - Lety';
        $this->data['content'] = 'admin/flights';

        $this->render();
    }

    public function edit_setgates() {
        $this->checkLogin();
        if ( ! in_array("setgates", $this->adminConfig['admin_items'][Session::get('permissions')])){
            url::redirect('/admin');
        }

        $db_cols = array(
            'gate_id' => '', 'typ_letadla_id' => ''
        );


        if ($_POST){
            $errors = array();

            // kontrola zda jsou vsechny hodnoty formulare vyplneny
            foreach ($_POST as $key => $value){
                if (empty($value)){
                    $errors[$key] = 'Vyplňte pole!';
                }
                else{
                    $db_cols[$key] = $value;
                }
            }
            // osetreni zadanych hodnot
            if ($_POST){

                if ($_POST['gate_id'] == "0"){
                    $errors['gate_id'] = 'Vyberte gate!';
                }
                if ($_POST['typ_letadla_id'] == "0"){
                    $errors['typ_letadla_id'] = 'Vyberte typ letadla!';
                }
                // pokud jsou vsechny hodnoty validni -> vlozime do databaze, jinak predame chyby
                if (empty($errors)){

                    // kontrola zda dany typ jiz neexistuje
                    $result = $this->db->select('SELECT 1 FROM typ_gate WHERE gate_id=' . $_POST['gate_id'] .' AND typ_letadla_id=' . $_POST['typ_letadla_id']);
                    if (!empty($result)){
                        $this->setVariable('insertResult', 'Toto pravidlo již existuje!');
                        $db_cols = array_fill_keys(array_keys($db_cols), '');
                    }
                    else {
                        $data = array();

                        foreach ($db_cols as $key => $value){
                            $data[$key] = $_POST[$key];
                        }

                        if ($this->db->insert('typ_gate', $data)){
                            $this->setVariable('insertResult', 'Pravidlo vloženo do databáze!');
                            $db_cols = array_fill_keys(array_keys($db_cols), '');
                        }
                        else{
                            $this->setVariable('insertResult', 'Pravidlo se nepodařilo vložit do databáze!');
                        }
                    }
                }
                else{
                    $this->setVariable('errors', $errors);
                }
            }

        }

        // vyber aktualnich polozek z db
        $result = $this->db->select('SELECT t1.id, t3.nazev AS gate_nazev, t2.nazev AS typ_nazev, t4.nazev AS terminal_nazev FROM typ_gate t1, letadlo_typ t2, gate t3, terminal t4 WHERE t4.id=t3.terminal_id AND t1.gate_id=t3.id AND t1.typ_letadla_id=t2.id ORDER BY t3.nazev, t4.nazev');
        $this->setVariable('gate_result', $result);

        $this->setVariable('gates', $this->db->select('SELECT t1.id, t1. nazev as gate_nazev, t2.nazev as terminal_nazev FROM gate t1, terminal t2 WHERE t2.id=t1.terminal_id ORDER BY t1.nazev, t2.nazev'));
        $this->setVariable('types', $this->db->select('SELECT * FROM letadlo_typ ORDER BY nazev'));
        $this->setVariable('admin_active', true);
        $this->setVariable('db_cols', $db_cols);

        $this->data['title'] = 'Administrace - povolení odletů z gate';
        $this->data['content'] = 'admin/setgates';

        $this->render();
    }

    public function edit_gates () {
        $this->checkLogin();
        // zkonrolujeme zda ma opravneni do teto sekce jinak redirect
        if ( ! in_array("gates", $this->adminConfig['admin_items'][Session::get('permissions')])){
            url::redirect('/admin');
        }

        $db_cols = array(
            'nazev' => '', 'terminal_id' => ''
        );

        if ($_POST){
            $errors = array();

            // kontrola zda jsou vsechny hodnoty formulare vyplneny
            foreach ($_POST as $key => $value){
                if (empty($value)){
                    $errors[$key] = 'Vyplňte pole!';
                }
                else{
                    $db_cols[$key] = $value;
                }
            }
            // osetreni zadanych hodnot
            if ($_POST['nazev']){
                // kontrola zda dany typ jiz neexistuje
                $result = $this->db->select('SELECT nazev FROM gate WHERE terminal_id=:terminal', array('terminal' => $_POST['terminal_id']));
                if (!empty($result)){
                    foreach ($result as $res){
                        if ($res->nazev == $_POST['nazev']){
                            $errors['nazev'] = 'Tento název již existuje!';
                            break;
                        }
                    }
                }
            }

            if ($_POST['terminal_id'] == "0"){
                $errors['terminal_id'] = 'Vyberte terminál!';
            }
            // pokud jsou vsechny hodnoty validni -> vlozime do databaze, jinak predame chyby
            if (empty($errors)){
                $data = array();

                foreach ($db_cols as $key => $value){
                    $data[$key] = $_POST[$key];
                }

                if ($this->db->insert('gate', $data)){
                    $this->setVariable('insertResult', 'Gate byl vložen do databáze!');
                    $db_cols = array_fill_keys(array_keys($db_cols), '');
                }
                else{
                    $this->setVariable('insertResult', 'Gate se nepodařilo vložit do databáze!');
                }
            }
            else{
                $this->setVariable('errors', $errors);
            }
        }

        // vyber aktualnich polozek z db
        $result = $this->db->select('SELECT t1.id, t1.nazev as gate_name, t2.nazev as terminal_name, t1.terminal_id FROM gate t1, terminal t2 WHERE t2.id=t1.terminal_id ORDER BY t1.nazev');
        $this->setVariable('gate_result', $result);

        $this->setVariable('terminals', $this->db->select('SELECT * FROM terminal ORDER BY nazev'));
        $this->setVariable('admin_active', true);
        $this->setVariable('db_cols', $db_cols);

        $this->data['title'] = 'Administrace - editace gate';
        $this->data['content'] = 'admin/gates';

        $this->render();
    }
    public function edit_terminals() {
        $this->checkLogin();

        // zkonrolujeme zda ma opravneni do teto sekce jinak redirect
        if ( ! in_array("terminals", $this->adminConfig['admin_items'][Session::get('permissions')])){
            url::redirect('/admin');
        }

        $db_cols = array(
            'nazev' => '', 'kapacita' => '', 'parkovacich_mist' => ''
        );

        if ($_POST){
            $errors = array();

            // kontrola zda jsou vsechny hodnoty formulare vyplneny
            foreach ($_POST as $key => $value){
                if (empty($value)){
                    $errors[$key] = 'Vyplňte pole!';
                }
                else{
                    $db_cols[$key] = $value;
                }
            }
            // osetreni zadanych hodnot
            if ($_POST['nazev']){
                // kontrola zda dany typ jiz neexistuje
                $result = $this->db->select('SELECT nazev FROM terminal');
                if (!empty($result)){
                    foreach ($result as $res){
                        if ($res->nazev == $_POST['nazev']){
                            $errors['nazev'] = 'Tento název již existuje!';
                            break;
                        }
                    }
                }
            }

            if ($_POST['kapacita']){
                if (!is_numeric($_POST['kapacita'])){
                    $errors['kapacita'] = 'Zadejte číselnou hodnotu!';
                }
            }
            if ($_POST['parkovacich_mist']){
                if (!is_numeric($_POST['parkovacich_mist'])){
                    $errors['parkovacich_mist'] = 'Zadejte číselnou hodnotu!';
                }
            }
            // pokud jsou vsechny hodnoty validni -> vlozime do databaze, jinak predame chyby
            if (empty($errors)){
                $data = array();

                foreach ($db_cols as $key => $value){
                    $data[$key] = $_POST[$key];
                }

                if ($this->db->insert('terminal', $data)){
                    $this->setVariable('insertResult', 'Terminál byl vložen do databáze!');
                    $db_cols = array_fill_keys(array_keys($db_cols), '');
                }
                else{
                    $this->setVariable('insertResult', 'Terminál se nepodařilo vložit do databáze!');
                }
            }
            else{
                $this->setVariable('errors', $errors);
            }
        }

        // vyber aktualnich polozek z db
        $result = $this->db->select('SELECT * FROM terminal ORDER BY nazev');
        $this->setVariable('terminal_result', $result);

        $this->setVariable('db_cols', $db_cols);
        $this->setVariable('admin_active', true);
        $this->data['title'] = 'Administrace - editace terminálů';
        $this->data['content'] = 'admin/terminals';

        $this->render();
    }

    public function edit_ptypes() {
        $this->checkLogin();

        // zkonrolujeme zda ma opravneni do teto sekce jinak redirect
        if ( ! in_array("plane_types", $this->adminConfig['admin_items'][Session::get('permissions')])){
            url::redirect('/admin');
        }

        $db_cols = array(
            'nazev' => '', 'pocet_sedadel' => '', 'typ_rozmisteni' => ''
        );

        $this->data['title'] = 'Administrace - editace typů letadel';
        $this->data['content'] = 'admin/planes-types';

        $this->setVariable('seatTypes', $this->config['seat_types']);

        if ($_POST){
            $errors = array();

            // kontrola zda jsou vsechny hodnoty formulare vyplneny
            foreach ($_POST as $key => $value){
                if (empty($value)){
                    $errors[$key] = 'Vyplňte pole!';
                }
                else{
                    $db_cols[$key] = $value;
                }
            }
            // osetreni zadanych hodnot
            if ($_POST['nazev']){
                // kontrola zda dany typ jiz neexistuje
                $result = $this->db->select('SELECT nazev FROM letadlo_typ');
                if (!empty($result)){
                    foreach ($result as $res){
                        if ($res->nazev == $_POST['nazev']){
                            $errors['nazev'] = 'Tento název již existuje!';
                            break;
                        }
                    }
                }
            }
            if ( ! ($_POST['typ_rozmisteni'] && isset($this->config['seat_types'][$_POST['typ_rozmisteni']]))){
                $errors['typ_rozmisteni'] = 'Vyberte typ rozmístění!';
            }
            if ($_POST['pocet_sedadel']){
                if (!is_numeric($_POST['pocet_sedadel'])){
                    $errors['pocet_sedadel'] = 'Zadejte číselnou hodnotu!';
                }
            }
            // pokud jsou vsechny hodnoty validni -> vlozime do databaze, jinak predame chyby
            if (empty($errors)){
                $data = array();

                foreach ($db_cols as $key => $value){
                    $data[$key] = $_POST[$key];
                }

                if ($this->db->insert('letadlo_typ', $data)){
                    $this->setVariable('insertResult', 'Typ letadla byl vložen do databáze!');
                    $db_cols = array_fill_keys(array_keys($db_cols), '');
                }
                else{
                    $this->setVariable('insertResult', 'Typ letadla se nepodařilo vložit do databáze!');
                }
            }
            else{
                $this->setVariable('errors', $errors);
            }
        }

        // vyber aktualnich polozek z db
        $result = $this->db->select('SELECT * FROM letadlo_typ ORDER BY nazev');
        $this->setVariable('types_result', $result);
        $this->setVariable('admin_active', true);
        $this->setVariable('db_cols', $db_cols);
        $this->render();
    }

    public function edit_planes() {

        $this->checkLogin();

        // zkonrolujeme zda ma opravneni do teto sekce jinak redirect
        if ( ! in_array("planes", $this->adminConfig['admin_items'][Session::get('permissions')])){
            url::redirect('/admin');
        }

        $db_cols = array(
            'vyrobce' => '', 'typ_id' => '', 'pocet_clenu' => '', 'vyroba' => '', 'revize' => ''
        );

        $this->data['title'] = 'Administrace - editace letadel';
        $this->data['content'] = 'admin/planes';

        $this->setVariable('planeTypes', $this->db->select('SELECT * FROM letadlo_typ ORDER BY nazev'));

        if ($_POST){
            $errors = array();

            // kontrola zda jsou vsechny hodnoty formulare vyplneny
            foreach ($_POST as $key => $value){
                if (empty($value)){
                    $errors[$key] = 'Vyplňte pole!';
                }
                else{
                    $db_cols[$key] = $value;
                }
            }
            // osetreni zadanych hodnot
            if ($_POST['pocet_clenu']){
                if (!is_numeric($_POST['pocet_clenu'])){
                    $errors['pocet_clenu'] = 'Zadejte číselnou hodnotu!';
                }
            }
            if (isset($_POST['typ_id'])){
                if ($_POST['typ_id'] == "0") {
                    $errors['typ_id'] = 'Vyberte typ letadla!';
                }
            }

            // pokud jsou vsechny hodnoty validni -> vlozime do databaze, jinak predame chyby
            if (empty($errors)){
                $data = array();

                foreach ($db_cols as $key => $value){
                    $data[$key] = $_POST[$key];
                }

                if ($this->db->insert('letadlo', $data)){
                    $this->setVariable('insertResult', 'Letadlo bylo vloženo do databáze!');
                    $db_cols = array_fill_keys(array_keys($db_cols), '');
                }
                else{
                    $this->setVariable('insertResult', 'Letadlo se nepodařilo vložit do databáze!');
                }
            }
            else{
                $this->setVariable('errors', $errors);
            }
        }

        // vyber aktualnich polozek z db
        $result = $this->db->select('SELECT t1.id, t1.vyrobce, t2.nazev, t1.pocet_clenu, t2.nazev, t1.revize, t1.vyroba  FROM letadlo t1, letadlo_typ t2 WHERE t1.typ_id=t2.id ORDER BY t1.vyrobce');
        $this->setVariable('planes_result', $result);
        $this->setVariable('admin_active', true);
        $this->setVariable('db_cols', $db_cols);
        $this->render();
    }

    public function login(){
        if ($permission = Session::get('permissions')){
            if (isset($this->adminConfig['admin_items'][$permission])){
                url::redirect('/admin');
            }
            else{
                Session::destroy('permissions');
            }
        }
        $this->data['title'] = 'Administrace - přihlášení';
        $this->data['content'] = 'admin/login';
        $this->setVariable('admin_active', true);
        if ($_POST){
            // overeni uctu
            if (isset($this->adminConfig['users'][$_POST['admin-name']]) && $_POST['admin-password'] == $this->adminConfig['users'][$_POST['admin-name']]){
                Session::set('permissions', $this->adminConfig['permissions'][$_POST['admin-name']]);
                if ($this->user) {
                    Session::destroy('user');
                }
                url::redirect('/admin');
            }
            else{
                $this->setVariable('admin_message', 'Přihlášení se nezdařilo');
            }
        }

        $this->render();
    }

    public function logout(){
        if (Session::get('permissions')){
            Session::destroy('permissions');
        }
        // redirect to homepage
        url::redirect('/');
    }

    public function main(){
        $this->checkLogin();

        $this->data['title'] = 'Administrace';
        $this->data['content'] = 'admin/home';
        $this->setVariable('admin_active', true);
        // content na zaklade pristupovych prav
        $this->setVariable('admin_content', $this->adminConfig['admin_items'][Session::get('permissions')]);

        $this->render();
    }


    private function loadAdminConfig(){
        return require 'app/config/admin.php';
    }
    private function loadConfig(){
        return require 'app/config/config.php';
    }

    private function checkLogin(){
        if ($permission = Session::get('permissions')){
            if ( ! isset($this->adminConfig['admin_items'][$permission])){
                Session::destroy('permissions');
                url::redirect('/admin-login');
            }
        }else{
            url::redirect('/admin-login');
        }
    }
}