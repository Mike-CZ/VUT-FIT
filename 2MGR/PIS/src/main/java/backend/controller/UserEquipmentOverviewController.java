package backend.controller;


import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import javax.faces.context.ExternalContext;
import javax.faces.context.FacesContext;
import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpSession;

import backend.AuthenticationBean;
import service.UserManagerInternal;
import models.*;

@Stateless
@ManagedBean(name="overview")
@ViewScoped
public class UserEquipmentOverviewController {
	
	@EJB
	private UserManagerInternal userManager;
	
	private Uzivatel self;
	

	private AuthenticationBean sessionBean;

	    
    @PostConstruct
    public void init() {
    	ExternalContext externalContext = FacesContext.getCurrentInstance().getExternalContext();

    	HttpServletRequest req = (HttpServletRequest) externalContext.getRequest();
  		HttpSession session = req.getSession();
		
		sessionBean = (AuthenticationBean) session.getAttribute("authenticationBean");
    	
	    setSelf(userManager.findUser(getSessionBean().getLogin()));
    }
    
    public Uzivatel getSelf(){
    	return self;
    }

	public void setSelf(Uzivatel self) {
		this.self = self;
	}

	private AuthenticationBean getSessionBean() {
		return sessionBean;
	}


}
