
#include "pebble.h"
#include "seven_segment.h"
#include "config.h"
#include "keys.h"
#include "mooncalc.h"
#include "math.h"

static Window *s_main_window;
static Layer *s_image_layer_hour_1;
static Layer *s_image_layer_hour_2;
static Layer *s_image_layer_minute_1;
static Layer *s_image_layer_minute_2;
static Layer *s_image_layer_second_1;
static Layer *s_image_layer_second_2;

static int digit_h_1 = 0;
static int digit_h_2 = 0;
static int digit_m_1 = 0;
static int digit_m_2 = 0;
static int digit_s_1 = 0;
static int digit_s_2 = 0;

static GBitmap *background_image;
static BitmapLayer *background_layer;

static char *sys_locale;



static TextLayer *text_sunrise_layer; 
static TextLayer *text_sunset_layer; 
static TextLayer *battery_layer;
static TextLayer *connection_layer;

static TextLayer *Date_Layer;
static TextLayer *cwLayer; //calender week

//static TextLayer *moonLayer;
static TextLayer *moonLayer_IMG;
GFont pFontMoon     = 0;

static TextLayer *weather_layer_1_temp; // Temperature
static TextLayer *weather_layer_3_location; // Location Name
static TextLayer *weather_layer_4_last_update; // Time of last data / time since last update
static TextLayer *weather_layer_7_string_1;

static TextLayer *text_TimeZone_layer; //24H/AM/PM and UTC-Offset


static TextLayer *runtime_layer_1; //no of days running on battery

static int last_battery_percent = 0;
static int battery_percent_on_exit = 0;
static time_t last_battery_charged_time = 0; //absolute time of last event
static int last_charge_state = 0; //last state of charging on exit (0: discharging; 1: plugged & charging; 2: plugged & full)
static time_t last_battery_period_time = 0; // last duration of charging/discharging period



// Weather and other global variables:
static time_t phone_last_updated = 0;
static char location_name[32];
static int  location_latitude   = (int)(LATITUDE*1E6); //in 1E6
static int  location_longitude  = (int)(LONGITUDE*1E6); //in 1E6
static int  weather_TEMP        = 0; //in degree C
static char weather_string_1[32];
static int  time_UTC_OFFSET     = (int)(TIMEZONE*3600); //in seconds
static char time_ZONE_NAME[10];
static char sun_rise[10] = "--:--";
static char sun_set[10] = "--:--";

//Colors:
GColor textcolor_background;
//GColor textcolor_sun;
//GColor textcolor_con;
//GColor textcolor_bat;
//GColor textcolor_date;
//GColor textcolor_cal;
//GColor textcolor_moon;
//GColor textcolor_weather;
//GColor textcolor_location;
//GColor textcolor_last_update;
//GColor textcolor_tz;
GColor textcolor_clock;
GColor textcolor_seconds;


// Settings variables (App Config):

static int InvertColors = INVERT_COLORS;
static int LightOn = LIGHT_ON;
static int DisplaySeconds = DISPLAY_SECONDS;
static int vibe_on_disconnect = VIBE_ON_DISC;
static int vibe_on_charged_full = VIBE_ON_FULL;
static int degree_f = DEGREE_F;
static char date_format[15] = DATE_FORMAT;



static bool initDone; // e.g. for avoiding "no BT" vibration with initial opening of the watchface
static bool doUpdateWeather;
static int LightIsOn = 0; //saves the state of the background light
static int init_battery_handler = 0;
static char hour_mode_str[4] = "24H";








void LoadData(void) {
  //if (persist_exists()) persist_read_string();
  int key = KEY_LOCATION_NAME;
  if (persist_exists(key)) persist_read_string(key, location_name, sizeof(location_name));
  key = KEY_LOCATION_LAT;
  if (persist_exists(key)) location_latitude = persist_read_int(key);
  key = KEY_LOCATION_LON;
  if (persist_exists(key)) location_longitude = persist_read_int(key);
  
  key = KEY_WEATHER_TEMP;
  if (persist_exists(key)) weather_TEMP = persist_read_int(key);
  /*
  key = KEY_WEATHER_TEMP_MIN;
  if (persist_exists(key)) weather_TEMP_MIN = persist_read_int(key);
  key = KEY_WEATHER_TEMP_MAX;
  if (persist_exists(key)) weather_TEMP_MAX = persist_read_int(key);
  key = KEY_WEATHER_PRESSURE;
  if (persist_exists(key)) weather_PRESSURE = persist_read_int(key);
  key = KEY_WEATHER_WIND_SPEED;
  if (persist_exists(key)) weather_WIND_SPEED = persist_read_int(key);
  */
  
  key = KEY_WEATHER_STRING_1;
  if (persist_exists(key)) persist_read_string(key, weather_string_1, sizeof(weather_string_1));
  
  /*
  key = KEY_WEATHER_HUMIDITY;
  if (persist_exists(key)) weather_HUMIDITY = persist_read_int(key);
  */
  
  key = KEY_TIME_UTC_OFFSET;
  if (persist_exists(key)) time_UTC_OFFSET = persist_read_int(key);
  
  key = KEY_TIME_LAST_UPDATE;
  if (persist_exists(key)) phone_last_updated = (time_t)(persist_read_int(key));
  
  key = KEY_TIME_ZONE_NAME;
  if (persist_exists(key)) persist_read_string(key, time_ZONE_NAME, sizeof(time_ZONE_NAME));
  
  
  
  key = KEY_BTY_LAST_PERCENT;
  if (persist_exists(key)) last_battery_percent = persist_read_int(key);
  key = KEY_BTY_PERCENT_EXIT;
  if (persist_exists(key)) battery_percent_on_exit = persist_read_int(key);
  key = KEY_BTY_LAST_CHARGED;
  if (persist_exists(key)) last_battery_charged_time = (time_t)(persist_read_int(key));
  
  key = KEY_BTY_LAST_STATE;
  if (persist_exists(key)) last_charge_state = persist_read_int(key);
  key = KEY_BTY_LAST_PERIOD;
  if (persist_exists(key)) last_battery_period_time = (time_t)(persist_read_int(key));
  
  
  init_battery_handler = 1;
  
  key = KEY_SUN_RISE;
  if (persist_exists(key)) persist_read_string(key, sun_rise, sizeof(sun_rise));
  
  key = KEY_SUN_SET;
  if (persist_exists(key)) persist_read_string(key, sun_set, sizeof(sun_set));
  
  
  //Settings:
  //key = KEY_SET_INVERT_COLOR;
  //if (persist_exists(key)) InvertColors = persist_read_int(key); (is done in window load ...)
  
  key = KEY_SET_LIGHT_ON;
  if (persist_exists(key)) LightOn = persist_read_int(key);
  
  key = KEY_SET_DISPLAY_SEC;
  if (persist_exists(key)) DisplaySeconds = persist_read_int(key);
  
  key = KEY_SET_VIBE_DISC;
  if (persist_exists(key)) vibe_on_disconnect = persist_read_int(key);
  
  key = KEY_SET_VIBE_FULL;
  if (persist_exists(key)) vibe_on_charged_full = persist_read_int(key);
  
  key = KEY_SET_DATE_FORMAT;
  if (persist_exists(key)) persist_read_string(key, date_format, sizeof(date_format));
  
}

