
#include "pebble.h"
#include "seven_segment.h"
#include "config.h"
#include "keys.h"
#include "mooncalc.h"
#include "math.h"
#ifdef PBL_SDK_3
  #include "effect_layer.h"
#endif

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
#ifdef PBL_SDK_3
  static InverterLayer *invert_all_layer;
#endif

static char *sys_locale;



static TextLayer *text_sunrise_layer; 
static TextLayer *text_sunset_layer; 
static TextLayer *battery_runtime_layer;
static TextLayer *connection_layer;

static TextLayer *Date_Layer;
static TextLayer *cwLayer; //calender week

static TextLayer *moonLayer_IMG;
GFont pFontMoon     = 0;

static TextLayer *weather_layer_1_temp; // Temperature
static TextLayer *weather_layer_3_location; // Location Name
static TextLayer *weather_layer_4_last_update; // Time of last data / time since last update
static TextLayer *weather_layer_7_string_1; //configurable, under actual temperature, 2 lines
static TextLayer *weather_layer_7_string_2; //configurable, under moon and battery info, up to 2 infos in one line

static TextLayer *text_TimeZone_layer; //24H/AM/PM and UTC-Offset

static int last_battery_percent = 0;
static int battery_percent_on_exit = 0;
static time_t last_battery_charged_time = 0; //absolute time of last event
static int last_charge_state = 0; //last state of charging on exit (0: discharging; 1: plugged & charging; 2: plugged & full)
static time_t last_battery_period_time = 0; // last duration of charging/discharging period

#ifndef PBL_SDK_3 //only on SDK 2.x
  static InverterLayer *s_battery_layer_fill; //fill battery with an InverterLayer
#else
  static EffectLayer *s_battery_layer_fill; //fill battery with an InverterLayer by an effect_invert_color (my own effect added to effect_layer type)
  //InverterLayer is recreated by including EffektLayer, but it uses the wrong inverting effect.
#endif



// Weather and other global variables:
static time_t phone_last_updated = 0;
static char location_name[32];
static int  location_latitude   = (int)(LATITUDE*1E6); //in 1E6
static int  location_longitude  = (int)(LONGITUDE*1E6); //in 1E6
static int  weather_TEMP        = 0; //in degree C
static char weather_string_1[32]; //under actual temp.
static char weather_string_2[32]; //string under moon/bat
static int  time_UTC_OFFSET     = (int)(TIMEZONE*3600); //in seconds
static char time_ZONE_NAME[10];
static char sun_rise[10] = "--:--";
static char sun_set[10] = "--:--";

//Colors:
GColor textcolor_background;
#ifdef PBL_COLOR
  GColor textcolor_sun;
  GColor textcolor_con;
  GColor textcolor_bat;
  GColor textcolor_date;
  GColor textcolor_cal;
  GColor textcolor_moon;
  GColor textcolor_weather;
  GColor textcolor_location;
  GColor textcolor_last_update;
  GColor textcolor_tz;
#endif
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
static int WeatherUpdateInterval = WEATHER_UPDATE_INTERVAL_MINUTE;



static bool initDone; // e.g. for avoiding "no BT" vibration with initial opening of the watchface
static bool doUpdateWeather;
static int LightIsOn = 0; //saves the state of the background light
static int init_battery_handler = 0;
static char hour_mode_str[4] = "24H";
static int actual_battery_percent = 70;
static time_t time_since_last_update = 0;



