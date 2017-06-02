package Labyrinth.Game;

import Labyrinth.GUI.ImageLoader;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.Serializable;
import java.util.Random;

public class MazeBoard implements Serializable {
	private MazeField mField[][];
	private int size;
	private MazeCard freeCard;
        private transient ImageLoader ld;        
        private transient BufferedImage c;
        private transient BufferedImage l;
        private transient BufferedImage f;
        
        private int x;
        private int y;

	public MazeBoard (int s){
		this.size = s;
		this.mField = new MazeField[s+1][s+1];
                this.ld = null;
                this.x = 0;
                this.y = 0;
        }
        
        public void loadImages(ImageLoader ld){
            this.ld = ld;
            this.c = ld.loadImage(System.getProperty("user.dir")+"/lib/labyrinth_L.jpg");  
            this.l = ld.loadImage(System.getProperty("user.dir")+"/lib/labyrinth_i.jpg");
            this.f = ld.loadImage(System.getProperty("user.dir")+"/lib/labyrinth_t.jpg");
        }

	
	public static MazeBoard createMazeBoard(int n, ImageLoader ld){
		MazeBoard board = new MazeBoard(n);	
		for (int x = 1; x <= n; x++){
			for (int y = 1; y <= n; y++){
				board.mField[x][y] = new MazeField(x, y);
			}
		}
		return board;
	}
	public void newGame(){
		Random rnd = new Random();
                int cType = 4;
                int tType = ((GamePanel.SIZE+1)/2) * ((GamePanel.SIZE+1)/2) - 4;
                int lType = 0;
                int maxCount = (GamePanel.SIZE * GamePanel.SIZE)/3 + 1;
                
		for (int x = 1; x <= this.size; x++){
			for (int y = 1; y <= this.size; y++){
				MazeCard tmp = null;
                                // pokud jde o rohovy kamen, nastavim typ C a otocim spravne
                                if (x == 1 && y == 1){
                                    tmp = MazeCard.create("C");
                                }
                                else if(x == GamePanel.SIZE && y == GamePanel.SIZE){
                                    tmp = MazeCard.create("C");
                                    tmp.turnRight();
                                    tmp.turnRight();
                                }
                                else if(x == 1 && y == GamePanel.SIZE){
                                    tmp = MazeCard.create("C");
                                    tmp.turnRight();
                                }
                                else if(x == GamePanel.SIZE && y == 1){
                                    tmp = MazeCard.create("C");
                                    tmp.turnRight();
                                    tmp.turnRight();
                                    tmp.turnRight();
                                }
                                // pokud jde o lichy radek i sloupec, vytvorim kamen ve tvaru T
                                else if (x % 2 != 0 && y % 2 != 0){
                                    tmp = MazeCard.create("F");
                                        if (x == GamePanel.SIZE){
                                            tmp.turnRight();
                                            tmp.turnRight();
                                        }
                                        if (y == GamePanel.SIZE){
                                            tmp.turnRight();
                                        }
                                        if (y == 1){
                                            tmp.turnRight();
                                            tmp.turnRight();
                                            tmp.turnRight();
                                        }
                                        if (x != 1 && x != GamePanel.SIZE && y != 1 && y != GamePanel.SIZE){
                                            // provedu nahodne natoceni zbylych kamenu
                                            int rand = rnd.nextInt(4);
                                            for (int n = 0; n < rand; n++){
                                                tmp.turnRight();
                                            }
                                        }
                                }
                                // vygeneruji nahodne zbyle kameny
                                else{
                                    while (true){
                                        int random = rnd.nextInt(3);
                                        if (random == 0){
                                            if (cType <= maxCount){
                                                tmp = MazeCard.create("C");
                                                cType++;
                                                int rand = rnd.nextInt(4);
                                                for (int n = 0; n < rand; n++){
                                                    tmp.turnRight();
                                                }
                                                break;
                                            }                                  
                                        }
                                        else if (random == 1){
                                            if (lType <= maxCount){
                                                tmp = MazeCard.create("L");
                                                lType++;
                                                int rand = rnd.nextInt(4);
                                                for (int n = 0; n < rand; n++){
                                                    tmp.turnRight();
                                                }
                                                break;
                                            }
                                        }
                                        else if (random == 2){
                                            if (tType <= maxCount){
                                                tmp = MazeCard.create("F");
                                                tType++;
                                                int rand = rnd.nextInt(4);
                                                for (int n = 0; n < rand; n++){
                                                    tmp.turnRight();
                                                }
                                                break;
                                            }
                                        }
                                    }
                                }
				this.mField[x][y].putCard(tmp);
			}
		}
		this.freeCard = MazeCard.create("C");
	}
	
	public MazeField get(int r, int c){
		if ((r > 0 && r <= this.size) && (c > 0 && r <= this.size)){
			return this.mField[r][c];
		}
		return null;
	}
	
	public MazeCard getFreeCard(){
		return this.freeCard;
	}
	
