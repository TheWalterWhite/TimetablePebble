#include <pebble.h>
#include <pebble_fonts.h>
#include "main.h"
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_left_layer;
static TextLayer *s_until_layer;
static TextLayer *s_date_layer;
static TextLayer *s_class_layer;
static Layer *s_battery_layer;
struct tm nextclasstime;
char currentDayName[10];
static char a[8];
static char date[10];
static int s_battery_level;
double timeleft;
char nextclass[10];

static ClaySettings settings;

static void set_info(tm nextclasstime, tm tick_time, char* blknum, int stringsize)  { //Stringsize has to be passed here because sizeof(blknum) returns either 0 or 4(idk) since it's a pointer
  timeleft = difftime(mktime(&nextclasstime), mktime(&tick_time));
  const time_t temp2 = timeleft; 
  struct tm* test = gmtime(&temp2); //Yes the time manager is called test. Take gmtime since timezone has already been converted
  strftime(a, sizeof(a), "%k:%M", test);
  strncpy(nextclass, blknum, stringsize);
}

static void next_class(char* nextclass_return, tm tick_time)  {

  // Sync day, month, year, etc to nextclasstime so we can manipulate just the hour and minute
  nextclasstime = tick_time; 
  
  // Make sure you're not some loser who uses this watchface on a weekend(and if you are set next block to 1)
  if((strncmp(currentDayName, "Sat", 2) == 0) || (strncmp(currentDayName, "Sun", 2) == 0 && (tick_time.tm_hour < 8 || (tick_time.tm_hour == 8 && tick_time.tm_min < 25))) || (strncmp(currentDayName, "Fri", 2) == 0 && (tick_time.tm_hour > 1 || (tick_time.tm_hour == 1 && tick_time.tm_min > 55)))) {
    strncpy(nextclass_return, settings.blk1name, sizeof(settings.blk1name));        // Saturdays are a bitch because %R won't display more than 24h so please don't use the watchface on a saturday
    strncpy(a, "", 1);
    text_layer_set_text(s_until_layer, "Monday");
    text_layer_set_text(s_time_left_layer, "");
  }  
     
   else if(strncmp(currentDayName, "Sun", 2) == 0){
    //nextclasstime.tm_wday = 1;
    nextclasstime.tm_hour = 8;
    nextclasstime.tm_min = 25;
    set_info(nextclasstime, tick_time, settings.blk1name, sizeof(settings.blk1name));
  }
  
  //Monday
  if(strncmp(currentDayName, "Mon", 2) == 0){
    
    if (strncmp(settings.bforemon, "", 1) != 0 && ((tick_time.tm_hour < settings.beforeschooltimes[0][0]) || (tick_time.tm_hour == settings.beforeschooltimes[0][0] && tick_time.tm_min < settings.beforeschooltimes[0][1]))){//Before Monday blk1
    nextclasstime.tm_hour = settings.beforeschooltimes[0][0];
    nextclasstime.tm_min = settings.beforeschooltimes[0][1];
    set_info(nextclasstime, tick_time, settings.bforemon, sizeof(settings.bforemon));
    }  
    
    else if (tick_time.tm_hour < 8 || (tick_time.tm_hour == 8 && tick_time.tm_min < 25)){
        nextclasstime.tm_hour = 8;
        nextclasstime.tm_min = 25;
        set_info(nextclasstime, tick_time, settings.blk1name, sizeof(settings.blk1name));
      }
           
    else if (tick_time.tm_hour < 9 || (tick_time.tm_hour == 9 && tick_time.tm_min < 21)){ //In block 1, next block is Advisory
        nextclasstime.tm_hour = 9;
        nextclasstime.tm_min = 21;
        set_info(nextclasstime, tick_time, "Advisory", 9);
        }          
    
    else if (tick_time.tm_hour < 10 || (tick_time.tm_hour == 10 && tick_time.tm_min < 6)){ //In Focus, next block is 2
              nextclasstime.tm_hour = 10;
              nextclasstime.tm_min = 6;
              set_info(nextclasstime, tick_time, settings.blk2name, sizeof(settings.blk2name));
          }

    else if (tick_time.tm_hour < 11 || (tick_time.tm_hour == 11 && tick_time.tm_min < 7)){ //In block 2, next block is 3
              nextclasstime.tm_hour = 11;
              nextclasstime.tm_min = 7;
              set_info(nextclasstime, tick_time, settings.blk3name, sizeof(settings.blk3name));
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 8)){ //In block 3, next block is Lunch
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 8;
      if(strncmp(settings.lunchactivitymon, "", 1) != 0 && (strncmp(settings.lunchactivitymon, " ", 1) != 0)){
        set_info(nextclasstime, tick_time, settings.lunchactivitymon, sizeof(settings.lunchactivitymon));
      }
      else{
        set_info(nextclasstime, tick_time, "Lunch", 6);
      }
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 56)){ //In Lunch, next block is 4
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 56;
              set_info(nextclasstime, tick_time, settings.blk4name, sizeof(settings.blk4name));
          }

    else if (tick_time.tm_hour < 13 || (tick_time.tm_hour == 13 && tick_time.tm_min < 57)){ //In block 4, next block is 5
              nextclasstime.tm_hour = 13;
              nextclasstime.tm_min = 57;
              set_info(nextclasstime, tick_time, settings.blk5name, sizeof(settings.blk5name));
          }
    
    else if (tick_time.tm_hour < 14 || (tick_time.tm_hour == 14 && tick_time.tm_min < 59)){ //In block 5, next block is either freedom or afterschoolmon
              nextclasstime.tm_hour = 14;
              nextclasstime.tm_min = 59;
              if(strncmp(settings.afterschoolmon, "", 1) != 0){
                set_info(nextclasstime, tick_time, settings.afterschoolmon, sizeof(settings.afterschoolmon));
              }
                else
                  set_info(nextclasstime, tick_time, "Freedom", 7);
    }
    
    else if ((strncmp(settings.afterschoolmon, "", 1) != 0 && ((tick_time.tm_hour < settings.afterschoolendtimes[0][0]))) || (tick_time.tm_hour == settings.afterschoolendtimes[0][0] && tick_time.tm_min < settings.afterschoolendtimes[0][1])){ //In after school activity, next block is freedom
              nextclasstime.tm_hour = settings.afterschoolendtimes[0][0];
              nextclasstime.tm_min = settings.afterschoolendtimes[0][1];
              set_info(nextclasstime, tick_time, "Freedom", 7);
          } 
    else {
      nextclasstime.tm_wday = 2;
      nextclasstime.tm_hour = 8;
      nextclasstime.tm_min = 25;
      set_info(nextclasstime, tick_time, settings.blk6name, sizeof(settings.blk6name));
    }
      
    }
  
  //Tuesday
  if(strncmp(currentDayName, "Tue", 2) == 0){
    if (tick_time.tm_hour < 8 || (tick_time.tm_hour == 8 && tick_time.tm_min < 25)){//Before Tuesday blk6
        nextclasstime.tm_hour = 8;
        nextclasstime.tm_min = 25;
        set_info(nextclasstime, tick_time, settings.blk6name, sizeof(settings.blk6name));
      }
           
    
    else if (tick_time.tm_hour < 9 || (tick_time.tm_hour == 9 && tick_time.tm_min < 21)){ //In block 6, next block is Focus
        nextclasstime.tm_hour = 9;
        nextclasstime.tm_min = 21;
        set_info(nextclasstime, tick_time, "Focus", 6);
        }          
    
    else if (tick_time.tm_hour < 10 || (tick_time.tm_hour == 10 && tick_time.tm_min < 6)){ //In Focus, next block is 7
              nextclasstime.tm_hour = 10;
              nextclasstime.tm_min = 6;
              set_info(nextclasstime, tick_time, settings.blk7name, sizeof(settings.blk7name));
          }

    else if (tick_time.tm_hour < 11 || (tick_time.tm_hour == 11 && tick_time.tm_min < 7)){ //In block 2, next block is 8
              nextclasstime.tm_hour = 11;
              nextclasstime.tm_min = 7;
              set_info(nextclasstime, tick_time, settings.blk8name, sizeof(settings.blk8name));
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 8)){ //In block 8, next block is Lunch
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 8;
      if(strncmp(settings.lunchactivitytue, "", 1) != 0){
        set_info(nextclasstime, tick_time, settings.lunchactivitytue, sizeof(settings.lunchactivitytue));
      }
      else{
        set_info(nextclasstime, tick_time, "Lunch", 6);
      }
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 56)){ //In Lunch, next block is 1
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 56;
              set_info(nextclasstime, tick_time, settings.blk1name, sizeof(settings.blk1name));
          }

    else if (tick_time.tm_hour < 13 || (tick_time.tm_hour == 13 && tick_time.tm_min < 57)){ //In block 1, next block is 2
              nextclasstime.tm_hour = 13;
              nextclasstime.tm_min = 57;
              set_info(nextclasstime, tick_time, settings.blk2name, sizeof(settings.blk2name));
          }
    
    else if (tick_time.tm_hour < 14 || (tick_time.tm_hour == 14 && tick_time.tm_min < 59)){ //In block 2, next block is either freedom or afterschoolmon
              nextclasstime.tm_hour = 14;
              nextclasstime.tm_min = 59;
              if(strncmp(settings.afterschooltue, "", 1) != 0){
                set_info(nextclasstime, tick_time, settings.afterschooltue, sizeof(settings.afterschooltue));
              }
                else
                  set_info(nextclasstime, tick_time, "Freedom", 7);
    }
    
    else if (strncmp(settings.afterschooltue, "", 1) != 0 && ((tick_time.tm_hour < settings.afterschoolendtimes[1][0]) || (tick_time.tm_hour == settings.afterschoolendtimes[1][0] && tick_time.tm_min < settings.afterschoolendtimes[1][1]))){ //In block 4, next block is 5            IGNORE ERROR HERE UNTIL IT STOPS COMPILING
              nextclasstime.tm_hour = settings.afterschoolendtimes[1][0];
              nextclasstime.tm_min = settings.afterschoolendtimes[1][1];
              set_info(nextclasstime, tick_time, "Freedom", 7);
          }
    else {
      nextclasstime.tm_wday = 3;
      nextclasstime.tm_hour = 8;
      nextclasstime.tm_min = 25;
      set_info(nextclasstime, tick_time, settings.blk3name, sizeof(settings.blk3name));
    }
      
}
  
  //Wednesday
  if(strncmp(currentDayName, "Wed", 2) == 0){
    if (tick_time.tm_hour < 8 || (tick_time.tm_hour == 8 && tick_time.tm_min < 25)){//Before Wednesday blk3
        nextclasstime.tm_hour = 8;
        nextclasstime.tm_min = 25;
        set_info(nextclasstime, tick_time, settings.blk3name, sizeof(settings.blk3name));
      }
           
    
    else if (tick_time.tm_hour < 9 || (tick_time.tm_hour == 9 && tick_time.tm_min < 21)){ //In block 3, next block is Focus
        nextclasstime.tm_hour = 9;
        nextclasstime.tm_min = 21;
        set_info(nextclasstime, tick_time, "Focus", 6);
        }          
    
    else if (tick_time.tm_hour < 10 || (tick_time.tm_hour == 10 && tick_time.tm_min < 6)){ //In Focus, next block is 4
              nextclasstime.tm_hour = 10;
              nextclasstime.tm_min = 6;
              set_info(nextclasstime, tick_time, settings.blk4name, sizeof(settings.blk4name));
          }

    else if (tick_time.tm_hour < 11 || (tick_time.tm_hour == 11 && tick_time.tm_min < 7)){ //In block 4, next block is 5
              nextclasstime.tm_hour = 11;
              nextclasstime.tm_min = 7;
              set_info(nextclasstime, tick_time, settings.blk5name, sizeof(settings.blk5name));
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 8)){ //In block 5, next block is Lunch
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 8;
      if(strncmp(settings.lunchactivitywed, "", 1) != 0){
        set_info(nextclasstime, tick_time, settings.lunchactivitywed, sizeof(settings.lunchactivitywed));
      }
      else{
        set_info(nextclasstime, tick_time, "Lunch", 6);
      }
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 56)){ //In Lunch, next block is 6
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 56;
              set_info(nextclasstime, tick_time, settings.blk6name, sizeof(settings.blk6name));
          }

    else if (tick_time.tm_hour < 13 || (tick_time.tm_hour == 13 && tick_time.tm_min < 57)){ //In block 1, next block is 2
              nextclasstime.tm_hour = 13;
              nextclasstime.tm_min = 57;
              set_info(nextclasstime, tick_time, settings.blk7name, sizeof(settings.blk7name));
    }
    
    else if (tick_time.tm_hour < 14 || (tick_time.tm_hour == 14 && tick_time.tm_min < 59)){ //In block 7, next block is either freedom or afterschoolmon
              nextclasstime.tm_hour = 14;
              nextclasstime.tm_min = 59;
              if(strncmp(settings.afterschoolwed, "", 1) != 0){
                set_info(nextclasstime, tick_time, settings.afterschoolwed, sizeof(settings.afterschoolwed));
              }
                else
                  set_info(nextclasstime, tick_time, "Freedom", 7);
    }
    
    else if (strncmp(settings.afterschoolwed, "", 1) != 0 && ((tick_time.tm_hour < settings.afterschoolendtimes[2][0]) || (tick_time.tm_hour == settings.afterschoolendtimes[2][0] && tick_time.tm_min < settings.afterschoolendtimes[2][1]))){ //In block 4, next block is 5            IGNORE ERROR HERE UNTIL IT STOPS COMPILING
              nextclasstime.tm_hour = settings.afterschoolendtimes[2][0];
              nextclasstime.tm_min = settings.afterschoolendtimes[2][1];
              set_info(nextclasstime, tick_time, "Freedom", 7);
          }
    else {
      nextclasstime.tm_wday = 4;
      nextclasstime.tm_hour = 8;
      nextclasstime.tm_min = 25;
      set_info(nextclasstime, tick_time, settings.blk8name, sizeof(settings.blk8name));
    }
      
}
  
  //Thursday
  if(strncmp(currentDayName, "Thu", 2) == 0){
    if (tick_time.tm_hour < 8 || (tick_time.tm_hour == 8 && tick_time.tm_min < 25)){//Before Thursday blk8
        nextclasstime.tm_hour = 8;
        nextclasstime.tm_min = 25;
        set_info(nextclasstime, tick_time, settings.blk8name, sizeof(settings.blk8name));
      }
           
    
    else if (tick_time.tm_hour < 9 || (tick_time.tm_hour == 9 && tick_time.tm_min < 21)){ //In block 8, next block is Focus
        nextclasstime.tm_hour = 9;
        nextclasstime.tm_min = 21;
        set_info(nextclasstime, tick_time, "Focus", 6);
        }          
    
    else if (tick_time.tm_hour < 10 || (tick_time.tm_hour == 10 && tick_time.tm_min < 6)){ //In Focus, next block is 1
              nextclasstime.tm_hour = 10;
              nextclasstime.tm_min = 6;
              set_info(nextclasstime, tick_time, settings.blk1name, sizeof(settings.blk1name));
          }

    else if (tick_time.tm_hour < 11 || (tick_time.tm_hour == 11 && tick_time.tm_min < 7)){ //In block 1, next block is 2
              nextclasstime.tm_hour = 11;
              nextclasstime.tm_min = 7;
              set_info(nextclasstime, tick_time, settings.blk2name, sizeof(settings.blk2name));
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 8)){ //In block 2, next block is Lunch
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 8;
      if(strncmp(settings.lunchactivitythu, "", 1) != 0){
        set_info(nextclasstime, tick_time, settings.lunchactivitythu, sizeof(settings.lunchactivitythu));
      }
      else{
        set_info(nextclasstime, tick_time, "Lunch", 6);
      }
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 56)){ //In Lunch, next block is 3
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 56;
              set_info(nextclasstime, tick_time, settings.blk3name, sizeof(settings.blk3name));
          }

    else if (tick_time.tm_hour < 13 || (tick_time.tm_hour == 13 && tick_time.tm_min < 57)){ //In block 3, next block is 4
              nextclasstime.tm_hour = 13;
              nextclasstime.tm_min = 57;
              set_info(nextclasstime, tick_time, settings.blk4name, sizeof(settings.blk4name));
          }
    
    else if (tick_time.tm_hour < 14 || (tick_time.tm_hour == 14 && tick_time.tm_min < 59)){ //In block 4, next block is either freedom or afterschoolmon
              nextclasstime.tm_hour = 14;
              nextclasstime.tm_min = 59;
              if(strncmp(settings.afterschoolthu, "", 1) != 0){
                set_info(nextclasstime, tick_time, settings.afterschoolthu, sizeof(settings.afterschoolthu));
              }
                else
                  set_info(nextclasstime, tick_time, "Freedom", 7);
    }
    
    else if (strncmp(settings.afterschoolthu, "", 1) != 0 && ((tick_time.tm_hour < settings.afterschoolendtimes[3][0]) || (tick_time.tm_hour == settings.afterschoolendtimes[3][0] && tick_time.tm_min < settings.afterschoolendtimes[3][1]))){ //In block 4, next block is 5            IGNORE ERROR HERE UNTIL IT STOPS COMPILING
              nextclasstime.tm_hour = settings.afterschoolendtimes[3][0];
              nextclasstime.tm_min = settings.afterschoolendtimes[3][1];
              set_info(nextclasstime, tick_time, "Freedom", 7);
          }
    else {
      nextclasstime.tm_wday = 5;
      nextclasstime.tm_hour = 8;
      nextclasstime.tm_min = 25;
      set_info(nextclasstime, tick_time, settings.blk5name, sizeof(settings.blk5name));
    }
      
}
  
  //Friday
  if(strncmp(currentDayName, "Fri", 2) == 0){
    if (tick_time.tm_hour < 8 || (tick_time.tm_hour == 8 && tick_time.tm_min < 25)){//Before Friday blk5
        nextclasstime.tm_hour = 8;
        nextclasstime.tm_min = 25;
        set_info(nextclasstime, tick_time, settings.blk5name, sizeof(settings.blk5name));
      }
           
    
    else if (tick_time.tm_hour < 9 || (tick_time.tm_hour == 9 && tick_time.tm_min < 21)){ //In block 5, next block is Focus
        nextclasstime.tm_hour = 9;
        nextclasstime.tm_min = 21;
        set_info(nextclasstime, tick_time, "Focus", 6);
        }          
    
    else if (tick_time.tm_hour < 10 || (tick_time.tm_hour == 10 && tick_time.tm_min < 22)){ //In Focus, next block is 6
              nextclasstime.tm_hour = 10;
              nextclasstime.tm_min = 22;
              set_info(nextclasstime, tick_time, settings.blk6name, sizeof(settings.blk6name));
          }

    else if (tick_time.tm_hour < 11 || (tick_time.tm_hour == 11 && tick_time.tm_min < 23)){ //In block 6, next block is Lunch
              nextclasstime.tm_hour = 11;
              nextclasstime.tm_min = 23;
      if(strncmp(settings.lunchactivityfri, "", 1) != 0){
        set_info(nextclasstime, tick_time, settings.lunchactivityfri, sizeof(settings.lunchactivityfri));
      }
      else{
        set_info(nextclasstime, tick_time, "Lunch", 6);
      }
          }

    else if (tick_time.tm_hour < 11 || (tick_time.tm_hour == 11 && tick_time.tm_min < 53)){ //In Lunch, next block is 7
              nextclasstime.tm_hour = 11;
              nextclasstime.tm_min = 53;
              set_info(nextclasstime, tick_time, settings.blk7name, sizeof(settings.blk7name));
          }

    else if (tick_time.tm_hour < 12 || (tick_time.tm_hour == 12 && tick_time.tm_min < 54)){ //In block 7, next block is 8
              nextclasstime.tm_hour = 12;
              nextclasstime.tm_min = 54;
              set_info(nextclasstime, tick_time, settings.blk8name, sizeof(settings.blk8name));
          }
    
    else if (tick_time.tm_hour < 13 || (tick_time.tm_hour == 13 && tick_time.tm_min < 55)){ //In block 8, next block is either freedom or afterschoolmon
              nextclasstime.tm_hour = 13;
              nextclasstime.tm_min = 55;
              if(strncmp(settings.afterschoolfri, "", 1) != 0){
                set_info(nextclasstime, tick_time, settings.afterschoolfri, sizeof(settings.afterschoolfri));
              }
                else
                  set_info(nextclasstime, tick_time, "Freedom", 7);
    }
    
    else if (strncmp(settings.afterschoolfri, "", 1) != 0 && ((tick_time.tm_hour < settings.afterschoolendtimes[4][0]) || (tick_time.tm_hour == settings.afterschoolendtimes[4][0] && tick_time.tm_min < settings.afterschoolendtimes[4][1]))){ //In block 4, next block is 5            IGNORE ERROR HERE UNTIL IT STOPS COMPILING
              nextclasstime.tm_hour = settings.afterschoolendtimes[4][0];
              nextclasstime.tm_min = settings.afterschoolendtimes[4][1];
              set_info(nextclasstime, tick_time, "Freedom", 7);
          }
    else {
      nextclasstime.tm_wday = 1;
      nextclasstime.tm_hour = 8;
      nextclasstime.tm_min = 25;
      set_info(nextclasstime, tick_time, settings.blk1name, sizeof(settings.blk1name));
    }
      
}
}