void SaveData(void) {
  persist_write_string (KEY_LOCATION_NAME, location_name);
  persist_write_int    (KEY_LOCATION_LAT,  location_latitude);
  persist_write_int    (KEY_LOCATION_LON,  location_longitude);
  
  persist_write_int    (KEY_WEATHER_TEMP,  weather_TEMP);
  /*
  persist_write_int    (KEY_WEATHER_TEMP_MIN,  weather_TEMP_MIN);
  persist_write_int    (KEY_WEATHER_TEMP_MAX,  weather_TEMP_MAX);
  persist_write_int    (KEY_WEATHER_PRESSURE,  weather_PRESSURE);
  persist_write_int    (KEY_WEATHER_WIND_SPEED,  weather_WIND_SPEED);
  */
  persist_write_string (KEY_WEATHER_STRING_1, weather_string_1);
  /*
  persist_write_int    (KEY_WEATHER_HUMIDITY,  weather_HUMIDITY);
  */
  
  persist_write_int    (KEY_TIME_UTC_OFFSET,  time_UTC_OFFSET);
  persist_write_int    (KEY_TIME_LAST_UPDATE,  (int)(phone_last_updated));
  persist_write_string (KEY_TIME_ZONE_NAME, time_ZONE_NAME);
  
  
  
  persist_write_int    (KEY_BTY_LAST_PERCENT,  last_battery_percent);
  BatteryChargeState actual_charge = battery_state_service_peek();
  persist_write_int    (KEY_BTY_PERCENT_EXIT,  actual_charge.charge_percent);
  persist_write_int    (KEY_BTY_LAST_CHARGED,  (int)(last_battery_charged_time));
  
  persist_write_int    (KEY_BTY_LAST_STATE,  last_charge_state);
  persist_write_int    (KEY_BTY_LAST_PERIOD,  (int)(last_battery_period_time));
  
  
  persist_write_string (KEY_SUN_RISE, sun_rise);
  persist_write_string (KEY_SUN_SET , sun_set);
  
}

void DisplayLastUpdated(void) {
  static char last_updated_buffer[10];
  time_t now = time(NULL);
  time_t t_diff = now - phone_last_updated;
  
  if (phone_last_updated != 0){
    //t_diff = 800000;
    int days    = t_diff / (24*3600);
    int hours   = (t_diff % (24*3600)) / 3600;
    int minutes = (t_diff % 3600) / 60;
    //int seconds = (t_diff % 60);
    if (days == 0){
      if (hours == 0){
        snprintf(last_updated_buffer, sizeof(last_updated_buffer), "%d m", minutes);
      } else {
        if (hours < 10)
          snprintf(last_updated_buffer, sizeof(last_updated_buffer), "%d:%02d h", hours, minutes);
        else
          snprintf(last_updated_buffer, sizeof(last_updated_buffer), "%d h", hours);
      }
    } else {
      if (days < 10)
        snprintf(last_updated_buffer, sizeof(last_updated_buffer), "%d:%02d d", days, hours);
      else
        snprintf(last_updated_buffer, sizeof(last_updated_buffer), "%d d", days);
    }
    text_layer_set_text(weather_layer_4_last_update, last_updated_buffer);
  } else {
    text_layer_set_text(weather_layer_4_last_update, "--:--");
  }
  
  
  static char last_battery_buffer_1[30];
  static char last_battery_buffer_2[10];
  
  if (last_battery_charged_time == 0) last_battery_charged_time = now;
  time_t t_diff_bat = now - last_battery_charged_time;
  if (last_battery_charged_time != 0){
    //struct tm *cur_time = localtime(&last_battery_charged_time);
    int days    = t_diff_bat / (24*3600);
    int hours   = (t_diff_bat % (24*3600)) / 3600;
    int minutes = (t_diff_bat % 3600) / 60;
    int seconds = (t_diff_bat % 60);
    //snprintf(last_battery_buffer_1, sizeof(last_battery_buffer_1), "%d d\n%02d:%02d h\n%d%%", days, hours, minutes, last_battery_percent);
    //text_layer_set_text(runtime_layer_1, last_battery_buffer_1);
    
    
    int pdays    = last_battery_period_time / (24*3600);
    int phours   = (last_battery_period_time % (24*3600)) / 3600;
    int pminutes = (last_battery_period_time % 3600) / 60;
    int pseconds = (last_battery_period_time % 60);
    
    if (pdays == 0){
      if (phours == 0){
        if (pminutes == 0){
          snprintf(last_battery_buffer_2, sizeof(last_battery_buffer_2), "%d s", pseconds);
        } else {
          if (pminutes < 10){
            snprintf(last_battery_buffer_2, sizeof(last_battery_buffer_2), "%d:%02d m", pminutes, pseconds);
          } else {
            snprintf(last_battery_buffer_2, sizeof(last_battery_buffer_2), "%d m", pminutes);
          }
        }
      } else {
        if (phours < 10)
          snprintf(last_battery_buffer_2, sizeof(last_battery_buffer_2), "%d:%02d h", phours, pminutes);
        else
          snprintf(last_battery_buffer_2, sizeof(last_battery_buffer_2), "%d h", phours);
      }
    } else {
      if (pdays < 10)
        snprintf(last_battery_buffer_2, sizeof(last_battery_buffer_2), "%d:%02d d", pdays, phours);
      else
        snprintf(last_battery_buffer_2, sizeof(last_battery_buffer_2), "%d d", pdays);
    }
    
    
    if (days == 0){
      if (hours == 0){
        if (minutes == 0){
          snprintf(last_battery_buffer_1, sizeof(last_battery_buffer_1), "%d s\n(%d%%)\n(%s)", seconds, last_battery_percent, last_battery_buffer_2);
        } else {
          if (minutes < 10)
            snprintf(last_battery_buffer_1, sizeof(last_battery_buffer_1), "%d:%02d m\n(%d%%)\n(%s)", minutes, seconds, last_battery_percent, last_battery_buffer_2);
          else
            snprintf(last_battery_buffer_1, sizeof(last_battery_buffer_1), "%d m\n(%d%%)\n(%s)", minutes, last_battery_percent, last_battery_buffer_2);
        }
      } else {
        if (hours < 10)
          snprintf(last_battery_buffer_1, sizeof(last_battery_buffer_1), "%d:%02d h\n(%d%%)\n(%s)", hours, minutes, last_battery_percent, last_battery_buffer_2);
        else
          snprintf(last_battery_buffer_1, sizeof(last_battery_buffer_1), "%d h\n(%d%%)\n(%s)", hours, last_battery_percent, last_battery_buffer_2);
      }
    } else {
      if (days < 10)
        snprintf(last_battery_buffer_1, sizeof(last_battery_buffer_1), "%d:%02d d\n(%d%%)\n(%s)", days, hours, last_battery_percent, last_battery_buffer_2);
      else
        snprintf(last_battery_buffer_1, sizeof(last_battery_buffer_1), "%d d\n(%d%%)\n(%s)", days, last_battery_percent, last_battery_buffer_2);
    }
    text_layer_set_text(runtime_layer_1, last_battery_buffer_1);
  } else {
    text_layer_set_text(runtime_layer_1, " ");
  }
  
  
}

