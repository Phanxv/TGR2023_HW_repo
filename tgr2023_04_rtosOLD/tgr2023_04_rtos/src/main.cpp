#include "main.h"
#include "task_mqtt.h"
#include "net_mqtt.h"
#include "task_mic.h"

// constants
#define TAG             "main"
#define SAMPLE_RATE     16000

// static function prototypes
void print_memory(void);
static void mqtt_callback(char* topic, byte* payload, unsigned int length);

// static variables
bool enable_flag = true;

StaticJsonDocument<128> cmd_doc;

static char cmd_buf[128];

QueueHandle_t evt_queue;

// Setup hardware
void setup() {
  Serial.begin(115200);
  hw_mic_init(SAMPLE_RATE);
  print_memory();
  evt_queue = xQueueCreate(3, sizeof(evt_msg_t));
  task_mic_init();
  task_mqtt_init(mqtt_callback);
}

// Main loop
void loop() {
  net_mqtt_loop();
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
  memcpy(cmd_buf, payload, length);
  cmd_buf[length] = '\0';
  deserializeJson(cmd_doc, cmd_buf);
  if (cmd_doc["ID"] == MQTT_DEV_ID) {
    if (cmd_doc["enable"] == true) {
      enable_flag = true;
      ESP_LOGI(TAG, "Got enable command");
    } else {
      enable_flag = false;
      ESP_LOGI(TAG, "Got disable command");
    }
  }
 }