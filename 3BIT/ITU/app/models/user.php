<?php namespace models;
use helpers\session;

class User extends \models\model{
    protected $userID;
    protected $userLogin;

    public function __construct(){
        parent::__construct();
    }

    public function get(){
        if ($id = Session::get('user')){
            $result = $this->db->select('SELECT * FROM user WHERE hash_id = :id LIMIT 1', array('id' => $id));
            $obj = new User();
            $obj->userID = $result[0]->id;
            $obj->userLogin = $result[0]->login;
            return $obj;
        }
        else{
            return false;
        }
    }

    public function create($userData){
        if (!mb_strlen($userData['login']) || !mb_strlen($userData['password'])){
            return false;
        }
        $result = $this->db->select('SELECT login FROM user WHERE login=:login LIMIT 1', array('login' => $userData['login']));
        if (count($result)){
            return false;
        }
        $data = array(
            'login' => $userData['login'],
            'password' => md5($userData['password']),
            'hash_id' => md5(time() + $userData['password']),
            'ip_address' => $this->getIP()
        );
        return $this->db->insert('user', $data);
    }

    public function login($userData){

        // pokud je prihlasen jako admin a snazi se prihlasit na uzivatele -> odhlasime
        if (Session::get('permissions')){
            Session::destroy('permissions');
        }
        $result = $this->db->select('SELECT hash_id FROM user WHERE login = :login AND password = :password LIMIT 1', array('login' => $userData['login'], 'password' => md5($userData['password'])));
        if (count($result)){
            Session::set('user', $result[0]->hash_id);
            return true;
        }
        return false;
    }
    public function logout(){
        Session::destroy('user');
    }

    public function getUserInfo(){
        return array(
            'id' => $this->userID,
            'login' => $this->userLogin
        );
    }

    private function getIP() {
        $ipaddress = '';
        if ($_SERVER['HTTP_CLIENT_IP'])
            $ipaddress = $_SERVER['HTTP_CLIENT_IP'];
        else if($_SERVER['HTTP_X_FORWARDED_FOR'])
            $ipaddress = $_SERVER['HTTP_X_FORWARDED_FOR'];
        else if($_SERVER['HTTP_X_FORWARDED'])
            $ipaddress = $_SERVER['HTTP_X_FORWARDED'];
        else if($_SERVER['HTTP_FORWARDED_FOR'])
            $ipaddress = $_SERVER['HTTP_FORWARDED_FOR'];
        else if($_SERVER['HTTP_FORWARDED'])
            $ipaddress = $_SERVER['HTTP_FORWARDED'];
        else if($_SERVER['REMOTE_ADDR'])
            $ipaddress = $_SERVER['REMOTE_ADDR'];
        else
            $ipaddress = 'UNKNOWN';

        return $ipaddress;
    }


}