<?php

namespace App\Http\Middleware;

use Closure;

class Admin {
    /**
     * Handle an incoming request.
     *
     * @param  \Illuminate\Http\Request $request
     * @param  \Closure $next
     * @return mixed
     */
    public function handle($request, Closure $next)
    {
        if (\Auth::guest()) {
            if ($request->ajax()) {
                return response('Unauthorized.', 401);
            }
            else {
                return redirect()->guest(route('admin.login'));
            }
        }
        else if (! \Auth::user()->hasRole('admin')) {
            \Auth::logout();
            return redirect()->guest(route('admin.login'));
        }
        return $next($request);
    }
}
