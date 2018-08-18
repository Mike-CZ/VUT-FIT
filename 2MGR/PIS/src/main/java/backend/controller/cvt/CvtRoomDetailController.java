package backend.controller.cvt;

import javax.ejb.EJB;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;

import service.RoomManager;
import models.*;


@ManagedBean(name="CVTRoomDetail")
@ViewScoped
public class CvtRoomDetailController {

	@EJB
	private RoomManager roomManager;
	
	private Integer id;
	
	public Mistnost getRoom() {
		Mistnost tmp = roomManager.findById(id);
		//if(tmp.getJeCvt() == false)
		//	return null;
		return tmp;
	}
	
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	
	public Integer getId() {
		return id;
	}

}
