#pragma once
#include <pebble.h>
// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
 typedef struct ClaySettings {
  GColor BackgroundColor;
  GColor ForegroundColor;
  GColor BatteryColor;
  GColor LowBatteryColor;
  char blk1name[8];
  char blk2name[8];
  char blk3name[8];
  char blk4name[8];
  char blk5name[8];
  char blk6name[8];
  char blk7name[8];
  char blk8name[8];
  int afterschoolendtimes[5][2];
  int beforeschooltimes[5][2];
  char afterschoolmon[8];
  char afterschooltue[8];
  char afterschoolwed[8];
  char afterschoolthu[8];
  char afterschoolfri[8];
  char lunchactivitymon[8];
  char lunchactivitytue[8];
  char lunchactivitywed[8];
  char lunchactivitythu[8];
  char lunchactivityfri[8];
  char bforemon[8];
  char bforetue[8];
  char bforewed[8];
  char bforethu[8];
  char bforefri[8];
} __attribute__((__packed__)) ClaySettings;

