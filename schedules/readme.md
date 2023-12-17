# Daily schedules
The `daily` file defines daily schedules that are ran by the arduino devices.
Each line specifies a servo command to be ran at a specific time (accuracy to the second),
and will run once a day.

## Format:
> device id:
>>  hour:minute:second: servo command

## Example
Where servo command specifies the new servo angle, in degrees, to be set.
For example, if we know that the "on" and "center" positions of a light swicth servo,
we can turn the light on at 8:30pm like so:

>> 20:30:00;1: 70  # on position
>> 20:30:01;1: 90  # return to center

