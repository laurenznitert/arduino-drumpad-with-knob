#define NORMALCLICK 1
#define DOUBLECLICK 2
#define LONGPRESS 3
#define EXTRALONGPRESS 4

#define backwardPin 2
#define forwardPin 3
#define switchPin 5

volatile byte backwardFlag = 0;
volatile byte forwardFlag = 0;

byte encoderPos = 0;
byte oldEncPos = 0;
byte reading = 0;

byte switchFlag = 0;
int switchReading;
int switchState = LOW;
int previousSwitchState = LOW;

//=================================================
//  MULTI-CLICK:  One Button, Multiple Events

// Button timing variables
int switchDebounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 250;            // max ms between clicks for a double click event
int holdTime = 1000;        // ms hold period: how long to wait for press+hold event
int longHoldTime = 3000;    // ms long hold period: how long to wait for press+hold event

// Button variables
boolean buttonVal = HIGH;   // value read from button
boolean buttonLast = HIGH;  // buffered value of the button's previous state
boolean DCwaiting = false;  // whether we're waiting for a double click (down)
boolean DConUp = false;     // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true;    // whether it's OK to do a single click
long downTime = -1;         // time the button was pressed down
long upTime = -1;           // time the button was released
boolean ignoreUp = false;   // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false;        // when held, whether to wait for the up event
boolean holdEventPast = false;    // whether or not the hold event happened already
boolean longHoldEventPast = false;// whether or not the long hold event happened already

void setupKnob() {
  pinMode(forwardPin, INPUT_PULLUP);
  pinMode(backwardPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);

  attachInterrupt(0, forward, RISING);
  attachInterrupt(1, backward, RISING);
}

void loopKnob() {
  int dial = checkDial();
  if(dial != 0)
  {
    midi_OnDialChanged(dial);
  }

  int button = checkButton();


  if(button > 0)
  {
    midi_OnButtonPressed(button);

    #if debug
    Serial.print("Button pressed: ");
    switch(button)
    {
      case NORMALCLICK: Serial.println("normal click"); break;
      case DOUBLECLICK: Serial.println("double click"); break;
      case LONGPRESS: Serial.println("long press"); break;
      case EXTRALONGPRESS: Serial.println("extra long press"); break;
    }
    #endif
  }
}

int checkDial() {
  int result = encoderPos - oldEncPos;
  oldEncPos = encoderPos;
  return result;
}

int checkButton() {
   int event = 0;
   buttonVal = digitalRead(switchPin);
   // Button pressed down
   if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > switchDebounce)
   {
       downTime = millis();
       ignoreUp = false;
       waitForUp = false;
       singleOK = true;
       holdEventPast = false;
       longHoldEventPast = false;
       if ((millis()-upTime) < DCgap && DConUp == false && DCwaiting == true)  DConUp = true;
       else  DConUp = false;
       DCwaiting = false;
   }
   // Button released
   else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > switchDebounce)
   {
       if (not ignoreUp)
       {
           upTime = millis();
           if (DConUp == false) DCwaiting = true;
           else
           {
               event = 2;
               DConUp = false;
               DCwaiting = false;
               singleOK = false;
           }
       }
   }
   // Test for normal click event: DCgap expired
   if (buttonVal == HIGH && (millis()-upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2)
   {
       event = 1;
       DCwaiting = false;
   }
   // Test for hold
   if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
       // Trigger "normal" hold
       if (not holdEventPast)
       {
           event = 3;
           waitForUp = true;
           ignoreUp = true;
           DConUp = false;
           DCwaiting = false;
           //downTime = millis();
           holdEventPast = true;
       }
       // Trigger "long" hold
       if ((millis() - downTime) >= longHoldTime)
       {
           if (not longHoldEventPast)
           {
               event = 4;
               longHoldEventPast = true;
           }
       }
   }
   buttonLast = buttonVal;
   return event;
}

void forward(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && backwardFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    forwardFlag = 0; //reset flags for the next turn
    backwardFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) {
    forwardFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
    #if debug
    Serial.println("Rotary: forward");
    #endif
  }
  sei(); //restart interrupts
}

void backward(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && forwardFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    forwardFlag = 0; //reset flags for the next turn
    backwardFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) {
    backwardFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
    #if debug
    Serial.println("Rotary: backward");
    #endif
  }
  sei(); //restart interrupts
}
