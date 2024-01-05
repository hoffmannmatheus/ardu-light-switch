# Arduino Switch
I made this little project to automate switches in my house using Arduino ESP32's, so lights can turn on/off on a schedule while I'm away.

## Overview 
The arduino code will first acquire the current time (using NTP servers), then load the switch schedule from a file hosted in the internet, and finally start looping. The schedule is polled every 10 minutes.

When the current time, accurate to the second (e.g. HH-MM-SS) matches a switch time from the schedule, the servo will actuate to either on or off.

Pretty simple! There's many improvevements that could be made, but this worked perfecly for a few weeks out of the house.

## Build
The build is very simple, using an Arduino ESP32 and 9g servo hot-glued to a little wooden base. I fed the servo through the VOUT pin from the Arduino. Here's what it looks like:
- picture of the two servos.
- gif actuating.

## Observability
There's minimal observability, which I can use to know if the switches are working. Its simply a Firebase real time database, which can be updated with Firebase API calls. It currently logs the times the switch started (e.g. maybe the power went out), and the last time the schedule was polled:

## Links
Some links that were useful for this project:
- ntp sync
- servo esp32
- arduino esp32