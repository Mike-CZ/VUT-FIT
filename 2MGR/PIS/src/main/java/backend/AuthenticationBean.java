package backend;



import models.*;
import javax.ejb.EJB;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import javax.faces.context.FacesContext;

import org.apache.commons.lang.BooleanUtils;
import service.UserManagerInternal;

@ManagedBean(name="authenticationBean")
@SessionScoped
public class AuthenticationBean
{
    private boolean authorized;
    private String login;
    private String password;
    private boolean admin;
    
	@EJB
	private UserManagerInternal personMgr;
    
    public AuthenticationBean()
    {
        authorized = false;
    }

    public boolean isAuthorized()
    {
        return authorized;
    }

    public void setAuthorized(boolean authorized)
    {
        this.authorized = authorized;
    }
    
    public String getLogin()
    {
        return login;
    }

    public void setLogin(String login)
    {
        this.login = login;
    }

    public String getPassword()
    {
        return password;
    }

    public void setPassword(String password)
    {
        this.password = password;
    }

    public String actionLogout()
    {
        setAuthorized(false);
        setAdmin(false);
        // TODO: find out how to show message after redirect
        // TMPFIX - Redirect to homepage
        
        FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("You have been logged out."));
        FacesContext.getCurrentInstance().getExternalContext().getFlash().setKeepMessages(true);
        //context.addMessage(clientId, message);
        //externalContext.getFlash().setKeepMessages(true);
        /*try {
			FacesContext.getCurrentInstance().getExternalContext().redirect(FacesContext.getCurrentInstance().getExternalContext().getRequestContextPath() + "/faces/login.xhtml?faces-redirect=true");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
        return "/faces/login.xhtml?faces-redirect=true";
    }
    
    public String actionLogin()
    {

    	Uzivatel user = personMgr.getUserLogin(login, password);

    	if(user != null)
    	{

    		setAdmin(BooleanUtils.toBoolean(user.getJeAdmin()));
    		setAuthorized(true);
    		
    		return "login";
    	}
    	else
    	{
    		FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Wrong username or password."));
    		return "failed";
    	}

   }

	public boolean isAdmin() {
		return admin;
	}

	public void setAdmin(boolean admin) {
		this.admin = admin;
	}
    
   
}