	public int shift (MazeField mf){
		if (mf.row() == 1 && mf.col() % 2 == 0 && !(this.x == GamePanel.SIZE && this.y == mf.col())){
			MazeCard tmpFree = this.mField[this.size][mf.col()].getCard();
			MazeCard following;
			MazeCard actual = this.mField[1][mf.col()].getCard();
			for (int x = 2; x <= this.size; x++){
				following = this.mField[x][mf.col()].getCard();
				this.mField[x][mf.col()].putCard(actual);
				actual = following;
			}
			this.mField[1][mf.col()].putCard(freeCard);
			this.freeCard = tmpFree;
                        this.x = 1;
                        this.y = mf.col();
                        return 1;
		}
		else if (mf.row() == this.size && mf.col() % 2 == 0 && !(this.x == 1 && this.y == mf.col())){
			MazeCard tmpFree = this.mField[1][mf.col()].getCard();
			MazeCard following;
			MazeCard actual = this.mField[this.size][mf.col()].getCard();
			for (int x = this.size - 1; x >= 1; x--){
				following = this.mField[x][mf.col()].getCard();
				this.mField[x][mf.col()].putCard(actual);
				actual = following;
			}
			this.mField[this.size][mf.col()].putCard(freeCard);
			this.freeCard = tmpFree;
                        this.x = GamePanel.SIZE;
                        this.y = mf.col();
                        return 2;
		}
		else if (mf.row() % 2 == 0 && mf.col() == 1 && !(this.x == mf.row() && this.y == GamePanel.SIZE)){
			MazeCard tmpFree = this.mField[mf.row()][this.size].getCard();
			MazeCard following;
			MazeCard actual = this.mField[mf.row()][1].getCard();
			for (int x = 2; x <= this.size; x++){
				following = this.mField[mf.row()][x].getCard();
				this.mField[mf.row()][x].putCard(actual);
				actual = following;
			}
			this.mField[mf.row()][1].putCard(freeCard);
			this.freeCard = tmpFree;
                        this.x = mf.row();
                        this.y = 1;
                        return 3;
		}
		else if (mf.row() % 2 == 0 && mf.col() == this.size && !(this.x == mf.row() && this.y == 1)){
			MazeCard tmpFree = this.mField[mf.row()][1].getCard();
			MazeCard following;
			MazeCard actual = this.mField[mf.row()][this.size].getCard();
			for (int x = this.size - 1; x >= 1; x--){
				following = this.mField[mf.row()][x].getCard();
				this.mField[mf.row()][x].putCard(actual);
				actual = following;
			}
			this.mField[mf.row()][this.size].putCard(freeCard);
			this.freeCard = tmpFree;
                        this.x = mf.row();
                        this.y = GamePanel.SIZE;
                        return 4;
		}
                return 0;
	}
	public void drawBoard(Graphics2D g2){
            for (int y = 1; y <= this.size; y++){
                for (int x = 1; x <= this.size; x++){
                    if (this.mField[x][y].getCard().getState().equals("C")){
                        g2.drawImage(ImageLoader.rotate(this.c, this.mField[x][y].getCard().getPosition()),((((11 - this.size)/2) * 40) + 180) + (this.mField[x][y].col() - 1) * 40,                               
                            ((((11 - this.size)/2) * 40)) + 80 + (this.mField[x][y].row() - 1) * 40, 40, 40, null);
                    }
                    else if (this.mField[x][y].getCard().getState().equals("L")){
                        g2.drawImage(ImageLoader.rotate(this.l, this.mField[x][y].getCard().getPosition()),((((11 - this.size)/2) * 40) + 180) + (this.mField[x][y].col() - 1) * 40, 
                            ((((11 - this.size)/2) * 40)) + 80 + (this.mField[x][y].row() - 1) * 40, 40, 40, null);
                    }
                    else if (this.mField[x][y].getCard().getState().equals("F")){
                        g2.drawImage(ImageLoader.rotate(this.f, this.mField[x][y].getCard().getPosition()),((((11 - this.size)/2) * 40) + 180) + (this.mField[x][y].col() - 1) * 40, 
                            ((((11 - this.size)/2) * 40)) + 80 + (this.mField[x][y].row() - 1) * 40, 40, 40, null);
                    }
                    
                }
            }
            g2.setColor(Color.WHITE);
            g2.drawString("Volný kámen:", ((((11 - this.size)/2) * 40) + 65), 
                            ((((11 - this.size)/2) * 40)) + 70 + (GamePanel.SIZE/2)*40 );
            if (this.freeCard.getState().equals("C")){
                g2.drawImage(ImageLoader.rotate(this.c, this.freeCard.getPosition()),((((11 - this.size)/2) * 40) + 80), 
                            ((((11 - this.size)/2) * 40)) + 80 + (GamePanel.SIZE/2)*40 , 40, 40, null);
            }
            else if((this.freeCard.getState().equals("L"))){
                g2.drawImage(ImageLoader.rotate(this.l, this.freeCard.getPosition()),((((11 - this.size)/2) * 40) + 80), 
                            ((((11 - this.size)/2) * 40)) + 80 + (GamePanel.SIZE/2)*40 , 40, 40, null);
            }
            else{
                g2.drawImage(ImageLoader.rotate(this.f, this.freeCard.getPosition()),((((11 - this.size)/2) * 40) + 80), 
                            ((((11 - this.size)/2) * 40)) + 80 + (GamePanel.SIZE/2)*40 , 40, 40, null);
            }
            
	}

    /**
     * @param ld the ld to set
     */
    public void setLd(ImageLoader ld) {
        this.ld = ld;
    }
	
}
