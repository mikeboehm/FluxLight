 /* Libraries for clock */
    #include <stdio.h>
    #include <string.h>
    #include <DS1302.h>

// Connect clock to 5v pin (Mike)

/* Set the appropriate digital I/O pin connections
  CE_PIN stands for "clock-enable"
  On some boards this may be labeled "RST", or */
    uint8_t CE_PIN   = 12; // RST
    uint8_t IO_PIN   = 11; // DAT
    uint8_t SCLK_PIN = 10; // CLK
    
/* Left to right when plugged into a breadboard
	VCC
	GND
	CLK
	DAT
	RST
*/    
	
 /* Create buffers */ 
	char buf[50];

/* Create a DS1302 object */
    DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);
	

	// Define pins for LEDs
	#define REDPIN 3
	#define GREENPIN 5
	#define BLUEPIN 6
	const int button = 2;
	int fadeTime = 100;
  
	// Define global variables
	// COlour values
	float redValue = 0;
	float greenValue = 0;
	float blueValue = 0;
	boolean state = 0;
	const int debounceDelay = 10;
	// Whether light is on or off
	int lightState = 0;
	//int colour[] = {255,142,0};
  
//	int colour[] = {77,153,25};
	int colour[] = {255,255,255};
	
	int timeOfAlarm[] = {7,0}; // 07:00
	int dawnDuration = 90; // 90 min
	int currentTime[] = {0,0}; // Midnight?
	int alarmTime;
	
	float redMax = colour[0];
	float greenMax = colour[1];
	float blueMax = colour[2];
	
	void setup() {
		alarmTime = (timeOfAlarm[0] * 60) + timeOfAlarm[1];
		alarmTime = alarmTime - dawnDuration;
		// Set pin modes
		pinMode(REDPIN, OUTPUT);
		pinMode(GREENPIN, OUTPUT);
		pinMode(BLUEPIN, OUTPUT);
		pinMode(button, INPUT);
		int lightState = 0;
		Serial.begin(9600);
			  
		digitalWrite(button, HIGH);
		
		Serial.println("FluxLight 3000 initiated...");	
		Serial.println(alarmTime);
		float test = colour[2]/4;		
		Serial.println(test,3);

		test = colour[2]/18;
		Serial.println(test,3);
		
		float myColour = colour[2];
		
		test = myColour/100;
		Serial.println(test,3);
		
		Serial.println(1.23456, 4);
		
/*
		for(int i = 0; i<=255; i++) {
			Serial.println(i);
			analogWrite(REDPIN, i);
			delay(100);
			analogWrite(GREENPIN, i);
			delay(100);
			analogWrite(BLUEPIN, i);
			delay(100);
		}
*/
	}
  
  int is_it_time_to_start_waking_up_yet() {
  	/* Get the current time and date from the chip */
	Time time = rtc.time();

	// Format time into DATETIME
	  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           time.yr, time.mon, time.date,
           time.hr, time.min, time.sec);
    
    int currentTimeInMinutes = (time.hr * 60) + time.min;
    Serial.print('current time in minutes ');
    Serial.println(currentTimeInMinutes);

    int turnOff = alarmTime + 180;

    Serial.print('turn off, in minutes ');
    Serial.println(turnOff);
    
    Serial.print('Alarm time, in minutes ');
    Serial.println(alarmTime);

    if(currentTimeInMinutes >= alarmTime & currentTimeInMinutes <= turnOff) {
		int step = currentTimeInMinutes - alarmTime;
		return step;
    } else {
    	return 0;
    }
           
	/* Print the formatted string to serial so we can see the time */
		Serial.println(buf);

  }
  
  void print_time() {
	/* Get the current time and date from the chip */
	Time time = rtc.time();

	// Format time into DATETIME
	  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           time.yr, time.mon, time.date,
           time.hr, time.min, time.sec);
           
	/* Print the formatted string to serial so we can see the time */
		Serial.println(buf);

}
  
  // Button debouncer
  boolean debounce(int pin) {
	boolean state;
	boolean previousState;
	
	previousState = digitalRead(pin);
	for (int counter = 0; counter < debounceDelay; counter++) {
		delay(1);
		state = digitalRead(pin);
		if (state != previousState) {
			counter = 0;
			previousState = state;
		}
	}
	
	if(state == LOW) {
	  return true;
	} else {
	  return false;
	}
  }
  
  void getTime() {
  
  }
  
  void loop(){
	int buttonPress = digitalRead(button);
  //  Serial.println('loop');
  //  Serial.print('button value: ');
  //  Serial.println(digitalRead(button));
	
	if(debounce(button)) {
  //  if(buttonPress == LOW) {
  		if(lightState == 0) {
  			fadeIn();
  			lightState = 1;
  		} else {
  			fadeOut();
  			lightState = 0;
  		}
//	  plusOne();
	}
//	getTime();
//	print_time();	

		int level = is_it_time_to_start_waking_up_yet();
		Serial.print('Level: ');
		Serial.println(level);
		make_it_light(100);
		delay(1000);
  }
  
  void make_it_light(int level) {
	  //  Write to pins	 
	  analogWrite(REDPIN, level);
	  analogWrite(GREENPIN, level);
	  analogWrite(BLUEPIN, level);

  }
  
  void fadeIn() {
	Serial.println("fadeIn");
	for(int i = 0; i <= fadeTime; i++) {
  //	Serial.print('i: ');
  //	Serial.println(i);
  //  Red  
  //	redValue = (colour[0] / fadeTime) * i;
  //	redValue = floor(redValue);
  
	  // Get colours
//	  Serial.println((redMax / fadeTime) * i);
	  redValue = floor((redMax / fadeTime) * i);
	  greenValue = floor((greenMax / fadeTime) * i);
	  blueValue = floor((blueMax / fadeTime) * i);
	 
	  // Debug
	  Serial.print(redValue);
	  Serial.print(", ");
	  Serial.print(greenValue);
	  Serial.print(", ");
	  Serial.println(blueValue);
	  
	  //  Write to pins	 
	  analogWrite(REDPIN, redValue);
	  analogWrite(GREENPIN, greenValue);
	  analogWrite(BLUEPIN, blueValue);
	  delay(3);
	}
  }
  
  void fadeOut() {
//  	float redValue;
//	float greenValue;
//	float blueValue;
	Serial.println("fadeOut");
	for(int i = fadeTime; i >= 0; i--) {
	  redValue = floor(((redMax / fadeTime) * i / fadeTime) * i);
	  greenValue = floor((greenMax / fadeTime) * i);
	  blueValue = floor((blueMax / fadeTime) * i);
	  Serial.print(redValue);
	  Serial.print(", ");
	  Serial.print(greenValue);
	  Serial.print(", ");
	  Serial.println(blueValue);
	  analogWrite(REDPIN, redValue);
	  analogWrite(GREENPIN, greenValue);
	  analogWrite(BLUEPIN, blueValue);
	  delay(3);
	}
  }

