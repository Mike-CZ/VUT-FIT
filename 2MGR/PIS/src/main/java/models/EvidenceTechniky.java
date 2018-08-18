package models;

import java.io.Serializable;


import javax.persistence.*;


import java.util.List;


/**
 * The persistent class for the evidence_techniky database table.
 * 
 */
@Entity
@Table(name="evidence_techniky")
@NamedQuery(name="EvidenceTechniky.findAll", query="SELECT e FROM EvidenceTechniky e")
public class EvidenceTechniky implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue(strategy=GenerationType.IDENTITY)
	private int evidID;

	private String serialID;

	//bi-directional many-to-one association to TypTechniky
	@ManyToOne
	@JoinColumn(name="fkTypeID")
	private TypTechniky typTechniky;

	//bi-directional many-to-one association to Uzivatel
	@ManyToOne
	@JoinColumn(name="fkUserID_vlastnik")
	private Uzivatel majitel;

	//bi-directional many-to-many association to Mistnost
	@ManyToMany
	@JoinTable(
		name="evidence_techniky_cvt"
		, joinColumns={
			@JoinColumn(name="fkIDZarizeni")
			}
		, inverseJoinColumns={
			@JoinColumn(name="fkIDMistnosti")
			}
		)
	private List<Mistnost> mistnosti;

	public EvidenceTechniky() {
	}

	public int getEvidID() {
		return this.evidID;
	}

	public void setEvidID(int evidID) {
		this.evidID = evidID;
	}

	public String getSerialID() {
		return this.serialID;
	}

	public void setSerialID(String serialID) {
		this.serialID = serialID;
	}

	public TypTechniky getTypTechniky() {
		return this.typTechniky;
	}

	public void setTypTechniky(TypTechniky typTechniky) {
		this.typTechniky = typTechniky;
	}

	public Uzivatel getMajitel() {
		return this.majitel;
	}

	public void setMajitel(Uzivatel majitel) {
		this.majitel = majitel;
	}

	public List<Mistnost> getMistnosti() {
		return this.mistnosti;
	}

	public void setMistnosti(List<Mistnost> mistnosti) {
		this.mistnosti = mistnosti;
	}

}