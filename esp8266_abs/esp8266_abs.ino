#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}


//Don't forget to change the number 192
// String apNames[] = {"Afghanistan", "Albania", "Algeria", "Andorra", "Angola", "Antigua and Barbuda", "Argentina", "Armenia", "Australia", "Austria", "Azerbaijan", "Bahamas", "Bahrain", "Bangladesh", "Barbados", "Belarus", "Belgium", "Belize", "Benin", "Bhutan", "Bolivia", "Bosnia and Herzegovina", "Botswana", "Brazil", "Brunei Darussalam", "Bulgaria", "Burkina Faso", "Burundi", "Cabo Verde", "Cambodia", "Cameroon", "Canada", "Central African Republic", "Chad", "Chile", "China", "Colombia", "Comoros", "Congo", "Costa Rica", "Cote dIvoire", "Croatia", "Cuba", "Cyprus", "Czech Republic", "North Korea", "Republic of the Congo", "Denmark", "Djibouti", "Dominica", "Dominican Republic", "Ecuador", "Egypt", "El Salvador", "Equatorial Guinea", "Eritrea", "Estonia", "Ethiopia", "Fiji", "Finland", "France", "Gabon", "Gambia", "Georgia", "Germany", "Ghana", "Greece", "Grenada", "Guatemala", "Guinea", "Guinea-Bissau", "Guyana", "Haiti", "Honduras", "Hungary", "Iceland", "India", "Indonesia", "Iran", "Iraq", "Ireland", "Israel", "Italy", "Jamaica", "Japan", "Jordan", "Kazakhstan", "Kenya", "Kiribati", "Kuwait", "Kyrgyzstan", "Republic of Lao", "Latvia", "Lebanon", "Lesotho", "Liberia", "Libya", "Liechtenstein", "Lithuania", "Luxembourg", "Macedonia", "Madagascar", "Malawi", "Malaysia", "Maldives", "Mali", "Malta", "Marshall Islands", "Mauritania", "Mauritius", "Mexico", "Micronesia", "Monaco", "Mongolia", "Montenegro", "Morocco", "Mozambique", "Myanmar", "Namibia", "Nauru", "Nepal", "Netherlands", "New Zealand", "Nicaragua", "Niger", "Nigeria", "Norway", "Oman", "Pakistan", "Palau", "Panama", "Papua New Guinea", "Paraguay", "Peru", "Philippines", "Poland", "Portugal", "Qatar", "South Korea", "Republic of Moldova", "Romania", "Russian Federation", "Rwanda", "Saint Kitts and Nevis", "Saint Lucia", "Samoa", "San Marino", "Sao Tome and Principe", "Saudi Arabia", "Senegal", "Serbia", "Seychelles", "Sierra Leone", "Singapore", "Slovakia", "Slovenia", "Solomon Islands", "Somalia", "South Africa", "South Sudan", "Spain", "Sri Lanka", "Sudan", "Suriname", "Swaziland", "Sweden", "Switzerland", "Syrian Arab Republic", "Tajikistan", "Thailand", "Timor-Leste", "Togo", "Tonga", "Trinidad and Tobago", "Tunisia", "Turkey", "Turkmenistan", "Tuvalu", "Uganda", "Ukraine", "United Arab Emirates", "United Kingdom", "Tanzania", "United States of America", "Uruguay", "Uzbekistan", "Vanuatu", "Venezuela", "Vietnam", "Yemen", "Zambia", "Zimbabwe"};
// byte apMACs[192][6];
// int apCount = 192;
//Source: https://www.reddit.com/r/AskReddit/comments/70gkke/what_is_the_best_name_youve_seen_for_wifi/
String apNames[] = {"Password is Taco", "Silence of the LANs", "Tell_My_WiFi_LoveHer", "The LAN before Time", "Bathroom Spycam #2", "Neighbor Cam 2 (Bathroom)", "Mom! It's this one!", "click here for internet", "The Ping in the North", "Everyday I’m Buffering", "The Promised LAN", "Rebellious Amish Family", "IT herz WAN IP", "8HzWANIP", "It Burns When IP", "This LAN is my LAN", "Wu Tang Lan", "Pretty Fly For A Wifi", "Bill Wi The Science Fi", "Routers Of Rohan", "John Wilkes Bluetooth", "Lord of the Pings", "House LANnister", "Marthin Router King", "Gluten-Free WiFi", "Cunnilinksys", "I don't know", "Not a Meth Lab", "FBI Surveillance Van", "You kids get off my LAN", "Yell penis for password", "get your own internet", "Titanic Synching", "Freeloaders", "Igotchufam", "ourwifi", "WillYouBeMyWifi", "WifiAmHome", "Searching...", "Wi believe I can Fi", "Abraham Linksys", "YerAWifiHarry"};
byte apMACs[42][6];
int apCount = 42;

int currentAP = 0;
int bPointer = 52;
char prefix = '#';

//Beacon Packet buffer
byte packet[128] = { 
  /* IEEE 802.11 Beacon Frame */
  /*0*/   0x80, 0x00, 0x00, 0x00,  //type/subtype = beacon
  /*4*/   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //dst addr = broadcast
  /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //src addr = random MAC
  /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //bss id = src addr
  /*22*/  0x90, 0x53, //seq number 1337 + frag number 0
  /*24*/  0xDE, 0xAD, 0x00, 0xBE, 0xEF, 0x00, 0xF0, 0x0D, //timestamp
  /*32*/  0x64, 0x00, //beacon interval (~100ms)
  /*34*/  0x01, 0x04, //capabilities info
  
  /* IEEE 802.11 wireless LAN managment frame */
  /*36*/  0x01, 0x08, //tag = Supported Rates, size
          0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 
  /*46*/  0x03, 0x01, 0xFF, //tag = Channel, size, random channel
  
  /*49*/  0x00, 0xFF, 0x23 //tag = SSID, size, prefix char
};


void fillMACs() {
  for (int j = 0; j < apCount; j++) {
    apMACs[j][0] = random(256);
    apMACs[j][1] = random(256);
    apMACs[j][2] = random(256);
    apMACs[j][3] = random(256);
    apMACs[j][4] = random(256);
    apMACs[j][5] = random(256);
  }
}


void setup() {
  delay(100);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1);
  packet[51] = prefix;
  fillMACs();
}


void loop() {
  if (currentAP == apCount) {
    currentAP = 0;
  }

  //Set random channel (pinning this variable wasn't giving me good results)
  byte channel = random(1, 11);
  wifi_set_channel(channel);
  packet[48] = channel;
  
  //Copying src addr and bssid to packet buffer
  packet[10] = packet[16] = apMACs[currentAP][0];
  packet[11] = packet[17] = apMACs[currentAP][1];
  packet[12] = packet[18] = apMACs[currentAP][2];
  packet[13] = packet[19] = apMACs[currentAP][3];
  packet[14] = packet[20] = apMACs[currentAP][4];
  packet[15] = packet[21] = apMACs[currentAP][5];

  //Copying SSID to packet buffer
  byte currLenght = apNames[currentAP].length();
  packet[50] = currLenght+1;
  for (int i = bPointer; i < bPointer+currLenght; i++) {
    packet[i] = apNames[currentAP][i-bPointer];
  }

  //Sending beacon. Feel free to play with the numbers here, this was the best combination I found.
  for (int i = 0; i < 8; i++) {
    wifi_send_pkt_freedom(packet, bPointer+currLenght, 0);
    delayMicroseconds(64);
  }
  delay(4);
  
  currentAP++;
}
