#include <pebble.h>
#include <pebble_fonts.h>
#include "main.h"
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_left_layer;
static TextLayer *s_until_layer;
static TextLayer *s_class_layer;
struct tm nextclasstime;
char currentDayName[10];
char currentTime24h[1];
static char a[8];
double timeleft;
// Name of next class, declared as an array 'cause why not
char nextclass[10] = "Subject";

static ClaySettings settings;

static void update_time()  {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm* tick_time = localtime(&temp);
  
  //Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
           "%H:%M" : "%I:%M", tick_time);
  
  // Get abbriviated day name and current time
  strftime(currentDayName, 4, "%a", tick_time);
  strftime(currentTime24h, sizeof(currentTime24h), "%R", tick_time);
  
  
  // Calls next_class to get the next class(Who would have thought??)
  next_class(nextclass, currentTime24h, *tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  text_layer_set_text(s_class_layer, nextclass);
  text_layer_set_text(s_time_left_layer, a);
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  update_time();
}

static void prv_default_settings() {
  strncpy(settings.blk1name, "Math", 5);
  strncpy(settings.blk2name, "Band", 5);
  strncpy(settings.blk3name, "English", 8);
  strncpy(settings.blk4name, "French", 7);
  strncpy(settings.blk5name, "IT", 3);
  strncpy(settings.blk6name, "Physics", 8);
  strncpy(settings.blk7name, "Chem", 5);
  strncpy(settings.blk8name, "Spare", 6);
  settings.afterschooldays[0] = true;
  int i;
  for(i = 1; i < 4; i++)
    settings.afterschooldays[i] = false;
    printf("%i", settings.afterschooldays[i]);
  strncpy(settings.afterschoolmon, "RnB", 4);
  strncpy(settings.afterschoolmon, "", 1);
  strncpy(settings.afterschoolmon, "", 1);
  strncpy(settings.afterschoolmon, "", 1);
  strncpy(settings.afterschoolmon, "", 1);
}

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}



// AppMessage receive handler
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  //Read block names
  Tuple *blk1_t = dict_find(iter, MESSAGE_KEY_blk1name);
  if(blk1_t) {
    strncpy(settings.blk1name, blk1_t->value->cstring, sizeof(blk1_t->value->cstring));}
  Tuple *blk2_t = dict_find(iter, MESSAGE_KEY_blk2name);
  if(blk2_t) {
    strncpy(settings.blk2name, blk2_t->value->cstring, sizeof(blk2_t->value->cstring));}
  Tuple *blk3_t = dict_find(iter, MESSAGE_KEY_blk3name);
  if(blk3_t) {
    strncpy(settings.blk3name, blk3_t->value->cstring, sizeof(blk3_t->value->cstring));}
  Tuple *blk4_t = dict_find(iter, MESSAGE_KEY_blk4name);
  if(blk4_t) {
    strncpy(settings.blk4name, blk4_t->value->cstring, sizeof(blk4_t->value->cstring));}
  Tuple *blk5_t = dict_find(iter, MESSAGE_KEY_blk5name);
  if(blk1_t) {
    strncpy(settings.blk5name, blk5_t->value->cstring, sizeof(blk5_t->value->cstring));}
  Tuple *blk6_t = dict_find(iter, MESSAGE_KEY_blk6name);
  if(blk6_t) {
    strncpy(settings.blk6name, blk6_t->value->cstring, sizeof(blk6_t->value->cstring));}
  Tuple *blk7_t = dict_find(iter, MESSAGE_KEY_blk7name);
  if(blk7_t) {
    strncpy(settings.blk7name, blk7_t->value->cstring, sizeof(blk7_t->value->cstring));}
  Tuple *blk8_t = dict_find(iter, MESSAGE_KEY_blk8name);
  if(blk8_t) {
    strncpy(settings.blk8name, blk8_t->value->cstring, sizeof(blk8_t->value->cstring));}
 
  prv_save_settings();
}
  