void DisplayData(void) {
  static char buffer_1[5];
  //static char buffer_2[5];
  //static char buffer_3[5];
  //static char buffer_4[5];
  //static char buffer_5[10];
  //static char buffer_6[10];
  //static char buffer_7[10];
  //static char buffer_8[10];
  static char buffer_9[20];
  //static char buffer_10[5];
  //static char buffer_11[5];
  
  if (degree_f)
    snprintf(buffer_1, sizeof(buffer_1), "%d", (int)(weather_TEMP*1.8+32));
  else
    snprintf(buffer_1, sizeof(buffer_1), "%d", weather_TEMP);
  text_layer_set_text(weather_layer_1_temp, buffer_1);
  
  /*
  //text_layer_set_text(weather_layer_2, weather_CONDITIONS);
  text_layer_set_text(weather_layer_2, " ");
  */
  text_layer_set_text(weather_layer_7_string_1, weather_string_1);
  //text_layer_set_text(weather_layer_7_string_1, "line 1\nline 2");
  
  text_layer_set_text(weather_layer_3_location, location_name);
  
  //text_layer_set_text(weather_layer_4, last_updated_buffer);
  
  /*
  snprintf(buffer_5, sizeof(buffer_5), "%d/%d", weather_TEMP_MIN, weather_TEMP_MAX);
  //text_layer_set_text(weather_layer_5, buffer_5);
  //text_layer_set_text(weather_layer_5, " ");
  */
  
  /*
  snprintf(buffer_6, sizeof(buffer_6), "%d hPa", weather_PRESSURE);
  //text_layer_set_text(weather_layer_6, buffer_6);
  text_layer_set_text(weather_layer_6, " ");
  */
  
  //weather_WIND_SPEED in cm/s (convert with *3.6/100 km/h and *2.236/100 mph)
  /*
  double conversion_factor = 3.6/100;
  char unit_str[8] = "%d km/h";
  if (degree_f){
    conversion_factor = 2.236/100;
    strcpy(unit_str, "%d mph");
  }
  snprintf(buffer_7, sizeof(buffer_7), unit_str, (int)(weather_WIND_SPEED*conversion_factor));
  
  
    
    
  //text_layer_set_text(weather_layer_7, buffer_7);
  text_layer_set_text(weather_layer_5, buffer_7);
  */
  
  /*
  snprintf(buffer_8, sizeof(buffer_8), "%d %%", weather_HUMIDITY);
  //text_layer_set_text(weather_layer_8, buffer_8);
  text_layer_set_text(weather_layer_8, " ");
  */
  
  //if (time_UTC_OFFSET == 0){
  //  snprintf(buffer_9, sizeof(buffer_9), "UTC");
  //  text_layer_set_text(text_TimeZone_layer, buffer_9);
  //} else {
  //  snprintf(buffer_9, sizeof(buffer_9), "UTC+%d (%s)", (int)(time_UTC_OFFSET/3600), time_ZONE_NAME);
  //  text_layer_set_text(text_TimeZone_layer, buffer_9);
  //}

  
  if (strlen(time_ZONE_NAME) > 0){
    snprintf(buffer_9, sizeof(buffer_9), "%s, %s", hour_mode_str, time_ZONE_NAME);
  } else {
    snprintf(buffer_9, sizeof(buffer_9), "%s, UTC+%d", hour_mode_str, (int)(time_UTC_OFFSET/3600));
  }
  text_layer_set_text(text_TimeZone_layer, buffer_9);
  
  text_layer_set_text(text_sunrise_layer, sun_rise);
  text_layer_set_text(text_sunset_layer, sun_set);
  
  DisplayLastUpdated();
}












