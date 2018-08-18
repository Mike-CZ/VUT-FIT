package backend.controller.room;

import java.util.List;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import javax.faces.context.FacesContext;

import service.RoomManager;

import service.TechniqueManager;
import models.*;

@ManagedBean(name="roomDetail")
@ViewScoped
public class RoomDetailController {

	@EJB
	private TechniqueManager equipmentManager;
	
	@EJB
	private RoomManager roomManager;
	
	private Integer id;
	
	private EvidenceTechniky equipment;
	
	private List<EvidenceTechniky> equipments;
	
	@PostConstruct
	public void init() {
	    id = Integer.valueOf(FacesContext.getCurrentInstance().getExternalContext().getRequestParameterMap().get("id"));       
		equipments = equipmentManager.findAll();
	}
	
	public Mistnost getRoom() {
		return roomManager.findById(id);
	}
	
	
	public void setEquipment(EvidenceTechniky equipment) {
		this.equipment = equipment;
	}
	
	public EvidenceTechniky getEquipment() {
		return equipment;
	}
	
	public List<EvidenceTechniky> getEquipments() {
		return equipments;
	}
	
	
	public void setEquipments(List<EvidenceTechniky> equipments) {
		this.equipments = equipments;
	}
	
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	
	public Integer getId() {
		return id;
	}
	
	
	public String actionAddEquip()
    {
		try {
			Mistnost room = getRoom();
			room.getEvidenceVMistnosti().add(equipment);
			roomManager.save(room);
			equipmentManager.save(equipment);
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Equipment added successfuly"));
			FacesContext.getCurrentInstance().getExternalContext().getFlash().setKeepMessages(true);
			return "detail";
		}
		catch(Exception e)
		{
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Failed to add equipment..."));
			e.printStackTrace();
		}
		
		return "failed";
    }

}
