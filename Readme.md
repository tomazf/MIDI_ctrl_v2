# MIDI ctrl_v2
#

8 button MIDI box with 4 color LED feedback (fixed color and blinking color)

default SETUP:
 
 SEND:<br>
  midi_channel = 4         (channel)<br>
  midi_note_offset = 49    (for buttons)<br>
  midi_vel = 127           (send velocity)<br>
 
 RECEIVE:<br>
  channel and note same as button mapping<br>
   state      OFF:  0 (velocity)<br>
   state     ON R:  1<br>
   state     ON G:  2<br>
   state     ON Y:  3<br>
   state     ON YY: 4<br>
   state    ON YYY: 5<br>
   state BLINK   R: 6<br>
   state BLINK   G: 7<br>
   state BLINK   Y: 8<br>
   state BLINK  YY: 9<br>
   state BLINK YYY: 10<br>
   state     ON R:  127<br>
