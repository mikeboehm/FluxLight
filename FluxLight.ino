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
  
	// Define global variables
	// COlour values
	float redValue = 0;
	float greenValue = 0;
	float blueValue = 0;
	boolean state = 0;
	const int debounceDelay = 10;
	// Whether light is on or off
//	int lightState = 0;
	//int colour[] = {255,142,0};
  
//	int colour[] = {77,153,25};
	
	
//	int sunriseTime[] = {7,0}; // 07:00am
//	int dawn[] = {7,0}; // 11:00pm
	int dawn[] = {7, 0}; // 11:00pm
	int preDawnDurationInMinutes = 20; // The time it takes to fade the red in
	int sunriseDurationInMinutes = 40; // The time it takes from red to white
	int	totalSequenceDuration = preDawnDurationInMinutes + sunriseDurationInMinutes;
	
	
	int autoShutoffDelay = 60; // How long to leave light on for
		
	int currentTime[] = {0,0}; // Set to zero
	
	// Time of dawn/sunrise in minutes
	int timeOfDawnInMinutes = (dawn[0] * 60) + dawn[1];
	
	// Calculate the time to start sequence
	int timeToBeginSunriseSequence = timeOfDawnInMinutes - totalSequenceDuration;
	
	
	// reading light settings
	int readingLightColour[] = {255,100,0};
	int lightMode = 0;
	int  fadeTime = 100;
	float redMax = readingLightColour[0];
	float greenMax = readingLightColour[1];
	float blueMax = readingLightColour[2];
	
	void setup() {
		// Set pin modes
		pinMode(REDPIN, OUTPUT);
		pinMode(GREENPIN, OUTPUT);
		pinMode(BLUEPIN, OUTPUT);
		pinMode(button, INPUT);
		int lightState = 0;
		digitalWrite(button, HIGH);


		Serial.begin(9600);
		Serial.println("FluxLight 3000 initiated...");	
		
		Serial.print("Dawn is at: ");
		Serial.print(dawn[0]);
		Serial.print(":");
		Serial.print(dawn[1]);
		Serial.print(" (");
		Serial.print(timeOfDawnInMinutes);
		Serial.println(")");

		
		Serial.print("Sunrise begins at ");
		Serial.println(timeToBeginSunriseSequence);

		// Float tests
		float test = readingLightColour[2]/4;		
		Serial.println(test,3);
		test = readingLightColour[2]/18;
		Serial.println(test,3);		
		float myColour = readingLightColour[2];		
		test = myColour/100;
		Serial.println(test,3);		
		Serial.println(1.23456, 4);
		
		Time time = rtc.time();	
		// Format time into DATETIME
		  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
	           time.yr, time.mon, time.date,
	           time.hr, time.min, time.sec);
	    int currentTimeInMinutes = (time.hr * 60) + time.min;      
	    Serial.print("Current time is: ");
	    Serial.println(buf);
		
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
	
	void readingLights() {
		if(lightMode == 0) {
			fadeIn();
			lightMode = 1;
		} else {
			fadeOut();
			lightMode = 0;		
		}
	}
	
	int set_level(){
		float value = 0; // Value of either Red or Green AND Blue RGBs
	//	check the time
	  	/* Get the current time and date from the chip */
		Time time = rtc.time();
	
		// Format time into DATETIME
		  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
	           time.yr, time.mon, time.date,
	           time.hr, time.min, time.sec);
	           
	    // time.day
	    // Day 1 is Sunday
	    int currentTimeInMinutes = (time.hr * 60) + time.min;      
	    Serial.print("Current time, in minutes ");
	    Serial.println(currentTimeInMinutes);
	    
	//		if time is <= dawnTime && time >= sequenceBeginTime {
		if(currentTimeInMinutes <= timeOfDawnInMinutes & currentTimeInMinutes >= timeToBeginSunriseSequence) {
			int minutesIntoSequence = totalSequenceDuration - (timeOfDawnInMinutes - currentTimeInMinutes);
			
			Serial.print("minutesIntoSequence: ");
			Serial.println(minutesIntoSequence);
			Serial.print("timeOfDawnInMinutes: ");
			Serial.println(timeOfDawnInMinutes);
			Serial.print("currentTimeInMinutes: ");
			Serial.println(currentTimeInMinutes);
			Serial.print("Number of seconds: ");
			Serial.println(time.sec);

			
			// if we're in preDawn mode
			if(minutesIntoSequence <= preDawnDurationInMinutes) {				
				Serial.println("pre dawn mode");
				// set RGB values accordingly
				value = (255/preDawnDurationInMinutes) * minutesIntoSequence;
//				value = (minutesIntoSequence/preDawnDurationInMinutes) * 255;
				redValue = floor(value);
				greenValue = 0;
				blueValue = 0;
			// else if we're in sunrise mode
			} else if (minutesIntoSequence > preDawnDurationInMinutes) {
				Serial.println("sunrise mode");
				Serial.print("sunriseDurationInMinutes");
				Serial.println(sunriseDurationInMinutes);

				Serial.print("(minutesIntoSequence-preDawnDurationInMinutes)");
				Serial.println((minutesIntoSequence-preDawnDurationInMinutes));

				
				// set RGB values accordingly				
				value = (255/sunriseDurationInMinutes) * (minutesIntoSequence-preDawnDurationInMinutes);

				Serial.print("floor(value)");
				Serial.println(floor(value));


//				value = ((minutesIntoSequence - sunriseDurationInMinutes)/sunriseDurationInMinutes) * 255;
				redValue = 255;
				greenValue = floor(value);
				blueValue = floor(value);
			// else if time is > dawnTime && time <= dawnTime + shutOffDelay
			} 
		} else if (currentTimeInMinutes > timeOfDawnInMinutes & currentTimeInMinutes < (timeOfDawnInMinutes + autoShutoffDelay)) {
			Serial.println("shutoff delay mode");

			// keep lights on
			redValue = 255;
			greenValue = 255;
			blueValue = 255;
		// we're neither in sunrise mode, nor shutOffDelay mode
		} else {
			Serial.println("Fast asleep");

			redValue = 0;
			greenValue = 0;
			blueValue = 0;
		}

		if(value) {
			Serial.print("value: ");
			Serial.println(value);
		}
		
		analogWrite(REDPIN, redValue);
		analogWrite(GREENPIN, greenValue);
		analogWrite(BLUEPIN, blueValue);
		
		Serial.print("R: ");
		Serial.print(redValue);
		Serial.print(" G: ");
		Serial.print(greenValue);
		Serial.print(" B: ");
		Serial.println(blueValue);
		Serial.println("");

		delay(3000);
