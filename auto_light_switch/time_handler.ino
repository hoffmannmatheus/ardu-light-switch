
#include "time.h"
#include "sntp.h"

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = -18000;  // GMT-5
const int   daylightOffset_sec = 3600;

bool receivedTimeSyncCallback = false;

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  receivedTimeSyncCallback = true;
}

void syncTime() {
  setColor(COLOR_WHITE);
  sntp_set_time_sync_notification_cb(timeavailable);
  sntp_servermode_dhcp(1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

bool didSyncTime() {
  return receivedTimeSyncCallback;
}

void getCurrentTime(char* currentTime, int size) {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  strftime(currentTime, size, "%H:%M:%S", &timeinfo);
}
