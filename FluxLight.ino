 /* Libraries for clock */
    #include <stdio.h>
    #include <string.h>
    #include <DS1302.h>

	// Time of alarm
	int dawn[] = {7, 00}; // eg 23:00



/* Clock */



/* 	Set the appropriate digital I/O pin connections
	CE_PIN stands for "clock-enable"
	On some boards this may be labeled "RST", or */
    uint8_t CE_PIN   = 9; // RST
    uint8_t IO_PIN   = 8; // DAT
    uint8_t SCLK_PIN = 7; // CLK

/* Left to right when plugged into a breadboard
	VCC
	GND
	CLK
	DAT
	RST
*/
	char buf[50]; // Buffer for time output
    DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN); // Create a DS1302 object
	int currentTime[] = {0,0};

	// Define pins for LEDs
	#define REDPIN 3
	#define GREENPIN 5
	#define BLUEPIN 6

	// Pin for the button
	const int button = 2;

	// Initial colour values
	float redValue = 0;
	float greenValue = 0;
	float blueValue = 0;

	// Switch debounce
	boolean state = 0; // Debounce state
	const int debounceDelay = 10;	// Switch debounce

	// Sunrise configuration
	int preDawnDurationInMinutes = 20; // The time it takes to fade the red in
	int sunriseDurationInMinutes = 40; // The time it takes from red to white
	int	totalSequenceDuration = preDawnDurationInMinutes + sunriseDurationInMinutes;
	int autoShutoffDelay = 60; // Number of minutes to leave light on for after dawn

	// Time of dawn/sunrise in minutes
	int timeOfDawnInMinutes = (dawn[0] * 60) + dawn[1];

	// Calculate the time to start sequence
	int timeToBeginSunriseSequence = timeOfDawnInMinutes - totalSequenceDuration;

	// Reading-light settings
	int readingLightColour[] = {255,100,0};
	int lightMode = 0;
	int  fadeTime = 255;
	float redMax = readingLightColour[0];
	float greenMax = readingLightColour[1];
	float blueMax = readingLightColour[2];

	void setup() {
		autoShutoffDelay = autoShutoffDelay * 60; // Convert to seconds

		// Set pin modes
		pinMode(REDPIN, OUTPUT);
		pinMode(GREENPIN, OUTPUT);
		pinMode(BLUEPIN, OUTPUT);
		pinMode(button, INPUT);
		digitalWrite(button, HIGH);
		
		int lightState = 0;


		Serial.begin(9600);
		// while the serial stream is not open, do nothing:
//		while (!Serial) ; // Uncomment this line when debugging using a leonardo
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

		Time time = rtc.time();
		// Format time into DATETIME
		  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
	           time.yr, time.mon, time.date,
	           time.hr, time.min, time.sec);
	    int currentTimeInMinutes = (time.hr * 60) + time.min;
	    Serial.print("Current time is: ");
	    Serial.println(buf);
	}

	void loop(){
		// Check for button press
		if (debounce(button)){
			readingLights();
		}

		// If the reading lights aren't on, continue the simulator
		if(lightMode == 0) {
			set_level();
		}
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
	
	// Turns on the reading-light
	void fadeIn() {
		Serial.println("fadeIn");
		for(int i = 0; i <= fadeTime; i++) {
			// Get colours
			float redValueFloat = (redMax / fadeTime) * i;
			float greenValueFloat = (greenMax / fadeTime) * i;
			float blueValueFloat = (blueMax / fadeTime) * i;
		  
			// Set colours
			redValue = floor(redValueFloat);
			greenValue = floor(greenValueFloat);
			blueValue = floor(blueValueFloat);
						
			//  Write to pins
			setLEDs(redValue, greenValue, blueValue);
		}
	}
	
	void setLEDs(int red, int green, int blue) {
		//  Write to pins
		analogWrite(REDPIN, red);
		analogWrite(GREENPIN, green);
		analogWrite(BLUEPIN, blue);
		delay(1);
		
		
		Serial.print(red);
		Serial.print(", ");
		Serial.print(green);
		Serial.print(", ");
		Serial.println(blue);
	}
	
	// Turns off the reading-light
	void fadeOut() {
		Serial.println("fadeOut");
		for(int i = fadeTime; i >= 0; i--) {
			float redValueFloat = (redMax / fadeTime) * i;
			float greenValueFloat = (greenMax / fadeTime) * i;
			float blueValueFloat = (blueMax / fadeTime) * i;
			
			redValue = floor(redValueFloat);
			greenValue = floor(greenValueFloat);
			blueValue = floor(blueValueFloat);
			
			//  Write to pins
			setLEDs(redValue, greenValue, blueValue);
		}
	}

	void set_level(){
		int timeOfDawnInSeconds = timeOfDawnInMinutes * 60;
		int timeToBeginSunriseSequenceInSeconds = timeToBeginSunriseSequence * 60;
		int preDawnDurationInSeconds = preDawnDurationInMinutes * 60;
		int sunriseDurationInSeconds = sunriseDurationInMinutes * 60;
		float value = 0; // Value of either Red or Green AND Blue RGBs
	//	check the time
	  	// Get current time
		Time time = rtc.time();

		// Format time into DATETIME
		snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
			time.yr, time.mon, time.date,
			time.hr, time.min, time.sec);

	    // time.day
	    // Day 1 is Sunday
	    int currentTimeInMinutes = (time.hr * 60) + time.min;
	    int currentTimeInSeconds = (currentTimeInMinutes * 60) + time.sec;

		if(time.sec == 0) {
			Serial.print("Current time is: ");
		    Serial.println(buf);

		}

		// if time is <= dawnTime & time >= sequenceBeginTime
		if(currentTimeInSeconds <= timeOfDawnInSeconds & currentTimeInSeconds >= timeToBeginSunriseSequenceInSeconds) {
			int minutesIntoSequence = totalSequenceDuration - (timeOfDawnInMinutes - currentTimeInMinutes);
			int secondsIntoSeqeunce = (minutesIntoSequence * 60) + time.sec;

			// if we're in preDawn mode
			if(secondsIntoSeqeunce <= preDawnDurationInSeconds) {
//				Serial.println("Pre-dawn mode");
				value = (255/(float)preDawnDurationInSeconds) * secondsIntoSeqeunce;
				redValue = floor(value);
				greenValue = 0;
				blueValue = 0;

			// else if we're in sunrise mode
			} else if (secondsIntoSeqeunce > preDawnDurationInSeconds) {
//				Serial.println("Sunrise mode");

				value = ((255/(float)sunriseDurationInSeconds) * ((secondsIntoSeqeunce-preDawnDurationInSeconds)));

				redValue = 255;
				greenValue = floor(value);
				blueValue = floor(value);
			// else if time is > dawnTime && time <= dawnTime + shutOffDelay
			}
		} else if (currentTimeInSeconds > timeOfDawnInSeconds & currentTimeInSeconds < (timeOfDawnInSeconds + autoShutoffDelay)) {
//			Serial.println("shutoff delay mode");

			// keep lights on
			redValue = 255;
			greenValue = 255;
			blueValue = 255;
		// we're neither in sunrise mode, nor shutOffDelay mode
		} else {
//			Serial.println("Fast asleep");

			redValue = 0;
			greenValue = 0;
			blueValue = 0;
		}

		setLEDs(redValue, greenValue, blueValue);
		
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
			if (state == 0) { // Button is currently being pressed
				sameCounter++;
				previousState = state;
			}
		}
		
		// If the state has been the same for the the requisite number of miliseconds
		if(sameCounter == debounceDelay) {
		  return true;
		} else {
		  return false;
		}
	}





