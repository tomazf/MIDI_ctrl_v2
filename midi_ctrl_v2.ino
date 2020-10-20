/*****************
   MIDI box - input on specific channel and send note button on press
   @Ferbi v0.2
      - added keypad mapping
      - added LED color slots
      - added blinking feature
*/

// All related to library "SkaarhojBI8":
#include "Wire.h"
#include "MCP23017.h"
#include "PCA9685.h"
#include "SkaarhojBI8.h"

#include <MIDI.h>

// OK for handling noteOFF witv velocity = 0
//MIDI_CREATE_DEFAULT_INSTANCE();

// receive velocity=0 with noteON
struct MySettings : public midi::DefaultSettings {
  static const bool HandleNullVelocityNoteOnAsNoteOff = false;
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MySettings);

// Setting up a board
SkaarhojBI8 board;

void setup() {

  //  don't do that on MIDI baud!
  //  Serial.begin(115200);                 // set up serial only for debug
  //  Serial.println("Serial Started");

  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);
  // By default the MIDI library sends everything THRU. We do NOT want that!
  MIDI.turnThruOff();
  // set handler
  MIDI.setHandleNoteOn(HandleNoteOn);
  // MIDI.setHandleNoteOff(HandleNoteOff);

  // Always initialize Wire before setting up the SkaarhojBI8 class!
  Wire.begin();

  // Set up the board
  board.begin(7, false);            // Address 7, dip-switch on board is OFF
  board.testSequence(40);           // Runs LED test sequence
  board.setDefaultColor(0);         // turn OFF all LEDs - colors:  0 - OFF, 1 - Y, 2 - R, 3 - G, 4 - YY, 5 - YYY
  board.setButtonColorsToDefault(); // turn off button LEDs
}

// button mappings
uint8_t buttonMap[] = {2, 1, 4, 8, 3, 5, 7, 6};             // HW fixed: button -> note
uint8_t buttonMapS[] = {2, 1, 5, 3, 6, 8, 7, 4};            // HW fixed: note -> button

// MIDI setup
uint8_t midi_channel = 4;
uint8_t num_buttons = 8;
uint8_t midi_vel = 127;
uint8_t midi_note_offset = 49;      // for sending and receiving - button
bool updated_state = false;
bool ledON = false;
int period_blink = 400;
unsigned long time_now = 0;

uint8_t LEDstateMap[8] = {0, 0, 0, 0, 0, 0, 0, 0};      // LED state - 8x LED - based on midi_note_offset and velocity
// state      OFF:  0
// state     ON R:  1
// state     ON G:  2
// state     ON Y:  3
// state     ON YY: 4
// state    ON YYY: 5
// state BLINK   R: 6
// state BLINK   G: 7
// state BLINK   Y: 8
// state BLINK  YY: 9
// state BLINK YYY: 10

//en kanal, različne note za gumbe, velocity določi stanje LEDice
//CH4, note 61, vel: 10 -> 61 = prvi gumb, 10=green
//CH4, note 61, vel: 20 -> 61 = prvi gumb, 20=red
//CH4, note 61, vel: 30 -> 61 = prvi gumb, 30=yellow
//CH4, note 62, vel: 20 -> 62 = drugi gumb, 20=red
//CH4, note 63, vel: 20 -> 63 = tretji gumb, 20=red

void loop() {

  for (int i = 1; i <= 8; i++) {
    if (board.buttonDown(i)) {                   // note send ON if button down
      MIDI.sendNoteOn(midi_note_offset + buttonMap[i - 1] - 1, midi_vel, midi_channel);
      delay(2);
    }
    if (board.buttonUp(i)) {                     // note send OFF if button up
      MIDI.sendNoteOff(midi_note_offset + buttonMap[i - 1] - 1, 0, midi_channel);
      delay(2);
    }
  }

  // read incoming MIDI data
  MIDI.read();

  // set LED color mode
  parseLEDstateMAP();       // only for color change
  ledBlinkCheck();          // continous blinking
}

void HandleNoteOn(byte channel, byte pitch, byte velocity)            // HEX: 93 pitch velocity
{
  if (channel == midi_channel) {                                      // check channel
    if ( (pitch >= midi_note_offset) && (pitch < midi_note_offset + num_buttons) ) {      // check note
      LEDstateMap[pitch - midi_note_offset] = velocity;                                   // received useful data for LED color, save
      updated_state = true;
    }
  }
}

/*
void HandleNoteOff(byte channel, byte pitch, byte velocity)           // HEX: 83 pitch velocity
{
  nota = pitch % 60;
  nextColor = 0;
  if (nota > 0 && nota <= 8)
  {
    board.setButtonColor(nota, nextColor);
  }
}
*/

void parseLEDstateMAP() {
  if (updated_state) {
    for (int i = 0; i <= 7; i++) {
      switch (LEDstateMap[i]) {
        case 0: board.setButtonColor(buttonMapS[i], 0); break;
        case 1: board.setButtonColor(buttonMapS[i], 2); break;
        case 2: board.setButtonColor(buttonMapS[i], 3); break;
        case 3: board.setButtonColor(buttonMapS[i], 1); break;
        case 4: board.setButtonColor(buttonMapS[i], 4); break;
        case 5: board.setButtonColor(buttonMapS[i], 5); break;
        default: break;
      }
    }
    updated_state = false;
  }
}

void ledBlinkCheck() {
  if (millis() >= time_now + period_blink) {
    time_now += period_blink;
    if (!ledON) {
      for (int i = 0; i <= 7; i++) {
        if (LEDstateMap[i] > 5 && LEDstateMap[i] <= 10)
          board.setButtonColor(buttonMapS[i], LEDstateMap[i] - 5);
      }
      ledON = true;
    }
    else {
      for (int i = 0; i <= 7; i++) {
        if (LEDstateMap[i] > 5 && LEDstateMap[i] <= 10)
          board.setButtonColor(buttonMapS[i], 0);
      }
      ledON = false;
    }
  }
}
