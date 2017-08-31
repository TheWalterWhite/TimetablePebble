#pragma once
#include <pebble.h>
// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  char blk1name[8];
  char blk2name[8];
  char blk3name[8];
  char blk4name[8];
  char blk5name[8];
  char blk6name[8];
  char blk7name[8];
  char blk8name[8];
  bool afterschooldays[4];
  bool lunchactivitydays[4];
  int afterschoolendtimes[4][2];
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
} __attribute__((__packed__)) ClaySettings;

static void prv_default_settings();
static void prv_load_settings();
static void prv_save_settings();
static void update_time();
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void init(void);
static void deinit(void);