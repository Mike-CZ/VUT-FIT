/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Labyrinth.Game;

import java.io.Serializable;

public class QuestCard implements Serializable {
    private int cardID;
    private MazeField field;
    private boolean inGame;
    
    public QuestCard(int ID, MazeField f){
        this.cardID = ID;
        this.field = f;
        this.inGame = true;
    }
    
    /**
     * @return the cardID
     */
    public int getCardID() {
        return cardID;
    }

    /**
     * @return the field
     */
    public MazeField getField() {
        return field;
    }

    /**
     * @param field the field to set
     */
    public void setField(MazeField field) {
        this.field = field;
    }

    /**
     * @return the inGame
     */
    public boolean isInGame() {
        return inGame;
    }

    /**
     * @param inGame the inGame to set
     */
    public void setInGame(boolean inGame) {
        this.inGame = inGame;
    }
    
    
}
