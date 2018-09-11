using System;
using System.Drawing;
using System.Threading;
using Tinkerforge;

// Tested with 
// > mcs /reference:System.Drawing.dll /target:exe /out:Example.exe /reference:Tinkerforge.dll ExampleScribble.cs

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID
	private static int WIDTH = 128;
	private static int HEIGHT = 64;

	private static void DrawBitmap(BrickletOLED128x64V2 oled, Bitmap bitmap)
	{
		bool[] pixels = new bool[HEIGHT*WIDTH];

		for (int row = 0; row < HEIGHT; row++)
		{
			for (int column = 0; column < WIDTH; column++)
			{
				pixels[row*WIDTH + column] = bitmap.GetPixel(column, row).GetBrightness() > 0;
			}
		}

		oled.WritePixels(0, 0, (byte)(WIDTH-1), (byte)(HEIGHT-1), pixels);
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletOLED128x64V2 oled = new BrickletOLED128x64V2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Clear display
		oled.ClearDisplay();

		// Draw rotating line
		Bitmap bitmap = new Bitmap(WIDTH, HEIGHT);
		int originX = WIDTH / 2;
		int originY = HEIGHT / 2;
		int length = HEIGHT / 2 - 2;
		int angle = 0;

		Console.WriteLine("Press enter to exit");

		while (!Console.KeyAvailable)
		{
			double radians = Math.PI * angle / 180.0;
			int x = (int)(originX + length * Math.Cos(radians));
			int y = (int)(originY + length * Math.Sin(radians));

			using (Graphics g = Graphics.FromImage(bitmap))
			{
				g.FillRectangle(Brushes.Black, 0, 0, WIDTH, HEIGHT);
				g.DrawLine(Pens.White, originX, originY, x, y);
			}

			DrawBitmap(oled, bitmap);
			Thread.Sleep(25);

			angle++;
		}

		Console.ReadLine();
		ipcon.Disconnect();
	}
}