// Called once per second
static void handle_second_tick(struct tm* current_time, TimeUnits units_changed) {
  
  if (LightOn == 3){
    if (LightIsOn){
      light_enable(0);
      LightIsOn = 0;
    } else {
      light_enable(1);
      LightIsOn = 1;
    }
  } else if (LightOn == 2){
    if (!LightIsOn){
      light_enable(1);
      LightIsOn = 1;
    }
  } else {
    if (LightIsOn){
      light_enable(0);
      LightIsOn = 0;
    }
  }
  
  if(clock_is_24h_style() == true) {
    digit_h_1 = current_time->tm_hour/10;
    digit_h_2 = current_time->tm_hour%10;
    snprintf(hour_mode_str, sizeof(hour_mode_str), "%s", "24H");
  } else {
    int hour12 = current_time->tm_hour;
    if ((hour12 > 11)){
      snprintf(hour_mode_str, sizeof(hour_mode_str), "%s", "PM");
      hour12-=12;
    } else snprintf(hour_mode_str, sizeof(hour_mode_str), "%s", "AM");
    if (hour12 == 0) hour12 = 12;
    digit_h_1 = hour12/10;
    digit_h_2 = hour12%10;
  }
  
  digit_m_1 = current_time->tm_min/10;
  digit_m_2 = current_time->tm_min%10;
  
  digit_s_1 = current_time->tm_sec/10;
  digit_s_2 = current_time->tm_sec%10;
  
  if (units_changed & HOUR_UNIT){
    layer_mark_dirty(s_image_layer_hour_1);
    layer_mark_dirty(s_image_layer_hour_2);
  }
  if (units_changed & MINUTE_UNIT){
    layer_mark_dirty(s_image_layer_minute_1);
    layer_mark_dirty(s_image_layer_minute_2);
  }
  if (DisplaySeconds){
    layer_mark_dirty(s_image_layer_second_1);
    layer_mark_dirty(s_image_layer_second_2);
  }
  
  
  static char date_buffer[15];
  if (units_changed & HOUR_UNIT) {
    strftime(date_buffer, sizeof(date_buffer), /*"%a, %d.%m."*/date_format, current_time);
    text_layer_set_text(Date_Layer, date_buffer);
  }
  
  
  //static char moon_buffer[7];
  static char moon[] = "m";
  if (units_changed & HOUR_UNIT) {
    // -------------------- Moon_phase
		//static int moonphase_number = 0;
    //moonphase_number += 1;
    int moonphase_number = 0;
    moonphase_number = calc_moonphase_number(location_latitude);
    moon[0] = (unsigned char)(moonphase_char_number(moonphase_number));
    
    text_layer_set_text(moonLayer_IMG, moon);
    
    /*
    snprintf(moon_buffer, sizeof(moon_buffer), "(%d)", moonphase_number);
    //text_layer_set_text(moonLayer, moon_buffer);
    text_layer_set_text(moonLayer, " ");
		//text_layer_set_text(moonLayer, MOONPHASE_NAME_LANGUAGE[moonphase_number]); 
    */
		// -------------------- Moon_phase	  
		  
		// -------------------- Calendar week  
	  static char cw_text[] = "XX00";
    if (strcmp("fr_FR", sys_locale) == 0) {
		  strftime(cw_text, sizeof(cw_text), TRANSLATION_CW_FR, current_time);
    } else if (strcmp("de_DE", sys_locale) == 0) {
      strftime(cw_text, sizeof(cw_text), TRANSLATION_CW_DE, current_time);
    } else { //default
      strftime(cw_text, sizeof(cw_text), TRANSLATION_CW_EN, current_time);
    }
		text_layer_set_text(cwLayer, cw_text); 
		// ------------------- Calendar week 
  }
  
  
  
  
  if (initDone || doUpdateWeather){
    if (units_changed & MINUTE_UNIT) {//MINUTE_UNIT, SECOND_UNIT
      if ((current_time->tm_min%WEATHER_UPDATE_INTERVAL_MINUTE == 0) || doUpdateWeather) { 
        
        doUpdateWeather = false;
        
        // Begin dictionary
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        
        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);
        
        // Send the message!
        app_message_outbox_send();
        
        //text_layer_set_text(weather_layer_2, "***");
        APP_LOG(APP_LOG_LEVEL_INFO, "Weather Update requested");
      }
    }
  }
  
  
  
  /*if (units_changed & MINUTE_UNIT)*/ //DisplayLastUpdated();
  DisplayData();
  
  
  
} // ---- end handle_second_tick() ----


static void handle_battery(BatteryChargeState charge_state) {
  
  //charge_state_int = 0: Not plugged (discharging)
  //charge_state_int = 1: plugged and charging
  //charge_state_int = 2: plugged and full
  
  static char battery_text[] = "C: 100%";
  static BatteryChargeState old_charge_state;
  int old_charge_state_int = 0; //not stitic, is calculated from old_charge_state
  
  
  
  if (init_battery_handler){ //call after Load()
    old_charge_state_int = last_charge_state;
  } else {
    if (old_charge_state.is_plugged){
      if (old_charge_state.is_charging) old_charge_state_int = 1; else old_charge_state_int = 2;
    }
  }
  old_charge_state = charge_state;

  if (charge_state.is_plugged){
    if (charge_state.is_charging) last_charge_state = 1; else last_charge_state = 2;
  } else last_charge_state = 0; // = actual charge state
  
  /* (DEBUG)
  static char moon_buffer[10];
  snprintf(moon_buffer, sizeof(moon_buffer), "%d -> %d", old_charge_state_int, last_charge_state);
  text_layer_set_text(moonLayer, moon_buffer);
  */

  if (old_charge_state_int != last_charge_state){
    if (LightOn < 2){
      if (LightOn == 1){
        if (charge_state.is_charging)
          light_enable(0);
        else
          light_enable(charge_state.is_plugged);
      }
    }
  }
  
  time_t tdiff = time(NULL) - last_battery_charged_time;
    
  if ((old_charge_state_int == 0) && (last_charge_state == 1)){ //discharging --> charging
    last_battery_period_time = tdiff;
    last_battery_charged_time = time(NULL);
    last_battery_percent = charge_state.charge_percent;
  } else if ((old_charge_state_int == 0) && (last_charge_state == 0)){ //discharging --> discharging
    if ((init_battery_handler) && (battery_percent_on_exit+10 < charge_state.charge_percent)){ //if higher battery percent as on last exit
      last_battery_period_time = tdiff;
      last_battery_charged_time = time(NULL);
      last_battery_percent = charge_state.charge_percent;
    }
  } else if ((old_charge_state_int == 1) && (last_charge_state == 2)){ //charging --> full
    if (tdiff > 10*60) last_battery_period_time = tdiff; else last_battery_period_time += tdiff; //small values are added to old time (because after battery is full, it charges about 6 min. and gets this event a second time)
    last_battery_charged_time = time(NULL);
    last_battery_percent = charge_state.charge_percent;
    
    if (vibe_on_charged_full){
      // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
      static const uint32_t const segments[] = { 30 };
      VibePattern pat = {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
      };
      vibes_enqueue_custom_pattern(pat);
    }
    
  } else if ((old_charge_state_int == 1) && (last_charge_state == 0)){ //charging --> discharging
    last_battery_period_time = tdiff;
    last_battery_charged_time = time(NULL);
    last_battery_percent = charge_state.charge_percent;
  } else if ((old_charge_state_int == 2) && (last_charge_state == 0)){ //full --> discharging
    //last_battery_period_time = tdiff;
    last_battery_charged_time = time(NULL);
    last_battery_percent = charge_state.charge_percent;
  }
    
    
  DisplayLastUpdated();
  
  
  
  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "C: %d%%", charge_state.charge_percent);
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }
  text_layer_set_text(battery_layer, battery_text);
  
  init_battery_handler = 0;
  
}



