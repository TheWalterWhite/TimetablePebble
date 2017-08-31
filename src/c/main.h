#pragma once
#include <pebble.h>
// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  char blk1name[10];
  bool testbool;
  char blk2name[10];
  char blk3name[10];
  char blk4name[10];
  char blk5name[10];
  char blk6name[10];
  char blk7name[10];
  char blk8name[10];
  bool afterschooldays[5];
  int afterschoolendtimes[4][2];
  char afterschoolmon[20];
  char afterschooltue[20];
  char afterschoolwed[20];
  char afterschoolthu[20];
  char afterschoolfri[20];
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