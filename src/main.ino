#define midienabled true
//#define debug true

void setup() {
  #if debug
    Serial.begin(115200);
  #endif

  setupSettings();
  setupKnob();
  setupPad(); 
  setupMidi();
}

void loop() {
  loopSettings();
  loopKnob();
  loopPad();
  //loopMidi();
}
