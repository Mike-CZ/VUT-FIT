<?php namespace controllers;
use core\view;
use helpers\session;
use models\user;

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
        Session::regenerate();
        $user = new User();
        $this->user = $user->get();
        if ($this->user){
            $this->setVariable('user', $this->user->getUserInfo());
        }
        $this->config = $this->loadConfig();

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