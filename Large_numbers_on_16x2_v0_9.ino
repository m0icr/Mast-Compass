

/*
A set of custom made large numbers for a 16x2 LCD using the
LiquidCrystal librabry. Works with displays compatible with the
Hitachi HD44780 driver.

The Cuicuit:
LCD RS pin to D12
LCD Enable pin to D11
LCD D4 pin to D5
LCD D5 pin to D4
LCD D6 pin to D3
LCD D7 pin to D2
LCD Vee tied to a pot to control brightness
LCD Vss and R/W tied to ground
LCD Vcc to +5V
Ronivaldo Sampaio (ronivaldo@gmail.com) for 3 lines
2013/05/15 -> change for character with only 3 lines
Original Made by Michael Pilcher
2/9/2010

character created using:
http://mikeyancey.com/hamcalc/lcd_characters.php
*/

// include the library
#include <LiquidCrystal.h>

/***************

i2C library


****************/


//#include <LiquidTWI_23017.h>  // Liquid Crystal I2C Library
//#define lcd_address 0 // I2C LCD Address
//#define lcd_end 20 // set width of LCD
//#define lcd_lines 4 // Number of lines on 

#include <Wire.h>  //i2c library for the Digital Compass


// initialize the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//LiquidTWI lcd(lcd_address);  // set the LCD I2C address to 0x27 for a 16 chars and 2 line display	


const int hmc5883Address = 0x1E; //0011110b, I2C 7bit address for compass
const byte hmc5883ModeRegister = 0x02;
const byte hmcContinuousMode = 0x00;
const byte hmcDataOutputXMSBAddress = 0x03;

// the 8 arrays that form each segment of the custom numbers
byte custChar[8][8] =
{
	{
		B11100,
		B11110,
		B11110,
		B11110,
		B11110,
		B11110,
		B11110,
		B11100
	},
	{
		B00111,
		B01111,
		B01111,
		B01111,
		B01111,
		B01111,
		B01111,
		B00111
	},
	{
		B11111,
		B11111,
		B00000,
		B00000,
		B00000,
		B00000,
		B11111,
		B11111
	},
	{
		B11110,
		B11100,
		B00000,
		B00000,
		B00000,
		B00000,
		B11000,
		B11100
	},
	{
		B01111,
		B00111,
		B00000,
		B00000,
		B00000,
		B00000,
		B00011,
		B00111
	},
	{
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B11111,
		B11111
	},
	{
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00111,
		B01111
	},
	{
		B11111,
		B11111,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000,
		B00000
	}
};


uint8_t bignums[10][2][3] =
{
	{
		{ '\x1', '\x7', '\x0' },		// digit 0
		{ '\x1', '\x5', '\x0' }
	},
	{
		{ '\x20', '\x20', '\x0' },		// digit 1
		{ '\x20', '\x20', '\x0' }
	},
	{
		{ '\x4', '\x2', '\x0' },		// digit 2
		{ '\x1', '\x5', '\x5' }
	},
	{
		{ '\x4', '\x2', '\x0' },		// digit 3
		{ '\x6', '\x5', '\x0' }
	},
	{
		{ '\x1', '\x5', '\x1' },		// digit 4
		{ '\x20', '\x20', '\x0' }
	},
	{
		{ '\x1', '\x2', '\x3' },		// digit 5
		{ '\x6', '\x5', '\x0' }
	},
	{
		{ '\x1', '\x2', '\x3' },		// digit 6
		{ '\x1', '\x5', '\x0' }
	},
	{
		{ '\x1', '\x7', '\x0' },		// digit 7
		{ '\x20', '\x20', '\x0' }
	},
	{
		{ '\x1', '\x2', '\x0' },		// digit 8
		{ '\x1', '\x5', '\x0' }
	},
	{
		{ '\x1', '\x2', '\x0' },		// digit 9
		{ '\x6', '\x5', '\x0' }
	}
};



// table of bearing for compass meter
// table = {min_bearing, max_bearing, pointer_col

int meter_table[21][3] =
{
	{ 171, 188, 0 },      //S
	{189, 206, 1 },
	{ 207, 224, 2 },
	{ 225, 242, 3 },
	{ 243, 260, 4 },
	{ 261, 278, 5 },     //W
	{279, 296, 6},
	{ 297, 314, 7 },
	{ 315, 332, 8 },
	{ 333, 350, 9 },     
	{ 351, 360, 10 },       //N
	{ 0, 8, 10 },	       //N
	{ 9, 26, 11 },
	{ 27, 44, 12 },
	{ 45, 62, 13 },       
	{ 63, 80, 14 },
	{ 81, 98, 15 },      //E
	{99, 116, 16},
	{ 117, 134, 17 },
	{135, 152, 18},
	{ 153, 170, 19 }

		
};



