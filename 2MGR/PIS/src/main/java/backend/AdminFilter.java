package backend;

import java.io.IOException;

import javax.servlet.*;
import javax.servlet.http.*;

public final class AdminFilter implements Filter 
{
    @SuppressWarnings("unused")
	private FilterConfig filterConfig = null;

    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException 
    {
    	HttpServletRequest req = (HttpServletRequest) request;
    	HttpServletResponse res = (HttpServletResponse) response;
        HttpSession session = ((HttpServletRequest) request).getSession();
        AuthenticationBean auth = (AuthenticationBean) session.getAttribute("authenticationBean");
        if (auth != null && auth.isAuthorized() && auth.isAdmin())
        {
            chain.doFilter(request, response);
        }
        else
        {
           	res.sendRedirect(req.getContextPath() + "/faces/error/error_notauthorized.xhtml");
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
