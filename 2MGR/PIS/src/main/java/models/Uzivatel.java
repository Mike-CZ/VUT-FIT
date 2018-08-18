package models;

import java.io.Serializable;
import javax.persistence.*;
import java.util.List;


/**
 * The persistent class for the uzivatel database table.
 * 
 */
@Entity
@NamedQuery(name="Uzivatel.findAll", query="SELECT u FROM Uzivatel u")
public class Uzivatel implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue(strategy=GenerationType.IDENTITY)
	private int userID;

	private Integer cislo_domu;

	private String heslo;

	private int jeAdmin;

	private String jmeno;

	private String login;

	private String mesto;

	private String prijmeni;

	private Integer psc;

	private String rodne_cislo;

	private String ulice;

	//bi-directional many-to-one association to EvidenceTechniky
	@OneToMany(fetch = FetchType.EAGER, mappedBy="majitel")
	private List<EvidenceTechniky> vlastnenaTechnika;

	//bi-directional many-to-one association to Mistnost
	@ManyToOne
	@JoinColumn(name="fkKancelar")
	private Mistnost kancelar;

	public Uzivatel() {
	}

	public int getUserID() {
		return this.userID;
	}

	public void setUserID(int userID) {
		this.userID = userID;
	}

	public Integer getCislo_domu() {
		return this.cislo_domu;
	}

	public void setCislo_domu(Integer cislo_domu) {
		this.cislo_domu = cislo_domu;
	}

	public String getHeslo() {
		return this.heslo;
	}

	public void setHeslo(String heslo) {
		this.heslo = heslo;
	}

//	public int getJeAdmin() {
//		return this.jeAdmin;
//	}
//
//	public void setJeAdmin(int jeAdmin) {
//		this.jeAdmin = jeAdmin;
//	}
//	
	public boolean getJeAdmin() {
		return this.jeAdmin != 0 ? true : false;
	}

	public void setJeAdmin(boolean jeAdmin) {
		this.jeAdmin = jeAdmin ? 1 : 0;
	}

	public String getJmeno() {
		return this.jmeno;
	}

	public void setJmeno(String jmeno) {
		this.jmeno = jmeno;
	}

	public String getLogin() {
		return this.login;
	}

	public void setLogin(String login) {
		this.login = login;
	}

	public String getMesto() {
		return this.mesto;
	}

	public void setMesto(String mesto) {
		this.mesto = mesto;
	}

	public String getPrijmeni() {
		return this.prijmeni;
	}

	public void setPrijmeni(String prijmeni) {
		this.prijmeni = prijmeni;
	}

	public Integer getPsc() {
		return this.psc;
	}

	public void setPsc(Integer psc) {
		this.psc = psc;
	}

	public String getRodne_cislo() {
		return this.rodne_cislo;
	}

	public void setRodne_cislo(String rodne_cislo) {
		this.rodne_cislo = rodne_cislo;
	}

	public String getUlice() {
		return this.ulice;
	}

	public void setUlice(String ulice) {
		this.ulice = ulice;
	}

	public List<EvidenceTechniky> getVlastnenaTechnika() {
		return this.vlastnenaTechnika;
	}

	public void setVlastnenaTechnika(List<EvidenceTechniky> vlastnenaTechnika) {
		this.vlastnenaTechnika = vlastnenaTechnika;
	}

	public EvidenceTechniky addVlastnenaTechnika(EvidenceTechniky vlastnenaTechnika) {
		getVlastnenaTechnika().add(vlastnenaTechnika);
		vlastnenaTechnika.setMajitel(this);

		return vlastnenaTechnika;
	}

	public EvidenceTechniky removeVlastnenaTechnika(EvidenceTechniky vlastnenaTechnika) {
		getVlastnenaTechnika().remove(vlastnenaTechnika);
		vlastnenaTechnika.setMajitel(null);

		return vlastnenaTechnika;
	}

	public Mistnost getKancelar() {
		return this.kancelar;
	}

	public void setKancelar(Mistnost kancelar) {
		this.kancelar = kancelar;
	}

}