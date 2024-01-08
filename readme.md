# Arduino Switch
I made this little project to automate switches in my house using Arduino ESP32's, so lights can turn on/off on a schedule while I'm away.

## Overview 
The arduino code will first acquire the current time (using NTP servers), then load the switch schedule from a file hosted in the internet. The schedule is polled every 10 minutes.

The main loop will try matching the current time against the schedule, accurate to the second (format HH-MM-SS). If it matches a switch time, the servo will actuate to either on or off.

There's many improvevements that could be made, but this worked perfecly for a few weeks out of the house.

## Build
The build is very simple, using an Arduino ESP32 and 9g servo hot-glued to a little wooden base. I fed the servo through the [VBUS pin from the Arduino](https://support.arduino.cc/hc/en-us/articles/360014779679-Enable-5-V-power-on-the-VUSB-or-VBUS-pin-on-Nano-boards). Here's what it looks like:

![PXL_20231210_211808794](https://github.com/hoffmannmatheus/ardu-light-switch/assets/889815/5d71c075-0074-457d-bf74-8e47ec0ff5c4)

### Quick demo

https://github.com/hoffmannmatheus/ardu-light-switch/assets/889815/6ec2157e-cc65-41dc-8db1-219891c0769b

## Observability
There's minimal observability, which I can use to know if the switches are working. Its simply a Firebase real time database, which can be updated with Firebase API calls. It currently logs the times the switch started (e.g. maybe the power went out), and the last time the schedule was polled:

![image](https://github.com/hoffmannmatheus/ardu-light-switch/assets/889815/360ca43d-56e3-4b23-8015-76ede268a676)


## Links
Some links that were useful for this project:
- [Arduino ESP32](https://docs.arduino.cc/hardware/nano-esp32)
- [ESP32 Servo lib](https://www.arduino.cc/reference/en/libraries/esp32servo/)
- [ESP32 System time](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html#rtc-timer-clock-sources)
