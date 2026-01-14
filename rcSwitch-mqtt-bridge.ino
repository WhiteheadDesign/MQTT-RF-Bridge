#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <RCSwitch.h>

unsigned long code;
int size, protocol, pulselength, repeat;

#include "config.h"

AdafruitIO_Feed *lightcontrol = io.feed("light-control");
                  //Feed int            //adafruit Feed name

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);

  while (!Serial)
    ;

  Serial.print("Connecting to Adafruit IO");
  mySwitch.enableTransmit(D8);


  io.connect();

  lightcontrol->onMessage(handleMessage);

  while (io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  lightcontrol->get();

  Serial.println();
  Serial.println(io.statusText());
}

void loop() {

  io.run();
}
void handleMessage(AdafruitIO_Data *data) {
  String Recived = data->toString();
  Serial.print("Raw message: ");
  Serial.println(Recived);

  Serial.print("Message length: ");
  Serial.println(Recived.length());

  StaticJsonDocument<512> doc;
  deserializeJson(doc, Recived);


  // WHY DOES ADAFRUIT LIMIT TO 52 CHARACTERS
  code = doc["CO"];
  size = doc["SI"];
  protocol = doc["PR"];
  pulselength = doc["PL"];
  repeat = doc["RE"];

  Serial.print("protocol = ");
  Serial.println(protocol);
  Serial.print("pulselength = ");
  Serial.println(pulselength);
  Serial.print("repeat = ");
  Serial.println(repeat);
  Serial.print("size = ");
  Serial.println(size);
  Serial.print("code = ");
  Serial.println(code);

  mySwitch.setProtocol(protocol);
  mySwitch.setPulseLength(pulselength);
  mySwitch.setRepeatTransmit(repeat);
  mySwitch.send(code, size);
}
