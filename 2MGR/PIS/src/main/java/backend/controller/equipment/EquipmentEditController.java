package backend.controller.equipment;

import java.util.List;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import javax.faces.context.FacesContext;

import models.EvidenceTechniky;
import models.TypTechniky;
import models.Uzivatel;
import service.TechniqueManager;
import service.TechniqueTypeManager;
import service.UserManagerInternal;

@ManagedBean(name="equipmentEdit")
@ViewScoped
public class EquipmentEditController {

	@EJB
	private TechniqueManager equipmentManager;
	
	private Integer id;
	
	@EJB
	private TechniqueTypeManager techniqueTypeManager;
	
	@EJB
	private UserManagerInternal userManager;
	
	private EvidenceTechniky equipment;
	private List<TypTechniky> types;
	private List<Uzivatel> users;

	private TypTechniky defaultEquipType;
	private Uzivatel defaultUser;
	
	private Uzivatel findMyself(){
		if (equipment.getMajitel() == null) {
			return null;
		}
		for(int i = 0; i < users.size(); ++i){
			if(users.get(i).getUserID() == equipment.getMajitel().getUserID())
				return users.get(i);
		}
		return null;
	}
	
	private TypTechniky findMyType(){
		for(int i = 0; i < users.size(); ++i){
			if(types.get(i).getTypeID() == equipment.getTypTechniky().getTypeID())
				return types.get(i);
		}
		return null;
	}
	
	@PostConstruct
    public void init() {
		
	    id = Integer.valueOf(FacesContext.getCurrentInstance().getExternalContext().getRequestParameterMap().get("id"));       

		equipment = equipmentManager.findById(id);
		types = techniqueTypeManager.findAll();
    	users = userManager.findAll();
    	// Find myself in users
   		setDefaultUser(findMyself());
    	setDefaultEquipType(findMyType());
    }

	public EvidenceTechniky getEquipment() {
		return equipment;
	}

	public List<TypTechniky> getEquipmentTypes() {
    	return types;
    }
    
    public List<Uzivatel> getUsers() {
    	return users;
    }
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	
	public Integer getId() {
		return id;
	}

	public Uzivatel getDefaultUser() {
		return defaultUser;
	}

	public void setDefaultUser(Uzivatel defaultUser) {
		this.defaultUser = defaultUser;
	}

	public TypTechniky getDefaultEquipType() {
		return defaultEquipType;
	}

	public void setDefaultEquipType(TypTechniky defaultEquipType) {
		this.defaultEquipType = defaultEquipType;
	}
	
	
	public String actionEdit()
    {
		try {
			equipment.setMajitel(getDefaultUser());
			equipment.setTypTechniky(getDefaultEquipType());
			equipmentManager.save(equipment);
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Equipment added successfully"));
			FacesContext.getCurrentInstance().getExternalContext().getFlash().setKeepMessages(true);
			//equipment = new EvidenceTechniky();
			return "list.xhtml?faces-redirect=true";
		}
		catch(Exception e)
		{
			FacesContext.getCurrentInstance().addMessage(null, new FacesMessage("Failed to add equipment..." + e.getMessage()));
			e.printStackTrace();
		}
		
		return "add";
    }
}
