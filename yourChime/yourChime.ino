//yourChime Arduino Sketch by Michael G. Hale 18.05.22
// constants won't change. They're used here to set pin numbers:
const byte buttonPin = 9;     // the number of the egg/tea pin
const byte solenoidPin = 11;  // the number of the solenoid pin

// Variables will change:
//int solenoidState = LOW;         // the current state of the output pin
byte buttonState;          // the current buttonReading from the input pin
byte lastButtonState = 0;  // the previous buttonReading from the input pin

byte stateNumber = 0;  //holds number of button presses(0...2)
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastButtonDebounceTime = 0;  // the last time the output pin was toggsolenoid
unsigned long debounceDelay = 50;          // the debounce time; increase if the output flickers
unsigned long stateCheckTime;
unsigned short etButtonDelay = 2000;
unsigned short mettaBhavanaButtonDelay = 4000;
unsigned short pomodoroButtonDelay = 6000;
unsigned long switchPollDelay = 125;
unsigned long pomodoroWorkTime = 1200000;
unsigned long pomodoroShortBreak = 300000;
unsigned long pomodoroLongBreak = 1800000;
unsigned long mettaBhavanaPeriod = 300000;
unsigned long tea = 180000;
unsigned long startWaitingTime;
unsigned long startTimeChime = 0;
//switch char
char stateChar;
//state bools
byte inTea;
byte inMettaBhavana;
byte inPomodoro;
int ms;
//vars for loops (i&j in pomodoro and k in Metta Bhavana)
int i;
int j;
int k;

byte buttonReading;


// trigger solenoid:
int fireSolenoid() {
  digitalWrite(solenoidPin, HIGH);
  unsigned long timer1 = millis();
  while (millis() - timer1 < 200) {}
  digitalWrite(solenoidPin, LOW);
  Serial.print("\nFired Solenoid");
}

void pollSwitches() {
  // read the state of the switch into a local variable:
  buttonReading = digitalRead(buttonPin);
  //Serial.print("done Reading");
  // check to see if you just pressed a button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  Serial.print(buttonReading);
  // If the switch changed, due to noise or pressing:
  if (buttonReading != lastButtonState) {
    // reset the debouncing timer
    lastButtonDebounceTime = millis();
    Serial.print("\nbutton pressed or noise");
  }


  if ((millis() - lastButtonDebounceTime) > debounceDelay) {
    // whatever the buttonReading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    Serial.print("\nButton debounced");

    // if the button state has changed:
    if (buttonReading != buttonState) {
      Serial.print("\nReached second button reading not equal to buttonState");
      buttonState = buttonReading;
      stateCheckTime = millis();
      // select state from button push time
      if (buttonState == 1) {
        Serial.print("\nbutton state = 1");
        stateNumber = 0;
        Serial.print("got et");
        
        stateChar = 'm';
        Serial.print("\ngot m");
        
        stateChar = 'p';
        Serial.print("\ngot p");
        //Serial.print("\netBool=1");
        //delay(200);
      }
    }
  }
 
}
// save the Reading. Next time through the loop, it'll be the lastEt/MedButtonState:
//Serial.println("\nreached lastbuttonState");
//delay(200);



void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(solenoidPin, OUTPUT);

  // set initial solenoid state
  digitalWrite(solenoidPin, 0);
  Serial.begin(9600);
}

void loop() {
  pollSwitches();
  Serial.print("\n");
  Serial.print(stateChar);
  switch (stateChar) {


    case 'e':

      Serial.print("\nEntered tea");
      inTea = 1;
      if (startTimeChime == 0) { startTimeChime = millis(); }
      if (millis() - startTimeChime < 200) { fireSolenoid(); }
      if (millis() - startWaitingTime > tea) {
        fireSolenoid();
        startTimeChime = 0;
        Serial.print("\nLeaving tea");
        inTea = 0;
      }
      break;

    case 'm':
      Serial.print("\nEntered Metta Bhavana");
      inMettaBhavana = 1;
      if (startTimeChime == 0) { startTimeChime = millis(); }
      if (millis() - startTimeChime < 200) { fireSolenoid(); }


      Serial.print("\nIn Metta Bhavana loop cycle ");
      Serial.print(k);
      if ((millis() - startWaitingTime > mettaBhavanaPeriod) && (k < 6)) {
        fireSolenoid();
        Serial.print("\nFired solenoid on round ");
        Serial.print(k);
        k++;
        startWaitingTime = millis();
      }
      if (k >= 6) {
        startTimeChime = 0;
        Serial.print("\nLeaving Metta Bhavana");
      }


      break;

    case 'p':
      //pomodoro day business logic
      Serial.print("\nreached pomodoro1");
      fireSolenoid();
      for (j = 0; j < 3; j++) {
        Serial.print("\nEntered Pomodoro outer loop ");
        Serial.print(j);
        for (i = 0; i < 4; i++) {
          Serial.print("\nEntered Pomodoro inner loop");
          Serial.print("\nPomodoro Work Time ");
          Serial.print(i);
          delay(pomodoroWorkTime);  //commented out delays for rapid testing with serial output
                                    //while (millis() - timer2 < 500){}
                                    //Serial.print("\nreached pomodoro2");
          fireSolenoid();
          Serial.print("\nPomodoro Short Break");
          delay(pomodoroShortBreak);
          //while (millis() - timer2 < 500){}
          //Serial.print("\nreached pomodoro3");
          fireSolenoid();
        }
        if (j == 2) { break; }  //edge case not needed on last run as one works 6 hours a day!
        Serial.print("\nPomodoro Long Break");
        delay(pomodoroLongBreak);
        fireSolenoid();
      }
      break;
  }
 // save the Reading. Next time through the loop, it'll be the lastButtonState
  lastButtonState = buttonReading;

}
