package models;

import org.hibernate.annotations.Fetch;
import org.hibernate.annotations.FetchMode;

import java.io.Serializable;
import javax.persistence.*;
import java.util.List;


/**
 * The persistent class for the mistnost database table.
 * 
 */
@Entity
@Table(name="mistnost")
@NamedQuery(name="Mistnost.findAll", query="SELECT m FROM Mistnost m")
public class Mistnost implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue(strategy=GenerationType.IDENTITY)
	private int mistnostID;

	private String nazev;
	
	private int jeCvt;

	//bi-directional many-to-many association to EvidenceTechniky
	@ManyToMany(mappedBy="mistnosti", fetch=FetchType.EAGER)
    @Fetch(value = FetchMode.SUBSELECT)
	private List<EvidenceTechniky> evidenceVMistnosti;

	//bi-directional many-to-one association to Uzivatel
	@OneToMany(fetch = FetchType.EAGER, mappedBy="kancelar")
	private List<Uzivatel> uzivatele;

	public Mistnost() {
	}

	public int getMistnostID() {
		return this.mistnostID;
	}

	public void setMistnostID(int mistnostID) {
		this.mistnostID = mistnostID;
	}

	public String getNazev() {
		return this.nazev;
	}

	public void setNazev(String nazev) {
		this.nazev = nazev;
	}

	public List<EvidenceTechniky> getEvidenceVMistnosti() {
		return this.evidenceVMistnosti;
	}

	public void setEvidenceVMistnosti(List<EvidenceTechniky> evidenceVMistnosti) {
		this.evidenceVMistnosti = evidenceVMistnosti;
	}

	public List<Uzivatel> getUzivatele() {
		return this.uzivatele;
	}

	public void setUzivatele(List<Uzivatel> uzivatele) {
		this.uzivatele = uzivatele;
	}

	public Uzivatel addUzivatele(Uzivatel uzivatele) {
		getUzivatele().add(uzivatele);
		uzivatele.setKancelar(this);

		return uzivatele;
	}

	public Uzivatel removeUzivatele(Uzivatel uzivatele) {
		getUzivatele().remove(uzivatele);
		uzivatele.setKancelar(null);

		return uzivatele;
	}
	
	public boolean getJeCvt() {
		return jeCvt != 0;
	}

	public void setJeCvt(boolean jeCvt) {
		this.jeCvt = jeCvt == true ? 1 : 0;
	}

	@Override
	public String toString()
	{
		return Integer.toString(mistnostID);
	}
	
	@Override
	public int hashCode()
	{
		return mistnostID;
	}

}