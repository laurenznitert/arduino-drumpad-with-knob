#define threshold 50
#define padDebounce 30
#define pad1 A4

int velocity = 0;
bool playing = false;
unsigned long timer = 0;
unsigned long blinkMillis = 0;
int blinkTime = 20;

void setupPad() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pad1, INPUT);
}

void loopPad() {
    if(millis() - blinkMillis > blinkTime)
    {
      digitalWrite(LED_BUILTIN, LOW);
    }

    int reading = analogRead(pad1);

    if (reading >= threshold && !playing) {
      if (millis() - timer >= padDebounce) {
        playing = true;
        playNote(pad1, reading);
      }
    }
    else if (reading >= threshold && playing) {
      playNote(pad1, reading);
    }
    else if (reading < threshold && playing) {
      #if debug
      Serial.print(String(velocity, HEX) + " ");
      #endif

      playDrumNote(midiNote, velocity);

      digitalWrite(LED_BUILTIN, HIGH);
      blinkMillis = millis();

      velocity = 0;
      playing = false;
      timer = millis();
    }
}

void playNote (int pad, int volume) {
  float v = map(volume, threshold, 1023, 0, 127);
  if (v > 127) v = 127;
  if (v > velocity) velocity = v;
}
