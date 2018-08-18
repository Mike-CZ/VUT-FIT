package backend.controller.cvt;


import java.util.List;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import service.RoomManager;
import models.*;

@Stateless
@ManagedBean(name="CVTOverview")
@ViewScoped
public class CvtEquipmentOverviewController {
	
	@EJB
	private RoomManager roomManager;
	
	private List<Mistnost> cvt_rooms;

	    
    @PostConstruct
    public void init() {
    	setCvt_rooms(roomManager.findAllCVTRooms());
    }

    public List<Mistnost> getCvt_rooms() {
		return cvt_rooms;
	}

	public void setCvt_rooms(List<Mistnost> cvt_rooms) {
		this.cvt_rooms = cvt_rooms;
	}

}