static void handle_bluetooth(bool connected) {
  if( !connected && initDone)
  {
    if (vibe_on_disconnect){
      // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
      static const uint32_t const segments[] = { 40 };
      VibePattern pat = {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
      };
      vibes_enqueue_custom_pattern(pat);
    }
  }
  text_layer_set_text(connection_layer, connected ? "BT" : "  ");
  if (connected && initDone){
    doUpdateWeather = true;
  }
}







static void layer_update_callback_hour_1(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, textcolor_background);
  graphics_fill_rect(ctx, GRect(0, 0, 26, 41), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  switch (digit_h_1){
    case 1: 
    seven_segment_paint_1(ctx, 41);
    break;
    case 2: 
    seven_segment_paint_2(ctx, 41);
    break;
    default:
    break;
  }
}

static void layer_update_callback_hour_2(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, textcolor_background);
  graphics_fill_rect(ctx, GRect(0, 0, 26, 41), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  switch (digit_h_2){
    case 1: 
    seven_segment_paint_1(ctx, 41);
    break;
    case 2: 
    seven_segment_paint_2(ctx, 41);
    break;
    case 3: 
    seven_segment_paint_3(ctx, 41);
    break;
    case 4: 
    seven_segment_paint_4(ctx, 41);
    break;
    case 5: 
    seven_segment_paint_5(ctx, 41);
    break;
    case 6: 
    seven_segment_paint_6(ctx, 41);
    break;
    case 7: 
    seven_segment_paint_7(ctx, 41);
    break;
    case 8: 
    seven_segment_paint_8(ctx, 41);
    break;
    case 9: 
    seven_segment_paint_9(ctx, 41);
    break;
    case 0: 
    seven_segment_paint_0(ctx, 41);
    break;
    default:
    break;
  }
}
static void layer_update_callback_minute_1(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, textcolor_background);
  graphics_fill_rect(ctx, GRect(0, 0, 26, 41), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  switch (digit_m_1){
    case 1: 
    seven_segment_paint_1(ctx, 41);
    break;
    case 2: 
    seven_segment_paint_2(ctx, 41);
    break;
    case 3: 
    seven_segment_paint_3(ctx, 41);
    break;
    case 4: 
    seven_segment_paint_4(ctx, 41);
    break;
    case 5: 
    seven_segment_paint_5(ctx, 41);
    break;
    case 6: 
    seven_segment_paint_6(ctx, 41);
    break;
    case 7: 
    seven_segment_paint_7(ctx, 41);
    break;
    case 8: 
    seven_segment_paint_8(ctx, 41);
    break;
    case 9: 
    seven_segment_paint_9(ctx, 41);
    break;
    case 0: 
    seven_segment_paint_0(ctx, 41);
    break;
    default:
    break;
  }
}
static void layer_update_callback_minute_2(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, textcolor_background);
  graphics_fill_rect(ctx, GRect(0, 0, 26, 41), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  switch (digit_m_2){
    case 1: 
    seven_segment_paint_1(ctx, 41);
    break;
    case 2: 
    seven_segment_paint_2(ctx, 41);
    break;
    case 3: 
    seven_segment_paint_3(ctx, 41);
    break;
    case 4: 
    seven_segment_paint_4(ctx, 41);
    break;
    case 5: 
    seven_segment_paint_5(ctx, 41);
    break;
    case 6: 
    seven_segment_paint_6(ctx, 41);
    break;
    case 7: 
    seven_segment_paint_7(ctx, 41);
    break;
    case 8: 
    seven_segment_paint_8(ctx, 41);
    break;
    case 9: 
    seven_segment_paint_9(ctx, 41);
    break;
    case 0: 
    seven_segment_paint_0(ctx, 41);
    break;
    default:
    break;
  }
}

static void layer_update_callback_second_1(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, textcolor_background);
  graphics_fill_rect(ctx, GRect(0, 0, 10, 15), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_seconds);
  switch (digit_s_1){
    case 1: 
    seven_segment_paint_1(ctx, 15);
    break;
    case 2: 
    seven_segment_paint_2(ctx, 15);
    break;
    case 3: 
    seven_segment_paint_3(ctx, 15);
    break;
    case 4: 
    seven_segment_paint_4(ctx, 15);
    break;
    case 5: 
    seven_segment_paint_5(ctx, 15);
    break;
    case 6: 
    seven_segment_paint_6(ctx, 15);
    break;
    case 7: 
    seven_segment_paint_7(ctx, 15);
    break;
    case 8: 
    seven_segment_paint_8(ctx, 15);
    break;
    case 9: 
    seven_segment_paint_9(ctx, 15);
    break;
    case 0: 
    seven_segment_paint_0(ctx, 15);
    break;
    default:
    break;
  }
  if (!DisplaySeconds){
    seven_segment_clear(ctx, 15);
  }
}
static void layer_update_callback_second_2(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, textcolor_background);
  graphics_fill_rect(ctx, GRect(0, 0, 10, 15), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_seconds);
  switch (digit_s_2){
    case 1: 
    seven_segment_paint_1(ctx, 15);
    break;
    case 2: 
    seven_segment_paint_2(ctx, 15);
    break;
    case 3: 
    seven_segment_paint_3(ctx, 15);
    break;
    case 4: 
    seven_segment_paint_4(ctx, 15);
    break;
    case 5: 
    seven_segment_paint_5(ctx, 15);
    break;
    case 6: 
    seven_segment_paint_6(ctx, 15);
    break;
    case 7: 
    seven_segment_paint_7(ctx, 15);
    break;
    case 8: 
    seven_segment_paint_8(ctx, 15);
    break;
    case 9: 
    seven_segment_paint_9(ctx, 15);
    break;
    case 0: 
    seven_segment_paint_0(ctx, 15);
    break;
    default:
    break;
  }
  if (!DisplaySeconds){
    seven_segment_clear(ctx, 15);
  }
}