static void update_time()  {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm* tick_time = localtime(&temp);
  
  //Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
           "%H:%M" : "%l:%M", tick_time);
  
  // Get abbriviated day name, day number, month name
  char currentMonthDay[2];
  char currentMonthName[4];
  strftime(currentDayName, 4, "%a", tick_time);
  strftime(currentMonthDay, 4, "%e", tick_time);
  strftime(currentMonthName, 4, "%b", tick_time);
  
  // Calls next_class to get the next class(Who would have thought??)
  next_class(nextclass, *tick_time);
  
  //Combine date elements
  if (tick_time->tm_mday > 9){ // Pad 2 digit numbers with a space as single digits are already padded
  snprintf(date, 12, "%s %s %s", currentDayName, currentMonthDay, currentMonthName);
  }
  else{
  snprintf(date, 10, "%s%s %s", currentDayName, currentMonthDay, currentMonthName);
  }
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  text_layer_set_text(s_date_layer, date);
  text_layer_set_text(s_class_layer, nextclass);
  text_layer_set_text(s_time_left_layer, a);
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  update_time();
  printf("settings saved");
}

static void prv_default_settings() { //Try and keep block names 7 characters max or they don't fit on a pebble time
  settings.BackgroundColor = GColorWhite;
  settings.ForegroundColor = GColorBlack;
  settings.BatteryColor = GColorBlack;
  settings.LowBatteryColor = GColorRed;
  strncpy(settings.blk1name, "Math", 4);
  strncpy(settings.blk2name, "Band", 4);
  strncpy(settings.blk3name, "English", 7);
  strncpy(settings.blk4name, "French", 6);
  strncpy(settings.blk5name, "IT", 2);
  strncpy(settings.blk6name, "Physics", 7);
  strncpy(settings.blk7name, "Chem", 4);
  strncpy(settings.blk8name, "Spare", 5);
  settings.afterschoolendtimes[0][0] = 16; //4:30pm monday
  settings.afterschoolendtimes[0][1] = 30; 
  settings.beforeschooltimes[1][0] = 7; //Tuesday 7:25
  settings.beforeschooltimes[1][1] = 25;
  settings.beforeschooltimes[2][0] = 7; //Wednesday 7:25
  settings.beforeschooltimes[2][1] = 25;
  strncpy(settings.bforemon, "", 1);
  strncpy(settings.bforetue, "", 1);
  strncpy(settings.bforewed, "", 1);
  strncpy(settings.bforethu, "", 1);
  strncpy(settings.bforefri, "", 1);
  strncpy(settings.afterschoolmon, "RnB", 3);
  strncpy(settings.afterschooltue, "", 1);
  strncpy(settings.afterschoolwed, "", 1);
  strncpy(settings.afterschoolthu, "", 1);
  strncpy(settings.afterschoolfri, "", 1);
  strncpy(settings.lunchactivitymon, "", 4);
  strncpy(settings.lunchactivitytue, "", 1);
  strncpy(settings.lunchactivitywed, "", 4);
  strncpy(settings.lunchactivitythu, "", 1);
  strncpy(settings.lunchactivityfri, "", 1);
  printf("defaults loaded");
}

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));

  printf("settings loaded");
}


