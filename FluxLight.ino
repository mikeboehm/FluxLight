// color swirl! connect an RGB LED to the PWM pins as indicated
// in the #defines
// public domain, enjoy!
 

#define REDPIN 3
#define GREENPIN 5
#define BLUEPIN 6
const int button = 2;

const int debounceDelay = 10;
// Whether light is on or off
int lightState = 0;
 
void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(button, INPUT);
  int lightState = 0;
  Serial.begin(9600);
  
  analogWrite(REDPIN, 255);
  analogWrite(GREENPIN, 142);
  analogWrite(BLUEPIN, 0);
  
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
  Serial.println("fadeIn");
  for(int i = 0; i <= 255; i++) {
//    Serial.print('i: ');
//    Serial.println(i);
    analogWrite(REDPIN, i);
    analogWrite(GREENPIN, i);
    analogWrite(BLUEPIN, i);
    delay(3);
  }
}

void fadeOut() {
  Serial.println("fadeOut");
  for(int i = 255; i >= 0; i--) {
    analogWrite(REDPIN, i);
    analogWrite(GREENPIN, i);
    analogWrite(BLUEPIN, i);
    delay(3);
  }
}