void print_time(char *s, int size_s, time_t time_diff, int mode){
  
  //mode = 0: 
  //    7 s
  // 1:15 m
  //   10 m
  // 1:05 h
  //   10 h
  // 1:03 d
  //   10 d
  // 3000 d
  
  //mode > 0: 
  //    7 s
  //    1 m
  //   10 m
  //    1 h
  //   10 h
  //    1 d
  //   10 d
  // 3000 d
  
  int days    = time_diff / (24*3600);
  int hours   = (time_diff % (24*3600)) / 3600;
  int minutes = (time_diff % 3600) / 60;
  int seconds = (time_diff % 60);


  if (days == 0){
    if (hours == 0){
      if (minutes == 0){
        snprintf(s, size_s, "%d s", seconds);
      } else {
        if ((minutes < 10) && (mode == 0))
          snprintf(s, size_s, "%d:%02d m", minutes, seconds);
        else
          snprintf(s, size_s, "%d m", minutes);
      }
    } else {
      if ((hours < 10)  && (mode == 0))
        snprintf(s, size_s, "%d:%02d h", hours, minutes);
      else
        snprintf(s, size_s, "%d h", hours);
    }
  } else {
    if ((days < 10) && (mode == 0))
      snprintf(s, size_s, "%d:%02d d", days, hours);
    else
      snprintf(s, size_s, "%d d", days);
  }
}




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
  
  key = KEY_WEATHER_UPDATE_INT;
  if (persist_exists(key)) WeatherUpdateInterval = persist_read_int(key);
  
  key = KEY_WEATHER_STRING_1;
  if (persist_exists(key)) persist_read_string(key, weather_string_1, sizeof(weather_string_1));
  key = KEY_WEATHER_STRING_2;
  if (persist_exists(key)) persist_read_string(key, weather_string_2, sizeof(weather_string_2));
  
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
  
  
  
  key = KEY_SET_INVERT_COLOR;
  if (persist_exists(key)) InvertColors = persist_read_int(key);
  
  key = KEY_SET_DEGREE_F;
  if (persist_exists(key)) degree_f = persist_read_int(key);
  
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
  persist_write_int    (KEY_WEATHER_UPDATE_INT, WeatherUpdateInterval);
  persist_write_string (KEY_WEATHER_STRING_1, weather_string_1);
  persist_write_string (KEY_WEATHER_STRING_2, weather_string_2);
  
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
  
  persist_write_int(KEY_SET_INVERT_COLOR, InvertColors);
  persist_write_int(KEY_SET_DISPLAY_SEC, DisplaySeconds);
  persist_write_int(KEY_SET_LIGHT_ON, LightOn);
  persist_write_int(KEY_SET_VIBE_DISC, vibe_on_disconnect);
  persist_write_int(KEY_SET_VIBE_FULL, vibe_on_charged_full);
  persist_write_int(KEY_SET_DEGREE_F, degree_f);
  persist_write_string(KEY_SET_DATE_FORMAT, date_format);
  
}

void DisplayLastUpdated(void) {
  static char last_updated_buffer[10];
  time_t now = time(NULL);
  time_since_last_update = now - phone_last_updated;
  
  //display time since last weather update:
  if (phone_last_updated != 0){
    print_time(last_updated_buffer, sizeof(last_updated_buffer), time_since_last_update, 1);
    text_layer_set_text(weather_layer_4_last_update, last_updated_buffer);
  } else {
    text_layer_set_text(weather_layer_4_last_update, "--:--");
  }
  
  //display battery stats:
  static char battery_buffer_1[20];
  static char battery_buffer_2[10];
  
  if (last_battery_charged_time == 0) last_battery_charged_time = now;
  time_t t_diff_bat = now - last_battery_charged_time;
  if (last_charge_state == 2){
    //if battery if full and plugged, show charge time:
    print_time(battery_buffer_2, sizeof(battery_buffer_2), last_battery_period_time, 0);
  } else if (last_battery_charged_time != 0){
    //if battery if discharging or charging, show the charging/dischraging time:
    print_time(battery_buffer_2, sizeof(battery_buffer_2), t_diff_bat, 0);
  } else {
    strcpy(battery_buffer_2, " ");
  }
  if (last_charge_state == 2){
    snprintf(battery_buffer_1, sizeof(battery_buffer_1), "*%d%%\n%s", actual_battery_percent, battery_buffer_2);
  } else {
    snprintf(battery_buffer_1, sizeof(battery_buffer_1), "%d%%\n%s", actual_battery_percent, battery_buffer_2);
  }
  text_layer_set_text(battery_runtime_layer, battery_buffer_1);
}

