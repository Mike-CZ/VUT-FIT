<?php namespace controllers;
use helpers\url;
use models\user;
use helpers\session;

class Welcome extends \controllers\page{

    protected $adminConfig;

	public function __construct(){
		parent::__construct();
        $this->adminConfig = $this->loadAdminConfig();
		$this->language->load('welcome');
	}

	public function index() {
        $this->data['title'] = 'Vítejte';
        $this->data['content'] = 'home';

        $this->setVariable('home_active', true);
        $this->render();
    }

    public function contact() {
        $this->data['title'] = 'Kontakt';
        $this->data['content'] = 'contact';

        $this->setVariable('contact_active', true);
        $this->render();
    }

    public function edit_user() {
        $this->data['title'] = 'Upravit profil';
        $this->data['content'] = 'user/edit-user';
        $permission = Session::get('permissions');

        $db_cols = array(
            'jmeno' => '', 'rodne_cislo' => '', 'narozen' => ''
        );

        if ($_GET['id'] != '') {
            if ( ! $this->user && !isset($this->adminConfig['admin_items'][$permission])) {
                url::redirect('/login');
                return;
            }
            else if (! $this->user && ! in_array("users", $this->adminConfig['admin_items'][Session::get('permissions')])) {
                url::redirect('/admin');
            }
            else {
                if (!isset($this->adminConfig['admin_items'][$permission]) && $_GET['id'] != $this->user->getUserInfo()['id']) {
                    $this->setVariable('err', 'Nemáte oprávnění k editaci tohoto profilu!');
                }
                else {
                    $this->setVariable('user_edit', $this->db->select('SELECT * FROM user WHERE id=:id LIMIT 1', array('id' => $_GET['id'])));
                }
            }
            if ($_POST) {

                foreach ($_POST as $key => $value){
                    $db_cols[$key] = $value;
                }

                if ($_POST['rodne_cislo'] != '' && !$this->verifyRC($_POST['rodne_cislo'])) {
                    $this->setVariable('editMessage', 'Neplatné rodné číslo!');
                }
                else {
                    $this->db->update('user', $db_cols, array('id' => $_GET['id']));
                    $this->setVariable('user_edit', $this->db->select('SELECT * FROM user WHERE id=:id LIMIT 1', array('id' => $_GET['id'])));
                    if (! $this->user) {
                        url::redirect('/admin-users');
                    }
                    else {
                        url::redirect('/profil');
                    }
                }
            }

            $this->setVariable('user_id', $_GET['id']);
        }
        else {
            $this->setVariable('err', 'Nemáte oprávnění k editaci tohoto profilu!');
        }


        $this->render();
    }

