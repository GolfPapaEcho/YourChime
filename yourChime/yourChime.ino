//yourChime Arduino Sketch by Michael G. Hale 18.05.22
// constants won't change. They're used here to set pin numbers:
const byte buttonPin = 9;     // the number of the egg/tea pin
const byte solenoidPin = 11;  // the number of the solenoid pin
//byte LEDPin = 13;
// Variables will change:
//int solenoidState = LOW;         // the current state of the output pin
byte buttonState;          // the current buttonReading from the input pin
byte lastButtonState = 0;  // the previous buttonReading from the input pin

byte stateNumber;  //holds number of button presses(0...3)
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastButtonDebounceTime = 0;  // the last time the output pin was toggsolenoid
unsigned long debounceDelay = 50;          // the debounce time; increase if the output flickers
unsigned long stateCheckTime;
unsigned short etButtonDelay = 2000;
unsigned short mettaBhavanaButtonDelay = 4000;
unsigned short pomodoroButtonDelay = 6000;
unsigned long switchPollDelay = 125;
unsigned long pomodoroWorkTime = 20000;//for testing set to 20000 for production set to 1200000;
unsigned long pomodoroLongBreak = 30000;//for testing set to 30000 sec for production set to 1800000;
unsigned long mettaBhavanaPeriod = 5000;// for testing set to 5000 for production set to 300000; //used for both Metta Bhavana and pomodoroShortBreak
unsigned long tea = 180000;
unsigned long startWaitingTime;
unsigned long startTimeChime = 0;

//state bools
byte inTea;
byte inMettaBhavana;
byte inPomodoro;
byte waitType; //0 for pomodoroWorkTime 1 short break 2 long break
int ms;
//vars for loops (i&j in pomodoro and k in Metta Bhavana)
int i = 0;
int j = 0;
int k = 0;


byte buttonReading;


// trigger solenoid:
int fireSolenoid() {
  digitalWrite(solenoidPin, HIGH);
  unsigned long timer1 = millis();
  while (millis() - timer1 < 200) {}
  digitalWrite(solenoidPin, LOW);
  Serial.print("\nFired Solenoid");
}
void flashLight(){
  for (int l = 0; l < stateNumber; l++){
    digitalWrite(LED_BUILTIN, 1);
    delay(500);
    digitalWrite(LED_BUILTIN, 0);
    delay(500);
  }
}

void pollSwitches() {
  // read the state of the switch into a local variable:
  buttonReading = digitalRead(buttonPin);
  //Serial.print("done Reading");
  // check to see if you just pressed a button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  //Serial.print(buttonReading);
  // If the switch changed, due to noise or pressing:
  if (buttonReading != lastButtonState) {
    // reset the debouncing timer
    lastButtonDebounceTime = millis();
    Serial.print("\nbutton pressed or noise");
  }


  if ((millis() - lastButtonDebounceTime) > debounceDelay) {
    // whatever the buttonReading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    //Serial.print("\nButton debounced");

    // if the button state has changed:
    if (buttonReading != buttonState) {
      buttonState = buttonReading;
      stateCheckTime = millis();
      // select state from button push time
      if (buttonState == 1) {
        //do nothing here as we want to dectect falling edge
      } else {
        stateNumber++;
        if (stateNumber > 3) {stateNumber = 0;}
        startTimeChime = 0;
        inTea = 0;
        inMettaBhavana = 0;
        inPomodoro = 0;
        startWaitingTime = millis();
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
  pinMode(LED_BUILTIN, OUTPUT);

  // set initial solenoid state
  digitalWrite(solenoidPin, 0);
  Serial.begin(9600);
}

void loop() {
  pollSwitches();
  Serial.print("\n");
  Serial.print(stateNumber);
  switch (stateNumber) {


    case 1:

      Serial.print("\nEntered tea");
      if (startTimeChime == 0) { startTimeChime = millis(); }
      if (millis() - startTimeChime < 200) { fireSolenoid(); }
      if (inTea == 0){flashLight(); inTea = 1;}
      if (millis() - startWaitingTime > tea) {
        fireSolenoid();
        flashLight();
        startTimeChime = 0;
        startWaitingTime = 0;
        stateNumber = 0;
        Serial.print("\nLeaving tea");
        inTea = 0;
        inMettaBhavana = 0;
        inPomodoro = 0;
      }
      break;

    case 2:
      Serial.print("\nEntered Metta Bhavana");
      if (startTimeChime == 0) { startTimeChime = millis(); }
      if (millis() - startTimeChime < 200) { fireSolenoid(); }
      if (inMettaBhavana == 0){flashLight(); inMettaBhavana = 1; }
      Serial.print("\nIn Metta Bhavana loop cycle ");
      Serial.print(k);
      if ((millis() - startWaitingTime > mettaBhavanaPeriod) && (k < 6)) {
        fireSolenoid();
        Serial.print("\nFired solenoid on round ");
        Serial.print(k);
        k++;
        startWaitingTime = millis();
      }
      if (k > 6) {
        startTimeChime = 0;
        startWaitingTime = 0;
        stateNumber = 0;
        inMettaBhavana = 0;
        Serial.print("\nLeaving Metta Bhavana");
      }
      break;

    case 3:
      //pomodoro day business logic
      Serial.print("\nreached pomodoro1");
      if (startTimeChime == 0) { startTimeChime = millis(); }
      if (millis() - startTimeChime < 200) { fireSolenoid(); }
      if (inPomodoro == 0){flashLight(); i = 0; waitType = 0; inPomodoro = 1;}
      switch (waitType){
        case 0:
          Serial.print("\nPomodoro Work Time ");
          Serial.print(waitType);
          if (millis() - startWaitingTime > pomodoroWorkTime){
            fireSolenoid();
            waitType = 1;
            startWaitingTime = millis();
          }
        break;
        
        case 1:
          Serial.print("\nPomodoro Short Break");
          if(millis() - startWaitingTime > mettaBhavanaPeriod){
            fireSolenoid();
            if(i < 4){
              waitType = 0;
              i++;
              startWaitingTime = millis();
              }
            else if(j <= 2){
                i = 0;
                j++;
                waitType = 2;
                startWaitingTime = millis();
            }
            else{
              //set tea time chime to finish days work
              
              if(millis() - startWaitingTime > tea){
                fireSolenoid();
              inPomodoro = 0; 
              stateNumber = 0;
              startTimeChime = 0;
              startWaitingTime = 0;
            }
          }        
        break;
        
        case 2:  
              Serial.print("\nPomodoro Long Break");
              if(millis() - startWaitingTime > pomodoroLongBreak){
                fireSolenoid();
                j++;
                if(j <= 2){
                  waitType = 0;
                  startWaitingTime = millis();
                }
               
            }
          break;  
          //delay(pomodoroShortBreak);
          //while (millis() - timer2 < 500){}
          //Serial.print("\nreached pomodoro3");
          }
          } 
          
        
        //if (j == 2) { break; }  //edge case not needed on last run as one works 6 hours a day!
        
        //delay(pomodoroLongBreak);
       // fireSolenoid();
      //inTea = 0;
      //inMettaBhavana = 0;
      //inPomodoro = 0
      break;
  }
  // save the Reading. Next time through the loop, it'll be the lastButtonState
  lastButtonState = buttonReading;
}
