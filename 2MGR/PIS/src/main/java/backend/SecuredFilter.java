package backend;

import java.io.IOException;

import javax.servlet.*;
import javax.servlet.http.*;


/*
 * 
 * Content only for logged in users
 * 
 */
public final class SecuredFilter implements Filter 
{
    @SuppressWarnings("unused")
	private FilterConfig filterConfig = null;

    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException 
    {
		HttpServletRequest req = (HttpServletRequest) request;
		HttpSession session = req.getSession();
		HttpServletResponse res = (HttpServletResponse) response;
		
		
		AuthenticationBean auth = (AuthenticationBean) session.getAttribute("authenticationBean");
		
		if (auth == null || !auth.isAuthorized()) {
			res.sendRedirect(req.getContextPath() + "/faces/error/error_nologin.xhtml");
		} else {
			chain.doFilter(request, response);
		}
    }

    public void destroy() 
    {
    }

    public void init(FilterConfig filterConfig) 
    {
        this.filterConfig = filterConfig;
    }
}
