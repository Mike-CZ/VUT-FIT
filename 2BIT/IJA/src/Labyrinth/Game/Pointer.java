
package Labyrinth.Game;

import java.awt.Color;
import java.awt.Graphics;
import java.io.Serializable;


public class Pointer implements Serializable {
    private int x;
    private int y;
    private boolean isActive;
    
    public Pointer (MazeField mf){
        this.x = 2;
        this.y = 1;
        this.isActive = true;
    }
    
    public void draw(Graphics g){
        int constX = 0;
        int constY = 0;
        if (this.y == 1){
            constX = -40;
        }
        if (this.y == GamePanel.SIZE){
            constX = 40;
        }
        if (this.x == 1){
            constY = -40;
        }
        if (this.x == GamePanel.SIZE){
            constY = 40;
        }
        
        if (this.isIsActive()){
            g.setColor(Color.GREEN);
        }
        else{
            g.setColor(Color.RED);
        }
        g.fillOval(((((11 - GamePanel.SIZE)/2) * 40) + 195) + (getY()-1) * 40 + constX
                ,((((11 - GamePanel.SIZE)/2) * 40) + 95)  + (getX()-1) * 40 + constY, 10, 10);
    }
    
    public void movePointer(){
        if (y == 1 && x < GamePanel.SIZE){
            x += 2;
        }
        if (x >= GamePanel.SIZE && y == 1){
            x = GamePanel.SIZE;
            y = 2;
            return;
        }
        if (x == GamePanel.SIZE && y < GamePanel.SIZE){
            y += 2;
        }
        if (y >= GamePanel.SIZE && x == GamePanel.SIZE){
            y = GamePanel.SIZE;
            x = GamePanel.SIZE -1;
            return;
        }
        if (y == GamePanel.SIZE && x > 1){
            x -= 2;
        }
        if (y == GamePanel.SIZE && x <= 1){
            x = 1;
            y = GamePanel.SIZE-1;
            return;
        }
        if (x == 1 && y > 1){
            y -= 2;
        }
        if (x == 1 && y <= 1){
            x = 2;
            y = 1;
        }
    }
    /**
     * @return the x
     */
    public int getX() {
        return x;
    }

    /**
     * @return the y
     */
    public int getY() {
        return y;
    }

    /**
     * @param isActive the isActive to set
     */
    public void setIsActive(boolean isActive) {
        this.isActive = isActive;
    }

    /**
     * @return the isActive
     */
    public boolean isIsActive() {
        return isActive;
    }
}
