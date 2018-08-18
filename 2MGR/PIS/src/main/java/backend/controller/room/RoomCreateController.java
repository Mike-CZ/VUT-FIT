package backend.controller.room;

import javax.ejb.EJB;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import javax.faces.context.FacesContext;

import service.RoomManagerException;
import service.RoomManager;
import models.*;

@ManagedBean(name="roomCreate")
@ViewScoped
public class RoomCreateController {

	@EJB
	private RoomManager roomManager;
	
	private Mistnost room;
	
	public RoomCreateController() {
		room = new Mistnost();
	}
	
	public Mistnost getRoom() {
		return room;
	}
	
	public String actionInsertNew()
    {
		try {
			roomManager.add(room);
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Room added successfully"));
			FacesContext.getCurrentInstance().getExternalContext().getFlash().setKeepMessages(true);
			room = new Mistnost();
			return "list.xhtml?faces-redirect=true";
		}
		catch (RoomManagerException e) {
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Room already exists.."));
		}
		catch(Exception e)
		{
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Failed to add user..."));
			e.printStackTrace();
		}
		
		return "failed";
    }

}