void DisplayData(void) {
  static char buffer_1[12];
  static char buffer_9[20];
  
  #ifdef ITERATE_TEMP
    weather_TEMP++;
    if (weather_TEMP>50) weather_TEMP = -50;
  #endif
  
  
  if (degree_f)
    snprintf(buffer_1, sizeof(buffer_1), "%d°F", (int)(weather_TEMP*1.8+32));
  else
    snprintf(buffer_1, sizeof(buffer_1), "%d°C", weather_TEMP);
  text_layer_set_text(weather_layer_1_temp, buffer_1);
  
  #ifdef PBL_COLOR
  if (InvertColors > 1){
    
    if (weather_TEMP >= 40){
      textcolor_weather = GColorRed;  // >= 40°C
    } else if (weather_TEMP >= 30){
      textcolor_weather = GColorOrange; // 30-39°C
    } else if (weather_TEMP >= 26){
      textcolor_weather = GColorChromeYellow;  // 26-29°C
    } else if (weather_TEMP >= 23){
      textcolor_weather = GColorSpringBud;      // 23-25°C
    } else if (weather_TEMP >= 20){
      textcolor_weather = GColorGreen;          // 20-22°C
    } else if (weather_TEMP >= 18){
      textcolor_weather = GColorMediumAquamarine; // 18-19°C
    } else if (weather_TEMP >= 15){
      textcolor_weather = GColorIslamicGreen;   // 15-17°C
    } else if (weather_TEMP >= 10){
      textcolor_weather = GColorJaegerGreen;    // 10-14°C
    } else if (weather_TEMP >= 6){
      textcolor_weather = GColorTiffanyBlue;    // 6-9°C
    } else if (weather_TEMP >= 2){
      textcolor_weather = GColorElectricBlue;           // 2-5°C
    } else if (weather_TEMP >= -1){
      textcolor_weather = GColorCyan;        // -1 - +1°C
    } else if (weather_TEMP >= -10){
      textcolor_weather = GColorVividCerulean;  // -10 - -2°C
    } else if (weather_TEMP >= -20){
      textcolor_weather = GColorPictonBlue;       // -20 - -11°C
    } else if (weather_TEMP >= -30){
      textcolor_weather = GColorBlueMoon;           // -30 - -21°C
    } else {
      textcolor_weather = GColorCobaltBlue;     // < -30°C
    }  
    text_layer_set_text_color(weather_layer_1_temp, textcolor_weather);
  }
  #endif
  
  
  
  
  text_layer_set_text(weather_layer_7_string_1, weather_string_1);
  text_layer_set_text(weather_layer_7_string_2, weather_string_2);
  text_layer_set_text(weather_layer_3_location, location_name);

  
  if (strlen(time_ZONE_NAME) > 0){
    snprintf(buffer_9, sizeof(buffer_9), "%s, %s", hour_mode_str, time_ZONE_NAME);
  } else {
    snprintf(buffer_9, sizeof(buffer_9), "%s, UTC+%d", hour_mode_str, (int)(time_UTC_OFFSET/3600));
  }
  text_layer_set_text(text_TimeZone_layer, buffer_9);
  
  text_layer_set_text(text_sunrise_layer, sun_rise);
  text_layer_set_text(text_sunset_layer, sun_set);
}












// Called once per second of DisplaySeconds otherwise once per minute.
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
      //APP_LOG(APP_LOG_LEVEL_INFO, "modulo = %d (tm_min = %d; update_interval = %d)", current_time->tm_min%WeatherUpdateInterval, current_time->tm_min, WeatherUpdateInterval);
      //if ((current_time->tm_min%WeatherUpdateInterval == 0) || doUpdateWeather) { 
      if ((time_since_last_update >= (WeatherUpdateInterval*60)) || doUpdateWeather) { 
        
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
  #ifdef ITERATE_TEMP
    DisplayData();
  #endif
  DisplayLastUpdated();
  
  
  
} // ---- end handle_second_tick() ----


