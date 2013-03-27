  
	// Define pins	  
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
	
	float redMax = colour[0];
	float greenMax = colour[1];
	float blueMax = colour[2];
	
	void setup() {
		// Set pin modes
		pinMode(REDPIN, OUTPUT);
		pinMode(GREENPIN, OUTPUT);
		pinMode(BLUEPIN, OUTPUT);
		pinMode(button, INPUT);
		int lightState = 0;
		Serial.begin(9600);
			  
		digitalWrite(button, HIGH);
		
		Serial.println("FluxLight 3000 initiated...");	

		float test = colour[2]/4;		
		Serial.println(test,3);

		test = colour[2]/18;
		Serial.println(test,3);
		
		float myColour = colour[2];
		
		test = myColour/100;
		Serial.println(test,3);
		
		Serial.println(1.23456, 4);
		
		for(int i = 0; i<=255; i++) {
			Serial.println(i);
			analogWrite(REDPIN, i);
			delay(100);
			analogWrite(GREENPIN, i);
			delay(100);
			analogWrite(BLUEPIN, i);
			delay(100);
		}
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
  }
  
  // Fades the light up and down
  void plusOne() {
  	Serial.print("lightState ");
  	Serial.print(lightState);
  	Serial.print(" ");
  	Serial.print("Blue value: ");
  	Serial.println(blueValue);
	if(floor(redValue) == 255 | lightState == 1) {
		lightState = 1;
//		redValue--;
//		greenValue--;
//		blueValue--;
	} else if (floor(blueValue) == 0 | lightState == 0) {
		lightState = 0;
//		redValue++;
//		greenValue++;
		 blueValue++;
	}
	  analogWrite(REDPIN, redValue);
	  analogWrite(GREENPIN, greenValue);
	  analogWrite(BLUEPIN, blueValue);
	  delay(3);

	
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

