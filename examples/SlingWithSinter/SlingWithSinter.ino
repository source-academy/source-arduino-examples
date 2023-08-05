#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
#endif

#include <sinter.h>
#include "Timer5.h"
#include "SevSegSource.h"
#include "internal_functions.h"
#include "arduino_secrets.h"

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = HOSTNAME;
int        port     = 1883;
char hello_topic[30] = SECRET_KEY;
char run_topic[30]   = SECRET_KEY;
char stop_topic[30]  = SECRET_KEY;
char ping_topic[30]  = SECRET_KEY;
char status_topic[30]  = SECRET_KEY;
char display_topic[30] = SECRET_KEY;
int messageSize = 0;

char heap[0x4000];
unsigned char program_svm[5000];
unsigned int program_svm_len = 0;

enum state_type_t {  
  IDLE = 0,
  RUN,
  HELLO
};
state_type_t state = HELLO;
bool isRunning = false;

void setup() {
  Serial.begin(9600);

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("You're connected to the network");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }
  Serial.println("You're connected to the MQTT broker!");

  strcat(hello_topic, "/hello");
  strcat(run_topic, "/run");
  strcat(stop_topic, "/stop");
  strcat(ping_topic, "/ping");
  strcat(status_topic, "/status");
  strcat(display_topic, "/display");
  
  // Client to device topics
  mqttClient.subscribe(run_topic);
  mqttClient.subscribe(stop_topic);
  mqttClient.subscribe(ping_topic);
  mqttClient.onMessage(onMqttMessage);
  
  sinter_setup_heap(heap, 0x4000);
  setupInternals();

  MyTimer5.begin(100000);
  MyTimer5.attachInterrupt(Timer5_IRQ);
  MyTimer5.start();
}

void Timer5_IRQ(void) {
  mqttClient.poll();
}

void onMqttMessage(int msgSize) {  
  messageSize = msgSize;
  String topic = mqttClient.messageTopic();
  if (topic == run_topic) {
    state = RUN;
  } else if (topic == stop_topic) {
    sinter_stop();
    state = IDLE;
  } else if (topic == ping_topic) {
    echo();
  }
}

void loop() {
  switch (state) {
    case HELLO:
      mqttClient.beginMessage(hello_topic);
      mqttClient.print(123);
      mqttClient.endMessage();
      state = IDLE;
    case IDLE:
      break;
    case RUN:
      isRunning = true;
      parseProgram(messageSize / 2);
      runProgram(messageSize / 2);
      isRunning = false;
      state = IDLE;
      break;
  }
}

//========= FOR RUN STATE =========//

void parseProgram(unsigned int len) {
  program_svm_len = len;
  for (unsigned int i = 0; i < len; i++) {
    char msb = (char) mqttClient.read();
    char lsb = (char) mqttClient.read();
    char curr[] = {msb, lsb};
    program_svm[i] = (unsigned char) strtol(curr, 0, 16);
  }
}

void runProgram(int messageSize) {
  sinter_value_t result;
  sinter_fault_t fault = sinter_run(program_svm, program_svm_len, &result);
  // Serial.print("Program exited with fault ");
  // Serial.print(fault);
  // Serial.print(" and result type ");
  // Serial.print(result.type);
  // Serial.print(" (");
  // Serial.print(result.integer_value);
  // Serial.print(", ");
  // Serial.print(result.boolean_value);
  // Serial.print(", ");
  // Serial.print(result.float_value);
  // Serial.print(", ");
  // Serial.print(result.string_value);
  // Serial.print(")\n");
  
  mqttClient.beginMessage(display_topic);
  mqttClient.print("Program exited with fault ");
  mqttClient.print(fault);
  mqttClient.print(" and result type ");
  mqttClient.print(result.type);
  mqttClient.print(" (");
  mqttClient.print(result.integer_value);
  mqttClient.print(", ");
  mqttClient.print(result.boolean_value);
  mqttClient.print(", ");
  mqttClient.print(result.float_value);
  mqttClient.print(", ");
  mqttClient.print(result.string_value);
  mqttClient.print(")\n");
  mqttClient.endMessage();
}

//======== FOR PING STATE ========//
void echo() {
  mqttClient.beginMessage(status_topic);
  mqttClient.print(isRunning);
  mqttClient.endMessage();
}