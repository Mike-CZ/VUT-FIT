
package Labyrinth.Game;

import java.awt.Color;
import java.awt.Graphics;
import java.io.Serializable;
import java.util.ArrayList;

public class Player implements Serializable {
    private int playerID;
    private MazeField field;
    private boolean isActive;
    private int cardsCount;
    private ArrayList<PlayerCard> cards;
    private int cardToFind;
    private boolean isWinner;
    private int searchedCard;
    
    public boolean checkWinner(){
        if (0 == cards.size()){
            this.isWinner = true;
            return true;
        }
        else{
            this.searchedCard = this.cards.get(this.cards.size()-1).getCardID();
        }
        return false;
    }
    
    
    public Player (int playerID, MazeField field, boolean isActive){
        this.playerID = playerID;
        this.field = field;
        this.isActive = isActive;
        this.cardsCount = 0;
    }
    
    public void drawPlayer(Graphics g){
        switch(this.playerID){
            case 0:
                g.setColor(Color.BLUE);
                break;
            case 1:
                g.setColor(Color.RED);
                break;
            case 2:
                g.setColor(Color.YELLOW);
                break;
            case 3:
                g.setColor(Color.GREEN);
                break;                            
        }
        g.fillOval(((((11 - GamePanel.SIZE)/2) * 40) + 190) + (this.field.col() - 1) * 40
                ,((((11 - GamePanel.SIZE)/2) * 40) + 90)  + (this.field.row() - 1) * 40, 20, 20);
    }
    
    /**
     * @return the playerID
     */
    public int getPlayerID() {
        return playerID;
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
     * @return the isActive
     */
    public boolean isIsActive() {
        return isActive;
    }

    /**
     * @param isActive the isActive to set
     */
    public void setIsActive(boolean isActive) {
        this.isActive = isActive;
    }

    /**
     * @return the cardsCount
     */
    public int getCardsCount() {
        return cardsCount;
    }

    public void addCard(){
        this.cardsCount++;
    }

    /**
     * @return the cards
     */
    public ArrayList<PlayerCard> getCards() {
        return cards;
    }

    /**
     * @param cards the cards to set
     */
    public void setCards(ArrayList<PlayerCard> cards) {
        this.cards = cards;
    }

    /**
     * @return the isWinner
     */
    public boolean isIsWinner() {
        return isWinner;
    }

    /**
     * @return the searchedCard
     */
    public int getSearchedCard() {
        return searchedCard;
    }
}