void setup()
{

	// start serial lcd
	//lcd.begin(lcd_end, lcd_lines); // initialize the LCD  

	Wire.begin(); // Start the i2c communication
	Wire.beginTransmission(hmc5883Address);  //Begin communication with compass
	Wire.write(hmc5883ModeRegister);  //select the mode register
	Wire.write(hmcContinuousMode); //continuous measurement mode
	Wire.endTransmission();

	// assignes each segment a write number
	loadchars();
	
	// start serial lcd
	// lcd.begin(lcd_end, lcd_lines); // initialize the LCD  
	// sets the LCD's rows and colums:

	lcd.begin(20, 4);

	// creates the custom fonts in LCD
	loadchars();


	lcd.setCursor(0, 0);
	lcd.print("Remote compass and");
	lcd.setCursor(0, 1);
	lcd.print("rotator controller");
 lcd.setCursor(0, 2);
 lcd.print("by G8FKH & M0ICR");
 lcd.setCursor(0, 3);
	lcd.print("www.drownedrats.uk");


	delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setup");
  lcd.setCursor(0, 1);
  lcd.print("> Compass only");
 lcd.setCursor(0, 2);
 lcd.print("  Rotator Controller");
 lcd.setCursor(0, 3);
  lcd.print("  Calibrate/ config");
    delay(8000);
	lcd.clear();
	
	

	// display compass meter
	lcd.setCursor(0, 0);
	lcd.print("S....W....N....E....");
	lcd.setCursor(0, 1);
	lcd.print("^-------------------");

	

	

}





void loop()
{
	
	
	
	
	int x, y, z; //triple axis data.
	//Tell the HMC5883L where to begin reading the data
	Wire.beginTransmission(hmc5883Address);
	Wire.write(hmcDataOutputXMSBAddress);  //Select register 3, X MSB register
	Wire.endTransmission();

	//Read data from each axis
	Wire.requestFrom(hmc5883Address, 6);
	if (6 <= Wire.available())
	{
		x = Wire.read() << 8; //X msb
		x |= Wire.read();   //X lsb
		z = Wire.read() << 8; //Z msb
		z |= Wire.read();   //Z lsb
		y = Wire.read() << 8; //Y msb
		y |= Wire.read();   //Y lsb    
	}

	int angle = atan2(-y, x) / M_PI * 180;
	if (angle < 0)
	{
		angle = angle + 360;
	}
	printDEG(angle);
}

void printDEG(int deg) {
	int m, c, d, number;

	number = deg * 10;
	if (number > 999) {
		m = (number - (number % 1000)) / 1000;
		number = number % 1000;
	}
	else {
		m = 0;
	}

	if (number > 99) {
		c = (number - (number % 100)) / 100;
		number = number % 100;
	}
	else {
		c = 0;
	}

	if (number > 9) {
		d = (number - (number % 10)) / 10;
		number = number % 10;
	}
	else {
		d = 0;
	}


	// This is where we display the bearing

	//lcd.setCursor(0, 0);
	//lcd.print("S....W....N....E....");
	//lcd.setCursor(0, 1);
	//lcd.print("^-------------------");
	lcd.setCursor(15, 2);
	lcd.print(" O");
	lcd.setCursor(0, 2);
  lcd.print("go2");
	lcd.setCursor(0, 3);
	lcd.print("185");
  lcd.setCursor(18, 3);
  lcd.print("<<");
  
	//char* Direction = calc_direction(deg);

	// display the compass bearing on lcd meter
	display_compass_meter(deg);


	// lcd.setCursor(15, 3);




	printbigchar(m, 4, 2);
	printbigchar(c, 8, 2);
	printbigchar(d, 12, 2);
	delay(800);
}



// Display a compass meter as one line across the display


void display_compass_meter(int bearing){

	static int cursor_location = 0;
	int pointer = 0;

	for (pointer = 0; pointer < 21; pointer++)
	{
		if (bearing >= meter_table[pointer][0] && bearing <= meter_table[pointer][1])
		{
			// found where we need to write the pointer
			// clear old pointer
			lcd.setCursor(cursor_location, 1);
			lcd.print("-");
			//now print new cursor
			lcd.setCursor(meter_table[pointer][2], 1);
			lcd.print("^");
			// update cursor position
			cursor_location = meter_table[pointer][2];
		}
	}











}



// load bit maps into custom characters



void loadchars() {

	for (int i = 0; i < 8; i++){
		//create a new custom character
		lcd.createChar(i, custChar[i]);
	}

}


// Print large digits on 4 line display
// Digit = 0 - 9,prints numbers at column position
// Digit = 10 will clear the digit from column position
// Column values = 0, 4, 8, 12
// row values = 0, 1 or 2

void printbigchar(byte digit, byte col, byte row) {
	
	int ROWS = 2;			// number of rows in font
	int COLS = 3;			// number of columns in font
	
	if (digit > 9) return;
	for (int i = 0; i < ROWS; i++) {
		lcd.setCursor(col, row + i);
		for (int j = 0; j < COLS; j++) {
			lcd.write(bignums[digit][i][j]);

		}
		//lcd.write(254);
	}

}
