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
byte lastMedButtonState = 1;  //previous medRading
 
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
    //unsigned long timer2 = millis();
    Serial.print("\nreached pomodoro1");
    fireSolenoid();
    etBool = 0;
    medBool = 0;
    int i;
    int j;
    Serial.print("\nreached past set bools to zero");
    for(j = 0; j++; j < 3){ 
      for(i = 0; i++; i < 2){
        delay(PomodoroWorkTime);
    //while (millis() - timer2 < 500){}
    //Serial.print("\nreached pomodoro2");
        fireSolenoid();
        delay(PomodoroShortBreak);
    //while (millis() - timer2 < 500){}
    //Serial.print("\nreached pomodoro3");
        fireSolenoid();
        }
      if (j == 2){break;} //edge case not needed on last run as one work 6 hours a day!
      delay(PomodoroLongBreak);
      fireSolenoid();   
      }
    }
  
  if (etBool && (medBool == 0) && (millis() - lastEtDebounceTime > interButtonDelay)){
    fireSolenoid();
    etBool = 0;
    delay(Tea);
    fireSolenoid();
    }

  if (medBool && (etBool == 0) && (millis() - lastMedDebounceTime > interButtonDelay)){
    //unsigned long timer3 = millis();
    int i;
    fireSolenoid();
    for(i = 0; i++; i < 6) {
      delay(MettaBhavanaPeriod);
      fireSolenoid();
      }
    medBool = 0;
    }  


}