static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  
  // Read first item
  Tuple *t = dict_read_first(iterator);
  
  int restart = 0;
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_LOCATION_NAME:
      //APP_LOG(APP_LOG_LEVEL_INFO, "KEY_LOCATION_NAME received");
      snprintf(location_name, sizeof(location_name), "%s", t->value->cstring);
      //APP_LOG(APP_LOG_LEVEL_INFO, "KEY_LOCATION_NAME written data");
      phone_last_updated = time(NULL);
      //APP_LOG(APP_LOG_LEVEL_INFO, "KEY_LOCATION_NAME updated last updated time.");
      break;
    case KEY_LOCATION_LAT:
      location_latitude = (int)t->value->int32;
      break;
    case KEY_LOCATION_LON:
      location_longitude = (int)t->value->int32;
      break;
    case KEY_WEATHER_TEMP:
      weather_TEMP = (int)t->value->int32;
      //APP_LOG(APP_LOG_LEVEL_INFO, "KEY_WEATHER_TEMP finished");
      break;
    /*
    case KEY_WEATHER_TEMP_MIN:
      weather_TEMP_MIN = (int)t->value->int32;
      break;
    case KEY_WEATHER_TEMP_MAX:
      weather_TEMP_MAX = (int)t->value->int32;
      break;
    case KEY_WEATHER_PRESSURE:
      weather_PRESSURE = (int)t->value->int32;
      break;
    case KEY_WEATHER_WIND_SPEED:
      weather_WIND_SPEED = (int)t->value->int32;
      break;
    */
    case KEY_WEATHER_STRING_1:
      snprintf(weather_string_1, sizeof(weather_string_1), "%s", t->value->cstring);
      text_layer_set_text(weather_layer_7_string_1, weather_string_1);
      //APP_LOG(APP_LOG_LEVEL_INFO, "weather_string_1 = %s", weather_string_1);
      break;
    /*
    case KEY_WEATHER_HUMIDITY:
      weather_HUMIDITY = (int)t->value->int32;
      break;
    */
    case KEY_TIME_UTC_OFFSET:
      time_UTC_OFFSET = -(int)t->value->int32;
      break;
    case KEY_TIME_ZONE_NAME:
      snprintf(time_ZONE_NAME, sizeof(time_ZONE_NAME), "%s", t->value->cstring);
      break;
    case KEY_SUN_RISE:
      snprintf(sun_rise, sizeof(sun_rise), "%s", t->value->cstring);
      break;
    case KEY_SUN_SET:
      snprintf(sun_set, sizeof(sun_set), "%s", t->value->cstring);
      break;
    case KEY_SET_INVERT_COLOR:
      if (InvertColors != (int)t->value->int32) restart = 1;
      InvertColors = (int)t->value->int32;
      persist_write_int(KEY_SET_INVERT_COLOR, InvertColors);
      break;
    case KEY_SET_LIGHT_ON:
      LightOn = (int)t->value->int32;
      persist_write_int(KEY_SET_LIGHT_ON, LightOn);
      break;
    case KEY_SET_DISPLAY_SEC:
      DisplaySeconds = (int)t->value->int32;
      persist_write_int(KEY_SET_DISPLAY_SEC, DisplaySeconds);
      layer_mark_dirty(s_image_layer_second_1);
      layer_mark_dirty(s_image_layer_second_2);
      break;
      
    case KEY_SET_VIBE_DISC:
      vibe_on_disconnect = (int)t->value->int32;
      persist_write_int(KEY_SET_VIBE_DISC, vibe_on_disconnect);
      break;
    case KEY_SET_VIBE_FULL:
      vibe_on_charged_full = (int)t->value->int32;
      persist_write_int(KEY_SET_VIBE_FULL, vibe_on_charged_full);
      break;
      
    case KEY_SET_DEGREE_F:
      if (degree_f != (int)t->value->int32) restart = 1;
      degree_f = (int)t->value->int32;
      persist_write_int(KEY_SET_DEGREE_F, degree_f);
      doUpdateWeather = true;
      break;
      
    case KEY_SET_DATE_FORMAT:
      snprintf(date_format, sizeof(date_format), "%s", t->value->cstring);
      APP_LOG(APP_LOG_LEVEL_ERROR, "date_format in watchface = %s", date_format);
      persist_write_string(KEY_SET_DATE_FORMAT, date_format);
      handle_second_tick(tick_time, SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT);
      break;
      
    
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  if (restart) window_stack_pop_all(true); //true means animated = slide out
  
  DisplayData();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}