static void handle_battery(BatteryChargeState charge_state) {
  
  //charge_state_int = 0: Not plugged (discharging)
  //charge_state_int = 1: plugged and charging
  //charge_state_int = 2: plugged and full
  
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
        if (charge_state.is_charging){
          light_enable(0);
          LightIsOn = 0;
        } else {
          light_enable(charge_state.is_plugged);
          if (charge_state.is_plugged) LightIsOn = 1; else LightIsOn = 0;
        }
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
  
  //save last battery percent for displaying in DisplayLastUpdated():
  actual_battery_percent = charge_state.charge_percent;
  
  
  #ifndef PBL_SDK_3 //only on SDK 2.x
    //GRect(41, 21, 38, 11): size of InverterLayer
    layer_set_frame(inverter_layer_get_layer(s_battery_layer_fill), GRect(41, 21, (int)38*actual_battery_percent/100, 11));
    layer_set_hidden(inverter_layer_get_layer(s_battery_layer_fill), false);
  #else
    layer_set_frame(effect_layer_get_layer(s_battery_layer_fill), GRect(41, 21, (int)38*actual_battery_percent/100, 11));
    layer_set_hidden(effect_layer_get_layer(s_battery_layer_fill), false);
  #endif
    
    
  DisplayLastUpdated();
  
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
  text_layer_set_text(connection_layer, connected ? "Bluetooth" : "----");
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

static void layer_update_callback_battery(Layer *layer, GContext* ctx) {
  //clear layer content:
  graphics_context_set_fill_color(ctx, textcolor_background);
  graphics_fill_rect(ctx, GRect(0, 0, 10, 15), 0, GCornerNone);
  //draw:
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, textcolor_seconds);
  graphics_draw_rect(ctx, GRect(0, 0, 43, 15));
}
static void layer_update_callback_battery_fill(Layer *layer, GContext* ctx) {
  
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
      snprintf(location_name, sizeof(location_name), "%s", t->value->cstring);
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
      break;
    case KEY_WEATHER_UPDATE_INT:
      WeatherUpdateInterval = (int)t->value->int32;
      break;
    case KEY_WEATHER_STRING_1:
      snprintf(weather_string_1, sizeof(weather_string_1), "%s", t->value->cstring);
      text_layer_set_text(weather_layer_7_string_1, weather_string_1);
      //APP_LOG(APP_LOG_LEVEL_INFO, "weather_string_1 = %s", weather_string_1);
      break;
    case KEY_WEATHER_STRING_2:
      snprintf(weather_string_2, sizeof(weather_string_2), "%s", t->value->cstring);
      text_layer_set_text(weather_layer_7_string_2, weather_string_2);
      //APP_LOG(APP_LOG_LEVEL_INFO, "weather_string_2 = %s", weather_string_2);
      break;
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
      if ((int32_t)InvertColors != (int)t->value->int32) restart = 1;
      InvertColors = (int)t->value->int32;
      break;
    case KEY_SET_LIGHT_ON:
      LightOn = (int)t->value->int32;
      break;
    case KEY_SET_DISPLAY_SEC:
      DisplaySeconds = (int)t->value->int32;
      layer_mark_dirty(s_image_layer_second_1);
      layer_mark_dirty(s_image_layer_second_2);
      tick_timer_service_unsubscribe();
      if (DisplaySeconds)
        tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
      else
        tick_timer_service_subscribe(MINUTE_UNIT, &handle_second_tick);
      break;
      
    case KEY_SET_VIBE_DISC:
      vibe_on_disconnect = (int)t->value->int32;
      break;
    case KEY_SET_VIBE_FULL:
      vibe_on_charged_full = (int)t->value->int32;
      break;
      
    case KEY_SET_DEGREE_F:
      //if (degree_f != (int)t->value->int32) restart = 1;
      degree_f = (int)t->value->int32;
      doUpdateWeather = true;
      break;
      
    case KEY_SET_DATE_FORMAT:
      snprintf(date_format, sizeof(date_format), "%s", t->value->cstring);
      //APP_LOG(APP_LOG_LEVEL_ERROR, "date_format in watchface = %s", date_format);
      handle_second_tick(tick_time, SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT);
      break;
    
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  SaveData();
  
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
  LightIsOn = 0;
  
  
  Layer *main_window_layer = window_get_root_layer(s_main_window);
  
  initDone = false;
  
  // --- Load Fonts --- 
  pFontMoon = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MOON_PHASES_SUBSET_30));
  
  
  LoadData();
  
  #ifndef PBL_COLOR
    //reset all color schemes on aplite platform
    if (InvertColors > 1) InvertColors = 0;
  #endif
  
  
  // --- Background Image ---
  #ifdef PBL_COLOR
    if (InvertColors == 2){
      background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_COLOR_PROFILE_1);
    } else {
      background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_BW);
    }
  #else
    background_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_BW);
  #endif
  background_layer = bitmap_layer_create(layer_get_frame(main_window_layer));
  bitmap_layer_set_bitmap(background_layer, background_image);
  #ifdef PBL_COLOR
    bitmap_layer_set_compositing_mode(background_layer, GCompOpAssign);
  #else
    if (InvertColors == 1)
      bitmap_layer_set_compositing_mode(background_layer, GCompOpAssignInverted);
    else
      bitmap_layer_set_compositing_mode(background_layer, GCompOpAssign);
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
    if (InvertColors == 2){
      textcolor_background  = GColorFromRGB(0, 0, 0);
      textcolor_sun         = GColorFromRGB(255, 255, 0);   //=GColorYellow //OK
      textcolor_con         = GColorFromRGB(0, 170, 255);   //GColorVividCerulean
      textcolor_bat         = GColorFromRGB(255, 0, 0);     //GColorFromRGB(170, 0, 0);     //GColorDarkCandyAppleRed
      textcolor_date        = GColorFromRGB(0, 170, 170);   //=GColorTiffanyBlue
      textcolor_cal         = GColorFromRGB(0, 170, 170);   //=GColorTiffanyBlue
      textcolor_moon        = GColorFromRGB(255, 255, 255); //OK
      textcolor_weather     = GColorFromRGB(0, 255, 170);   //GColorMediumSpringGreen
      textcolor_location    = GColorFromRGB(255, 170, 0);   //=GColorChromeYellow //OK
      textcolor_last_update = GColorFromRGB(150, 150, 200); //OK
      textcolor_tz          = GColorFromRGB(100, 100, 100); //OK
      textcolor_clock       = GColorFromRGB(0, 255, 0);
      textcolor_seconds     = GColorFromRGB(0, 170, 170);
    } else {
      //if (InvertColors == 1){
      //  textcolor = GColorBlack;
      //  textcolor_background = GColorWhite;
      //} else if (InvertColors == 0){
        textcolor = GColorWhite;
        textcolor_background = GColorBlack;
      //}
      textcolor_sun         = textcolor;
      textcolor_con         = textcolor;
      textcolor_bat         = textcolor;
      textcolor_date        = textcolor;
      textcolor_cal         = textcolor;
      textcolor_moon        = textcolor;
      textcolor_weather     = textcolor;
      textcolor_location    = textcolor;
      textcolor_last_update = textcolor;
      textcolor_tz          = textcolor;
      textcolor_clock       = textcolor;
      textcolor_seconds     = textcolor;
    }
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
  connection_layer = text_layer_create(GRect(47, 152, 50, 34));
  text_layer_set_text_color(connection_layer, textcolor_con);
  text_layer_set_background_color(connection_layer, GColorClear);
  text_layer_set_font(connection_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(connection_layer, GTextAlignmentCenter);
  text_layer_set_text(connection_layer, "----");
  layer_add_child(main_window_layer, text_layer_get_layer(connection_layer));

  // Battery state / runtime:
  battery_runtime_layer = text_layer_create(GRect(40, 15+2, 45, 15+20));
  text_layer_set_text_color(battery_runtime_layer, textcolor_bat);
  text_layer_set_background_color(battery_runtime_layer, GColorClear);
  text_layer_set_font(battery_runtime_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(battery_runtime_layer, GTextAlignmentCenter);
  text_layer_set_text(battery_runtime_layer, "100%\n0:00 d");
  layer_add_child(main_window_layer, text_layer_get_layer(battery_runtime_layer));
  
  
  #ifndef PBL_SDK_3 //only on SDK 2.x
    //fill battery with an InverterLayer
    s_battery_layer_fill = inverter_layer_create(GRect(41, 21, 38, 11));
    layer_set_hidden(inverter_layer_get_layer(s_battery_layer_fill), true);
    layer_add_child(main_window_layer, inverter_layer_get_layer(s_battery_layer_fill));
  #else //else use effect layer on basalt
    s_battery_layer_fill = effect_layer_create(GRect(41, 21, 38, 11));
    //textcolor_bat.argb
    if (InvertColors == 2){
      effect_layer_add_effect(s_battery_layer_fill, effect_invert_color, (void *)0b00110000);
    } else {
      effect_layer_add_effect(s_battery_layer_fill, effect_invert_color, (void *)0b00111111);
    }
    layer_set_hidden(effect_layer_get_layer(s_battery_layer_fill), true);
    layer_add_child(main_window_layer, effect_layer_get_layer(s_battery_layer_fill));
  #endif
  
  
  
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
  
  // Moon phase
  moonLayer_IMG = text_layer_create(GRect(3, 18, 33, 33));
  text_layer_set_text_color(moonLayer_IMG, textcolor_moon);
  text_layer_set_background_color(moonLayer_IMG, GColorClear);
  text_layer_set_font(moonLayer_IMG, pFontMoon);
  text_layer_set_text_alignment(moonLayer_IMG, GTextAlignmentCenter);
  layer_add_child(main_window_layer, text_layer_get_layer(moonLayer_IMG));
  
  // --- Weather Layers: ---
  
  // Create temperature Layer
  weather_layer_1_temp = text_layer_create(GRect(50, 10, 94, 30));
  //weather_layer_1_temp = text_layer_create(GRect(0, 10, 144, 30));
  text_layer_set_background_color(weather_layer_1_temp, GColorClear);
  text_layer_set_text_color(weather_layer_1_temp, textcolor_weather);
  text_layer_set_text_alignment(weather_layer_1_temp, GTextAlignmentRight);
  text_layer_set_text(weather_layer_1_temp, "---");
  text_layer_set_font(weather_layer_1_temp, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD)); //FONT_KEY_BITHAM_30_BLACK
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_1_temp));
  
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
  
  // Create String_1 Layer
  weather_layer_7_string_1 = text_layer_create(GRect(86, 54-15, 144-86-2, 30)); //TODO
  text_layer_set_background_color(weather_layer_7_string_1, GColorClear);
  text_layer_set_text_color(weather_layer_7_string_1, textcolor_weather);
  text_layer_set_text_alignment(weather_layer_7_string_1, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_7_string_1, "---\n---");
  text_layer_set_font(weather_layer_7_string_1, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_7_string_1));
  
  // Create String_2 Layer
  weather_layer_7_string_2 = text_layer_create(GRect(0, 50, 84, 15)); //TODO
  text_layer_set_background_color(weather_layer_7_string_2, GColorClear);
  text_layer_set_text_color(weather_layer_7_string_2, textcolor_weather);
  text_layer_set_text_alignment(weather_layer_7_string_2, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_7_string_2, "--- / ---");
  text_layer_set_font(weather_layer_7_string_2, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_7_string_2));
  
  // Create TimeZone Layer
  text_TimeZone_layer = text_layer_create(GRect(5, 132, 100, 20)); //TODO
  text_layer_set_background_color(text_TimeZone_layer, GColorClear);
  text_layer_set_text_color(text_TimeZone_layer, textcolor_tz);
  text_layer_set_text_alignment(text_TimeZone_layer, GTextAlignmentLeft);
  text_layer_set_text(text_TimeZone_layer, " ");
  text_layer_set_font(text_TimeZone_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(main_window_layer, text_layer_get_layer(text_TimeZone_layer));
    
  // --- END ---
  
  
  
  
  #ifdef PBL_SDK_3 //only on SDK 2.x
    //fill the window with an InverterLayer (must be created as the last element)
    invert_all_layer = inverter_layer_create(GRect(0, 0, 144, 168));
    layer_set_hidden(inverter_layer_get_layer(invert_all_layer), InvertColors != 1);
    layer_add_child(main_window_layer, inverter_layer_get_layer(invert_all_layer));
  #endif
  
  
  DisplayData();
  
  // Avoids a blank screen on watch start.
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  handle_second_tick(tick_time, SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT);
  handle_battery(battery_state_service_peek());
  handle_bluetooth(bluetooth_connection_service_peek());
  
  // --- Register Event Handlers ---
  if (DisplaySeconds)
    tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
  else
    tick_timer_service_subscribe(MINUTE_UNIT, &handle_second_tick);
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
  
  
  #ifndef PBL_SDK_3 //only on SDK 2.x
    inverter_layer_destroy(s_battery_layer_fill);
  #else
    effect_layer_destroy(s_battery_layer_fill);
  #endif
  
  
  text_layer_destroy(text_sunrise_layer);
  text_layer_destroy(text_sunset_layer);  
  text_layer_destroy(connection_layer);
  text_layer_destroy(battery_runtime_layer); 
  
  text_layer_destroy(Date_Layer);
  text_layer_destroy(cwLayer);
  
  text_layer_destroy(moonLayer_IMG);
  fonts_unload_custom_font(pFontMoon);
  
  text_layer_destroy(weather_layer_1_temp);
  text_layer_destroy(weather_layer_3_location);
  text_layer_destroy(weather_layer_4_last_update);
  text_layer_destroy(weather_layer_7_string_1);
  text_layer_destroy(weather_layer_7_string_2);
  text_layer_destroy(text_TimeZone_layer);
  
  // --- END ---
  
  #ifdef PBL_SDK_3
    inverter_layer_destroy(invert_all_layer);
  #endif
  
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
