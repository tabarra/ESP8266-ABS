#include <Arduino.h>
#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}

//Don't forget to change the number 192
String apNames[] = {"Afghanistan", "Albania", "Algeria", "Andorra", "Angola", "Antigua and Barbuda", "Argentina", "Armenia", "Australia", "Austria", "Azerbaijan", "Bahamas", "Bahrain", "Bangladesh", "Barbados", "Belarus", "Belgium", "Belize", "Benin", "Bhutan", "Bolivia", "Bosnia and Herzegovina", "Botswana", "Brazil", "Brunei Darussalam", "Bulgaria", "Burkina Faso", "Burundi", "Cabo Verde", "Cambodia", "Cameroon", "Canada", "Central African Republic", "Chad", "Chile", "China", "Colombia", "Comoros", "Congo", "Costa Rica", "Cote dIvoire", "Croatia", "Cuba", "Cyprus", "Czech Republic", "North Korea", "Republic of the Congo", "Denmark", "Djibouti", "Dominica", "Dominican Republic", "Ecuador", "Egypt", "El Salvador", "Equatorial Guinea", "Eritrea", "Estonia", "Ethiopia", "Fiji", "Finland", "France", "Gabon", "Gambia", "Georgia", "Germany", "Ghana", "Greece", "Grenada", "Guatemala", "Guinea", "Guinea-Bissau", "Guyana", "Haiti", "Honduras", "Hungary", "Iceland", "India", "Indonesia", "Iran", "Iraq", "Ireland", "Israel", "Italy", "Jamaica", "Japan", "Jordan", "Kazakhstan", "Kenya", "Kiribati", "Kuwait", "Kyrgyzstan", "Republic of Lao", "Latvia", "Lebanon", "Lesotho", "Liberia", "Libya", "Liechtenstein", "Lithuania", "Luxembourg", "Macedonia", "Madagascar", "Malawi", "Malaysia", "Maldives", "Mali", "Malta", "Marshall Islands", "Mauritania", "Mauritius", "Mexico", "Micronesia", "Monaco", "Mongolia", "Montenegro", "Morocco", "Mozambique", "Myanmar", "Namibia", "Nauru", "Nepal", "Netherlands", "New Zealand", "Nicaragua", "Niger", "Nigeria", "Norway", "Oman", "Pakistan", "Palau", "Panama", "Papua New Guinea", "Paraguay", "Peru", "Philippines", "Poland", "Portugal", "Qatar", "South Korea", "Republic of Moldova", "Romania", "Russian Federation", "Rwanda", "Saint Kitts and Nevis", "Saint Lucia", "Samoa", "San Marino", "Sao Tome and Principe", "Saudi Arabia", "Senegal", "Serbia", "Seychelles", "Sierra Leone", "Singapore", "Slovakia", "Slovenia", "Solomon Islands", "Somalia", "South Africa", "South Sudan", "Spain", "Sri Lanka", "Sudan", "Suriname", "Swaziland", "Sweden", "Switzerland", "Syrian Arab Republic", "Tajikistan", "Thailand", "Timor-Leste", "Togo", "Tonga", "Trinidad and Tobago", "Tunisia", "Turkey", "Turkmenistan", "Tuvalu", "Uganda", "Ukraine", "United Arab Emirates", "United Kingdom", "Tanzania", "United States of America", "Uruguay", "Uzbekistan", "Vanuatu", "Venezuela", "Vietnam", "Yemen", "Zambia", "Zimbabwe"};
byte apMACs[192][6];
int apCount = 192;

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
  for (int j = 0; j < apCount; j++)
    for (int k = 0; k < 6; k++)
      apMACs[j][k] = random(256);

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
  for (k = 0; k < 6; k++)
    packet[k + 10] = packet[k + 16] = apMACs[currentAP][k];

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
