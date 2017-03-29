#if midienabled
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

#define channel 10

int midiNote = 60;

void setupMidi() {
  #if midienabled
  MIDI.begin(MIDI_CHANNEL_OFF);
  #endif
}

void loopMidi() {
  #if midienabled

  #endif
}

void midi_OnDialChanged(int dial)
{
    midiNote += dial;
    if(midiNote > 127) midiNote = 0;
    if(midiNote < 0) midiNote = 127;

    playDrumNote(midiNote, 100);
}

void midi_OnButtonPressed(int button)
{
  switch(button)
  {
    case NORMALCLICK: playDrumNote(80, 100); break;
    // case DOUBLECLICK: Serial.println("double click"); break;
    // case LONGPRESS: Serial.println("long press"); break;
    // case EXTRALONGPRESS: Serial.println("extra long press"); break;
  }
}
void playDrumNote(int note, int vel)
{
  #if debug
  Serial.print("C:" + String(channel, DEC) + ", N:" + String(note, DEC) + ", V:" + String(vel, DEC));
  Serial.println();
  #endif

  #if midienabled
  MIDI.sendNoteOn(note, vel, channel);
  MIDI.sendNoteOff(note, 0, channel);
  #endif
}
