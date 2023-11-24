#ifndef __MAIN_H__
#define __MAIN_H__

// include files
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
#include <Esp.h>
#include <esp_log.h>

// constants
#define TASK_BUTTON_PRIO        4
#define TASK_MIC_PRIO        3
#define TASK_MQTT_PRIO          2

#define TASK_BUTTON_TYPE        0
#define TASK_MIC_TYPE        1

#define WIFI_SSID       "furue"
#define WIFI_PASSWORD   "Delta_006"
#define MQTT_DEV_ID     39000
#define MQTT_EVT_TOPIC  "tgr2023/Yakhum_Family/evt"
#define MQTT_CMD_TOPIC  "tgr2023/Yakhum_Family/cmd"

// type definitions
typedef struct evt_msg_t {
    int type;
    uint32_t timestamp;
    bool sound;
    uint32_t duration;
} evt_msg_t;

// shared variables
extern xQueueHandle evt_queue;
extern bool enable_flag;

// public function prototypes

#endif 