//		

	
	//		} 
	//			keep lights on
	//		else
	//			we're neither in sunrise mode, nor shutOffDelay mode
	//			do nothing		
			  
	}
  
  
  int get_level() {
  	/* Get the current time and date from the chip */
	Time time = rtc.time();

	// Format time into DATETIME
	  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           time.yr, time.mon, time.date,
           time.hr, time.min, time.sec);
    
    int currentTimeInMinutes = (time.hr * 60) + time.min;
    Serial.print("current time in minutes ");
    Serial.println(currentTimeInMinutes);

    int turnOff = timeOfDawnInMinutes + 180;

    Serial.print("turn off, in minutes ");
    Serial.println(turnOff);
    
    Serial.print("Alarm time, in minutes ");
    Serial.println(timeOfDawnInMinutes);

    if(currentTimeInMinutes >= timeOfDawnInMinutes & currentTimeInMinutes <= turnOff) {
		int step = currentTimeInMinutes - timeOfDawnInMinutes;
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
	int sameCounter = 0;
	
	previousState = digitalRead(pin);
	for (int counter = 0; counter < debounceDelay; counter++) {
		delay(1);
				
		state = digitalRead(pin);
		if (state == 0) {
			sameCounter++;
			previousState = state;
		}
	}
	
	if(sameCounter == debounceDelay) {
	  return true;
	} else {
	  return false;
	}
  }
  
	void loop(){
		// Check for button press
		if (debounce(button)){
			readingLights();
		}
		
		// If the reading lights aren't on
		if(lightMode == 0) {
			set_level();
		}		

//		Serial.print("Level: ");
//		Serial.println(level);
//		make_it_light(level);
		delay(3000);
	}
  
  void make_it_light(int level) {
	  //  Write to pins	 
	  analogWrite(REDPIN, level);
//	  analogWrite(GREENPIN, level);
//	  analogWrite(BLUEPIN, level);

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

