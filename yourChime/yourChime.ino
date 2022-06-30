//yourChime Arduino Sketch by Michael G. Hale 18.05.22 
// constants won't change. They're used here to set pin numbers:
const int etPin = 2;        // the number of the egg/tea pin
const int medPin = 4;// the number of the meditation pin
const int solenoidPin = 13; // the number of the solenoid pin

// Variables will change:
//int solenoidState = LOW;         // the current state of the output pin
int etButtonState;                // the current etReading from the input pin
int medButtonState;               // current medReading 
int lastEtButtonState = LOW;   // the previous etReading from the input pin
int lastMedButtonState = LOW;  //previous medRading
 
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastEtDebounceTime = 0;
unsigned long lastMedDebounceTime = 0; // the last time the output pin was toggsolenoid
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

  // trigger solenoid:
  int fireSolenoid(){
    digitalWrite(solenoidPin, HIGH);
    unsigned long timer1 = millis();
    while (millis() - timer1 < 200){}
    digitalWrite(solenoidPin, LOW);
  }

void setup() {
  pinMode(etPin, INPUT);
  pinMode(medPin, INPUT);
  pinMode(solenoidPin, OUTPUT);

  // set initial solenoid state
  digitalWrite(solenoidPin, LOW);


  
}

void loop() {
  // read the state of the switches into a local variables:
  int etReading = digitalRead(etPin);
  int medReading = digitalRead(medPin);
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
      if (etButtonState == HIGH) {
         fireSolenoid();
      }
    }
  }

    if ((millis() - lastMedDebounceTime) > debounceDelay) {
    // whatever the etReading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (medReading != medButtonState) {
      medButtonState = medReading;

      // only toggle the solenoid if the new button state is HIGH
      if (medButtonState == HIGH) {
        fireSolenoid();
      }
    }
  }

  
  
  // save the Reading. Next time through the loop, it'll be the lastEt/MedButtonState:
  lastEtButtonState = etReading;
  lastMedButtonState = medReading;
}
