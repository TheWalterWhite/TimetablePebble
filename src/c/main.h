#pragma once
#include <pebble.h>
// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  char blk1name[10];
  char blk2name[10];
  char blk3name[10];
  char blk4name[10];
  char blk5name[10];
  char blk6name[10];
  char blk7name[10];
  char blk8name[10];
  bool afterschooldays[5];
  char afterschoolmon[20];
  char afterschooltue[20];
  char afterschoolwed[20];
  char afterschoolthu[20];
  char afterschoolfri[20];
} ClaySettings;
