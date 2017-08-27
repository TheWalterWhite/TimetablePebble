#include <pebble.h>
#include <pebble_fonts.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_left_layer;
static TextLayer *s_until_layer;
static TextLayer *s_class_layer;
// Name of next class, declared as an array 'cause why not
char nextclass[10] = "Subject";

static void update_time()  {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
           "%H:%M" : "%I:%M", tick_time);
  
  
  // Time checking(check if time is equal to class start time, not if it's in between(much easier) then get the remaining time based on that)
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  text_layer_set_text(s_class_layer, nextclass);
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