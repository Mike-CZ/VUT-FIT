package backend.controller.room;

import javax.ejb.EJB;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;
import javax.faces.context.FacesContext;
import models.*;
import service.RoomManager;


@ManagedBean(name="roomEdit")
@RequestScoped
public class RoomEditController {

	@EJB
	private RoomManager roomManager;
	
	@ManagedProperty("#{param.id}")
	private Integer id;
	
	private Mistnost room;
	
	public Mistnost getRoom() {
		if (room == null) {
			room = roomManager.findById(id);
		}
		return room;
	}
	
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	
	public Integer getId() {
		return id;
	}

	public String actionEdit()
    {
		try {
			roomManager.save(room);
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Room was edited successfully"));
	        return "edit";
		}
		catch(Exception e)
		{
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Failed to edit room..."));
			e.printStackTrace();
		}
		
		return "failed";
    }
}
