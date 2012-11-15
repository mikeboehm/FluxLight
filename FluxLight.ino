

#define REDPIN 3
#define GREENPIN 5
#define BLUEPIN 6
const int button = 2;
int fadeTime = 100;

const int debounceDelay = 10;
// Whether light is on or off
int lightState = 0;
//int colour[] = {255,142,0};

int colour[] = {77,153,25};

void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(button, INPUT);
  int lightState = 0;
  Serial.begin(9600);
  
//  analogWrite(REDPIN, 255);
//  analogWrite(GREENPIN, 142);
//  analogWrite(BLUEPIN, 0);
  
  digitalWrite(button, HIGH);
  
  Serial.println("Hello Computer");
  
}



boolean debounce(int pin) {
  Serial.println('debounce');
  boolean state;
  boolean previousState;
  
  previousState = digitalRead(pin);
  for (int counter = 0; counter < debounceDelay; counter++) {
    delay(1);
    Serial.print(' Counter: ');
    Serial.println(counter);
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
  
//  if(debounce(button)) {
  if(buttonPress == LOW) {
    Serial.print("Light state: ");
    Serial.println(lightState);
//    analogWrite(REDPIN, 255);
    if(lightState == 0) {
      fadeIn();
      lightState = 1;
    } else {
      fadeOut();
      lightState = 0;
    }
  }
}

void fadeIn() {
  float redValue;
  int greenValue;
  int blueValue;
  Serial.println("fadeIn");
  for(int i = 0; i <= fadeTime; i++) {
//    Serial.print('i: ');
//    Serial.println(i);
//  Red  
//    redValue = (colour[0] / fadeTime) * i;
//    redValue = floor(redValue);

    // Get colours
    redValue = floor((colour[0] / fadeTime) * i);
    greenValue = floor((colour[1] / fadeTime) * i);
    blueValue = floor((colour[2] / fadeTime) * i);
   
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
  float redValue;
  int greenValue;
  int blueValue;
  Serial.println("fadeOut");
  for(int i = fadeTime; i >= 0; i--) {
    redValue = floor((colour[0] / fadeTime) * i);
    greenValue = floor((colour[1] / fadeTime) * i);
    blueValue = floor((colour[2] / fadeTime) * i);
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