static void main_window_load(Window *window) {
  
  // Use setlocale() to obtain the system locale for translation
  sys_locale = setlocale(LC_ALL, "");
  
  
  Layer *main_window_layer = window_get_root_layer(s_main_window);
  
  initDone = false;
  
  // --- Load Fonts --- 
  pFontMoon = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MOON_PHASES_SUBSET_30));
  
  if (persist_exists(KEY_SET_INVERT_COLOR)) InvertColors = persist_read_int(KEY_SET_INVERT_COLOR);
  if (persist_exists(KEY_SET_DEGREE_F)) degree_f = persist_read_int(KEY_SET_DEGREE_F);
  
  
  // --- Background Image ---
  if (degree_f){
    background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_F);
  } else {
    background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_C);
  }
  background_layer = bitmap_layer_create(layer_get_frame(main_window_layer));
  bitmap_layer_set_bitmap(background_layer, background_image);
  if (!InvertColors)
    bitmap_layer_set_compositing_mode(background_layer, GCompOpAssign);
  else
    #ifdef PBL_COLOR
    bitmap_layer_set_compositing_mode(background_layer, GCompOpAssign);
    #else
    bitmap_layer_set_compositing_mode(background_layer, GCompOpAssignInverted);
    #endif
  layer_add_child(main_window_layer, bitmap_layer_get_layer(background_layer));
  // --- END ---

  s_image_layer_hour_1 = layer_create(GRect(4, 94, 26, 41));
  layer_set_update_proc(s_image_layer_hour_1, layer_update_callback_hour_1);
  layer_add_child(main_window_layer, s_image_layer_hour_1);
  s_image_layer_hour_2 = layer_create(GRect(37, 94, 26, 41));
  layer_set_update_proc(s_image_layer_hour_2, layer_update_callback_hour_2);
  layer_add_child(main_window_layer, s_image_layer_hour_2);
  
  s_image_layer_minute_1 = layer_create(GRect(80, 94, 26, 41));
  layer_set_update_proc(s_image_layer_minute_1, layer_update_callback_minute_1);
  layer_add_child(main_window_layer, s_image_layer_minute_1);
  s_image_layer_minute_2 = layer_create(GRect(111, 94, 26, 41));
  layer_set_update_proc(s_image_layer_minute_2, layer_update_callback_minute_2);
  layer_add_child(main_window_layer, s_image_layer_minute_2);
  
  s_image_layer_second_1 = layer_create(GRect(113, 137, 10, 15));
  layer_set_update_proc(s_image_layer_second_1, layer_update_callback_second_1);
  layer_add_child(main_window_layer, s_image_layer_second_1);
  s_image_layer_second_2 = layer_create(GRect(126, 137, 10, 15));
  layer_set_update_proc(s_image_layer_second_2, layer_update_callback_second_2);
  layer_add_child(main_window_layer, s_image_layer_second_2);
  
  // --- Create Text-Layers:
  GColor textcolor = GColorWhite;
  
  #ifdef PBL_COLOR
    textcolor_background  = GColorFromRGB(0, 0, 0);
    GColor textcolor_sun         = GColorFromRGB(255, 255, 0);   //=GColorYellow //OK
    GColor textcolor_con         = GColorFromRGB(0, 170, 255);   //GColorVividCerulean
    GColor textcolor_bat         = GColorFromRGB(170, 0, 0);     //GColorDarkCandyAppleRed
    GColor textcolor_date        = GColorFromRGB(0, 170, 170);   //=GColorTiffanyBlue
    GColor textcolor_cal         = GColorFromRGB(0, 170, 170);   //=GColorTiffanyBlue
    GColor textcolor_moon        = GColorFromRGB(255, 255, 255); //OK
    GColor textcolor_weather     = GColorFromRGB(0, 255, 170);   //GColorMediumSpringGreen
    GColor textcolor_location    = GColorFromRGB(255, 170, 0);   //=GColorChromeYellow //OK
    GColor textcolor_last_update = GColorFromRGB(150, 150, 200); //OK
    GColor textcolor_tz          = GColorFromRGB(100, 100, 100); //OK
    textcolor_clock       = GColorFromRGB(0, 255, 0);
    textcolor_seconds     = GColorFromRGB(0, 170, 170);
  #else  
    if (InvertColors) textcolor = GColorBlack;
    GColor bkgcolor = GColorBlack;
    if (InvertColors) bkgcolor = GColorWhite;
    textcolor_background  = bkgcolor;
    GColor textcolor_sun         = textcolor;
    GColor textcolor_con         = textcolor;
    GColor textcolor_bat         = textcolor;
    GColor textcolor_date        = textcolor;
    GColor textcolor_cal         = textcolor;
    GColor textcolor_moon        = textcolor;
    GColor textcolor_weather     = textcolor;
    GColor textcolor_location    = textcolor;
    GColor textcolor_last_update = textcolor;
    GColor textcolor_tz          = textcolor;
    textcolor_clock       = textcolor;
    textcolor_seconds     = textcolor;
  #endif
    
  // Sunrise Text
  text_sunrise_layer = text_layer_create(GRect(7, 152, 50 /* width */, 30 /* height */)); 
  text_layer_set_text_color(text_sunrise_layer, textcolor_sun);
  text_layer_set_background_color(text_sunrise_layer, GColorClear );
  text_layer_set_font(text_sunrise_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(main_window_layer, text_layer_get_layer(text_sunrise_layer));
  
  // Sunset Text
  text_sunset_layer = text_layer_create(GRect(110, 152, 50 /* width */, 30 /* height */)); 
  text_layer_set_text_color(text_sunset_layer, textcolor_sun);
  text_layer_set_background_color(text_sunset_layer, GColorClear );
  text_layer_set_font(text_sunset_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(main_window_layer, text_layer_get_layer(text_sunset_layer));      
  
  // Connection
  connection_layer = text_layer_create(GRect(82, 152, /* width */ 50, 34 /* height */));
  text_layer_set_text_color(connection_layer, textcolor_con);
  text_layer_set_background_color(connection_layer, GColorClear);
  text_layer_set_font(connection_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(connection_layer, GTextAlignmentLeft);
  text_layer_set_text(connection_layer, "BT");
  layer_add_child(main_window_layer, text_layer_get_layer(connection_layer));

  // Battery state
  battery_layer = text_layer_create(GRect(40, 152, /* width */ 38, 34 /* height */));
  text_layer_set_text_color(battery_layer, textcolor_bat);
  text_layer_set_background_color(battery_layer, GColorClear);
  text_layer_set_font(battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(battery_layer, GTextAlignmentRight);
  text_layer_set_text(battery_layer, "100%");  
  layer_add_child(main_window_layer, text_layer_get_layer(battery_layer));  
  
  
  // Date text
  Date_Layer = text_layer_create(GRect(5, 63, 130 /* width */, 30 /* height */));
  text_layer_set_text_color(Date_Layer, textcolor_date);
  text_layer_set_background_color(Date_Layer, GColorClear );
  text_layer_set_font(Date_Layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(main_window_layer, text_layer_get_layer(Date_Layer));
  
  // Calendar Week
  cwLayer = text_layer_create(GRect(100, 70, 40 /* width */, 30 /* height */)); 
  text_layer_set_text_color(cwLayer, textcolor_cal);
  text_layer_set_background_color(cwLayer, GColorClear );
  text_layer_set_font(cwLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(cwLayer, GTextAlignmentRight);
  layer_add_child(main_window_layer, text_layer_get_layer(cwLayer));
  
  
  // Moonphase Text
  /*
  moonLayer = text_layer_create(GRect(3, 53, 33 , 30)); 
  text_layer_set_text_color(moonLayer, textcolor_moon);
  text_layer_set_background_color(moonLayer, GColorClear );
  text_layer_set_font(moonLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(moonLayer, GTextAlignmentCenter);
  layer_add_child(main_window_layer, text_layer_get_layer(moonLayer));
  */
  
  moonLayer_IMG = text_layer_create(GRect(3, 20, 33, 33));
  text_layer_set_text_color(moonLayer_IMG, textcolor_moon);
  text_layer_set_background_color(moonLayer_IMG, GColorClear);
  text_layer_set_font(moonLayer_IMG, pFontMoon);
  text_layer_set_text_alignment(moonLayer_IMG, GTextAlignmentCenter);
  layer_add_child(main_window_layer, text_layer_get_layer(moonLayer_IMG));
  
  // --- Weather Layers: ---
  
  // Create temperature Layer
  weather_layer_1_temp = text_layer_create(GRect(50, 10, 70, 30));
  text_layer_set_background_color(weather_layer_1_temp, GColorClear);
  text_layer_set_text_color(weather_layer_1_temp, textcolor);
  text_layer_set_text_alignment(weather_layer_1_temp, GTextAlignmentRight);
  text_layer_set_text(weather_layer_1_temp, "---");
  text_layer_set_font(weather_layer_1_temp, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD)); //FONT_KEY_BITHAM_30_BLACK
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_1_temp));
  
  /*
  // Create conditions Layer
  weather_layer_2 = text_layer_create(GRect(38, 18, 84-38, 15));
  text_layer_set_background_color(weather_layer_2, GColorClear);
  text_layer_set_text_color(weather_layer_2, textcolor_weather);
  text_layer_set_text_alignment(weather_layer_2, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_2, "");
  text_layer_set_font(weather_layer_2, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_2));
  */
  
  // Create location name Layer
  weather_layer_3_location = text_layer_create(GRect(3, -1, 104, 17));
  text_layer_set_background_color(weather_layer_3_location, GColorClear);
  text_layer_set_text_color(weather_layer_3_location, textcolor_location);
  text_layer_set_text_alignment(weather_layer_3_location, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_3_location, "---" /*"Loading Weather ..."*/);
  text_layer_set_font(weather_layer_3_location, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_3_location));
  
  // Create last updated Layer
  weather_layer_4_last_update = text_layer_create(GRect(104, -1, 40, 17));
  text_layer_set_background_color(weather_layer_4_last_update, GColorClear);
  text_layer_set_text_color(weather_layer_4_last_update, textcolor_last_update);
  text_layer_set_text_alignment(weather_layer_4_last_update, GTextAlignmentRight);
  text_layer_set_text(weather_layer_4_last_update, "---");
  text_layer_set_font(weather_layer_4_last_update, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_4_last_update));
  
  /*
  // Create Temp. Min/Max Layer (--> Wind Speed)
  weather_layer_5 = text_layer_create(GRect(86, 40, 144-86-2, 15));
  text_layer_set_background_color(weather_layer_5, GColorClear);
  text_layer_set_text_color(weather_layer_5, textcolor_weather);
  text_layer_set_text_alignment(weather_layer_5, GTextAlignmentRight);
  text_layer_set_text(weather_layer_5, "---/---");
  text_layer_set_font(weather_layer_5, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_5));
  
  // Create Pressure Layer
  weather_layer_6 = text_layer_create(GRect(38-1, 18+16, 84-38, 15)); //TODO
  text_layer_set_background_color(weather_layer_6, GColorClear);
  text_layer_set_text_color(weather_layer_6, textcolor_weather);
  text_layer_set_text_alignment(weather_layer_6, GTextAlignmentRight);
  text_layer_set_text(weather_layer_6, "---- hPa");
  text_layer_set_font(weather_layer_6, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_6));
  */
  
  // Create String_1 Layer
  weather_layer_7_string_1 = text_layer_create(GRect(86, 54-15, 144-86-2, 30)); //TODO
  text_layer_set_background_color(weather_layer_7_string_1, GColorClear);
  text_layer_set_text_color(weather_layer_7_string_1, textcolor_weather);
  text_layer_set_text_alignment(weather_layer_7_string_1, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_7_string_1, "---\n---");
  text_layer_set_font(weather_layer_7_string_1, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_7_string_1));
  
  /*
  // Create Humidity Layer
  weather_layer_8 = text_layer_create(GRect(38, 18+32, 84-38-2, 15)); //TODO
  text_layer_set_background_color(weather_layer_8, GColorClear);
  text_layer_set_text_color(weather_layer_8, textcolor_weather);
  text_layer_set_text_alignment(weather_layer_8, GTextAlignmentRight);
  text_layer_set_text(weather_layer_8, "--- %");
  text_layer_set_font(weather_layer_8, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_8));
  */
  
  // Create TimeZone Layer
  text_TimeZone_layer = text_layer_create(GRect(5, 132, 100, 20)); //TODO
  text_layer_set_background_color(text_TimeZone_layer, GColorClear);
  text_layer_set_text_color(text_TimeZone_layer, textcolor_tz);
  text_layer_set_text_alignment(text_TimeZone_layer, GTextAlignmentLeft);
  text_layer_set_text(text_TimeZone_layer, " ");
  text_layer_set_font(text_TimeZone_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(main_window_layer, text_layer_get_layer(text_TimeZone_layer));
  
  // Battery stats:
  runtime_layer_1 = text_layer_create(GRect(40, 15+3, 45, 15+40)); //TODO
  text_layer_set_background_color(runtime_layer_1, GColorClear);
  text_layer_set_text_color(runtime_layer_1, textcolor_bat);
  text_layer_set_text_alignment(runtime_layer_1, GTextAlignmentCenter);
  text_layer_set_text(runtime_layer_1, "10 d\n23:40 h\n100%");
  text_layer_set_font(runtime_layer_1, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(runtime_layer_1));
    
  // --- END ---
  
  
  LoadData();
  DisplayData();
  
  // Avoids a blank screen on watch start.
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  handle_second_tick(tick_time, SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT);
  handle_battery(battery_state_service_peek());
  handle_bluetooth(bluetooth_connection_service_peek());
  
  // --- Register Event Handlers ---
  tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  initDone = true;
  doUpdateWeather = false;
}

