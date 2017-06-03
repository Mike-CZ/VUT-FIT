<?php namespace controllers;
use core\view;
use helpers\session;
use models\user;
use helpers\url;

class Page extends \core\controller{

    protected $data;
    protected $config;
    protected $user;
    protected $db;

    public function __construct(){
        parent::__construct();
        $this->language->load('welcome');
        $this->data = array();
        $this->db = \helpers\database::get();
        Session::init();
        Session::regenerate();
        $user = new User();
        $this->user = $user->get();
        if ($this->user){
            $this->setVariable('user', $this->user->getUserInfo());
        }
        $this->config = $this->loadConfig();

        // automaticke odhlaseni
        $time = session::get('last_activity');
        if (!empty($time)) {
            if ((time() - $time) > 60*30) {
                if (Session::get('user')) {
                    Session::destroy('user');
                }
                if (Session::get('permissions')){
                    Session::destroy('permissions');
                }
                Session::destroy('last_activity');
                url::redirect('/');
            }
        }
        else {
            session::set('last_activity', time());
        }
    }

    protected function render() {

        View::rendertemplate('header', $this->data);
        View::render('content', $this->data);
        View::rendertemplate('footer', $this->data);
    }

    protected function setVariable($param, $val = null){
        if (is_array($param)){
            foreach ($param as $index => $value){
                $this->data['variables'][$index] = $value;
            }
        }
        else{
            $this->data['variables'][$param] = $val;
        }
    }
    private function loadConfig(){
        return require 'app/config/config.php';
    }


}