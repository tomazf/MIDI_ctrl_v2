# MIDI ctrl_v2
#

8 button MIDI box with 4 color LED feedback (fixed color and blinking color)

default SETUP:
 
 SEND:
  midi_channel = 4         (channel)
  midi_note_offset = 49    (for buttons)
  midi_vel = 127           (send velocity)
 
 RECEIVE:
  channel and note same as button mapping
   state      OFF:  0 (velocity)
   state     ON R:  1
   state     ON G:  2
   state     ON Y:  3 
   state     ON YY: 4
   state    ON YYY: 5
   state BLINK   R: 6
   state BLINK   G: 7
   state BLINK   Y: 8
   state BLINK  YY: 9
   state BLINK YYY: 10
 
