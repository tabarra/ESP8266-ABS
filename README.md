# ESP8266-ABS
A Lightweight ESP8266 Wifi AccessPoint Beacon Spammer based on [H-LK/ESP8266-SSID-Text-Broadcast](https://github.com/H-LK/ESP8266-SSID-Text-Broadcast). The objective of this code is to spam 802.11 beacon frames with arbitrary SSID tag. 

The code includes two lists of SSIDs, one is a list of countries, and the other was was collected from [this AskReddit thread](https://www.reddit.com/r/AskReddit/comments/70gkke/what_is_the_best_name_youve_seen_for_wifi/).  
```
"Password is Taco", "Silence of the LANs", "Tell_My_WiFi_LoveHer", "The LAN before Time", "Bathroom Spycam #2", "Neighbor Cam 2 (Bathroom)", "Mom! It's this one!", "click here for internet", "The Ping in the North", "Everyday I’m Buffering", "The Promised LAN", "Rebellious Amish Family", "IT herz WAN IP", "8HzWANIP", "It Burns When IP", "This LAN is my LAN", "Wu Tang Lan", "Pretty Fly For A Wifi", "Bill Wi The Science Fi", "Routers Of Rohan", "John Wilkes Bluetooth", "Lord of the Pings", "House LANnister", "Marthin Router King", "Gluten-Free WiFi", "Cunnilinksys", "I don't know", "Not a Meth Lab", "FBI Surveillance Van", "You kids get off my LAN", "Yell penis for password", "get your own internet", "Titanic Synching", "Freeloaders", "Igotchufam", "ourwifi", "WillYouBeMyWifi", "WifiAmHome", "Searching...", "Wi believe I can Fi", "Abraham Linksys", "YerAWifiHarry"
```

### Features (differences from H-LK's repository):

  - Arbitrary SSID tag length without padding. (don't forget to change the number of APs at lines 14 and 15)
  - Fixed one char prefix ('#'). Can be removed by modifying lines 18, 19, 38 and 58.
  - SRC and BSSID pinning. Some devices await for a few beacons in a minimum amount of time before showing it to the user. So while the program is running the SSID will always use the same SRC/BSSID. I also tested with channel pinning but it gave me bad results whenever it synced up with the device's channel scanning routine.

# Notes
 - Tested on NodeMCU (ESP12E) and Wemos D1 mini Pro.  
 - Working on ESP8266 Community 2.3.0.
 - The execution of this code might be illegal in your country.