static void main_window_unload(Window *window) {
  
  SaveData();
  
  
  // --- Background Image ---
  layer_remove_from_parent(bitmap_layer_get_layer(background_layer));
  bitmap_layer_destroy(background_layer);
  gbitmap_destroy(background_image);
  
  layer_destroy(s_image_layer_hour_1);
  layer_destroy(s_image_layer_hour_2);
  layer_destroy(s_image_layer_minute_1);
  layer_destroy(s_image_layer_minute_2);
  layer_destroy(s_image_layer_second_1);
  layer_destroy(s_image_layer_second_2);
  
  
  text_layer_destroy(text_sunrise_layer);
  text_layer_destroy(text_sunset_layer);  
  text_layer_destroy(connection_layer);
  text_layer_destroy(battery_layer); 
  
  text_layer_destroy(Date_Layer);
  text_layer_destroy(cwLayer);
  
  //text_layer_destroy(moonLayer);
  text_layer_destroy(moonLayer_IMG);
  fonts_unload_custom_font(pFontMoon);
  
  /*
  static TextLayer *weather_layer_1_temp; // Temperature
  static TextLayer *weather_layer_3_location; // Location Name
  static TextLayer *weather_layer_4_last_update; // Time of last data / time since last update
  static TextLayer *weather_layer_7_string_1;
  */
  
  text_layer_destroy(weather_layer_1_temp);
  //text_layer_destroy(weather_layer_2);
  text_layer_destroy(weather_layer_3_location);
  text_layer_destroy(weather_layer_4_last_update);
  //text_layer_destroy(weather_layer_5);
  //text_layer_destroy(weather_layer_6);
  text_layer_destroy(weather_layer_7_string_1);
  //text_layer_destroy(weather_layer_8);
  text_layer_destroy(text_TimeZone_layer);
  
  text_layer_destroy(runtime_layer_1);
  
  // --- END ---
  
  // --- Event Handlers ---
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
