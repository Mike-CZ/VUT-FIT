<?php

namespace App\Http\Controllers;

abstract class BaseController extends Controller
{

    /**
     * @var $title
     */
    protected $title = 'Databáze videozáznamů';

    /**
     * @var $viewData
     */
    private $viewData = [];

    /**
     * Push breadcrumb element
     *
     * @param $name string
     * @param $url string|null
     * @return void
     */
    protected function pushBreadcrumb(string $name, string $url = null)
    {
        \Breadcrumbs::after(function ($breadcrumbs) use ($name, $url) {
            $breadcrumbs->push($name, $url ?? null);
        });
    }

    /**
     * Set page title
     *
     * @param $title string
     * @return void
     */
    protected function setTitle(string $title)
    {
        $this->title = $title;
    }

    /**
     * Render view
     *
     * @param $view
     * @param array $data
     * @param array $mergeData
     * @return \Illuminate\Contracts\View\Factory|\Illuminate\View\View
     */
    protected function render($view, $data = [], $mergeData = [])
    {
        $this->setViewData($data + ['title' => $this->title]);
        return view($view, $this->viewData, $mergeData);
    }

    /**
     * Set view data
     *
     * @param $key string|array
     * @param $data string|null
     */
    protected function setViewData($key, $data = null)
    {
        if (is_null($data) && is_array($key)) {
            foreach ($key as $index => $data) {
                $this->viewData[$index] = $data;
            }
        } else {
            $this->viewData[$key] = $data;
        }
    }
}