static void battery_callback(BatteryChargeState state) {
  // Record the new battery level
  s_battery_level = state.charge_percent;
  layer_mark_dirty(s_battery_layer); // Make sure layer gets redrawn every time
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  // Find the width of the bar (total width = 114px)
  int width = (s_battery_level * 114) / 100;
  
  // Draw the background
 // graphics_context_set_fill_color(ctx, settings.BackgroundColor);
  //graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // Draw the bar
  if (s_battery_level > 20){
    graphics_context_set_fill_color(ctx, settings.BatteryColor);
  }
  else {
    graphics_context_set_fill_color(ctx, settings.LowBatteryColor);
  }
  graphics_fill_rect(ctx, GRect(0, 0, width, bounds.size.h), 0, GCornerNone);
}


// AppMessage receive handler
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  //Read block names
  Tuple *blk1_t = dict_find(iter, MESSAGE_KEY_blkonename);
  if(blk1_t) {
    strncpy(settings.blk1name, blk1_t->value->cstring + 0, 10);}
  Tuple *blk2_t = dict_find(iter, MESSAGE_KEY_blktwoname);
  if(blk2_t) {
    strncpy(settings.blk2name, blk2_t->value->cstring + 0, 10);}
  Tuple *blk3_t = dict_find(iter, MESSAGE_KEY_blkthreename);
  if(blk3_t) {
    strncpy(settings.blk3name, blk3_t->value->cstring + 0, 10);}
  Tuple *blk4_t = dict_find(iter, MESSAGE_KEY_blkfourname);
  if(blk4_t) {
    strncpy(settings.blk4name, blk4_t->value->cstring + 0, 10);}
  Tuple *blk5_t = dict_find(iter, MESSAGE_KEY_blkfivename);
  if(blk5_t) {
    strncpy(settings.blk5name, blk5_t->value->cstring + 0, 10);}
  Tuple *blk6_t = dict_find(iter, MESSAGE_KEY_blksixname);
  if(blk6_t) {
    strncpy(settings.blk6name, blk6_t->value->cstring + 0, 10);}
  Tuple *blk7_t = dict_find(iter, MESSAGE_KEY_blksevenname);
  if(blk7_t) {
    strncpy(settings.blk7name, blk7_t->value->cstring + 0, 10);}
  Tuple *blk8_t = dict_find(iter, MESSAGE_KEY_blkeightname);
  if(blk8_t) {
    strncpy(settings.blk8name, blk8_t->value->cstring + 0, 10);}
  // Lunch Activities
  Tuple *lunmon_t = dict_find(iter, MESSAGE_KEY_lunchactivitymon);
  if(lunmon_t) {
    strncpy(settings.lunchactivitymon, lunmon_t->value->cstring + 0, 10);}
  Tuple *luntue_t = dict_find(iter, MESSAGE_KEY_lunchactivitytue);
  if(luntue_t) {
    strncpy(settings.lunchactivitytue, luntue_t->value->cstring + 0, 10);}
  Tuple *lunwed_t = dict_find(iter, MESSAGE_KEY_lunchactivitywed);
  if(lunwed_t) {
    strncpy(settings.lunchactivitywed, lunwed_t->value->cstring + 0, 10);}
  Tuple *lunthu_t = dict_find(iter, MESSAGE_KEY_lunchactivitythu);
  if(lunthu_t) {
    strncpy(settings.lunchactivitythu, lunthu_t->value->cstring + 0, 10);}
  Tuple *lunfri_t = dict_find(iter, MESSAGE_KEY_lunchactivityfri);
  if(lunfri_t) {
    strncpy(settings.lunchactivityfri, lunfri_t->value->cstring + 0, 10);}
