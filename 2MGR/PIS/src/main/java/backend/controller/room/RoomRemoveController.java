package backend.controller.room;

import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;
import javax.faces.context.FacesContext;

import javax.ejb.EJB;

import service.RoomManager;
import models.Mistnost;

@ManagedBean(name="roomRemove")
@RequestScoped
public class RoomRemoveController {
	@EJB
	private RoomManager roomManager;
	
	@ManagedProperty("#{param.id}")
	private Integer id;
	
	FacesContext c;
	
	private Mistnost room;
	public Mistnost getRoom() {
		if (room == null) {
			room = roomManager.findById(id);
		}
		return room;
	}
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public String actionRemove() {
		c = FacesContext.getCurrentInstance();
		try {
			roomManager.remove(getRoom());
			c.addMessage(null, new FacesMessage("Room was successfully deleted."));
			//redirect("list.xhtml");
			c.getExternalContext().getFlash().setKeepMessages(true);
			return "list.xhtml?faces-redirect=true";
		} 
		catch (Exception e) {
			c.addMessage(null,  new FacesMessage("Could not delete room"));
			e.printStackTrace();
			return "list.xhtml?faces-redirect=true";
		}
	}
}