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
            if (Session::get('admin')){
                $this->user->logout();
            }
        }
        parent::__construct();
    }

    public function main(){
        $this->checkLogin();

        $this->data['title'] = 'Administrace';
        $this->data['content'] = 'admin/home';

        $this->setVariable('admin_active', true);

        $this->render();
    }

    public function delete() {
        $this->checkLogin();

        $this->data['title'] = 'Administrace';
        $this->data['content'] = 'admin/delete';

        if ($_GET) {
            if (!empty($_GET['id'])) {
                try {
                    $test = $this->db->select('SELECT 1 FROM otazky WHERE id=:id', array('id' => $_GET['id']));
                    if (!empty($test)) {
                        $this->db->delete('otazky', array('id' => $_GET['id']));
                        $this->setVariable('delete_message', 'Položka byla odstraněna');
                    }
                    else {
                        $this->setVariable('delete_message', 'Položka neexistuje');
                    }
                }
                catch ( \Exception $e) {
                    $this->setVariable('delete_message', 'Položku se nepodařilo odstranit');
                }
            }
        }

        $this->setVariable('admin_active', true);
        $this->setVariable('obtiznost_select', $this->config['obtiznost']);
        $this->setVariable('items', $this->db->select('SELECT * FROM otazky ORDER BY obtiznost'));
        $this->render();
    }

    public function insert() {
        $this->checkLogin();

        $db_cols = array(
            'otazka' => '', 'odpoved1' => '', 'odpoved2' => '', 'odpoved3' => '', 'odpoved4' => '', 'obtiznost' => '', 'spravna_odpoved' => ''
        );

        $this->data['title'] = 'Administrace';
        $this->data['content'] = 'admin/insert';

        $this->setVariable('admin_active', true);
        $this->setVariable('obtiznost_select', $this->config['obtiznost']);

        if($_POST) {

            foreach ($db_cols as $index => $val){
                $db_cols[$index] = $_POST[$index];
            }

            $uploadOk = 1;

            if (empty($_POST['otazka']) ) {
                $this->setVariable('uploadErr', 'Vyplňte otázku!');
                $uploadOk = 0;
            }
            else if (empty($_POST['odpoved1']) || empty($_POST['odpoved2']) || empty($_POST['odpoved3']) || empty($_POST['odpoved4'])) {
                $this->setVariable('uploadErr', 'Vyplňte odpovědi!');
                $uploadOk = 0;
            }
            else if ( ! ($_FILES['image']['error'] == 0)) {
                $this->setVariable('uploadErr', 'Vložte obrázek!');
                $uploadOk = 0;
            }

            if ($uploadOk == 1) {
                $check = @getimagesize($_FILES["image"]["tmp_name"]);
                if($check !== false) {
                    $uploadOk = 1;
                } else {
                    $this->setVariable('uploadErr', 'Zvolený soubor není obrázek!');
                    $uploadOk = 0;
                }

                if ($uploadOk == 1) {
                    $target_dir = dirname(__FILE__) . "/../../images/upload/";
                    $target_file = $target_dir . basename($_FILES["image"]["name"]);
                    $newFileName = $this->random_string(50) . '.jpg';
                    if (move_uploaded_file($_FILES["image"]["tmp_name"], $target_file )) {
                        imagejpeg(imagecreatefromstring(file_get_contents($target_file)), $target_dir . $newFileName);
                        @unlink($target_file);
                        $db_cols['obrazek'] = $newFileName;
                    } else {
                        $this->setVariable('uploadErr', 'Nastala chyba při nahrávání obrázku!');
                    }
                }
            }
            if ($uploadOk == 1) {
                if ($this->db->insert('otazky', $db_cols)){
                    $this->setVariable('uploadSuccess', 'Otázka byla úspěšně vložena!');
                    $db_cols = array_fill_keys(array_keys($db_cols), '');
                }
                else {
                    $this->setVariable('uploadErr', 'Nepodařilo se vložit otázku!');
                    $db_cols = array_fill_keys(array_keys($db_cols), '');
                }
            }
        }
        $this->setVariable('db_cols', $db_cols);
        $this->render();
    }

    public function logout(){
        if (Session::get('admin')){
            Session::destroy('admin');
        }
        // redirect to homepage
        url::redirect('/');
    }

    public function login() {
        $this->data['title'] = 'Administrace';
        $this->data['content'] = 'admin/login';

        if ($permission = Session::get('admin')){
            if (isset($this->adminConfig['users'][$permission])){
                url::redirect('/admin');
            }
            else{
                Session::destroy('admin');
            }
        }

        $this->setVariable('admin_active', true);

        if ($_POST){
            // overeni uctu
            if (isset($this->adminConfig['users'][$_POST['admin-name']]) && $_POST['admin-password'] == $this->adminConfig['users'][$_POST['admin-name']]){
                Session::set('admin', $this->adminConfig['users'][$_POST['admin-name']]);
                url::redirect('/admin');
            }
            else{
                $this->setVariable('admin_message', 'Přihlášení se nezdařilo');
            }
        }
        $this->render();
    }

    private function loadAdminConfig(){
        return require 'app/config/admin.php';
    }
    private function loadConfig(){
        return require 'app/config/config.php';
    }

    private function checkLogin(){
        if ($permission = Session::get('admin')){
            if ( ! isset($this->adminConfig['users'][$permission])){
                Session::destroy('admin');
                url::redirect('/admin-login');
            }
        }else{
            url::redirect('/admin-login');
        }
    }

    private function random_string($length) {
        $key = '';
        $keys = array_merge(range(0, 9), range('a', 'z'));

        for ($i = 0; $i < $length; $i++) {
            $key .= $keys[array_rand($keys)];
        }
        return $key;
    }
}