//  Tuple *endt_t = dict_find(iter, MESSAGE_KEY_afterschoolendtimes);      //This was too much work to justify. Set the end times in the defaults if you need to
  // After School Activities
  Tuple *aftmon_t = dict_find(iter, MESSAGE_KEY_afterschoolmon);
  if(aftmon_t) {
    strncpy(settings.afterschoolmon, aftmon_t->value->cstring + 0, 10);}
  Tuple *afttue_t = dict_find(iter, MESSAGE_KEY_afterschooltue);
  if(afttue_t) {
    strncpy(settings.afterschooltue, afttue_t->value->cstring + 0, 10);}
  Tuple *aftwed_t = dict_find(iter, MESSAGE_KEY_afterschoolwed);
  if(aftwed_t) {
    strncpy(settings.afterschoolwed, aftwed_t->value->cstring + 0, 10);}
  Tuple *aftthu_t = dict_find(iter, MESSAGE_KEY_afterschoolthu);
  if(aftthu_t) {
    strncpy(settings.afterschoolthu, aftthu_t->value->cstring + 0, 10);}
  Tuple *aftfri_t = dict_find(iter, MESSAGE_KEY_afterschoolfri);
  if(aftfri_t) {
    strncpy(settings.afterschoolfri, aftfri_t->value->cstring + 0, 10);}
  
  // Before School Activities
  Tuple *bformon_t = dict_find(iter, MESSAGE_KEY_bforemon);
    if(bformon_t) {
    strncpy(settings.bforemon, aftmon_t->value->cstring + 0, 10);}
  Tuple *bfortue_t = dict_find(iter, MESSAGE_KEY_bforetue);
  if(bfortue_t) {
    strncpy(settings.bforetue, afttue_t->value->cstring + 0, 10);}
  Tuple *bforwed_t = dict_find(iter, MESSAGE_KEY_bforewed);
  if(bforwed_t) {
    strncpy(settings.bforewed, aftwed_t->value->cstring + 0, 10);}
  Tuple *bforthu_t = dict_find(iter, MESSAGE_KEY_bforethu);
  if(bforthu_t) {
    strncpy(settings.bforethu, aftthu_t->value->cstring + 0, 10);}
  Tuple *bforfri_t = dict_find(iter, MESSAGE_KEY_bforefri);
  if(bforfri_t) {
    strncpy(settings.bforefri, aftfri_t->value->cstring + 0, 10);}
  // Background Color
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  if (bg_color_t) {
    settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);
    window_set_background_color(s_main_window, settings.BackgroundColor);
  }
  // Foreground Color
  Tuple *fg_color_t = dict_find(iter, MESSAGE_KEY_ForegroundColor);
  if (fg_color_t) {
    settings.ForegroundColor = GColorFromHEX(fg_color_t->value->int32);
    text_layer_set_text_color(s_time_layer, settings.ForegroundColor);
    text_layer_set_text_color(s_until_layer, settings.ForegroundColor);
    text_layer_set_text_color(s_class_layer, settings.ForegroundColor);
    text_layer_set_text_color(s_time_left_layer, settings.ForegroundColor); 
}
  // Battery Bar Color
  Tuple *bat_color_t = dict_find(iter, MESSAGE_KEY_BatteryColor);
  if (bat_color_t) {
    settings.BatteryColor = GColorFromHEX(bat_color_t->value->int32);
    battery_callback(battery_state_service_peek());
  }
  // Low Battery Bar Color
  Tuple *batlow_color_t = dict_find(iter, MESSAGE_KEY_LowBatteryColor);
  if (batlow_color_t) {
    settings.LowBatteryColor = GColorFromHEX(batlow_color_t->value->int32);
    battery_callback(battery_state_service_peek());
  }
  prv_save_settings();
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
  
  // Create date layer
  s_date_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Create middle layer 2
  s_class_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Create Bottom Layer
  s_time_left_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  
  // Create battery meter Layer
  s_battery_layer = layer_create(GRect(14, 160, 115, 2));
  layer_set_update_proc(s_battery_layer, battery_update_proc);
  
  //Layout for bottom time remaining
  text_layer_set_background_color(s_time_left_layer, GColorClear);
  text_layer_set_text_color(s_time_left_layer, settings.ForegroundColor);
  text_layer_set_font(s_time_left_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_left_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_time_left_layer), GRect(0, 50, bounds.size.w, bounds.size.h));
  
  // Layout for middle layer 1
  text_layer_set_background_color(s_until_layer, GColorClear);
  text_layer_set_text_color(s_until_layer, settings.ForegroundColor);
  text_layer_set_font(s_until_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_until_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_until_layer), GRect(0, 100, bounds.size.w, bounds.size.h));
  text_layer_set_text(s_until_layer, "Until");
  
  // Layout for date layer
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, settings.ForegroundColor);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_date_layer), GRect(0, 40, bounds.size.w, bounds.size.h));
  text_layer_set_text(s_date_layer, "Date");


  // Layout for middle layer 2
  text_layer_set_background_color(s_class_layer, GColorClear);
  text_layer_set_text_color(s_class_layer, settings.ForegroundColor);
  text_layer_set_font(s_class_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_class_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_class_layer), GRect(0, 125, bounds.size.w, bounds.size.h));
  
  // Layout for top time
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, settings.ForegroundColor);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0, 0, bounds.size.w, bounds.size.h));
  
  
  // Add all as children to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_left_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_until_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_class_layer));
  layer_add_child(window_layer, s_battery_layer);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_time_left_layer);
  text_layer_destroy(s_class_layer);
  text_layer_destroy(s_until_layer);
  text_layer_destroy(s_date_layer);
  layer_destroy(s_battery_layer);
}

static void init()  {
  prv_load_settings();
  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(1024, 1024);
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Register for battery level updates
  battery_state_service_subscribe(battery_callback);
  battery_callback(battery_state_service_peek());
  // Make sure time is displayed from the start
  update_time();
}

static void deinit()  {
  tick_timer_service_unsubscribe();
  app_message_deregister_callbacks();
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void)  {
  init();
  app_event_loop();
  deinit();
}