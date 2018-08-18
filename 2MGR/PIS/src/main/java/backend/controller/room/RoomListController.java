package backend.controller.room;

import java.util.List;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import models.*;
import service.RoomManager;

@ManagedBean(name="roomList")
@ViewScoped
public class RoomListController {

	@EJB
	private RoomManager roomManager;
	
	private List<Mistnost> rooms;
	
	
	@PostConstruct
	public void init() {
		rooms = roomManager.findAll();
	}
	
	public List<Mistnost> getRooms() {
		return rooms;
	}

}
