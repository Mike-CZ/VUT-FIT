<?php namespace controllers;
use helpers\url;
use models\user;

const QUESTIONS_COUNT = 10;

class Welcome extends \controllers\page{

	public function __construct(){
		parent::__construct();

		$this->language->load('welcome');
	}

	public function index() {
        $this->data['title'] = 'Vítejte';
        $this->data['content'] = 'main/home';

        $this->render();
    }

    public function score() {
        $this->data['title'] = 'Skóre';
        $this->data['content'] = 'main/score';


        $this->setVariable('spravne_odpovedi', $this->db->select('SELECT * FROM score ORDER BY score DESC LIMIT 10'));

        $this->render();
    }

    public function result() {
        $this->data['title'] = 'Výsledek';
        $this->data['content'] = 'main/result';
        $questions = array();
        $number = 0;
        $dif = 0;
        $bonus = 0;

        if ($_POST){

            if ($_POST['time'] > 0) {
                $bonus = $_POST['time'];
            }

            foreach($_POST as $index => $value) {
                if (strstr($index, 'question_')) {
                    $tmp_array = explode('_', $index);
                    $questions[$tmp_array[1]] = $value;
                }
            }
            $cond = '';
            $len = count($questions);
            $ind = 0;
            foreach ($questions as $index => $val){
                ++$ind;
                $cond .= 'id=' . $index . ($len != $ind ? ' OR ' : '');
            }
            $results = $this->db->select('SELECT * FROM otazky WHERE ' . $cond);

            foreach ($results as $res) {
                $dif = $res->obtiznost;
                if (isset($questions[$res->id])) {
                    if ($questions[$res->id] == $res->spravna_odpoved) {
                        ++$number;
                    }
                }
            }

            $score = (($dif * $number) * 100) + ($dif * $bonus);

            if ($this->user) {
                $this->db->insert('score', array(
                    'login' => $this->user->getUserInfo()['login'],
                    'score' => $score
                ));
            }

            $this->setVariable('spravne_odpovedi', $number);
            $this->setVariable('otazek', $len);
            $this->setVariable('skore', $score);

        }

        $this->render();
    }

    public function test() {
        $this->data['title'] = 'Test';
        $this->data['content'] = 'main/test';

        if ($_GET) {
            if (isset($this->config['obtiznost'][$_GET['dif']])) {
                $questions_all = $this->db->select('SELECT * FROM otazky WHERE obtiznost=:dif', array('dif' => $_GET['dif']));
                if (count($questions_all) >= QUESTIONS_COUNT) {
                    $keys = array_rand($questions_all, QUESTIONS_COUNT);
                    $questions = array();
                    foreach($keys as $index) {
                        array_push($questions, $questions_all[$index]);
                    }
                    if (!empty($questions)) {
                        $this->setVariable('questions', $questions);
                    }
                }
            }
        }

        $this->render();
    }

    public function test_menu() {
        $this->data['title'] = 'Test';
        $this->data['content'] = 'main/test-menu';

        $this->render();
    }

    public function login() {
        $this->data['title'] = 'Přihlášení';
        $this->data['content'] = 'main/login';

        if ($_POST){
            $user = new User();
            if ($user->login(array('login' => $_POST['login-name'], 'password' => $_POST['login-password']))){
                $this->user = $user->get();
                if ($this->user){
                    $this->setVariable('user', $this->user->getUserInfo());
                    url::redirect('/');
                }
                else{
                    $this->setVariable('loginError', 'Přihlášení se nezdařilo <i class="fa fa-exclamation"></i>');
                }
            }
            else{
                $this->setVariable('loginError', 'Přihlášení se nezdařilo <i class="fa fa-exclamation"></i>');
            }
        }
        $this->render();
    }

    public function logout() {
        if ($this->user){
            $this->user->logout();
            Url::redirect('/');
        }
        else{
            Url::redirect('/');
        }
    }

    public function register() {
        $this->data['title'] = 'Registrace';
        $this->data['content'] = 'main/register';
        $err = false;
        $db_cols = array(
            'login' => '', 'password' => ''
        );

        if ($_POST){

            if ($_POST['register-password'] != '' && !empty($_POST['register-password'])) {
                $db_cols['password'] = $_POST['register-password'];
            }
            else {
                $err = true;
                $this->setVariable('registerMessage', 'Vyplňte heslo <i class="fa fa-exclamation"></i>');
            }
            if ($_POST['register-name'] != '' && !empty($_POST['register-name'])) {
                $db_cols['login'] = $_POST['register-name'];
            }
            else {
                $err = true;
                $this->setVariable('registerMessage', 'Vyplňte uživatelské jméno <i class="fa fa-exclamation"></i>');
            }
            if (!$err) {

                $user = new User();
                if ($user->create($db_cols)){
                    $this->setVariable('registerSuccess', '<span>Registrace byla úspěšná. Nyní se můžete <a href="/login">přihlásit</a></span>');
                }
                else{
                    $this->setVariable('registerMessage', 'Uživatel s tímto loginem již existuje <i class="fa fa-exclamation"></i>');
                }
            }
        }
        $this->render();
    }
}
