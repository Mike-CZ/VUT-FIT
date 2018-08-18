package service;

import java.util.List;

import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import models.*;

@Stateless
public class TechniqueManager {

    @PersistenceContext
    private EntityManager em;

    public List<EvidenceTechniky> findAll() {
        return em.createQuery("SELECT et FROM EvidenceTechniky et", EvidenceTechniky.class).getResultList();
    }

    public void save(EvidenceTechniky et)
    {
        em.merge(et);
    }

    public void add(EvidenceTechniky et)
    {
        em.merge(et);
    }

    public void remove(EvidenceTechniky et)
    {
        em.remove(em.merge(et));
    }


    public EvidenceTechniky findById(int id) {
        List<EvidenceTechniky> tmp = em.createQuery("Select et FROM EvidenceTechniky et WHERE et.evidID = :id", EvidenceTechniky.class)
                .setParameter("id", id)
                .getResultList();

        if(tmp.size() != 1)
        {
            return null;
        }

        return tmp.get(0);
    }

}