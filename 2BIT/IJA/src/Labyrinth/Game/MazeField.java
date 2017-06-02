package Labyrinth.Game;

import java.io.Serializable;

public class MazeField implements Serializable {
	private int row;
	private int col;
	private MazeCard card;
	
	public MazeField(int r, int c){
		this.row = r;
		this.col = c;
		this.card = null;
	}
	
	public MazeCard getCard(){
		return this.card;
	}
	
	public void putCard(MazeCard c){
		this.card = c;
	}
	
	public int row(){
		return this.row;
	}
	
	public int col(){
		return this.col;
	}
}
