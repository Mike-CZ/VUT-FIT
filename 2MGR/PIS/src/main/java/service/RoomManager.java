package service;

import java.util.List;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import models.*;

@Stateless
public class RoomManager
{
    @PersistenceContext
    private EntityManager em;

    public RoomManager()
    {
    }

    public void save(Mistnost r)
    {
        em.merge(r);
    }

    public void remove(Mistnost r)
    {
        em.remove(em.merge(r));
    }

    public List<Mistnost> findAll()
    {
        return em.createQuery("SELECT r FROM Mistnost r", Mistnost.class).getResultList();
    }

    public List<Mistnost> findAllOffices()
    {
        return em.createQuery("SELECT r FROM Mistnost r WHERE r.jeCvt = 0", Mistnost.class).getResultList();
    }

    public List<Mistnost> findAllCVTRooms()
    {
        return em.createQuery("SELECT r FROM Mistnost r WHERE r.jeCvt <> 0", Mistnost.class).getResultList();
    }

    public Mistnost findById(int id)
    {
        List<Mistnost> tmp = em.createQuery("SELECT r FROM Mistnost r WHERE r.mistnostID = :id", Mistnost.class)
                .setParameter("id", id).getResultList();
        if(tmp.size() != 1)
            return null;

        return tmp.get(0);
    }


    public Mistnost findByName(String name)
    {
        List<Mistnost> tmp = em
                .createQuery("SELECT m FROM Mistnost m WHERE m.nazev = :name", Mistnost.class)
                .setParameter("name", name)
                .getResultList();

        if(tmp.size() != 1)
        {
            return null;
        }

        return tmp.get(0);
    }

    public void add(Mistnost m) throws RoomManagerException
    {
        if (findByName(m.getNazev()) != null) {
            throw new RoomManagerException("Room already exists");
        }

        save(m);
    }
}