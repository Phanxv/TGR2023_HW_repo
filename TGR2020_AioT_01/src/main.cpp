#include "main.h"
#include "net_mqtt.h"

// constants
#define TAG             "main"
#define BTN_PIN         0 
#define WIFI_SSID       "TGR17_2.4G"
#define WIFI_PASSWORD   ""
#define MQTT_EVT_TOPIC  "/TGR_39/Hardware/receive"
#define MQTT_CMD_TOPIC  "/TGR_39/Hardware/sent"
#define MQTT_DEV_ID     39

// static function prototypes
void print_memory(void);
static void mqtt_callback(char* topic, byte* payload, unsigned int length);

// static variables
static bool enable_flag = true;
// declare ArduinoJson objects for cmd_buf and evt_buf
StaticJsonDocument<128> cmd_buff;
StaticJsonDocument<128> evt_buff;

static char buf[128];

// Setup hardware
void setup() {
  Serial.begin(115200);
  print_memory();
  pinMode(BTN_PIN, INPUT_PULLUP);
  // connect to WiFi
  ESP_LOGI(TAG, "Connecting to  %s", WIFI_SSID);
  net_mqtt_init(WIFI_SSID, NULL);
  ESP_LOGI(TAG, "Connected to  %s", WIFI_SSID);
  ESP_LOGI(TAG, "Connecting to MQTT broker");
  net_mqtt_connect(MQTT_DEV_ID, MQTT_CMD_TOPIC, mqtt_callback);
  // connect to MQTT broker
  
}

// Main loop
void loop() {
  static uint32_t prev_millis = 0;
  //char i[2] = "1";
    if (digitalRead(BTN_PIN) == LOW){
    // check button bounce
      if(millis() - prev_millis > 1000){
      // publish button event
        prev_millis = millis();
        evt_buff["ID"] = MQTT_DEV_ID;
        evt_buff["timestamp"] = millis();
        evt_buff["pressed"] = true;
        serializeJson(evt_buff, buf);
        net_mqtt_publish(MQTT_EVT_TOPIC, buf);
      }   
    }

  // loop MQTT interval
  net_mqtt_loop();
  delay(10);
}

// Print memory information
void print_memory() {
  ESP_LOGI(TAG, "Total heap: %u", ESP.getHeapSize());
  ESP_LOGI(TAG, "Free heap: %u", ESP.getFreeHeap());
  ESP_LOGI(TAG, "Total PSRAM: %u", ESP.getPsramSize());
  ESP_LOGI(TAG, "Free PSRAM: %d", ESP.getFreePsram());
}

// callback function to handle MQTT message
 void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  ESP_LOGI(TAG, "Message arrived on topic %s", topic);
  ESP_LOGI(TAG, "Payload: %.*s", length, payload);
  // extract data from payload
  char tmp_buff[128];
  memcpy(tmp_buff, payload, length);
  tmp_buff[length] = 0;
  // check if the message is for this device
  deserializeJson(cmd_buff, tmp_buff);
  // configure enable/disable status
  ESP_LOGI(TAG, "cmd buff: %.*s", length, cmd_buff);
  enable_flag = cmd_buff["hum"];
 }