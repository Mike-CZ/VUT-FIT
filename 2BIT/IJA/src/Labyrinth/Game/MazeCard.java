package Labyrinth.Game;

import java.io.Serializable;
import java.util.EnumSet;

public class MazeCard implements Serializable {
	private EnumSet<CANGO> paths;
	private String type;
        private int position;
   
		
	public static enum CANGO{
		LEFT,
		UP,
		DOWN,
		RIGHT;
	}

	public static MazeCard create(String type){
		MazeCard card = new MazeCard();
		card.type = type;
                card.position = 0;
		
		if ("C".equals(type)){
			card.paths = EnumSet.of(CANGO.RIGHT, CANGO.DOWN);
		}
		else if ("L".equals(type)){
			card.paths = EnumSet.of(CANGO.DOWN, CANGO.UP);
		}		
		else if ("F".equals(type)){
			card.paths = EnumSet.of(CANGO.LEFT, CANGO.DOWN, CANGO.RIGHT);

		}
		else{
			throw new IllegalArgumentException();
		}
		return card;
	}
	
	public boolean canGo(MazeCard.CANGO dir){
		for (CANGO path : this.paths){
			if (path == dir){
				return true;
			}
		}
		return false;
	}
	
	public void turnRight(){
		EnumSet<CANGO> tmp = this.paths.clone();
		this.paths.clear();
                this.position = (this.getPosition()+1)%4;
		for (CANGO path : tmp){
			switch(path){
			case LEFT:
				this.paths.add(CANGO.UP);
				break;
			case UP:
				this.paths.add(CANGO.RIGHT);
				break;
				
			case DOWN:
				this.paths.add(CANGO.LEFT);
				break;
				
			case RIGHT:
				this.paths.add(CANGO.DOWN);
				break;
			}
		}
	}
	
	public String getState(){
		return this.type;
	}
        
        public int getPosition() {
        return position;
    }
}
