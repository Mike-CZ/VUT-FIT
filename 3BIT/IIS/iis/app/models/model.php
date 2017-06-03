<?php namespace models;

class Model {
    protected $db;

    public function __construct(){
        $this->db = \helpers\database::get();
    }

}