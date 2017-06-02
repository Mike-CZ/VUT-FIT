package Labyrinth.GUI;

import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class ImageLoader {
    private transient BufferedImage img;
    
    public BufferedImage loadImage (String path){
        File file = new File(path);
        try {
            this.img = ImageIO.read(file);
        } 
        catch (IOException e) {
            e.printStackTrace();
        }
        return this.img;
    }
   	public static BufferedImage rotate( BufferedImage inputImage, int count){
                        
            if (count == 0){
                return inputImage;
            }
            else if (count == 1){
                return ImageLoader.createRotated(inputImage, 1);
            }
            else if (count == 2){
                return ImageLoader.createRotated(inputImage, 2);
            }
            else if (count == 3){
                return ImageLoader.createRotated(inputImage, 3);
            }
            
            return inputImage;
	}
        
        private static BufferedImage createRotated(BufferedImage image, int degree){
            double rotate = 0.0;
            switch (degree){
                case 1:
                    rotate = Math.PI/2;
                    break;
                case 2:
                    rotate = Math.PI;
                    break;
                    
                case 3:
                    rotate = 1.5*Math.PI;
                    break;
            }
            
            AffineTransform at = AffineTransform.getRotateInstance(
            rotate, image.getWidth()/2, image.getHeight()/2.0);
            return createTransformed(image, at);
    }
        
        private static BufferedImage createTransformed(BufferedImage image, AffineTransform at){
            BufferedImage newImage = new BufferedImage(
            image.getWidth(), image.getHeight(),
            BufferedImage.TYPE_INT_ARGB);
            Graphics2D g = newImage.createGraphics();
            g.transform(at);
            g.drawImage(image, 0, 0, null);
            g.dispose();
            return newImage;
    }
}