    public function detail () {
        $this->data['title'] = 'Detail letu';
        $this->data['content'] = 'detail';
        $this->setVariable('reservation_active', true);

        if (!empty($_GET['id'])) {
            $result = $this->db->select('SELECT t4.id, t4.cena, t4.cas_odletu, t4.cil, t4.odlet, t4.doba_letu, t3.nazev as gate_nazev, t5.nazev as terminal_nazev, t1.vyrobce, t2.nazev as typ_nazev, t4.vydanych_vstupenek, t2.pocet_sedadel FROM letadlo t1, letadlo_typ t2, gate t3, let t4, terminal t5
                                     WHERE t5.id=t3.terminal_id AND t1.typ_id=t2.id AND t3.id=t4.gate_id AND t1.id=t4.letadlo_id AND t4.id=:id LIMIT 1', array('id' => $_GET['id']));
            $this->setVariable('let', $result);

            $this->setVariable('zobraz_detail', $_GET['detail']);

            $pocet = $this->db->select('SELECT count(*) as pocet FROM letenka WHERE let=:id', array('id' => $_GET['id']));

            if ($_POST && $this->user) {

                $db_cols = array('misto' => $this->getRandomSeat($_GET['id']) , 'trida' => '', 'cena' => '', 'cestujici' => $this->user->getUserInfo()['id'], 'let' => $_GET['id']);

                if ($_POST['trida'] == '0') {
                    $this->setVariable('err', 'Vyberte třídu!');
                }
                else if ( ! is_numeric($_POST['cena_final']) || $_POST['cena_final'] < 0 ) {
                    $this->setVariable('err', 'Zadejte platnou částku!');
                }
                else if ($this->user) {
                    $db_cols['cena'] = $_POST['cena_final'];
                    $db_cols['trida'] = $_POST['trida'];

                    if ($this->db->insert('letenka', $db_cols)){
                        $this->setVariable('success', 'Vaše letenka byla zarezervována! Zašlete Vaši platbu na účet 123456789/1234');
                        $pocet = $this->db->select('SELECT count(*) as pocet FROM letenka WHERE let=:id', array('id' => $_GET['id']));

                    }
                    else {
                        $this->setVariable('err', 'Nepodařilo se zarezervovat let!');
                    }
                }

            }
            $this->setVariable('pocet_letenek', $pocet);
        }

        $this->setVariable('class', $this->config['class']);

        $this->render();
    }

    public function profile () {
        $this->data['title'] = 'Profil';
        $this->data['content'] = 'user/profil';

        $this->setVariable('class', $this->config['class']);

        if ($this->user) {
            $result = $this->db->select('SELECT * FROM user WHERE id=:id LIMIT 1', array('id' => $this->user->getUserInfo()['id']));
            $this->setVariable('uzivatel', $result);
            $moje_lety = $this->db->select('SELECT t1.id as letenka_id, t1.misto, t1.trida, t1.cena, t2.cil, t2.odlet, t2.cas_odletu, t2.doba_letu, t2.id FROM letenka t1, let t2 WHERE t2.id=t1.let AND t1.cestujici=:uzivatel', array('uzivatel' => $this->user->getUserInfo()['id']));
            $this->setVariable('moje_lety', $moje_lety);
        }
        else {

        }

        $this->render();
    }

    public function reservation_home () {
        $this->data['title'] = 'Rezervace letenek';
        $this->data['content'] = 'reservation';


        $result = $this->db->select('SELECT t4.id, t4.cas_odletu, t4.cil, t4.odlet, t4.doba_letu, t3.nazev as gate_nazev, t1.vyrobce, t2.nazev as typ_nazev, t4.vydanych_vstupenek, t2.pocet_sedadel, t2.id as doge, (SELECT count(*) FROM letenka WHERE let=t4.id) as pocet_letenek FROM letadlo t1, letadlo_typ t2, gate t3, let t4
                                     WHERE t1.typ_id=t2.id AND t3.id=t4.gate_id AND t1.id=t4.letadlo_id');
        $this->setVariable('flights', $result);
        $this->setVariable('reservation_active', true);


        $this->render();
    }

    public function login() {
        $this->data['title'] = 'Přihlášení';
        $this->data['content'] = 'user/login';

        if ($_POST){
            $user = new User();
            if ($user->login(array('login' => $_POST['login'], 'password' => $_POST['password']))){
                $this->user = $user->get();
                if ($this->user){
                    $this->setVariable('user', $this->user->getUserInfo());
                }
                else{
                    $this->setVariable('loginError', 'Přihlášení se nezdařilo <i class="fa fa-frown-o"></i>');
                }
            }
            else{
                $this->setVariable('loginError', 'Přihlášení se nezdařilo <i class="fa fa-frown-o"></i>');
            }
        }
        $this->render();
    }

    public function logout() {
        if ($this->user){
            $this->user->logout();
            Url::previous();
        }
        else{
            Url::redirect('/');
        }
    }

    public function register() {
        $this->data['title'] = 'Registrace';
        $this->data['content'] = 'user/register';
        $err = false;
        $db_cols = array(
            'login' => '', 'password' => '', 'jmeno' => '', 'rodne_cislo' => '', 'narozen' => ''
        );

        if ($_POST){

            foreach ($_POST as $key => $value){
                $db_cols[$key] = $value;
                if (!$err && (($key == 'login' || $key == 'password') && empty($value))) {
                    $this->setVariable('registerMessage', '<span style="color: #f00;">Vyplňte ' . ($key == 'login' ? 'login' : 'heslo') . '!</span>');
                    $err = true;
                }
            }

            if (!empty($db_cols['rodne_cislo']) && !$this->verifyRC($db_cols['rodne_cislo'])) {
                $this->setVariable('registerMessage', '<span style="color: #f00;">Zadejte platné rodné číslo!</span>');
                $err = true;
            }

            if (!$err) {
                if ($db_cols['rodne_cislo'] == '') {
                    $db_cols['rodne_cislo'] = '0';
                }
                if ($db_cols['narozen'] == '') {
                    $db_cols['narozen'] = '0000-00-00';
                }
                $user = new User();
                if ($user->create($db_cols)){
                    $this->setVariable('registerMessage', '<span>Registrace byla úspěšná. Nyní se můžete <a href="/login">přihlásit</a></span>
                <script>
                    $("#register-form").css("display", "none");
                </script>
                ');
                }
                else{
                    if ($db_cols['rodne_cislo'] == '0') {
                        $db_cols['rodne_cislo'] = '';
                    }
                    if ($db_cols['narozen'] == '0000-00-00') {
                        $db_cols['narozen'] = '';
                    }
                    $this->setVariable('registerMessage', '<span style="color: #f00;">Registrace nebyla úspěšná. Zkuste zvolit jiný login!</span>');
                    $db_cols['password'] = '';
                    $this->setVariable('db_cols', $db_cols);
                }
            }
            else {
                $db_cols['password'] = '';
                $this->setVariable('db_cols', $db_cols);
            }
        }
        $this->render();
    }

    // zdroj: https://phpfashion.com/jak-overit-platne-ic-a-rodne-cislo
    private function verifyRC($rc)
    {
        // be liberal in what you receive
        if (!preg_match('#^\s*(\d\d)(\d\d)(\d\d)[ /]*(\d\d\d)(\d?)\s*$#', $rc, $matches)) {
            return FALSE;
        }

        list(, $year, $month, $day, $ext, $c) = $matches;

        if ($c === '') {
            $year += $year < 54 ? 1900 : 1800;
        } else {
            // kontrolní číslice
            $mod = ($year . $month . $day . $ext) % 11;
            if ($mod === 10) $mod = 0;
            if ($mod !== (int) $c) {
                return FALSE;
            }

            $year += $year < 54 ? 2000 : 1900;
        }

        // k měsíci může být připočteno 20, 50 nebo 70
        if ($month > 70 && $year > 2003) {
            $month -= 70;
        } elseif ($month > 50) {
            $month -= 50;
        } elseif ($month > 20 && $year > 2003) {
            $month -= 20;
        }

        // kontrola data
        if (!checkdate($month, $day, $year)) {
            return FALSE;
        }

        return TRUE;
    }
    // vygeneruje nahodne cislo sedadla v letadle
    private function getRandomSeat($let_id) {
        $seat_count = $this->db->select('SELECT t1.pocet_sedadel FROM letadlo_typ t1, letadlo t2, let t3  WHERE t1.id=t2.typ_id AND t2.id=t3.letadlo_id AND t3.id=:id LIMIT 1', array('id' => $let_id));
        if (!empty($seat_count)) {
            $number = rand(1, $seat_count[0]->pocet_sedadel);
            $result = $this->db->select('SELECT misto FROM letenka WHERE let=:id', array('id' => $let_id));
            $try = true;
            while($try) {
                $try = false;
                foreach($result as $res) {
                    if ($res == $number) {
                        $number = rand(1, $seat_count[0]->pocet_sedadel);
                        $try = true;
                        break;
                    }
                }
            }
            return $number;
        }
        else {
            return 0;
        }

    }

    private function loadAdminConfig(){
        return require 'app/config/admin.php';
    }

}
