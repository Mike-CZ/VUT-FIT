package models;

import java.io.Serializable;
import javax.persistence.*;
import java.util.List;


/**
 * The persistent class for the typ_techniky database table.
 * 
 */
@Entity
@Table(name="typ_techniky")
@NamedQuery(name="TypTechniky.findAll", query="SELECT t FROM TypTechniky t")
public class TypTechniky implements Serializable {
	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue(strategy=GenerationType.IDENTITY)
	private int typeID;

	private String nazev;

	private String typ_zarizeni;

	//bi-directional many-to-one association to EvidenceTechniky
	@OneToMany(mappedBy="typTechniky")
	private List<EvidenceTechniky> referencovanaTechnika;

	public TypTechniky() {
	}

	public int getTypeID() {
		return this.typeID;
	}

	public void setTypeID(int typeID) {
		this.typeID = typeID;
	}

	public String getNazev() {
		return this.nazev;
	}

	public void setNazev(String nazev) {
		this.nazev = nazev;
	}

	public String getTyp_zarizeni() {
		return this.typ_zarizeni;
	}

	public void setTyp_zarizeni(String typ_zarizeni) {
		this.typ_zarizeni = typ_zarizeni;
	}

	public List<EvidenceTechniky> getReferencovanaTechnika() {
		return this.referencovanaTechnika;
	}

	public void setReferencovanaTechnika(List<EvidenceTechniky> referencovanaTechnika) {
		this.referencovanaTechnika = referencovanaTechnika;
	}

	public EvidenceTechniky addReferencovanaTechnika(EvidenceTechniky referencovanaTechnika) {
		getReferencovanaTechnika().add(referencovanaTechnika);
		referencovanaTechnika.setTypTechniky(this);

		return referencovanaTechnika;
	}

	public EvidenceTechniky removeReferencovanaTechnika(EvidenceTechniky referencovanaTechnika) {
		getReferencovanaTechnika().remove(referencovanaTechnika);
		referencovanaTechnika.setTypTechniky(null);

		return referencovanaTechnika;
	}
	
	@Override
	public String toString()
	{
		return Integer.toString(typeID);
	}
	
	@Override
	public int hashCode()
	{
		return typeID;
	}

}