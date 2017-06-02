package Labyrinth.Game;

import java.io.Serializable;

public class PlayerCard implements Serializable {
    private int cardID;
    
    
    public PlayerCard(int id){
        this.cardID = id;
    }

    public int getCardID() {
        return cardID;
    }
  
}
