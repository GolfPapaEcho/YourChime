//yourChime Arduino Sketch by Michael G. Hale 18.05.22 
// constants won't change. They're used here to set pin numbers:
const byte etPin = 9;        // the number of the egg/tea pin
const byte medPin = 10;// the number of the meditation pin
const byte solenoidPin = 11; // the number of the solenoid pin

// Variables will change:
//int solenoidState = LOW;         // the current state of the output pin
byte etButtonState;                // the current etReading from the input pin
byte medButtonState;               // current medReading 
byte lastEtButtonState = 0;   // the previous etReading from the input pin
byte lastMedButtonState = 0;  //previous medRading
 
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastEtDebounceTime = 0;
unsigned long lastMedDebounceTime = 0; // the last time the output pin was toggsolenoid
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long interButtonDelay = 2000;
unsigned long switchPollDelay = 125;
unsigned long PomodoroWorkTime = 1200000;
unsigned long PomodoroShortBreak = 300000;
unsigned long PomodoroLongBreak = 1800000;
unsigned long MettaBhavanaPeriod =300000;
unsigned long Tea = 180000;
byte etBool;
byte medBool;
int ms;
//byte *eB;
//byte *mB;

byte etReading;
byte medReading;

  // trigger solenoid:
  int fireSolenoid(){
    digitalWrite(solenoidPin, HIGH);
    unsigned long timer1 = millis();
    while (millis() - timer1 < 200){}
    digitalWrite(solenoidPin, LOW);
    Serial.print("\nFired Solenoid");
  }

  void pollSwitches(){
    // read the state of the switches into a local variables:
    etReading = digitalRead(etPin);
    medReading = digitalRead(medPin);
    //Serial.print("done Reading");
    // check to see if you just pressed a button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (etReading != lastEtButtonState)  {
    // reset the debouncing timer
    lastEtDebounceTime = millis();
    }
    if (medReading != lastMedButtonState) {
      lastMedDebounceTime = millis();
    }

    if ((millis() - lastEtDebounceTime) > debounceDelay) {
      // whatever the etReading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:

      // if the button state has changed:
      if (etReading != etButtonState) {
        etButtonState = etReading;

        // only toggle the solenoid if the new button state is HIGH
        if (etButtonState == 1) {
          etBool = 1;
          //Serial.print("\netBool=1");
          //delay(200);
        }
      }
    }

    if ((millis() - lastMedDebounceTime) > debounceDelay) {
      // whatever the medReading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:

      // if the button state has changed:
      if (medReading != medButtonState) {
        medButtonState = medReading;

      // only toggle the solenoid if the new button state is HIGH
      if (medButtonState == 1) {
        medBool = 1;
        //Serial.print("\nmedBool=1");
        //delay(200);
      }
    }
  }
// save the Reading. Next time through the loop, it'll be the lastEt/MedButtonState:
  lastEtButtonState = etReading;
  //Serial.println("\nreached lastEtButtonState");
  //delay(200);
  lastMedButtonState = medReading;
  //Serial.println("\nreached lastMedButtonState");
  //delay(200);
} 



void setup() {
  pinMode(etPin, INPUT);
  pinMode(medPin, INPUT);
  pinMode(solenoidPin, OUTPUT);

  // set initial solenoid state
  digitalWrite(solenoidPin, 0);
  Serial.begin(9600);

  
}

void loop() {
  pollSwitches();

  if (etBool && medBool && ((millis() - lastEtDebounceTime > interButtonDelay) || (millis() - lastMedDebounceTime > interButtonDelay))){
    //pomodoro day business logic
    Serial.print("\nreached pomodoro1");
    fireSolenoid();
    etBool = 0;
    medBool = 0;
    int i;
    int j;
    Serial.print("\nreached past set bools to zero");
    for(j = 0; j < 3; j++){ 
      Serial.print("\nEntered Pomodoro outer loop ");
      Serial.print(j);
      for(i = 0; i < 4; i++){
        Serial.print("\nEntered Pomodoro inner loop");
        Serial.print("\nPomodoro Work Time ");
        Serial.print(i);
        delay(PomodoroWorkTime);  //commented out delays for rapid testing with serial output
    //while (millis() - timer2 < 500){}
    //Serial.print("\nreached pomodoro2");
        fireSolenoid();
        Serial.print("\nPomodoro Short Break");
        delay(PomodoroShortBreak);
    //while (millis() - timer2 < 500){}
    //Serial.print("\nreached pomodoro3");
        fireSolenoid();
        }
      if (j == 2){break;} //edge case not needed on last run as one works 6 hours a day!
      Serial.print("\nPomodoro Long Break");
      delay(PomodoroLongBreak);
      fireSolenoid();   
      }
    }
  
  if (etBool && (medBool == 0) && (millis() - lastEtDebounceTime > interButtonDelay)){
    Serial.print("\nEntered Tea");
    fireSolenoid();
    etBool = 0;
    delay(Tea);
    fireSolenoid();
    Serial.print("\nLeaving Tea");
    }

  if (medBool && (etBool == 0) && (millis() - lastMedDebounceTime > interButtonDelay)){
    //unsigned long timer3 = millis();
    Serial.print("\nEntered Metta Bhavana");
    int k;
    fireSolenoid();
    for (k = 0; k < 6; k++) {
      Serial.print("\nIn Metta Bhavana loop cycle ");
      Serial.print(k);
      delay(MettaBhavanaPeriod);
      fireSolenoid();
      }
    medBool = 0;
    Serial.print("\nLeaving Metta Bhavana");
    }  
//Serial.print("\nWaiting for input");

}