static void next_class(char* nextclass_return, char* currentTime24h, tm tick_time)  {

  // Sync day, month, year, etc
  nextclasstime = tick_time; 
    
  // Initialise the time manager for time left

  
  // Make sure you're not some loser who uses this watchface on a weekend(and if you are set next block to 1)
  if(strncmp(currentDayName, "Sat", 2) == 0 || strncmp(currentDayName, "Sun", 2) == 0){
    nextclasstime.tm_wday = 1;
    nextclasstime.tm_hour = 8;
    nextclasstime.tm_min = 25;
    timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
    const time_t temp2 = timeleft; 
    struct tm* test = localtime(&temp2); //Yes the time manager is called test.
    strftime(a, sizeof(a), "%R", test);
    strncpy(nextclass_return, settings.blk1name, sizeof(settings.blk1name));
  }
   //Monday
  if(strncmp(currentDayName, "Mon", 2) == 0){
    
    if (tick_time.tm_hour < 8 && tick_time.tm_min < 25){    //Before Monday blk1
      nextclasstime.tm_hour = 8;
      nextclasstime.tm_min = 25;
            // Get the difference in between now and the next class
      timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
      const time_t temp2 = timeleft; 
      struct tm* test = localtime(&temp2);
      strftime(a, sizeof(a), "%R", test);
      strncpy(nextclass_return, blk1, sizeof(blk1));
    }       
    
    else if (tick_time.tm_hour < 9 && tick_time.tm_min < 21 && tick_time.tm_hour > 8 && tick_time.tm_min > 25){ //In block 1, next block is advisory
        nextclasstime.tm_hour = 9;
        nextclasstime.tm_min = 21;
        timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
        const time_t temp2 = timeleft; 
        struct tm* test = localtime(&temp2);
        strftime(a, sizeof(a), "%R", test);
        strncpy(nextclass_return, "Advisory", 9);
        }          
    
    else if (tick_time.tm_hour < 10 && tick_time.tm_min < 6 && tick_time.tm_hour > 9 && tick_time.tm_min > 21){ //In Advisory, next block is 2
              nextclasstime.tm_hour = 10;
              nextclasstime.tm_min = 6;
              timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
              const time_t temp2 = timeleft; 
              struct tm* test = localtime(&temp2);
              strftime(a, sizeof(a), "%R", test);
              strncpy(nextclass_return, blk2, sizeof(blk2));
          }

    else if (tick_time.tm_hour < 11 && tick_time.tm_min < 7 && tick_time.tm_hour > 10 && tick_time.tm_min > 6){ //In block 2, next block is 3
              nextclasstime.tm_hour = 11;
              nextclasstime.tm_min = 7;
              timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
              const time_t temp2 = timeleft; 
              struct tm* test = localtime(&temp2);
              strftime(a, sizeof(a), "%R", test);
              strncpy(nextclass_return, blk3, sizeof(blk3));
          }

    else if (tick_time.tm_hour < 12 && tick_time.tm_min < 8 && tick_time.tm_hour > 11 && tick_time.tm_min > 7){ //In block 3, next block is Lunch
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 8;
              timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
              const time_t temp2 = timeleft; 
              struct tm* test = localtime(&temp2);
              strftime(a, sizeof(a), "%R", test);
              strncpy(nextclass_return, "Lunch", 6);
          }

    else if (tick_time.tm_hour < 12 && tick_time.tm_min < 56 && tick_time.tm_hour > 12 && tick_time.tm_min > 8){ //In Lunch, next block is 4
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 56;
              timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
              const time_t temp2 = timeleft; 
              struct tm* test = localtime(&temp2);
              strftime(a, sizeof(a), "%R", test);
              strncpy(nextclass_return, blk4, sizeof(blk4));
          }

    else if (tick_time.tm_hour < 1 && tick_time.tm_min < 57 && tick_time.tm_hour > 12 && tick_time.tm_min > 56){ //In block 4, next block is 5
              nextclasstime.tm_hour = 1;
              nextclasstime.tm_min = 57;
              timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
              const time_t temp2 = timeleft; 
              struct tm* test = localtime(&temp2);
              strftime(a, sizeof(a), "%R", test);
              strncpy(nextclass_return, blk5, sizeof(blk5));
          }
    
    else if (tick_time.tm_hour < 2 && tick_time.tm_min < 59 && tick_time.tm_hour > 1 && tick_time.tm_min > 57){ //In block 5, next block is either freedom or afterschoolmon
              nextclasstime.tm_hour = 2;
              nextclasstime.tm_min = 59;
              timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
              const time_t temp2 = timeleft; 
              struct tm* test = localtime(&temp2);
              strftime(a, sizeof(a), "%R", test);
              if(settings.afterschooldays[0] == true){
                strncpy(nextclass_return, afterschoolmon, sizeof(afterschoolmon));
              }
                else
                  strncpy(nextclass_return, "Freedom", 7);
    }
    
    else if (tick_time.tm_hour < 1 && tick_time.tm_min < 57 && tick_time.tm_hour > 12 && tick_time.tm_min > 56){ //In block 4, next block is 5
              nextclasstime.tm_hour = 1;
              nextclasstime.tm_min = 57;
              timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
              const time_t temp2 = timeleft; 
              struct tm* test = localtime(&temp2);
              strftime(a, sizeof(a), "%R", test);
              strncpy(nextclass_return, "Freedom", 7);
          }
      
    }
  }


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
update_time();
}



static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Create top layer
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Create middle layer 1
  s_until_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Create middle layer 2
  s_class_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Create Bottom Layer
  s_time_left_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  //Layout for bottom time remaining
  text_layer_set_background_color(s_time_left_layer, GColorClear);
  text_layer_set_text_color(s_time_left_layer, GColorBlack);
  text_layer_set_font(s_time_left_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_left_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_time_left_layer), GRect(0, 50, bounds.size.w, bounds.size.h));
  text_layer_set_text(s_time_left_layer, "12:34");
  
  // Layout for middle layer 1
  text_layer_set_background_color(s_until_layer, GColorClear);
  text_layer_set_text_color(s_until_layer, GColorBlack);
  text_layer_set_font(s_until_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_until_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_until_layer), GRect(0, 100, bounds.size.w, bounds.size.h));
  text_layer_set_text(s_until_layer, "Until");
  
  // Layout for middle layer 2
  text_layer_set_background_color(s_class_layer, GColorClear);
  text_layer_set_text_color(s_class_layer, GColorBlack);
  text_layer_set_font(s_class_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_class_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_class_layer), GRect(0, 125, bounds.size.w, bounds.size.h));
  
  // Layout for top time
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0, 0, bounds.size.w, bounds.size.h));
  
  
  // Add all as children to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_left_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_until_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_class_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_time_left_layer);
  text_layer_destroy(s_class_layer);
  text_layer_destroy(s_until_layer);
}

static void init()  {
  prv_load_settings();
  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure time is displayed from the start
  update_time();
}

static void deinit()  {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void)  {
  init();
  app_event_loop();
  deinit();
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}