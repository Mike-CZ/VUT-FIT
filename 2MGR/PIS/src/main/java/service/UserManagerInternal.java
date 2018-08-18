package service;


import java.util.List;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import models.*;
@Stateless
public class UserManagerInternal
{
    @PersistenceContext
    private EntityManager em;

    public UserManagerInternal()
    {
    }

    public void save(Uzivatel p)
    {
        em.merge(p);
    }

    public void remove(Uzivatel p)
    {
        em.remove(em.merge(p));
    }

    public Uzivatel findById(int id)
    {
        List<Uzivatel> tmp = em.createQuery("SELECT r FROM Uzivatel r WHERE r.userID = :id", Uzivatel.class)
                .setParameter("id", id).getResultList();
        if(tmp.size() != 1)
            return null;

        return tmp.get(0);
    }

    public Uzivatel findUser(String login)
    {
        List<Uzivatel> tmp = em.createQuery("SELECT p FROM Uzivatel p WHERE p.login = :login", Uzivatel.class)
                .setParameter("login", login).getResultList();
        if(tmp.size() != 1)
        {
            return null;
        }

        return tmp.get(0);
    }

    public Uzivatel findUserById(int id)
    {
        List<Uzivatel> tmp = em.createQuery("SELECT p FROM Uzivatel p WHERE p.userID = :id", Uzivatel.class)
                .setParameter("id", id).getResultList();
        if(tmp.size() != 1)
        {
            return null;
        }

        return tmp.get(0);
    }

    public List<Uzivatel> findAll()
    {
        return em.createQuery("SELECT p FROM Uzivatel p", Uzivatel.class).getResultList();
    }

    public Uzivatel getUserLogin(String login, String password)
    {
        Uzivatel usr = findUser(login);
        try {
            String a = Password.getSaltedHash("admin");
            int ab = 6;
        } catch (Exception e) {
            e.printStackTrace();
        }
        if(usr == null)
        {
            return null;
        }
        else
        {
            try {
                if(Password.check(password, usr.getHeslo()))
                {
                    return usr;
                }
                else
                {
                    return null;
                }
            } catch (Exception e) {
                // TODO log exception?
                return null;
            }
        }
    }


    public void addUserEncodePassword(Uzivatel usr) throws Exception
    {
        if(findUser(usr.getLogin()) != null){
            throw new UserManagerException("User already exists");
        }

        usr.setHeslo(Password.getSaltedHash(usr.getHeslo()));
        this.save(usr);
    }

}