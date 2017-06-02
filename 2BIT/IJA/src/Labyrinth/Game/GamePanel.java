
package Labyrinth.Game;

import Labyrinth.GUI.ImageLoader;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JPanel;

/**
 *
 * @author pelan_000
 */
public class GamePanel extends JPanel implements Runnable, KeyListener {
    
    public static int WIDTH = 800;
    public static int HEIGHT = 600;
    private Thread thread;
    private boolean isRunning;
    private BufferedImage image;
    private Graphics2D g;
    private final int fps = 30;
    private final int targetTime = 1000 /fps;
    public static int SIZE;
    private int playersCount;
    private MazeBoard mb;
    private Handler handler;
    private ArrayList<Player> players;
    private transient ImageLoader ld;
    private Pointer pointer;
    public static int cardsNumber;
    private boolean endGame;
    private transient File loadFile;

    // inicializace panelu
    public GamePanel(int size, int playerCount, int cardsNumber, File file) {
	super();
        GamePanel.SIZE = size;
        GamePanel.cardsNumber = cardsNumber;
	this.setPreferredSize(new Dimension(GamePanel.WIDTH-10, GamePanel.HEIGHT-10));
	this.setFocusable(true);
	this.requestFocus();
        this.playersCount = playerCount;
        this.mb = null;
        this.players = null;
        this.handler = null;
        this.ld = null;
        this.pointer = null;
        this.endGame = false;
        this.loadFile = file;
}
    @Override
    public void addNotify() {
        super.addNotify();
        if ( thread == null){
            thread = new Thread( this );
            thread.start();
        }
        addKeyListener(this);
    }
    // herni smycka
    public void run() {
        this.init();

        long startTime;
        long urdTime;
        long waitTime;

        while (isRunning){
            startTime = System.nanoTime();

            this.update();
            this.render();
            this.draw();

            urdTime = (System.nanoTime() - startTime) / 1000000;
            waitTime = this.targetTime - urdTime;

            try {
                if (waitTime < 0){
                    waitTime = 10;
                }
                Thread.sleep(waitTime);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
    
    private void draw() {
        Graphics g2 = getGraphics();
        g2.drawImage(image, 0, 0, null);
        g2.dispose();
    }
    
    private void render() {
        g.setColor(Color.BLACK);
        g.fillRect(0, 0, GamePanel.WIDTH, GamePanel.HEIGHT);
        
        if (!this.endGame){
            mb.drawBoard(g);
            handler.drawAll(g);
        }
        else{
            g.setColor(Color.WHITE);
            for (Player pl: this.players){
                if(pl.isIsWinner()){
                    g.drawString("Vítězí hráč: " + (pl.getPlayerID()+1), GamePanel.WIDTH/2 -35, GamePanel.HEIGHT/2);
                }
            }
            
        }
        

    }
    
    void update() {
        for (Player pl: this.players){
            if (!this.endGame){
                this.endGame = pl.checkWinner();
            }
        }
        if (!this.endGame){
            handler.checkCards();
        }
        
    }
    
    private void init() {
        isRunning = true;
        endGame = false;
        image = new BufferedImage(GamePanel.WIDTH, GamePanel.HEIGHT, BufferedImage.TYPE_INT_RGB);
        g = (Graphics2D) image.getGraphics();
        // pokud loadFile je null, vytvorim novou hru, jinak nactu rozehranou
        if (this.loadFile == null){
            this.ld = new ImageLoader();
            this.mb = MazeBoard.createMazeBoard(GamePanel.SIZE, this.ld);
            this.mb.loadImages(this.ld);
            mb.newGame();
            this.pointer = new Pointer(this.mb.get(2, 1));
            this.handler = new Handler(this.mb, this.playersCount, GamePanel.SIZE, GamePanel.cardsNumber, this.pointer, this.ld);
            this.players = handler.createPlayers(this.playersCount);  
            this.handler.createCardsPack(GamePanel.cardsNumber);
            this.handler.createPlayersCards(GamePanel.cardsNumber);
            this.handler.setCardsToPlayers();
        }
        else{
            try {
                FileInputStream in = new FileInputStream(this.loadFile);
                ObjectInputStream obj = new ObjectInputStream(in);
                this.mb = (MazeBoard) obj.readObject();
                this.pointer = (Pointer) obj.readObject();
                this.handler = (Handler) obj.readObject();
                this.players = (ArrayList) obj.readObject();
                this.ld = new ImageLoader();
                this.mb.setLd(this.ld);
                this.mb.loadImages(ld);
                this.handler.setLd(this.ld);
                this.handler.getTextures();
                this.handler.setPointer(this.pointer);
                this.handler.setMb(this.mb);
                this.handler.setPlayers(this.players);
                this.playersCount = handler.getPlayersCount();
                GamePanel.SIZE = handler.getSize();
                GamePanel.cardsNumber = handler.getCardsNumber();
                in.close();
            } catch (FileNotFoundException ex) {
                Logger.getLogger(GamePanel.class.getName()).log(Level.SEVERE, null, ex);
            } catch (IOException ex) {
                Logger.getLogger(GamePanel.class.getName()).log(Level.SEVERE, null, ex);
            } catch (ClassNotFoundException ex) {
                Logger.getLogger(GamePanel.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getExtendedKeyCode();
        
        if (key == KeyEvent.VK_UP){
            handler.movePlayer(MazeCard.CANGO.UP);
        }
        if (key == KeyEvent.VK_DOWN){
            handler.movePlayer(MazeCard.CANGO.DOWN);
        }
        if (key == KeyEvent.VK_LEFT){
            handler.movePlayer(MazeCard.CANGO.LEFT);
        }
        if (key == KeyEvent.VK_RIGHT){
            handler.movePlayer(MazeCard.CANGO.RIGHT);
        }
        if (key == KeyEvent.VK_Q){
            handler.setActivity();
        }
        if (key == KeyEvent.VK_R){
            handler.rotateFreeCard();
        }
        if (key == KeyEvent.VK_SPACE){
            this.handler.shiftBoard(mb.get(this.pointer.getX(), this.pointer.getY()));
        }
        if (key == KeyEvent.VK_T){
            this.pointer.movePointer();
        }
        if (key == KeyEvent.VK_S){
            try {
                try (FileOutputStream out = new FileOutputStream("save-" + new Date().getTime() + ".dat")) {
                    ObjectOutputStream obj = new ObjectOutputStream(out);
                    obj.writeObject(this.mb);
                    obj.writeObject(this.pointer);
                    obj.writeObject(this.handler);
                    obj.writeObject(this.players);
                }
            } catch (FileNotFoundException ex) {
                Logger.getLogger(GamePanel.class.getName()).log(Level.SEVERE, null, ex);
            } catch (IOException ex) {
                Logger.getLogger(GamePanel.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
    }
    
}
