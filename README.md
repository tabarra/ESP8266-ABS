# ESP8266-ABS
A Lightweight ESP8266 Wifi AccessPoint Beacon Spammer based on [H-LK/ESP8266-SSID-Text-Broadcast](https://github.com/H-LK/ESP8266-SSID-Text-Broadcast). The objective of this code is to spam 802.11 beacon frames with arbitrary SSID tag.  
  
### Features (differences from H-LK's repository):

  - Arbitrary SSID tag length without padding. (don't forget to change the number of APs at lines 10 and 11)
  - Fixed one char prefix ('#'). Can be removed by modifying lines 14, 15, 34 and 54.
  - SRC and BSSID pinning. Some devices await for a few beacons in a minimum amount of time before showing it to the user. So while the program is running the SSID will always use the same SRC/BSSID. I also tested with channel pinning but it gave me bad results whenever it synced up with the device's channel scanning routine.

# Notes
 - Tested on NodeMCU and Wemos D1 mini Pro.  
 - Working on ESP8266 Community 2.3.0.
 - The execution of this code might be illegal in your country.
