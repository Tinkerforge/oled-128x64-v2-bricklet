import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletOLED128x64V2;

public class ExampleScribble {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID
	private static final short WIDTH = 128;
	private static final short HEIGHT = 64;

	static void drawImage(BrickletOLED128x64V2 oled, BufferedImage image) throws Exception {
		boolean[] pixels = new boolean[HEIGHT * WIDTH];
		short h, w;

		for (h = 0; h < HEIGHT; h++) {
			for (w = 0; w < WIDTH; w++) {
				pixels[h*WIDTH + w] = (image.getRGB(w, h) & 0x00FFFFFF) > 0;
			}
		}
		oled.writePixels(0, 0, WIDTH-1, HEIGHT-1, pixels);
	}

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletOLED128x64V2 oled = new BrickletOLED128x64V2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Clear display
		oled.clearDisplay();

		// Draw rotating line
		BufferedImage image = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_ARGB);
		int originX = WIDTH / 2;
		int originY = HEIGHT / 2;
		int length = HEIGHT / 2 - 2;
		int angle = 0;

		System.out.println("Press ctrl+c to exit");

		while (true) {
			double radians = Math.toRadians(angle);
			int x = (int)(originX + length * Math.cos(radians));
			int y = (int)(originY + length * Math.sin(radians));
			Graphics g = image.createGraphics();

			g.setColor(Color.black);
			g.fillRect(0, 0, WIDTH, HEIGHT);
			g.setColor(Color.white);
			g.drawLine(originX, originY, x, y);
			g.dispose();

			drawImage(oled, image);
			Thread.sleep(25);

			angle++;
		}
	}
}
