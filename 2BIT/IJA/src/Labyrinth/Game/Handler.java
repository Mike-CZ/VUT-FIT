package Labyrinth.Game;

import Labyrinth.GUI.ImageLoader;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class Handler implements Serializable {
    private MazeBoard mb;
    private int playersCount;
    private int size;
    private int cardsNumber;
    private ArrayList<Player> players;
    private Pointer pointer;
    private ArrayList<QuestCard> list;
    private ArrayList<PlayerCard> cards;
    private transient BufferedImage [] qCardsTexture;
    private transient BufferedImage [] sCardsTexture;
    private transient BufferedImage cardBack;
    private transient ImageLoader ld;
    
    
    public Handler(MazeBoard mb, int count, int size, int cnumber, Pointer pointer, ImageLoader ld){
        this.mb = mb;
        this.players = new ArrayList<Player>();
        this.playersCount = count;
        this.size = size;
        this.cardsNumber = cnumber;
        this.pointer = pointer;
        this.list = new ArrayList<QuestCard>();
        this.cards = new ArrayList<PlayerCard>();
        this.ld = ld;
        
    }
    
    public void getTextures(){
        loadCardTexture();
        loadSearchedCardsTextures();
    }
    
    
    public void checkCards(){
        for (QuestCard card: this.list){
            for (Player pl: this.players){
                if (pl.getField().row() == card.getField().row() && pl.getField().col() == card.getField().col() &&
                        card.getCardID() == pl.getSearchedCard()){
                    if (card.isInGame()){
                        card.setInGame(false);
                        pl.addCard();
                        pl.getCards().remove(pl.getCards().size()-1);
                        setActivity();
                    }
                }
            }
        }
    }
    
    public ArrayList<Player> createPlayers(int size){        
        for (int x = 0; x < size; x++){
            switch(x){
                case 0:
                players.add(new Player(x, this.mb.get(1, 1), true));
                    break;
                case 2:
                players.add(new Player(x, this.mb.get(GamePanel.SIZE, GamePanel.SIZE), false));    
                    break;
                case 3:
                players.add(new Player(x, this.mb.get(GamePanel.SIZE, 1), false));    
                    break;
                case 1:
                players.add(new Player(x, this.mb.get(1, GamePanel.SIZE), false));    
                    break;
            }
        }
        return players;
    }
    public void createCardsPack(int n){
        loadCardTexture();
        Random rnd = new Random();
        boolean containsCard;
        int xC;
        int yC;
        for (int x = 0; x < n; x++){
            while (true){
                xC = rnd.nextInt(GamePanel.SIZE)+1;
                yC = rnd.nextInt(GamePanel.SIZE)+1;
                containsCard = false;
                for (QuestCard card: list){
                    if (card.getField().row() == xC && card.getField().col() == yC){
                        containsCard = true;
                        break;
                    }
                }
                for (Player pl: this.players){
                    if (pl.getField().row() == xC && pl.getField().col() == yC){
                        containsCard = true;
                        break;
                    }
                }
                if (!containsCard){
                    list.add(new QuestCard(x, this.mb.get(xC, yC)));
                    break;
                }
            }
        }          
    }
    public ArrayList<PlayerCard> createPlayersCards (int n){
        loadSearchedCardsTextures();
        for (int x = 0; x < n; x++){
            cards.add(new PlayerCard(x));
        }
        // zamicham balickem karet
        Collections.shuffle(cards);
        return cards;
    }
    
    public void setCardsToPlayers(){
        int cardsToGet = GamePanel.cardsNumber / this.getPlayersCount();
        int index = 0;
        for (Player pl : this.players){
            ArrayList<PlayerCard> tmp = new ArrayList<PlayerCard>();
            for (int x = index * cardsToGet; x < (index+1) * cardsToGet; x++ ){
                tmp.add(this.cards.get(x));
            }
            index++;
            pl.setCards(tmp);
        }
    }
        
    public void setActivity(){
        int index = 0;
        for (Player pl : this.players){
            if (pl.isIsActive()){
                index = pl.getPlayerID();
                pl.setIsActive(false);
            }
        }
        players.get((index+1)%this.getPlayersCount()).setIsActive(true);
        this.pointer.setIsActive(true);
    }
    
    public void movePlayer(MazeCard.CANGO dir){
        for (Player pl : this.players){
            if (pl.isIsActive()){
                // pokud nemohu jit timto smere, nic nedelam
                if (!pl.getField().getCard().canGo(dir)){
                    return;
                }
                int x = pl.getField().row();
                int y = pl.getField().col();
                switch(dir){
                    case UP:
                        x--;
                        break;  
                    case DOWN:
                        x++;
                        break;
                    case LEFT:
                        y--;
                        break;
                    case RIGHT:
                        y++;
                        break;
                }
                // kontrola hranic
                if (x > GamePanel.SIZE){
                    x = GamePanel.SIZE;
                }
                else if(x < 1){
                    x = 1;
                }
                if (y > GamePanel.SIZE){
                    y = GamePanel.SIZE;
                }
                else if(y < 1){
                    y = 1;
                }
                // musim zkontrolovat, zda se da dostat na pozadovany kamen
                if (dir == MazeCard.CANGO.DOWN){
                    if (!mb.get(x, y).getCard().canGo(MazeCard.CANGO.UP)){
                        break;
                    }
                }
                else if (dir == MazeCard.CANGO.UP){
                    if (!mb.get(x, y).getCard().canGo(MazeCard.CANGO.DOWN)){
                        break;
                    }                    
                }
                else if (dir == MazeCard.CANGO.LEFT){
                    if (!mb.get(x, y).getCard().canGo(MazeCard.CANGO.RIGHT)){
                        break;
                    }
                }
                else if (dir == MazeCard.CANGO.RIGHT){
                    if (!mb.get(x, y).getCard().canGo(MazeCard.CANGO.LEFT)){
                        break;
                    }
                }
                
                pl.setField(mb.get(x, y));
            }
        }
    }
    
    // obalovaci funkce, musim pripadne posunout i hrace a poklady pokud stoji na kamenu, ktery se vysune
    public void shiftBoard(MazeField mf){
        if (this.pointer.isIsActive()){
            int direction = this.mb.shift(mf);
            if (direction != 0){
                this.pointer.setIsActive(false);
            }
            for (Player pl: this.players){
                if (direction == 1 && pl.getField().col() == mf.col()){
                    if (pl.getField().row() == GamePanel.SIZE){
                        pl.setField(this.mb.get(1, pl.getField().col()));
                    }
                    else{
                        pl.setField(this.mb.get(pl.getField().row()+1, pl.getField().col()));
                    }
                }
                else if (direction == 2 && pl.getField().col() == mf.col()){
                    if (pl.getField().row() == 1){
                        pl.setField(this.mb.get(GamePanel.SIZE, pl.getField().col()));
                    }
                    else{
                        pl.setField(this.mb.get(pl.getField().row()-1, pl.getField().col()));
                    }
                }
                else if (direction == 3 && pl.getField().row() == mf.row()){
                    if (pl.getField().col() == GamePanel.SIZE){
                        pl.setField(this.mb.get(pl.getField().row(), 1));
                    }
                    else{
                        pl.setField(this.mb.get(pl.getField().row(), pl.getField().col()+1));
                    }
                }
                else if (direction == 4 && pl.getField().row() == mf.row()){
                    if (pl.getField().col() == 1){
                        pl.setField(this.mb.get(pl.getField().row(), GamePanel.SIZE));
                    }
                    else{
                        pl.setField(this.mb.get(pl.getField().row(), pl.getField().col()-1));
                    }
                }
            }
            for (QuestCard card: this.list){
                if (direction == 1 && card.getField().col() == mf.col()){
                    if (card.getField().row() == GamePanel.SIZE){
                        card.setField(this.mb.get(1, card.getField().col()));
                    }
                    else{
                        card.setField(this.mb.get(card.getField().row()+1, card.getField().col()));
                    }
                }
                else if (direction == 2 && card.getField().col() == mf.col()){
                    if (card.getField().row() == 1){
                        card.setField(this.mb.get(GamePanel.SIZE, card.getField().col()));
                    }
                    else{
                        card.setField(this.mb.get(card.getField().row()-1, card.getField().col()));
                    }
                }
                else if (direction == 3 && card.getField().row() == mf.row()){
                    if (card.getField().col() == GamePanel.SIZE){
                        card.setField(this.mb.get(card.getField().row(), 1));
                    }
                    else{
                        card.setField(this.mb.get(card.getField().row(), card.getField().col()+1));
                    }
                }
                else if (direction == 4 && card.getField().row() == mf.row()){
                    if (card.getField().col() == 1){
                        card.setField(this.mb.get(card.getField().row(), GamePanel.SIZE));
                    }
                    else{
                        card.setField(this.mb.get(card.getField().row(), card.getField().col()-1));
                    }
                }
            }
        }        
    }
    
    public void rotateFreeCard(){
        this.mb.getFreeCard().turnRight();
    }
    
    public void drawPlayers(Graphics g){
        for (Player pl : this.players){
            pl.drawPlayer(g);
        }
    }
    
    public void drawActivePlayer(Graphics g){
        for (Player pl : this.players){
            if (pl.isIsActive()){
                g.setColor(Color.WHITE);
                g.drawString("Na tahu je hráč: " + Integer.toString(pl.getPlayerID()+1), GamePanel.WIDTH /2 - 50, 50);
            }
        }
    }
    public void drawCards(Graphics g){
        for (QuestCard card : this.list){
            if (card.isInGame()){
                g.setColor(Color.BLACK);
                g.drawImage(this.qCardsTexture[card.getCardID()],((((11 - GamePanel.SIZE)/2) * 40) + 180) + (card.getField().col() - 1) * 40,                               
                            ((((11 - GamePanel.SIZE)/2) * 40)) + 80 + (card.getField().row() - 1) * 40, 40, 40, null);
            }
        }
    }
    
    public void drawPlayersInfo(Graphics g){
        for (Player pl: this.players){
            switch(pl.getPlayerID()){
                case 0:
                    g.setColor(Color.WHITE);
                    g.drawString("Získaných karet: " + pl.getCardsCount(), 30, 50);
                    g.setColor(Color.BLUE);
                    g.setFont(new Font("default", Font.BOLD, 16));
                    g.drawString("Hráč 1", 30, 30);
                    g.setFont(new Font("default", Font.PLAIN, 12));
                    if(pl.isIsActive()){
                        g.drawImage(this.sCardsTexture[pl.getSearchedCard()], 30, 60, 80, 110, null);
                    } 
                    else{
                        g.drawImage(this.cardBack, 30, 60, 80, 110, null);
                    }
                    break;
                case 1:
                    g.setColor(Color.WHITE);
                    g.drawString("Získaných karet: " + pl.getCardsCount(), GamePanel.WIDTH - 130,  50);
                    g.setColor(Color.RED);
                    g.setFont(new Font("default", Font.BOLD, 16));
                    g.drawString("Hráč 2", GamePanel.WIDTH - 130, 30);
                    g.setFont(new Font("default", Font.PLAIN, 12));
                    if(pl.isIsActive()){
                        g.drawImage(this.sCardsTexture[pl.getSearchedCard()], GamePanel.WIDTH - 130, 60, 80, 110, null);
                    }      
                    else{
                        g.drawImage(this.cardBack, GamePanel.WIDTH - 130, 60, 80, 110, null);
                    }
                    break;
                 case 2:
                    g.setColor(Color.WHITE);
                    g.drawString("Získaných karet: " + pl.getCardsCount(), GamePanel.WIDTH - 130, GamePanel.HEIGHT - 150);
                    g.setColor(Color.YELLOW);
                    g.setFont(new Font("default", Font.BOLD, 16));
                    g.drawString("Hráč 3", GamePanel.WIDTH - 130, GamePanel.HEIGHT - 170);
                    g.setFont(new Font("default", Font.PLAIN, 12));
                    if(pl.isIsActive()){
                        g.drawImage(this.sCardsTexture[pl.getSearchedCard()], GamePanel.WIDTH - 130, GamePanel.HEIGHT - 140, 80, 110, null);
                    }
                    else{
                        g.drawImage(this.cardBack, GamePanel.WIDTH - 130, GamePanel.HEIGHT - 140, 80, 110, null);
                    }
                    break;
                 case 3:
                    g.setColor(Color.WHITE);
                    g.drawString("Získaných karet: " + pl.getCardsCount(), 30, GamePanel.HEIGHT - 150);
                    g.setColor(Color.GREEN);
                    g.setFont(new Font("default", Font.BOLD, 16));
                    g.drawString("Hráč 4", 30, GamePanel.HEIGHT - 170);
                    g.setFont(new Font("default", Font.PLAIN, 12));
                    if(pl.isIsActive()){
                        g.drawImage(this.sCardsTexture[pl.getSearchedCard()], 30, GamePanel.HEIGHT - 140, 80, 110, null);
                    }   
                    else{
                        g.drawImage(this.cardBack, 30, GamePanel.HEIGHT - 140, 80, 110, null);
                    }
                    break;
            }
        }
    }
    
    public void drawAll(Graphics g){
        drawActivePlayer(g);
        this.pointer.draw(g);
        drawCards(g);
        drawPlayers(g);
        drawPlayersInfo(g);
    }
    
    // nactu textury pro karty do pole
    private void loadCardTexture(){
        this.qCardsTexture = new BufferedImage[24];
        BufferedImage texture = ld.loadImage(System.getProperty("user.dir")+"/lib/treasures.png");
        int index = 0;  
        for (int col = 0; col < texture.getHeight(); col += 40){
            for (int row = 0; row < texture.getWidth(); row += 40){
                this.qCardsTexture [index] = texture.getSubimage(row, col, 40, 40);
                index++;
            } 
        } 
    }
    // nactu textury pro hledane karty
     private void loadSearchedCardsTextures(){
        this.sCardsTexture = new BufferedImage[24];
        BufferedImage texture = ld.loadImage(System.getProperty("user.dir")+"/lib/cards.png");
        this.cardBack = ld.loadImage(System.getProperty("user.dir")+"/lib/cardback.png");
        int index = 0;  
        for (int col = 0; col < texture.getHeight(); col += 110){
            for (int row = 0; row < texture.getWidth(); row += 80){
                this.sCardsTexture [index] = texture.getSubimage(row, col, 80, 110);
                index++;
            } 
        } 
    }

    /**
     * @param pointer the pointer to set
     */
    public void setPointer(Pointer pointer) {
        this.pointer = pointer;
    }

    /**
     * @param ld the ld to set
     */
    public void setLd(ImageLoader ld) {
        this.ld = ld;
    }

    /**
     * @param mb the mb to set
     */
    public void setMb(MazeBoard mb) {
        this.mb = mb;
    }

    /**
     * @param players the players to set
     */
    public void setPlayers(ArrayList<Player> players) {
        this.players = players;
    }

    /**
     * @return the playersCount
     */
    public int getPlayersCount() {
        return playersCount;
    }

    /**
     * @return the size
     */
    public int getSize() {
        return size;
    }

    /**
     * @return the cardsNumber
     */
    public int getCardsNumber() {
        return cardsNumber;
    }
}
