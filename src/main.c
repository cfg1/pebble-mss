
#include "pebble.h"
#include "seven_segment.h"
#include "config.h"
#include "keys.h"
#include "mooncalc.h"
#include "math.h"
#include "effect_layer.h"

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

static Layer *background_paint_layer;

static char *sys_locale;



static TextLayer *text_sunrise_layer; 
static TextLayer *text_sunset_layer; 
static TextLayer *battery_runtime_layer;
static TextLayer *connection_layer;

static TextLayer *Date_Layer;
static TextLayer *cwLayer; //calender week

static TextLayer *moonLayer_IMG;
GFont pFontMoon     = 0;
GFont pFontClimacons = 0;

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

#ifdef PBL_PLATFORM_APLITE
  static InverterLayer *s_battery_layer_fill; //fill battery with an InverterLayer
#else
  static EffectLayer *s_battery_layer_fill; //fill battery with an InverterLayer by an effect_invert_color (my own effect added to effect_layer type)
#endif



// Weather and other global variables:
static time_t phone_last_updated = 0;
static time_t station_data_last_updated = 0;
static char location_name[32];
static int  location_latitude   = (int)(LATITUDE*1E6); //in 1E6
static int  location_longitude  = (int)(LONGITUDE*1E6); //in 1E6
static int  weather_TEMP        = 0; //in degree C
static int  WeatherIcon         = (int)'I'; //sun
static char weather_string_1[32]; //under actual temp.
static char weather_string_2[32]; //string under moon/bat
static char time_ZONE_NAME[10];
//static char sun_rise[10] = "--:--";
//static char sun_set[10] = "--:--";
static time_t sun_rise_unix_loc = 0;
static time_t sun_set_unix_loc  = 0;

//Colors:
GColor textcolor_clock;
GColor textcolor_seconds;
GColor background_color_clock;
GColor textcolor_sun;
GColor textcolor_con;
GColor textcolor_bat;
uint8_t textcolor_bat_uint8;
GColor bkgrcolor_bat;
uint8_t bkgrcolor_bat_uint8;
GColor textcolor_date;
GColor textcolor_cal;
GColor textcolor_moon; //not used any more. Its done through get_weather_icon_color(0);
GColor textcolor_weather;
GColor textcolor_location;
GColor textcolor_last_update;
GColor textcolor_tz;

GColor background_color_lines;
GColor background_color_date;
GColor background_color_weather;
GColor background_color_moon; //and weather_icon
GColor background_color_location;
GColor background_color_last_update;
GColor background_color_status;



// Settings variables (App Config):

static int ColorProfile = INVERT_COLORS;
static int LightOn = LIGHT_ON;
static int DisplaySeconds = DISPLAY_SECONDS; //=2 means the seconds are only on after shaking
static int DisplaySecondsTimeout = 5; //in seconds
static int vibe_on_disconnect = VIBE_ON_DISC;
static int vibe_on_charged_full = VIBE_ON_FULL;
static int vibe_on_hour         = VIBE_ON_HOUR;
static int degree_f = DEGREE_F;
static char date_format[20] = DATE_FORMAT;
static int WeatherUpdateInterval = WEATHER_UPDATE_INTERVAL_MINUTE;
static int ShowTimeSinceStationData = 0;
static int TimeZoneFormat = 1;
static int AppFirstStart = 1;
static int MoonPhase = 0;



// Runtime variables:

static bool initDone; // e.g. for avoiding "no BT" vibration with initial opening of the watchface
static bool doUpdateWeather;
static int LightIsOn = 0; //saves the state of the background light
static int init_battery_handler = 0;
static char hour_mode_str[4] = "24H";
static int actual_battery_percent = 70;
static time_t time_since_last_update = 0;
static time_t time_since_last_data = 0;
static time_t t_diff_bat = 0;
static int NightMode = 0;
static int WeatherUpdateReceived = 0;

static int SecOnShakingOn = 1;
static int SecondsTimeoutCounter = 0;

static struct tm *tick_time; // must be global so that its content will not be overidden from other stuff.

static int warning_color_last_update = 0;
static int warning_color_location = 0;



static void set_cwLayer_size(void);
static void apply_color_profile(void);


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

void replace_degree(char *s, int size_s){
  int i;
  //APP_LOG(APP_LOG_LEVEL_INFO, "size_s = %d", size_s);
  for (i=1; i<size_s; i++){
    //APP_LOG(APP_LOG_LEVEL_INFO, "i = %d   s[i-1] = %c   s[i] = %c", i, s[i-1], s[i]);
    if ((s[i-1] == '_') && (s[i] == '_')){
      s[i-1] = (char)194;
      s[i]   = (char)176;
    }
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
  
  key = KEY_WEATHER_ICON;
  if (persist_exists(key)) WeatherIcon = persist_read_int(key);
  
  key = KEY_WEATHER_UPDATE_INT;
  if (persist_exists(key)) WeatherUpdateInterval = persist_read_int(key);
  
  key = KEY_WEATHER_STRING_1;
  if (persist_exists(key)) persist_read_string(key, weather_string_1, sizeof(weather_string_1));
  key = KEY_WEATHER_STRING_2;
  if (persist_exists(key)) persist_read_string(key, weather_string_2, sizeof(weather_string_2));
  
  key = KEY_TIME_LAST_UPDATE;
  if (persist_exists(key)) phone_last_updated = (time_t)(persist_read_int(key));
  
  key = KEY_WEATHER_DATA_TIME;
  if (persist_exists(key)) station_data_last_updated = (time_t)(persist_read_int(key));
  
  key = KEY_TIME_ZONE_NAME;
  if (persist_exists(key)) persist_read_string(key, time_ZONE_NAME, sizeof(time_ZONE_NAME));
  
  key = KEY_SET_MOON_PHASE;
  if (persist_exists(key)) MoonPhase = persist_read_int(key);
  
  
  
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
  
  key = KEY_SUN_RISE_UNIX;
  if (persist_exists(key)) sun_rise_unix_loc = (time_t)(persist_read_int(key));
  
  key = KEY_SUN_SET_UNIX;
  if (persist_exists(key)) sun_set_unix_loc = (time_t)(persist_read_int(key));
  
  
  
  key = KEY_SET_INVERT_COLOR;
  if (persist_exists(key)) ColorProfile = persist_read_int(key);
  
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
  
  key = KEY_SET_VIBE_HOUR;
  if (persist_exists(key)) vibe_on_hour = persist_read_int(key);
  
  key = KEY_SET_TZ_FORMAT;
  if (persist_exists(key)) TimeZoneFormat = persist_read_int(key);
  
  key = KEY_SET_UPDATE_TIME;
  if (persist_exists(key)) ShowTimeSinceStationData = persist_read_int(key);
  
  key = KEY_SET_DATE_FORMAT;
  if (persist_exists(key)) persist_read_string(key, date_format, sizeof(date_format));
  
  key = KEY_WARN_LOCATION;
  if (persist_exists(key)) warning_color_location = persist_read_int(key);
  
  key = KEY_DETECT_FIRST_START;
  if (persist_exists(key)) AppFirstStart = persist_read_int(key); else AppFirstStart = 1;
}

void SaveData(void) {
  persist_write_string (KEY_LOCATION_NAME, location_name);
  persist_write_int    (KEY_LOCATION_LAT,  location_latitude);
  persist_write_int    (KEY_LOCATION_LON,  location_longitude);
  
  persist_write_int    (KEY_WEATHER_TEMP,  weather_TEMP);
  persist_write_int    (KEY_WEATHER_ICON,  WeatherIcon);
  persist_write_int    (KEY_WEATHER_UPDATE_INT, WeatherUpdateInterval);
  persist_write_string (KEY_WEATHER_STRING_1, weather_string_1);
  persist_write_string (KEY_WEATHER_STRING_2, weather_string_2);
  
  persist_write_int    (KEY_TIME_LAST_UPDATE,  (int)(phone_last_updated));
  persist_write_int    (KEY_WEATHER_DATA_TIME, (int)station_data_last_updated);
  persist_write_string (KEY_TIME_ZONE_NAME, time_ZONE_NAME);
  persist_write_int    (KEY_SET_MOON_PHASE, MoonPhase);
  
  
  
  persist_write_int    (KEY_BTY_LAST_PERCENT,  last_battery_percent);
  BatteryChargeState actual_charge = battery_state_service_peek();
  persist_write_int    (KEY_BTY_PERCENT_EXIT,  actual_charge.charge_percent);
  persist_write_int    (KEY_BTY_LAST_CHARGED,  (int)(last_battery_charged_time));
  
  persist_write_int    (KEY_BTY_LAST_STATE,  last_charge_state);
  persist_write_int    (KEY_BTY_LAST_PERIOD,  (int)(last_battery_period_time));
  
  persist_write_int    (KEY_SUN_RISE_UNIX,  (int)sun_rise_unix_loc);
  persist_write_int    (KEY_SUN_SET_UNIX,  (int)sun_set_unix_loc);
  
  persist_write_int(KEY_SET_INVERT_COLOR, ColorProfile);
  persist_write_int(KEY_SET_DISPLAY_SEC, DisplaySeconds);
  persist_write_int(KEY_SET_LIGHT_ON, LightOn);
  persist_write_int(KEY_SET_VIBE_DISC, vibe_on_disconnect);
  persist_write_int(KEY_SET_VIBE_FULL, vibe_on_charged_full);
  persist_write_int(KEY_SET_VIBE_HOUR, vibe_on_hour);
  persist_write_int(KEY_SET_TZ_FORMAT, TimeZoneFormat);
  persist_write_string(KEY_SET_DATE_FORMAT, date_format);
  persist_write_int(KEY_SET_UPDATE_TIME, ShowTimeSinceStationData);
    
  persist_write_int(KEY_WARN_LOCATION, warning_color_location);
  
  persist_write_int(KEY_SET_DEGREE_F, degree_f);
  
}

void DisplayLastUpdated(void) {
  static char last_updated_buffer[10];
  time_t now = time(NULL); // now is in UTC on SDK >= 3
  
  int warning_color_last_update_old = warning_color_last_update;
  
  if (ShowTimeSinceStationData){
    time_since_last_data = now - station_data_last_updated;
    //APP_LOG(APP_LOG_LEVEL_INFO, "X: \n(1) = %d\nnow = %d\n(3) = %d\ntime_UTC_OFFSET = %d", 
    //  (int)time_since_last_update, (int)now, (int)station_data_last_updated, (int)time_UTC_OFFSET);
  } else {
    //APP_LOG(APP_LOG_LEVEL_INFO, "THIS SHOULD NOT BE HERE !!!");
  }
  time_since_last_update = now - phone_last_updated;
  
  //display time since last weather update:
  if (time_since_last_update < 10*365*24*3600){ // < 10 years
    if (ShowTimeSinceStationData){
      if (time_since_last_data < 10*365*24*3600){ // < 10 years
        print_time(last_updated_buffer, sizeof(last_updated_buffer), time_since_last_data, 1);
      } else {
        snprintf(last_updated_buffer, sizeof(last_updated_buffer), "--:--");
      }
    } else {
      print_time(last_updated_buffer, sizeof(last_updated_buffer), time_since_last_update, 1);
    }
    //print_time(last_updated_buffer, sizeof(last_updated_buffer), time_since_last_update, 1);
    text_layer_set_text(weather_layer_4_last_update, last_updated_buffer);
    //#ifdef PBL_COLOR
      if (ShowTimeSinceStationData){
        if (time_since_last_data >= 2*3600){ // >= 2h
          warning_color_last_update = 1;
        } else warning_color_last_update = 0;
      } else {
        if (time_since_last_update > (WeatherUpdateInterval*60)){
          warning_color_last_update = 1;
        } else warning_color_last_update = 0;
      }
    //#endif
  } else {
    text_layer_set_text(weather_layer_4_last_update, "--:--");
    //#ifdef PBL_COLOR
      warning_color_last_update = 1;
    //#endif
  }
  
  if (warning_color_last_update != warning_color_last_update_old){
    apply_color_profile();
  }
  
  //display battery stats:
  static char battery_buffer_1[20];
  static char battery_buffer_2[10];
  
  if (last_battery_charged_time == 0) last_battery_charged_time = now;
  t_diff_bat = now - last_battery_charged_time;
  if (last_charge_state == 2){
    //if battery if full and plugged, show charge time:
    print_time(battery_buffer_2, sizeof(battery_buffer_2), last_battery_period_time, 0);
  } else if (last_battery_charged_time != 0){
    //if battery if discharging or charging, show the charging/dischraging time:
    print_time(battery_buffer_2, sizeof(battery_buffer_2), t_diff_bat, 0);
  } else {
    strcpy(battery_buffer_2, " ");
  }
  if (last_charge_state == 1){
    snprintf(battery_buffer_1, sizeof(battery_buffer_1), "*%d%%\n%s", actual_battery_percent, battery_buffer_2);
  } else {
    snprintf(battery_buffer_1, sizeof(battery_buffer_1), "%d%%\n%s", actual_battery_percent, battery_buffer_2);
  }
  text_layer_set_text(battery_runtime_layer, battery_buffer_1);
}



void DisplayData(void) {
  static char buffer_1[12];
  
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
  if (ColorProfile > 1){
    GColor textcolor_weather_int;
    if (weather_TEMP >= 40){
      textcolor_weather_int = GColorRed;  // >= 40°C
    } else if (weather_TEMP >= 28){
      textcolor_weather_int = GColorOrange; // 28-39°C
    } else if (weather_TEMP >= 26){
      textcolor_weather_int = GColorChromeYellow;  // 26-27°C
    } else if (weather_TEMP >= 23){
      textcolor_weather_int = GColorChromeYellow;      // 23-25°C
    } else if (weather_TEMP >= 20){
      textcolor_weather_int = GColorGreen;          // 20-22°C
    } else if (weather_TEMP >= 18){
      textcolor_weather_int = GColorMalachite; // 18-19°C
    } else if (weather_TEMP >= 15){
      textcolor_weather_int = GColorIslamicGreen;   // 15-17°C
    } else if (weather_TEMP >= 10){
      textcolor_weather_int = GColorJaegerGreen;    // 10-14°C
    } else if (weather_TEMP >= 6){
      textcolor_weather_int = GColorDarkGray;    // 6-9°C
    } else if (weather_TEMP >= 2){
      textcolor_weather_int = GColorElectricBlue;           // 2-5°C
    } else if (weather_TEMP >= -1){
      textcolor_weather_int = GColorCyan;        // -1 - +1°C
    } else if (weather_TEMP >= -10){
      textcolor_weather_int = GColorVividCerulean;  // -10 - -2°C
    } else if (weather_TEMP >= -20){
      textcolor_weather_int = GColorPictonBlue;       // -20 - -11°C
    } else if (weather_TEMP >= -30){
      textcolor_weather_int = GColorBlueMoon;           // -30 - -21°C
    } else {
      textcolor_weather_int = GColorCobaltBlue;     // < -30°C
    }  
    text_layer_set_text_color(weather_layer_1_temp, textcolor_weather_int);
  }
  #endif
  
  
  
  
  text_layer_set_text(weather_layer_7_string_1, weather_string_1);
  text_layer_set_text(weather_layer_7_string_2, weather_string_2);
  text_layer_set_text(weather_layer_3_location, location_name);
  
  
  struct tm* sun_time = localtime(&sun_rise_unix_loc);
  static char sun_rise_text[10];
  static char sun_set_text[10];
  if(clock_is_24h_style()) {
    strftime(sun_rise_text, sizeof(sun_rise_text), "%H:%M", sun_time);
  } else {
    strftime(sun_rise_text, sizeof(sun_rise_text), "%I:%M", sun_time);
  }
  text_layer_set_text(text_sunrise_layer, sun_rise_text);
  sun_time = localtime(&sun_set_unix_loc);
  if(clock_is_24h_style()) {
    strftime(sun_set_text, sizeof(sun_set_text), "%H:%M", sun_time);
  } else {
    strftime(sun_set_text, sizeof(sun_set_text), "%I:%M", sun_time);
  }
  text_layer_set_text(text_sunset_layer, sun_set_text);
  
  //reset the localtime internal variables to the current time, so that the tick_handler is getting the right ones.
  //The handle_second_tick() might be executed after running localtime!
  time_t now = time(NULL);
  tick_time = localtime(&now);
  
  DisplayLastUpdated();
  
}






#ifdef PBL_COLOR
static GColor get_weather_icon_color(int nr){
  if (ColorProfile == 0) return GColorWhite;
  if (ColorProfile == 1) return GColorBlack; //if (gcolor_equal(background_color_moon, GColorWhite)) return GColorBlack;
  //if (ColorProfile == 1) return GColorWhite;
  if (nr < 33) return GColorWhite;
  if (nr > 106) return GColorWhite;
  switch (nr){
    case 33: return GColorFromHEX(0xFFFFFF); //GColorVividCerulean; //Cloud
    case 34: return GColorIcterine;
    case 35: return GColorPictonBlue;
    case 36: return GColorFromHEX(0x55FFFF); //GColorBabyBlueEyes; //Rain
    case 37: return GColorChromeYellow;
    case 38: return GColorBlueMoon;
    case 39: return GColorFromHEX(0x55FFFF);
    case 40: return GColorChromeYellow;
    case 41: return GColorBlueMoon;
    case 42: return GColorPictonBlue;
    case 43: return GColorOrange;
    case 44: return GColorBlueMoon;
    case 45: return GColorCadetBlue;
    case 46: return GColorRajah;
    case 47: return GColorBlueMoon;
    case 48: return GColorWhite;
    case 49: return GColorPastelYellow;
    case 50: return GColorFromHEX(0x55AAAA); //GColorCadetBlue
    case 51: return GColorSunsetOrange; //hail (Hagel)
    case 52: return GColorWhite;
    case 53: return GColorWhite;
    case 54: return GColorWhite;
    case 55: return GColorWhite;
    case 56: return GColorWhite;
    case 57: return GColorCeleste; //snow
    case 58: return GColorYellow;
    case 59: return GColorCyan;
    case 60: return GColorLightGray; //fog
    case 61: return GColorPastelYellow;
    case 62: return GColorCadetBlue;
    case 63: return GColorLightGray; //haze (Dunst)
    case 64: return GColorChromeYellow;
    case 65: return GColorCadetBlue;
    case 66: return GColorCeleste; //wind
    case 67: return GColorWhite;
    case 68: return GColorWhite;
    case 69: return GColorWhite;
    case 70: return GColorRed;
    case 71: return GColorOrange;
    case 72: return GColorWhite;
    case 73: return GColorYellow;
    case 74: return GColorOrange;
    case 75: return GColorOrange;
    case 76: return GColorWhite;
    case 77: return GColorWhite;
    case 78: return GColorWhite;
    case 79: return GColorWhite;
    case 80: return GColorWhite;
    case 81: return GColorWhite;
    case 82: return GColorWhite;
    case 83: return GColorWhite;
    case 84: return GColorWhite;
    case 85: return GColorWhite;
    case 86: return GColorWhite;
    case 87: return GColorWhite;
    case 88: return GColorOrange; //tornado
    case 89: return GColorWhite;
    case 90: return GColorBabyBlueEyes; // temp_low
    case 91: return GColorWhite;
    case 92: return GColorWhite;
    case 93: return GColorRed; // temp_high
    case 94: return GColorWhite;
    case 95: return GColorWhite;
    case 96: return GColorWhite;
    case 97: return GColorWhite;
    case 98: return GColorWhite;
    case 99: return GColorWhite;
    case 100: return GColorWhite;
    case 101: return GColorWhite;
    case 102: return GColorWhite;
    case 103: return GColorWhite;
    case 104: return GColorWhite;
    case 105: return GColorWhite;
    case 106: return GColorWhite;
  }
  return GColorBlue;
}

static GColor get_weather_icon_bkgr_color(int nr){
  if (ColorProfile == 0) return GColorBlack;
  //if (gcolor_equal(background_color_moon, GColorWhite)) return GColorWhite;
  if (ColorProfile == 1) return GColorWhite;
  if (nr < 33) return GColorBlack;
  if (nr > 106) return GColorBlack;
  switch (nr){
    case 33: return GColorVividCerulean; //Cloud
    case 34: return GColorVividCerulean; //Cloud and Sun
    case 35: return GColorFromHEX(0x000055); //GColorPictonBlue;
    case 36: return GColorFromHEX(0x555555); //GColorBabyBlueEyes;
    case 37: return GColorFromHEX(0x0055AA);
    case 38: return GColorFromHEX(0x000055);
    case 39: return GColorFromHEX(0x555555);
    case 40: return GColorFromHEX(0x0055AA);
    case 41: return GColorFromHEX(0x000055);
    case 42: return GColorFromHEX(0x555555);
    case 43: return GColorFromHEX(0x0055AA);
    case 44: return GColorFromHEX(0x000055);
    case 45: return GColorFromHEX(0x555555);
    case 46: return GColorFromHEX(0x0055AA);
    case 47: return GColorFromHEX(0x000055);
    case 48: return GColorElectricBlue;
    case 49: return GColorFromHEX(0x00AAFF);
    case 50: return GColorFromHEX(0x000055);
    case 51: return GColorFromHEX(0x0055AA); //hail (Hagel)
    case 52: return GColorBlack;
    case 53: return GColorBlack;
    case 54: return GColorBlack;
    case 55: return GColorBlack;
    case 56: return GColorBlack;
    case 57: return GColorFromHEX(0x555555); //snow
    case 58: return GColorFromHEX(0x0055AA);
    case 59: return GColorFromHEX(0x000055);
    case 60: return GColorWhite; //fog
    case 61: return GColorFromHEX(0x0055AA);
    case 62: return GColorWhite;
    case 63: return GColorWhite; //haze (Dunst)
    case 64: return GColorWhite;
    case 65: return GColorWhite;
    case 66: return GColorFromHEX(0x0055AA); //wind
    case 67: return GColorBlack;
    case 68: return GColorBlack;
    case 69: return GColorBlack;
    case 70: return GColorFromHEX(0x555555);
    case 71: return GColorFromHEX(0x555555);
    case 72: return GColorBlack;
    case 73: return GColorFromHEX(0x00AAFF);
    case 74: return GColorFromHEX(0x0055AA);
    case 75: return GColorFromHEX(0x0055AA);
    case 76: return GColorBlack;
    case 77: return GColorBlack;
    case 78: return GColorBlack;
    case 79: return GColorBlack;
    case 80: return GColorBlack;
    case 81: return GColorBlack;
    case 82: return GColorBlack;
    case 83: return GColorBlack;
    case 84: return GColorBlack;
    case 85: return GColorBlack;
    case 86: return GColorBlack;
    case 87: return GColorBlack;
    case 88: return GColorFromHEX(0x555555); //tornado
    case 89: return GColorBlack;
    case 90: return GColorFromHEX(0x000055); // temp_low
    case 91: return GColorBlack;
    case 92: return GColorBlack;
    case 93: return GColorFromHEX(0xFFFF00); // temp_high
    case 94: return GColorBlack;
    case 95: return GColorBlack;
    case 96: return GColorBlack;
    case 97: return GColorBlack;
    case 98: return GColorBlack;
    case 99: return GColorBlack;
    case 100: return GColorBlack;
    case 101: return GColorBlack;
    case 102: return GColorBlack;
    case 103: return GColorBlack;
    case 104: return GColorBlack;
    case 105: return GColorBlack;
    case 106: return GColorBlack;
  }
  return GColorFromHEX(0x000055);
}
#endif





// Called once per second of DisplaySeconds otherwise once per minute.
static void handle_second_tick(struct tm* current_time, TimeUnits units_changed) {
  
  static struct tm current_time_copy;
  current_time_copy = *current_time;
  
    
    
    
  //units_changed = SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT;
  
  
  #ifdef PBL_COLOR
    GColor background_color_moon_old = background_color_moon;
  #endif
  
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
  
  #ifdef GET_TIME_FROM_STRING
    char time_String[10];
    if(clock_is_24h_style() == true) {
      strftime(time_String, sizeof(time_String), "%H:%M:%S", &current_time_copy);
      snprintf(hour_mode_str, sizeof(hour_mode_str), "%s", "24H");
    } else {
      strftime(time_String, sizeof(time_String), "%I:%M:%S", &current_time_copy);
      strftime(hour_mode_str, sizeof(hour_mode_str), "%p", &current_time_copy);
    }
  #endif
  
  if (units_changed & HOUR_UNIT){
    #ifdef GET_TIME_FROM_STRING
      digit_h_1 = ((int)time_String[0]-48);
      digit_h_2 = ((int)time_String[1]-48);
    #else
    if(clock_is_24h_style() == true) {
      digit_h_1 = current_time_copy.tm_hour/10;
      digit_h_2 = current_time_copy.tm_hour%10;
      snprintf(hour_mode_str, sizeof(hour_mode_str), "%s", "24H");
    } else {
      int hour12 = current_time_copy.tm_hour;
      if ((hour12 > 11)){
        snprintf(hour_mode_str, sizeof(hour_mode_str), "%s", "PM");
        hour12-=12;
      } else snprintf(hour_mode_str, sizeof(hour_mode_str), "%s", "AM");
      if (hour12 == 0) hour12 = 12;
      digit_h_1 = hour12/10;
      digit_h_2 = hour12%10;
    }
    #endif
  }
  
  if (units_changed & MINUTE_UNIT){
    #ifdef GET_TIME_FROM_STRING
      digit_m_1 = ((int)time_String[3]-48);
      digit_m_2 = ((int)time_String[4]-48);
    #else
      digit_m_1 = current_time_copy.tm_min/10;
      digit_m_2 = current_time_copy.tm_min%10;
    #endif
  }
  
  #ifdef GET_TIME_FROM_STRING
    digit_s_1 = ((int)time_String[6]-48);
    digit_s_2 = ((int)time_String[7]-48);
  #else
    digit_s_1 = current_time_copy.tm_sec/10;
    digit_s_2 = current_time_copy.tm_sec%10;
  #endif
  
  
  static int digit_s_1_old = 10;
  //static int digit_s_2_old = 0;
  static int vibe_hour_old = -1;
  if (units_changed & HOUR_UNIT){
    layer_mark_dirty(s_image_layer_hour_1);
    layer_mark_dirty(s_image_layer_hour_2);
    
    if (vibe_hour_old < 0) vibe_hour_old = current_time_copy.tm_hour;
    if (vibe_on_hour && (vibe_hour_old != current_time_copy.tm_hour)){
      // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
      static const uint32_t segments[] = { 150, 70, 150 };
      VibePattern pat = {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
      };
      vibes_enqueue_custom_pattern(pat);
      
      vibe_hour_old = current_time_copy.tm_hour;
    }
  }
  if (units_changed & MINUTE_UNIT){
    layer_mark_dirty(s_image_layer_minute_1);
    layer_mark_dirty(s_image_layer_minute_2);
  }
  if (DisplaySeconds){
    if (digit_s_1_old != digit_s_1){ //should save energy
      layer_mark_dirty(s_image_layer_second_1);
      digit_s_1_old = digit_s_1;
    }
    layer_mark_dirty(s_image_layer_second_2);
  }
  
  
  
  static char date_buffer[20];
  if (units_changed & HOUR_UNIT) {
    strftime(date_buffer, sizeof(date_buffer), /*"%a, %d.%m."*/date_format, &current_time_copy);
    text_layer_set_text(Date_Layer, date_buffer);
  }
  
  
  //static char moon_buffer[7];
  static char moon[] = "m";
  static char weather_icon[] = "I";
  #ifdef PBL_COLOR
    GColor weather_icon_color = GColorWhite;
  #endif
  static int NightModeOld = -1;
  
  //calculate NightMode:
  if (units_changed & MINUTE_UNIT){
  //#ifdef PBL_SDK_3
    
    //sun_set_unix_loc = 1439406657+3600*2;
    //APP_LOG(APP_LOG_LEVEL_INFO, "sun_rise_unix_loc = %d", (int)sun_rise_unix_loc);
    //APP_LOG(APP_LOG_LEVEL_INFO, "sun_set_unix_loc = %d", (int)sun_set_unix_loc);
    struct tm* sun_rise_time = localtime(&sun_rise_unix_loc);
    struct tm  sun_rise_copy = *sun_rise_time;
    struct tm* sun_set_time  = localtime(&sun_set_unix_loc);
    struct tm  sun_set_copy  = *sun_set_time;
    NightMode = 0;
    /*
    APP_LOG(APP_LOG_LEVEL_INFO, "sun_rise_time->tm_hour = %d", sun_rise_copy.tm_hour);
    APP_LOG(APP_LOG_LEVEL_INFO, "sun_rise_time->tm_min = %d", sun_rise_copy.tm_min);
    APP_LOG(APP_LOG_LEVEL_INFO, "sun_set_time->tm_hour = %d", sun_set_copy.tm_hour);
    APP_LOG(APP_LOG_LEVEL_INFO, "sun_set_time->tm_min = %d", sun_set_copy.tm_min);
    APP_LOG(APP_LOG_LEVEL_INFO, "current_time_copy.tm_hour = %d", current_time_copy.tm_hour);
    APP_LOG(APP_LOG_LEVEL_INFO, "current_time_copy.tm_min = %d", current_time_copy.tm_min);
    */
    if (sun_rise_copy.tm_hour > current_time_copy.tm_hour) NightMode = 1;
    if ((sun_rise_copy.tm_hour == current_time_copy.tm_hour) && (sun_rise_copy.tm_min > current_time_copy.tm_min)) NightMode = 1;
    if (sun_set_copy.tm_hour < current_time_copy.tm_hour) NightMode = 1;
    if ((sun_set_copy.tm_hour == current_time_copy.tm_hour) && (sun_set_copy.tm_min <= current_time_copy.tm_min)) NightMode = 1;
  /*
    //if ((current_time_copy.tm_hour > 6) && (current_time_copy.tm_hour < 20) ) NightMode = 0; else NightMode = 1;
  #else
    if ((current_time_copy.tm_hour > 6) && (current_time_copy.tm_hour < 20) ) NightMode = 0; else NightMode = 1;
  #endif
  */
  }
  if (MoonPhase == 1) NightMode = 1; //moon is set to allways displayed
  if (MoonPhase == 2) NightMode = 0; //moon is set to never displayed, allways display weather icon
    
  //APP_LOG(APP_LOG_LEVEL_INFO, "NightMode = %d", NightMode);
  
  #ifndef ITERATE_TEMP
    if ((WeatherUpdateReceived) || (units_changed & HOUR_UNIT) || (NightMode != NightModeOld)){
      WeatherUpdateReceived = 0;
      
      if (!NightMode){
        //static int wi_counter = 33;
        text_layer_set_font(moonLayer_IMG, pFontClimacons);
        layer_set_frame(text_layer_get_layer(moonLayer_IMG), GRect(51+X_OFFSET, 15+Y_OFFSET, 33, 33));
        
        //wi_counter++; if (wi_counter>106) wi_counter = 33;
        //wi_counter = WeatherIcon;
        weather_icon[0] = (unsigned char)WeatherIcon;//wi_counter;
        text_layer_set_text(moonLayer_IMG, weather_icon);
        
        #ifdef PBL_COLOR
          weather_icon_color = get_weather_icon_color((int)weather_icon[0]);
          background_color_moon = get_weather_icon_bkgr_color((int)weather_icon[0]);
          text_layer_set_text_color(moonLayer_IMG, weather_icon_color);
        #endif
      }
    }
  #else
    text_layer_set_font(moonLayer_IMG, pFontClimacons);
    layer_set_frame(text_layer_get_layer(moonLayer_IMG), GRect(51+X_OFFSET, 15+Y_OFFSET, 33, 33));
  
    static int wi_counter = 33;
    wi_counter++; if (wi_counter>106) wi_counter = 33;
    weather_icon[0] = (unsigned char)wi_counter;
    text_layer_set_text(moonLayer_IMG, weather_icon);
    
    #ifdef PBL_COLOR
      weather_icon_color = get_weather_icon_color((int)weather_icon[0]);
      background_color_moon = get_weather_icon_bkgr_color((int)weather_icon[0]);
      text_layer_set_text_color(moonLayer_IMG, weather_icon_color);
    #endif
  #endif
  
  if (NightMode) if ((units_changed & HOUR_UNIT) || (NightMode != NightModeOld)) {
    // -------------------- Moon_phase
		//static int moonphase_number = 0;
    //moonphase_number += 1;
    int moonphase_number = 0;
    moonphase_number = calc_moonphase_number(location_latitude);
    moon[0] = (unsigned char)(moonphase_char_number(moonphase_number));
    
    
    text_layer_set_font(moonLayer_IMG, pFontMoon);
    layer_set_frame(text_layer_get_layer(moonLayer_IMG), GRect(51+X_OFFSET, 21+Y_OFFSET, 33, 33));
    text_layer_set_text(moonLayer_IMG, moon);
    #ifdef PBL_COLOR
      weather_icon_color = get_weather_icon_color(0);
      background_color_moon = get_weather_icon_bkgr_color(0);
      text_layer_set_text_color(moonLayer_IMG, weather_icon_color);
    #endif
    
    
    /*
    snprintf(moon_buffer, sizeof(moon_buffer), "(%d)", moonphase_number);
    //text_layer_set_text(moonLayer, moon_buffer);
    text_layer_set_text(moonLayer, " ");
		//text_layer_set_text(moonLayer, MOONPHASE_NAME_LANGUAGE[moonphase_number]); 
    */
		// -------------------- Moon_phase	  
  }
  NightModeOld = NightMode;
  #ifdef PBL_COLOR
    if (!gcolor_equal(background_color_moon, background_color_moon_old)){
      layer_mark_dirty(background_paint_layer);
    }
  #endif
  
  
	if (units_changed & HOUR_UNIT){
		// -------------------- Calendar week  
	  static char cw_text[] = "XX00";
    if (strcmp("fr_FR", sys_locale) == 0) {
		  strftime(cw_text, sizeof(cw_text), TRANSLATION_CW_FR, &current_time_copy);
    } else if (strcmp("de_DE", sys_locale) == 0) {
      strftime(cw_text, sizeof(cw_text), TRANSLATION_CW_DE, &current_time_copy);
    } else { //default
      strftime(cw_text, sizeof(cw_text), TRANSLATION_CW_EN, &current_time_copy);
    }
		text_layer_set_text(cwLayer, cw_text); 
		// ------------------- Calendar week 
  }
  
  
  static char buffer_9[20];
  if (units_changed & MINUTE_UNIT){
    if (TimeZoneFormat == 0){
      time_t UTC_TIME_UNIX = time(NULL);
      struct tm* utc_time;
      utc_time = gmtime(&UTC_TIME_UNIX);
      if(clock_is_24h_style() == true) {
        strftime(buffer_9, sizeof(buffer_9), "%R UTC", utc_time);
      } else {
        strftime(buffer_9, sizeof(buffer_9), "%I:%M UTC", utc_time);
      }
    } else if (TimeZoneFormat == 1){
      snprintf(buffer_9, sizeof(buffer_9), "%s", time_ZONE_NAME);
    } else if (TimeZoneFormat == 2){
      snprintf(buffer_9, sizeof(buffer_9), "%s, %s", hour_mode_str, time_ZONE_NAME);
    }
    text_layer_set_text(text_TimeZone_layer, buffer_9);
  }
  
  
  
  //Request weather data:
  if (initDone || doUpdateWeather){
    if ((units_changed & MINUTE_UNIT) || doUpdateWeather) {//MINUTE_UNIT, SECOND_UNIT
      //APP_LOG(APP_LOG_LEVEL_INFO, "modulo = %d (tm_min = %d; update_interval = %d)", current_time->tm_min%WeatherUpdateInterval, current_time->tm_min, WeatherUpdateInterval);
      //if ((current_time->tm_min%WeatherUpdateInterval == 0) || doUpdateWeather) { 
      //APP_LOG(APP_LOG_LEVEL_INFO, "tslu = %d", (int)time_since_last_update);
      //APP_LOG(APP_LOG_LEVEL_INFO, "wuis = %d", (int)(WeatherUpdateInterval));
      if (((int)time_since_last_update >= (WeatherUpdateInterval*60-60)) || doUpdateWeather) { 
        
        doUpdateWeather = false;
        
        // Begin dictionary
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        
        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);
        
        // Send the message!
        app_message_outbox_send();
        
        //text_layer_set_text(weather_layer_2, "***");
        //APP_LOG(APP_LOG_LEVEL_INFO, "Weather Update requested");
        
        /*
        #ifdef PBL_COLOR
          textcolor_last_update = GColorBlue;
          text_layer_set_text_color(weather_layer_4_last_update, textcolor_last_update);
        #endif
        */
      }
    }
  }
  
  
  
  /*if (units_changed & MINUTE_UNIT)*/ //DisplayLastUpdated();
  #ifdef ITERATE_TEMP
    DisplayData();
  #endif
  if ((time_since_last_update <= 60) || (t_diff_bat <= 600) || (units_changed & MINUTE_UNIT)){ 
    DisplayLastUpdated(); 
  }
  
  if (DisplaySeconds >= 2){
    if (SecOnShakingOn){
      SecondsTimeoutCounter++;
      //APP_LOG(APP_LOG_LEVEL_INFO, "SecondsTimeoutCounter = %d", SecondsTimeoutCounter);
      switch (DisplaySeconds){
        case 2: DisplaySecondsTimeout = 5; break;
        case 3: DisplaySecondsTimeout = 15; break;
        case 4: DisplaySecondsTimeout = 30; break;
        case 5: DisplaySecondsTimeout = 60; break;
        default: DisplaySecondsTimeout = 15; break;
      }
      if (SecondsTimeoutCounter > DisplaySecondsTimeout+1){
        SecOnShakingOn = 0;
        layer_mark_dirty(s_image_layer_second_1);
        layer_mark_dirty(s_image_layer_second_2);
        tick_timer_service_unsubscribe();
        tick_timer_service_subscribe(MINUTE_UNIT, &handle_second_tick);
        //APP_LOG(APP_LOG_LEVEL_INFO, "SecOnShakingOn = 0;");
      }
    }
  }
  
  
  //reset the localtime internal variables to the current time, so that the tick_handler is getting the right ones.
  //The handle_second_tick() might be executed after running localtime!
  time_t now = time(NULL);
  tick_time = localtime(&now);
  
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
      static const uint32_t const segments[] = { 500 };
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
  
  
  #ifdef PBL_PLATFORM_APLITE //only on SDK 2.x
    //GRect(41, 21, 38, 11): size of InverterLayer
    layer_set_frame(inverter_layer_get_layer(s_battery_layer_fill), GRect(3, 21, (int)38*actual_battery_percent/100, 11));
    layer_set_hidden(inverter_layer_get_layer(s_battery_layer_fill), false);
    if (actual_battery_percent <= 20){
      if (!ColorProfile){
        textcolor_bat = GColorBlack;
        bkgrcolor_bat = GColorWhite;
      } else {
        textcolor_bat = GColorWhite;
        bkgrcolor_bat = GColorBlack;
      }
    } else {
      if (ColorProfile){
        textcolor_bat = GColorBlack;
        bkgrcolor_bat = GColorWhite;
      } else {
        textcolor_bat = GColorWhite;
        bkgrcolor_bat = GColorBlack;
      }
    }
  #else
    layer_set_frame(effect_layer_get_layer(s_battery_layer_fill), GRect(3+X_OFFSET, 21+Y_OFFSET, (int)38*actual_battery_percent/100, 11));
    //layer_set_frame(effect_layer_get_layer(s_battery_layer_fill), GRect(0, 0, 180, 180));
    layer_set_hidden(effect_layer_get_layer(s_battery_layer_fill), false);
    
    uint8_t variable_color = 0;
    if (actual_battery_percent > 80){
      variable_color = 0b11000100; // 90 % - 100 %
    } else if (actual_battery_percent > 40){
      variable_color = 0b11000100; // 50 % -  80 % 
    } else if (actual_battery_percent > 30){
      variable_color = 0b11000100; // 40 %          before setting 40 % to the color of 50-80%: light orange (GColorChromeYellow) 0b11111000
    } else if (actual_battery_percent > 20){
      variable_color = 0b11110100; // 30 %          dark orange (GColorOrange)
    } else {
      variable_color = 0b11110000; //  0 % -  20 %  red (GColorRed)
    }
  
  
    if (ColorProfile == 0) {
      textcolor_bat_uint8 = 0b11111111; //white
      bkgrcolor_bat_uint8 = 0b11000000; //black
    } else if (ColorProfile == 1) {
      textcolor_bat_uint8 = 0b11000000; //black
      bkgrcolor_bat_uint8 = 0b11111111; //white
    } else {
          textcolor_bat_uint8 = 0b11111111;
          bkgrcolor_bat_uint8 = variable_color;
        }
    //On all Profiles, make battery white on red if <= 20%:
    if (actual_battery_percent <= 20){
      textcolor_bat_uint8 = 0b11111111;
      bkgrcolor_bat_uint8 = variable_color;
    }
  
  
  
    GlobalInverterColor = textcolor_bat_uint8 & 0b00111111;
    GlobalBkgColor      = bkgrcolor_bat_uint8 & 0b00111111;
    textcolor_bat       = (GColor8){.argb = textcolor_bat_uint8};
    bkgrcolor_bat       = (GColor8){.argb = bkgrcolor_bat_uint8};
  #endif
  text_layer_set_text_color(battery_runtime_layer, textcolor_bat);
  layer_mark_dirty(background_paint_layer);
    
    
  DisplayLastUpdated();
  
  init_battery_handler = 0;
  
}



static void handle_bluetooth(bool connected) {
  if (initDone){
  if( !connected )
    {
      if (vibe_on_disconnect){
        // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
        static const uint32_t segments[] = { 200, 70, 200, 70, 200 };
        VibePattern pat = {
          .durations = segments,
          .num_segments = ARRAY_LENGTH(segments),
        };
        vibes_enqueue_custom_pattern(pat);
      }
    } else {
      if (vibe_on_disconnect){ //could be vibe_on_connection
        // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
        static const uint32_t segments[] = { 70, 70, 150, 150, 70, 70, 150, 150 };
        VibePattern pat = {
          .durations = segments,
          .num_segments = ARRAY_LENGTH(segments),
        };
        vibes_enqueue_custom_pattern(pat);
      }
    }
  }
  text_layer_set_text(connection_layer, connected ? "Bluetooth" : "---------");
  #ifdef PBL_COLOR
    if (!connected) text_layer_set_text_color(connection_layer, GColorRed); else text_layer_set_text_color(connection_layer, textcolor_con);
  #endif
  if (connected && initDone){
    doUpdateWeather = true;
  }
}







static void layer_update_callback_hour_1(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 26, 41), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  switch (digit_h_1){
    case 1: 
    seven_segment_paint_1(ctx, 41, GPoint(0,0));
    break;
    case 2: 
    seven_segment_paint_2(ctx, 41, GPoint(0,0));
    break;
    default:
    break;
  }
}

static void layer_update_callback_hour_2(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 26, 41), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  switch (digit_h_2){
    case 1: 
    seven_segment_paint_1(ctx, 41, GPoint(0,0));
    break;
    case 2: 
    seven_segment_paint_2(ctx, 41, GPoint(0,0));
    break;
    case 3: 
    seven_segment_paint_3(ctx, 41, GPoint(0,0));
    break;
    case 4: 
    seven_segment_paint_4(ctx, 41, GPoint(0,0));
    break;
    case 5: 
    seven_segment_paint_5(ctx, 41, GPoint(0,0));
    break;
    case 6: 
    seven_segment_paint_6(ctx, 41, GPoint(0,0));
    break;
    case 7: 
    seven_segment_paint_7(ctx, 41, GPoint(0,0));
    break;
    case 8: 
    seven_segment_paint_8(ctx, 41, GPoint(0,0));
    break;
    case 9: 
    seven_segment_paint_9(ctx, 41, GPoint(0,0));
    break;
    case 0: 
    seven_segment_paint_0(ctx, 41, GPoint(0,0));
    break;
    default:
    break;
  }
}
static void layer_update_callback_minute_1(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 26, 41), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  switch (digit_m_1){
    case 1: 
    seven_segment_paint_1(ctx, 41, GPoint(0,0));
    break;
    case 2: 
    seven_segment_paint_2(ctx, 41, GPoint(0,0));
    break;
    case 3: 
    seven_segment_paint_3(ctx, 41, GPoint(0,0));
    break;
    case 4: 
    seven_segment_paint_4(ctx, 41, GPoint(0,0));
    break;
    case 5: 
    seven_segment_paint_5(ctx, 41, GPoint(0,0));
    break;
    case 0: 
    seven_segment_paint_0(ctx, 41, GPoint(0,0));
    break;
    default:
    break;
  }
}
static void layer_update_callback_minute_2(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 26, 41), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  switch (digit_m_2){
    case 1: 
    seven_segment_paint_1(ctx, 41, GPoint(0,0));
    break;
    case 2: 
    seven_segment_paint_2(ctx, 41, GPoint(0,0));
    break;
    case 3: 
    seven_segment_paint_3(ctx, 41, GPoint(0,0));
    break;
    case 4: 
    seven_segment_paint_4(ctx, 41, GPoint(0,0));
    break;
    case 5: 
    seven_segment_paint_5(ctx, 41, GPoint(0,0));
    break;
    case 6: 
    seven_segment_paint_6(ctx, 41, GPoint(0,0));
    break;
    case 7: 
    seven_segment_paint_7(ctx, 41, GPoint(0,0));
    break;
    case 8: 
    seven_segment_paint_8(ctx, 41, GPoint(0,0));
    break;
    case 9: 
    seven_segment_paint_9(ctx, 41, GPoint(0,0));
    break;
    case 0: 
    seven_segment_paint_0(ctx, 41, GPoint(0,0));
    break;
    default:
    break;
  }
}

static void layer_update_callback_second_1(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 10, 15), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_seconds);
  if (!DisplaySeconds){
    return;
  }
  //APP_LOG(APP_LOG_LEVEL_INFO, "Seconds_1_Update");
  if (DisplaySeconds >= 2) if (!SecOnShakingOn){
    seven_segment_15_paint_segment_4(ctx, GPoint(0,0));
    return;
  }
  switch (digit_s_1){
    case 1: 
    seven_segment_paint_1(ctx, 15, GPoint(0,0));
    break;
    case 2: 
    seven_segment_paint_2(ctx, 15, GPoint(0,0));
    break;
    case 3: 
    seven_segment_paint_3(ctx, 15, GPoint(0,0));
    break;
    case 4: 
    seven_segment_paint_4(ctx, 15, GPoint(0,0));
    break;
    case 5: 
    seven_segment_paint_5(ctx, 15, GPoint(0,0));
    break;
    case 0: 
    seven_segment_paint_0(ctx, 15, GPoint(0,0));
    break;
    default:
    break;
  }
}
static void layer_update_callback_second_2(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 10, 15), 0, GCornerNone);
  graphics_context_set_stroke_color(ctx, textcolor_seconds);
  if (!DisplaySeconds){
    return;
  }
  //APP_LOG(APP_LOG_LEVEL_INFO, "Seconds_2_Update");
  if (DisplaySeconds >= 2) if (!SecOnShakingOn){
    seven_segment_15_paint_segment_4(ctx, GPoint(0,0));
    return;
  }
  switch (digit_s_2){
    case 1: 
    seven_segment_paint_1(ctx, 15, GPoint(0,0));
    break;
    case 2: 
    seven_segment_paint_2(ctx, 15, GPoint(0,0));
    break;
    case 3: 
    seven_segment_paint_3(ctx, 15, GPoint(0,0));
    break;
    case 4: 
    seven_segment_paint_4(ctx, 15, GPoint(0,0));
    break;
    case 5: 
    seven_segment_paint_5(ctx, 15, GPoint(0,0));
    break;
    case 6: 
    seven_segment_paint_6(ctx, 15, GPoint(0,0));
    break;
    case 7: 
    seven_segment_paint_7(ctx, 15, GPoint(0,0));
    break;
    case 8: 
    seven_segment_paint_8(ctx, 15, GPoint(0,0));
    break;
    case 9: 
    seven_segment_paint_9(ctx, 15, GPoint(0,0));
    break;
    case 0: 
    seven_segment_paint_0(ctx, 15, GPoint(0,0));
    break;
    default:
    break;
  }
}


static void layer_update_callback_background(Layer *layer, GContext* ctx){
  //clear all with background_color_clock:
  graphics_context_set_fill_color(ctx, background_color_clock);
  graphics_context_set_stroke_color(ctx, background_color_clock);
  graphics_fill_rect(ctx, GRect(0, 0, 144, 168), 0, 0);
  
  //draw outlines:
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, background_color_lines);
  graphics_draw_line(ctx, GPoint(110,   0), GPoint(110,  15));
  graphics_draw_line(ctx, GPoint(  0,  16), GPoint(168,  16));
  graphics_draw_line(ctx, GPoint( 47,  17), GPoint( 47,  49));
  graphics_draw_line(ctx, GPoint( 85,  17), GPoint( 85,  68));
  graphics_draw_line(ctx, GPoint(  0,  50), GPoint( 84,  50));
  graphics_draw_line(ctx, GPoint(  0,  69), GPoint(168,  69));
  graphics_draw_line(ctx, GPoint(  0,  90), GPoint(168,  90));
  graphics_draw_line(ctx, GPoint(  0, 154), GPoint(168, 154));
  
  #ifdef PBL_COLOR
    //draw background areas:
    graphics_context_set_fill_color(ctx, background_color_clock);
    graphics_context_set_stroke_color(ctx, background_color_clock);
    graphics_fill_rect(ctx, GRect(0, 91, 144, 153-91+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_date);
    graphics_context_set_stroke_color(ctx, background_color_date);
    graphics_fill_rect(ctx, GRect(0, 70, 144, 89-70+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_status);
    graphics_context_set_stroke_color(ctx, background_color_status);
    graphics_fill_rect(ctx, GRect(0, 155, 144, 167-155+1), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_weather);
    graphics_context_set_stroke_color(ctx, background_color_weather);
    graphics_fill_rect(ctx, GRect( 0, 51, 85, 18), 0, 0);
    graphics_fill_rect(ctx, GRect(86, 17, 58, 52), 0, 0);
    
    graphics_context_set_fill_color(ctx, background_color_moon);
    graphics_context_set_stroke_color(ctx, background_color_moon);
    graphics_fill_rect(ctx, GRect(48, 17, 37, 33), 0, 0);
  
  
    if (warning_color_location){
      graphics_context_set_fill_color(ctx, GColorRed);
      graphics_context_set_stroke_color(ctx, GColorRed);
    } else {
      graphics_context_set_fill_color(ctx, background_color_location);
      graphics_context_set_stroke_color(ctx, background_color_location);
    }
    graphics_fill_rect(ctx, GRect(0, 0, 110, 16), 0, 0);
    
    if (warning_color_last_update){
      graphics_context_set_fill_color(ctx, GColorRed);
      graphics_context_set_stroke_color(ctx, GColorRed);
    } else {
      graphics_context_set_fill_color(ctx, background_color_last_update);
      graphics_context_set_stroke_color(ctx, background_color_last_update);
    }
    graphics_fill_rect(ctx, GRect(111, 0, 33, 16), 0, 0);
  
  #endif
  
  //draw dots of time:
  graphics_context_set_fill_color(ctx, textcolor_clock);
  graphics_context_set_stroke_color(ctx, textcolor_clock);
  graphics_fill_rect(ctx, GRect(69, 102, 7, 7), 0, 0);
  graphics_fill_rect(ctx, GRect(69, 124, 7, 7), 0, 0);
  
  //draw arrows of sun rise/set:
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, textcolor_sun);
  graphics_draw_line(ctx, GPoint(  3, 157), GPoint(  3, 165));
  graphics_draw_line(ctx, GPoint(  2, 158), GPoint(  4, 158));
  graphics_draw_line(ctx, GPoint(  1, 159), GPoint(  5, 159));
  graphics_draw_line(ctx, GPoint(106, 157), GPoint(106, 165));
  graphics_draw_line(ctx, GPoint(105, 164), GPoint(107, 164));
  graphics_draw_line(ctx, GPoint(104, 163), GPoint(108, 163));
  
  //draw battery:
  graphics_context_set_fill_color(ctx, bkgrcolor_bat);
  graphics_context_set_stroke_color(ctx, bkgrcolor_bat);
  GRect layer_bounds = GRect(0, 17, 84-38+1, 49-17+1);
  graphics_fill_rect(ctx, layer_bounds, 0, 0);
  
  graphics_context_set_fill_color(ctx, GColorClear);
  graphics_context_set_stroke_color(ctx, textcolor_bat);
  graphics_draw_line(ctx, GPoint( 0+1, 0+2+17), GPoint(41+1, 0+2+17));
  graphics_draw_line(ctx, GPoint( 0+1,14+2+17), GPoint(41+1,14+2+17));
  graphics_draw_line(ctx, GPoint( 0+1, 0+2+17+Y_OFFSET), GPoint( 0+1,14+2+17));
  
  graphics_draw_line(ctx, GPoint(41+1, 0+2+17), GPoint(41+1, 4+2+17));
  graphics_draw_line(ctx, GPoint(41+1,10+2+17), GPoint(41+1,14+2+17));
  graphics_draw_line(ctx, GPoint(43+1, 4+2+17), GPoint(43+1,10+2+17));
  graphics_draw_line(ctx, GPoint(41+1, 4+2+17), GPoint(43+1, 4+2+17));
  graphics_draw_line(ctx, GPoint(41+1,10+2+17), GPoint(43+1,10+2+17));
}
  
static void apply_color_profile(void){
  /*
  GColor textcolor_background;
  #ifdef PBL_COLOR
    GColor textcolor_sun;
    GColor textcolor_con;
    GColor textcolor_bat;
    uint8_t textcolor_bat_uint8;
    GColor bkgrcolor_bat;
    uint8_t bkgrcolor_bat_uint8;
    GColor textcolor_date;
    GColor textcolor_cal;
    GColor textcolor_moon;
    GColor textcolor_weather;
    GColor textcolor_location;
    GColor textcolor_last_update;
    GColor textcolor_tz;
  
    GColor background_color_lines;
    GColor background_color_clock;
    GColor background_color_date;
    GColor background_color_weather;
    GColor background_color_moon; //and weather_icon
    GColor background_color_location;
    GColor background_color_last_update;
    GColor background_color_status;
  #endif
  GColor textcolor_clock;
  GColor textcolor_seconds;
  */
  #ifdef PBL_COLOR
    if (ColorProfile == 1){ //Black on White
      textcolor_clock              = GColorBlack;
      textcolor_seconds            = GColorBlack;
      textcolor_tz                 = GColorBlack;
      textcolor_cal                = GColorBlack; //calendar week
      background_color_clock       = GColorWhite;
      
      textcolor_date               = GColorBlack;
      background_color_date        = GColorWhite;
      
      textcolor_weather            = GColorBlack;
      background_color_weather     = GColorWhite;
      
      textcolor_moon               = GColorBlack;
      background_color_moon        = GColorWhite;
      
      textcolor_sun                = GColorBlack;
      textcolor_con                = GColorBlack; //connection
      background_color_status      = GColorWhite;
      
      textcolor_location           = GColorBlack;
      background_color_location    = GColorWhite;
      
      textcolor_last_update        = GColorBlack;
      background_color_last_update = GColorWhite;
  
      background_color_lines       = GColorBlack;    
    } else if (ColorProfile == 5){ //Black Bkgr. and green clock
      textcolor_clock              = GColorFromHEX(0x00FF00);;
      textcolor_seconds            = GColorFromHEX(0x00AAAA);;
      textcolor_tz                 = GColorFromHEX(0x555555);; //OK
      textcolor_cal                = GColorFromHEX(0x00AAAA);;   //=GColorTiffanyBlue //calendar week
      background_color_clock       = GColorBlack;
      
      textcolor_date               = GColorFromHEX(0x00AAAA);;   //=GColorTiffanyBlue
      background_color_date        = GColorBlack;
      
      textcolor_weather            = GColorFromHEX(0x00FFAA);   //GColorMediumSpringGreen
      background_color_weather     = GColorBlack;
      
      textcolor_moon               = GColorWhite;
      background_color_moon        = GColorBlack;
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);;
      textcolor_con                = GColorFromHEX(0x00AAFF);;   //GColorVividCerulean  //connection
      background_color_status      = GColorBlack;
      
      textcolor_location           = GColorFromHEX(0xFFAA00);;   //=GColorChromeYellow
      background_color_location    = GColorFromHEX(0x005500);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);;
      background_color_last_update = GColorFromHEX(0x005500);
  
      background_color_lines       = GColorWhite;
    } else if (ColorProfile == 13){ //colored high contrast (blue clock on yellow)
      textcolor_clock              = GColorFromRGB(0, 0, 85);
      textcolor_seconds            = GColorFromRGB(0, 170, 170);
      textcolor_tz                 = GColorFromRGB(85, 85, 85); //OK
      textcolor_cal                = GColorFromRGB(0, 170, 170);   //=GColorTiffanyBlue; //calendar week
      background_color_clock       = GColorFromHEX(0xFFFF55);
      
      textcolor_date               = GColorFromRGB(170, 0, 85); //= GColorJazzberryJam; ;
      background_color_date        = GColorFromHEX(0xFFFF00);
      
      textcolor_weather            = GColorBlack;
      background_color_weather     = GColorWhite;
      
      textcolor_moon               = GColorWhite;
      background_color_moon        = GColorBlack;
      
      textcolor_sun                = GColorFromRGB(255, 255, 0);   //=GColorYellow //OK
      textcolor_con                = GColorFromRGB(0, 170, 255);   //GColorVividCerulean //connection
      background_color_status      = GColorBlack;
      
      textcolor_location           = GColorBlack;
      background_color_location    = GColorWhite;
      
      textcolor_last_update        = GColorBlack;
      background_color_last_update = GColorWhite;
  
      background_color_lines       = GColorFromRGB(170, 170, 170);
    } else if (ColorProfile == 14){ //TESTING (Blue with colored background)
      textcolor_clock              = GColorFromHEX(0x00FFFF);
      textcolor_seconds            = GColorFromHEX(0xFF5500);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_cal                = GColorFromHEX(0x00AAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000055);
      
      textcolor_date               = GColorFromHEX(0xAAFFFF);
      background_color_date        = GColorFromHEX(0x0000AA);
      
      textcolor_weather            = GColorFromHEX(0x55FF00);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFAA00);
      background_color_location    = GColorFromHEX(0x550000);
      
      textcolor_last_update        = GColorFromHEX(0xFFAAFF);
      background_color_last_update = GColorFromHEX(0x550055);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 6){ //TESTING (Blue with black Bkgr.)
      textcolor_clock              = GColorFromHEX(0x0055FF);
      textcolor_seconds            = GColorFromHEX(0x00AAFF);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_cal                = GColorFromHEX(0x00AAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000000);
      
      textcolor_date               = GColorFromHEX(0x0055FF);
      background_color_date        = GColorFromHEX(0x000000);
      
      textcolor_weather            = GColorFromHEX(0x55FF00);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0x00AAFF);
      background_color_location    = GColorFromHEX(0x000055);
      
      textcolor_last_update        = GColorFromHEX(0xFFAAFF);
      background_color_last_update = GColorFromHEX(0x550055);
  
      background_color_lines       = GColorFromHEX(0x0000AA);
    } else if (ColorProfile == 4){ //TESTING (Red with black Bkgr.)
      textcolor_clock              = GColorFromHEX(0xFF0000);
      textcolor_seconds            = GColorFromHEX(0xFF5500);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_cal                = GColorFromHEX(0xAA00AA); //calendar week
      background_color_clock       = GColorFromHEX(0x000000);
      
      textcolor_date               = GColorFromHEX(0xFF0000);
      background_color_date        = GColorFromHEX(0x000000);
      
      textcolor_weather            = GColorFromHEX(0xFF5555);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFAAAA);
      background_color_location    = GColorFromHEX(0x550000);
      
      textcolor_last_update        = GColorFromHEX(0xFFAAFF);
      background_color_last_update = GColorFromHEX(0x550055);
  
      background_color_lines       = GColorFromHEX(0xAA0000);
    } else if (ColorProfile == 7){ //TESTING (Red-Red)
      textcolor_clock              = GColorFromHEX(0xFFFF00);
      textcolor_seconds            = GColorFromHEX(0xFFFF00);
      textcolor_tz                 = GColorFromHEX(0xAAAAAA); //OK
      textcolor_cal                = GColorFromHEX(0xFFAA55); //calendar week
      background_color_clock       = GColorFromHEX(0x550000);
      
      textcolor_date               = GColorFromHEX(0xFFFFFF);
      background_color_date        = GColorFromHEX(0xAA0000);
      
      /*
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      */
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x550000);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x550055);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 8){ //TESTING (Green-Green)
      textcolor_clock              = GColorFromHEX(0xFFFF00);
      textcolor_seconds            = GColorFromHEX(0xFFFF00);
      textcolor_tz                 = GColorFromHEX(0xAAAAAA); //OK
      textcolor_cal                = GColorFromHEX(0xAAFF55); //calendar week
      background_color_clock       = GColorFromHEX(0x005500);
      
      textcolor_date               = GColorFromHEX(0xFFFFFF);
      background_color_date        = GColorFromHEX(0x00AA00);
      
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x005500);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x005555);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 9){ //TESTING (Blue-Blue)
      textcolor_clock              = GColorFromHEX(0x00FFFF);
      textcolor_seconds            = GColorFromHEX(0x00FFFF);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_cal                = GColorFromHEX(0xAAAAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000055);
      
      textcolor_date               = GColorFromHEX(0xFFFFFF);
      background_color_date        = GColorFromHEX(0x0000AA);
      
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x000055);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x005555);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 10){ //TESTING (light red)
      textcolor_clock              = GColorFromHEX(0x550000);
      textcolor_seconds            = GColorFromHEX(0x550000);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_cal                = GColorFromHEX(0x550000); //calendar week
      background_color_clock       = GColorFromHEX(0xFFAA55);
      
      textcolor_date               = GColorFromHEX(0x550000);
      background_color_date        = GColorFromHEX(0xFFAA55);
      
      textcolor_weather            = GColorFromHEX(0x000000);
      background_color_weather     = GColorFromHEX(0xFFFFAA);
      
      textcolor_moon               = GColorFromHEX(0x000000);
      background_color_moon        = GColorFromHEX(0xFFFFFF);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0x000000);
      background_color_location    = GColorFromHEX(0xFFAA00);
      
      textcolor_last_update        = GColorFromHEX(0x000000);
      background_color_last_update = GColorFromHEX(0xFFFF00);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 11){ //TESTING (light green)
      textcolor_clock              = GColorFromHEX(0x000000);
      textcolor_seconds            = GColorFromHEX(0x000000);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_cal                = GColorFromHEX(0x005500); //calendar week
      background_color_clock       = GColorFromHEX(0x00FF00);
      
      textcolor_date               = GColorFromHEX(0x005500);
      background_color_date        = GColorFromHEX(0x00FF00);
      
      textcolor_weather            = GColorFromHEX(0x000000);
      background_color_weather     = GColorFromHEX(0xAAFFAA);
      
      textcolor_moon               = GColorFromHEX(0x000000);
      background_color_moon        = GColorFromHEX(0xFFFFFF);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x00AA00);
      
      textcolor_last_update        = GColorFromHEX(0x000000);
      background_color_last_update = GColorFromHEX(0x00FF00);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 12){ //TESTING 10
      textcolor_clock              = GColorFromHEX(0x000000);
      textcolor_seconds            = GColorFromHEX(0x000000);
      textcolor_tz                 = GColorFromHEX(0xAAAAAA); //OK
      textcolor_cal                = GColorFromHEX(0xAAAAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x00AAFF);
      
      textcolor_date               = GColorFromHEX(0x000000);
      background_color_date        = GColorFromHEX(0x00AAFF);
      
      textcolor_weather            = GColorFromHEX(0x000000);
      background_color_weather     = GColorFromHEX(0xAAFFFF);
      
      textcolor_moon               = GColorFromHEX(0x000000);
      background_color_moon        = GColorFromHEX(0xFFFFFF);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0x000000);
      background_color_location    = GColorFromHEX(0x00AAFF);
      
      textcolor_last_update        = GColorFromHEX(0x000000);
      background_color_last_update = GColorFromHEX(0x55FFFF);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 2){ //TESTING (White on Black with Colors)
      textcolor_clock              = GColorFromHEX(0xFFFFFF);
      textcolor_seconds            = GColorFromHEX(0xFFFFFF);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_cal                = GColorFromHEX(0xAAAAAA); //calendar week
      background_color_clock       = GColorFromHEX(0x000000);
      
      textcolor_date               = GColorFromHEX(0xFFFFFF);
      background_color_date        = GColorFromHEX(0x000000);
      
      textcolor_weather            = GColorFromHEX(0xFFFFFF);
      background_color_weather     = GColorFromHEX(0x000000);
      
      textcolor_moon               = GColorFromHEX(0xFFFFFF);
      background_color_moon        = GColorFromHEX(0x000000);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0xFFFFFF);
      background_color_location    = GColorFromHEX(0x000000);
      
      textcolor_last_update        = GColorFromHEX(0xFFFFFF);
      background_color_last_update = GColorFromHEX(0x000000);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else if (ColorProfile == 3){ //TESTING (Black on White with Colors)
      textcolor_clock              = GColorFromHEX(0x000000);
      textcolor_seconds            = GColorFromHEX(0x000000);
      textcolor_tz                 = GColorFromHEX(0x555555); //OK
      textcolor_cal                = GColorFromHEX(0xAAAAAA); //calendar week
      background_color_clock       = GColorFromHEX(0xFFFFFF);
      
      textcolor_date               = GColorFromHEX(0x000000);
      background_color_date        = GColorFromHEX(0xFFFFFF);
      
      textcolor_weather            = GColorFromHEX(0x000000);
      background_color_weather     = GColorFromHEX(0xFFFFFF);
      
      textcolor_moon               = GColorFromHEX(0x000000);
      background_color_moon        = GColorFromHEX(0xFFFFFF);
      
      textcolor_sun                = GColorFromHEX(0xFFFF00);   //=GColorYellow //OK
      textcolor_con                = GColorFromHEX(0x00AAFF);   //GColorVividCerulean //connection
      background_color_status      = GColorFromHEX(0x000000);
      
      textcolor_location           = GColorFromHEX(0x000000);
      background_color_location    = GColorFromHEX(0xFFFFFF);
      
      textcolor_last_update        = GColorFromHEX(0x000000);
      background_color_last_update = GColorFromHEX(0xFFFFFF);
  
      background_color_lines       = GColorFromHEX(0xAAAAAA);
    } else { //default = BW
      textcolor_clock              = GColorWhite;
      textcolor_seconds            = GColorWhite;
      textcolor_tz                 = GColorWhite;
      textcolor_cal                = GColorWhite; //calendar week
      background_color_clock       = GColorBlack;
      
      textcolor_date               = GColorWhite;
      background_color_date        = GColorBlack;
      
      textcolor_weather            = GColorWhite;
      background_color_weather     = GColorBlack;
      
      textcolor_moon               = GColorWhite;
      background_color_moon        = GColorBlack;
      
      textcolor_sun                = GColorWhite;
      textcolor_con                = GColorWhite; //connection
      background_color_status      = GColorBlack;
      
      textcolor_location           = GColorWhite;
      background_color_location    = GColorBlack;
      
      textcolor_last_update        = GColorWhite;
      background_color_last_update = GColorBlack;
  
      background_color_lines       = GColorWhite;    
    }
  #endif
    
  // --- Create Text-Layers:
  #ifndef PBL_COLOR
    GColor textcolor = GColorWhite;
    if (ColorProfile) textcolor = GColorBlack;
    GColor bkgcolor = GColorBlack;
    if (ColorProfile) bkgcolor = GColorWhite;
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
    background_color_clock  = bkgcolor;
    background_color_lines = textcolor;
  #endif
  
  layer_mark_dirty(background_paint_layer);
  
  text_layer_set_text_color(text_sunrise_layer, textcolor_sun);
  text_layer_set_text_color(text_sunset_layer, textcolor_sun);
  text_layer_set_text_color(connection_layer, textcolor_con);
  text_layer_set_text_color(battery_runtime_layer, textcolor_bat);
  text_layer_set_text_color(Date_Layer, textcolor_date);
  text_layer_set_text_color(cwLayer, textcolor_cal);
  text_layer_set_text_color(moonLayer_IMG, textcolor_moon);
  text_layer_set_text_color(weather_layer_1_temp, textcolor_weather);
  text_layer_set_text_color(weather_layer_3_location, textcolor_location);
  text_layer_set_text_color(weather_layer_4_last_update, textcolor_last_update);
  text_layer_set_text_color(weather_layer_7_string_1, textcolor_weather);
  text_layer_set_text_color(weather_layer_7_string_2, textcolor_weather);
  text_layer_set_text_color(text_TimeZone_layer, textcolor_tz);
  
  #ifdef PBL_COLOR
    if (warning_color_location) text_layer_set_text_color(weather_layer_3_location, GColorWhite);
    if (warning_color_last_update) text_layer_set_text_color(weather_layer_4_last_update, GColorWhite);
  #else
    /*
    layer_set_hidden(inverter_layer_get_layer(s_warning_color_location), (bool)!warning_color_location);
    layer_set_hidden(inverter_layer_get_layer(s_warning_color_last_updated), (bool)!warning_color_last_update);
    */
    GColor text = GColorBlack;
    GColor bkgr = GColorWhite;
    if (ColorProfile){
      text = GColorWhite;
      bkgr = GColorBlack;
    }
    if (warning_color_location){
      text_layer_set_text_color(weather_layer_3_location, text);
      text_layer_set_background_color(weather_layer_3_location, bkgr);
    } else text_layer_set_background_color(weather_layer_3_location, GColorClear);
    if (warning_color_last_update){
      text_layer_set_text_color(weather_layer_4_last_update, text);
      text_layer_set_background_color(weather_layer_4_last_update, bkgr);
    } else text_layer_set_background_color(weather_layer_4_last_update, GColorClear);
  #endif
  
  handle_battery(battery_state_service_peek());
  
  #ifdef PBL_COLOR
    DisplayData(); //set correct color of temperature
  #endif
}

  
static void set_cwLayer_size(void){
  if (DisplaySeconds){
    if (TimeZoneFormat == 1){
      text_layer_set_text_alignment(cwLayer, GTextAlignmentCenter);
      layer_set_frame(text_layer_get_layer(cwLayer), GRect(0+X_OFFSET, 135+Y_OFFSET, 144, 20));
    } else {
      text_layer_set_text_alignment(cwLayer, GTextAlignmentLeft);
      layer_set_frame(text_layer_get_layer(cwLayer), GRect(72+X_OFFSET, 135+Y_OFFSET, 64, 20));
    }
  } else {
    text_layer_set_text_alignment(cwLayer, GTextAlignmentRight); // this must be done before layer_set_frame for alignment on Aplite.
    layer_set_frame(text_layer_get_layer(cwLayer), GRect(72+X_OFFSET, 135+Y_OFFSET, 64, 20));
  }
}



static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  
  // Read first item
  Tuple *t = dict_read_first(iterator);
  
  int restart = 0;
  time_t now = time(NULL);
  tick_time = localtime(&now);
  
  bool Settings_received = false;

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
      
    case KEY_WEATHER_DATA_TIME:
      station_data_last_updated = (int)t->value->int32;
      break;
      
    case KEY_LOCATION_NAME:
      snprintf(location_name, sizeof(location_name), "%s", t->value->cstring);
      phone_last_updated = time(NULL); // save the time the data arrived
      //APP_LOG(APP_LOG_LEVEL_INFO, "KEY_LOCATION_NAME updated last updated time.");
      break;
    case KEY_LOCATION_LAT:
      location_latitude = (int)t->value->int32;
      break;
    case KEY_LOCATION_LON:
      location_longitude = (int)t->value->int32;
      break;
    case KEY_WARN_LOCATION:
      if (warning_color_location != (int)t->value->int32){
        warning_color_location = (int)t->value->int32;
        apply_color_profile();
      } 
      break;
    case KEY_WEATHER_TEMP:
      #ifndef ITERATE_TEMP
        weather_TEMP = (int)t->value->int32;
      #endif
      break;
    case KEY_WEATHER_ICON:
      WeatherIcon = (int)t->value->int32;
      break;
    case KEY_WEATHER_UPDATE_INT:
      WeatherUpdateInterval = (int)t->value->int32;
      Settings_received = true;
      break;
    case KEY_WEATHER_STRING_1:
      snprintf(weather_string_1, sizeof(weather_string_1), "%s", t->value->cstring);
      replace_degree(weather_string_1, sizeof(weather_string_1));
      //text_layer_set_text(weather_layer_7_string_1, weather_string_1);
      //APP_LOG(APP_LOG_LEVEL_INFO, "weather_string_1 = %s", weather_string_1);
      break;
    case KEY_WEATHER_STRING_2:
      snprintf(weather_string_2, sizeof(weather_string_2), "%s", t->value->cstring);
      replace_degree(weather_string_2, sizeof(weather_string_2));
      //text_layer_set_text(weather_layer_7_string_2, weather_string_2);
      //APP_LOG(APP_LOG_LEVEL_INFO, "weather_string_2 = %s", weather_string_2);
      break;
    case KEY_TIME_ZONE_NAME:
      snprintf(time_ZONE_NAME, sizeof(time_ZONE_NAME), "%s", t->value->cstring);
      Settings_received = true;
      break;
    case KEY_SET_MOON_PHASE:
      MoonPhase = (int)t->value->int32;
      break;
    case KEY_SUN_RISE_UNIX:
      sun_rise_unix_loc = (time_t)t->value->int32;
      break;
    case KEY_SUN_SET_UNIX:
      sun_set_unix_loc = (time_t)t->value->int32;
      break;
    case KEY_SET_INVERT_COLOR:
      /*
      #ifndef PBL_COLOR
        if ((int32_t)ColorProfile != (int)t->value->int32) restart = 1;
        if ((ColorProfile == 0) && ((int)t->value->int32) > 1) restart = 0;
      #endif
      */
      ColorProfile = (int)t->value->int32;
      #ifndef PBL_COLOR
        //reset all color schemes on aplite platform
        if (ColorProfile > 1) ColorProfile = 0;
      #endif
      doUpdateWeather = true; //must be done when a configuration was received //TODO: save this and check on startup to avoid not updating after color scheme selection.
      apply_color_profile();
      break;
    case KEY_SET_LIGHT_ON:
      LightOn = (int)t->value->int32;
      break;
    case KEY_SET_DISPLAY_SEC:
      DisplaySeconds = (int)t->value->int32;
      //APP_LOG(APP_LOG_LEVEL_INFO, "DisplaySeconds = %d", DisplaySeconds);
      set_cwLayer_size();
      layer_mark_dirty(s_image_layer_second_1);
      layer_mark_dirty(s_image_layer_second_2);
      tick_timer_service_unsubscribe();
      if (DisplaySeconds >= 2){
        SecOnShakingOn = 1;
        SecondsTimeoutCounter = 0;
      }
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
    case KEY_SET_VIBE_HOUR:
      vibe_on_hour = (int)t->value->int32;
      break;
      
    case KEY_SET_TZ_FORMAT:
      TimeZoneFormat = (int)t->value->int32;
      Settings_received = true;
      set_cwLayer_size();
      break;
      
    case KEY_SET_UPDATE_TIME:
      ShowTimeSinceStationData = (int)t->value->int32;
      break;
      
    case KEY_SET_DATE_FORMAT:
      snprintf(date_format, sizeof(date_format), "%s", t->value->cstring);
      //APP_LOG(APP_LOG_LEVEL_ERROR, "date_format in watchface = %s", date_format);
      Settings_received = true;
      break;
      
      /*
    case KEY_SET_LABEL_INDEX_1:
      weather_label_1_info_number = (int)t->value->int32;
      break;
    case KEY_SET_LABEL_INDEX_2:
      weather_label_2_info_number = (int)t->value->int32;
      break;
    case KEY_SET_LABEL_INDEX_3:
      weather_label_3_info_number = (int)t->value->int32;
      break;
    case KEY_SET_LABEL_INDEX_4:
      weather_label_4_info_number = (int)t->value->int32;
      break;
    case KEY_SET_LABEL_INDEX_5:
      weather_label_5_info_number = (int)t->value->int32;
      break;
    case KEY_SET_LABEL_INDEX_6:
      weather_label_6_info_number = (int)t->value->int32;
      break;
    case KEY_SET_LABEL_INDEX_7:
      weather_label_7_info_number = (int)t->value->int32;
      break;
    case KEY_SET_LABEL_INDEX_8:
      weather_label_8_info_number = (int)t->value->int32;
      break;
      */
      
    case KEY_SET_DEGREE_F:
      //if (degree_f != (int)t->value->int32) restart = 1;
      degree_f = (int)t->value->int32;
      doUpdateWeather = true;
      break;
    
    default:
      //APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  SaveData();
  
  WeatherUpdateReceived = 1; //this indicates that the weather icon should be displayed if not in night mode.
  
  if (Settings_received) handle_second_tick(tick_time, SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT);
  if (restart) window_stack_pop_all(true); //true means animated = slide out
  
  DisplayData();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  if (DisplaySeconds < 2) return;
  SecOnShakingOn = 1;
  SecondsTimeoutCounter = 0;
  tick_timer_service_unsubscribe();
  time_t now = time(NULL);
  tick_time = localtime(&now);
  handle_second_tick(tick_time, SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT);
  tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
}



static void main_window_load(Window *window) {
  
  // Use setlocale() to obtain the system locale for translation
  sys_locale = setlocale(LC_ALL, "");
  LightIsOn = 0;
  
  
  Layer *main_window_layer = window_get_root_layer(s_main_window);
  
  initDone = false;
  
  // --- Load Fonts --- 
  pFontMoon = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MOON_PHASES_SUBSET_24));
  pFontClimacons = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CLIMACONS_32));
  
  //NightMode = 1;
  
  LoadData();
  
  #ifndef PBL_COLOR
    //reset all color schemes on aplite platform
    if (ColorProfile > 1) ColorProfile = 0;
  #endif
  
  background_paint_layer = layer_create(GRect(0+X_OFFSET, 0+Y_OFFSET, 144, 168));
  layer_set_update_proc(background_paint_layer, layer_update_callback_background);
  layer_add_child(main_window_layer, background_paint_layer);
  
  s_image_layer_hour_1 = layer_create(GRect(4+X_OFFSET, 94+Y_OFFSET, 26, 41));
  layer_set_update_proc(s_image_layer_hour_1, layer_update_callback_hour_1);
  layer_add_child(main_window_layer, s_image_layer_hour_1);
  s_image_layer_hour_2 = layer_create(GRect(37+X_OFFSET, 94+Y_OFFSET, 26, 41));
  layer_set_update_proc(s_image_layer_hour_2, layer_update_callback_hour_2);
  layer_add_child(main_window_layer, s_image_layer_hour_2);
  
  s_image_layer_minute_1 = layer_create(GRect(80+X_OFFSET, 94+Y_OFFSET, 26, 41));
  layer_set_update_proc(s_image_layer_minute_1, layer_update_callback_minute_1);
  layer_add_child(main_window_layer, s_image_layer_minute_1);
  s_image_layer_minute_2 = layer_create(GRect(111+X_OFFSET, 94+Y_OFFSET, 26, 41));
  layer_set_update_proc(s_image_layer_minute_2, layer_update_callback_minute_2);
  layer_add_child(main_window_layer, s_image_layer_minute_2);
  
  s_image_layer_second_1 = layer_create(GRect(113+X_OFFSET, 137+Y_OFFSET, 10, 15));
  layer_set_update_proc(s_image_layer_second_1, layer_update_callback_second_1);
  layer_add_child(main_window_layer, s_image_layer_second_1);
  s_image_layer_second_2 = layer_create(GRect(126+X_OFFSET, 137+Y_OFFSET, 10, 15));
  layer_set_update_proc(s_image_layer_second_2, layer_update_callback_second_2);
  layer_add_child(main_window_layer, s_image_layer_second_2);
  
  
  GColor textcolor = GColorWhite;
    
  // Sunrise Text
  text_sunrise_layer = text_layer_create(GRect(7+X_OFFSET, 152+Y_OFFSET, 50 /* width */, 30 /* height */)); 
  text_layer_set_text_color(text_sunrise_layer, textcolor);
  text_layer_set_background_color(text_sunrise_layer, GColorClear );
  text_layer_set_font(text_sunrise_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(main_window_layer, text_layer_get_layer(text_sunrise_layer));
  
  // Sunset Text
  text_sunset_layer = text_layer_create(GRect(110+X_OFFSET, 152+Y_OFFSET, 50 /* width */, 30 /* height */)); 
  text_layer_set_text_color(text_sunset_layer, textcolor);
  text_layer_set_background_color(text_sunset_layer, GColorClear );
  text_layer_set_font(text_sunset_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(main_window_layer, text_layer_get_layer(text_sunset_layer));      
  
  // Connection
  connection_layer = text_layer_create(GRect(47+X_OFFSET, 152+Y_OFFSET, 50, 34));
  text_layer_set_text_color(connection_layer, textcolor);
  text_layer_set_background_color(connection_layer, GColorClear);
  text_layer_set_font(connection_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(connection_layer, GTextAlignmentCenter);
  text_layer_set_text(connection_layer, "----");
  layer_add_child(main_window_layer, text_layer_get_layer(connection_layer));
  
  // Battery state / runtime:
  battery_runtime_layer = text_layer_create(GRect(2+X_OFFSET, 15+2+Y_OFFSET, 45, 15+20));
  text_layer_set_text_color(battery_runtime_layer, textcolor);
  text_layer_set_background_color(battery_runtime_layer, GColorClear);
  text_layer_set_font(battery_runtime_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(battery_runtime_layer, GTextAlignmentCenter);
  text_layer_set_text(battery_runtime_layer, "100%\n0:00 d");
  layer_add_child(main_window_layer, text_layer_get_layer(battery_runtime_layer));
  
  #ifdef PBL_PLATFORM_APLITE
    //fill battery with an InverterLayer
    s_battery_layer_fill = inverter_layer_create(GRect(3+X_OFFSET, 21+Y_OFFSET, 38, 11));
    layer_set_hidden(inverter_layer_get_layer(s_battery_layer_fill), true);
    layer_add_child(main_window_layer, inverter_layer_get_layer(s_battery_layer_fill));
  #else //else use effect layer on basalt
    s_battery_layer_fill = effect_layer_create(GRect(3+X_OFFSET, 21+Y_OFFSET, 38, 11));
    effect_layer_add_effect(s_battery_layer_fill, effect_invert_color, (void *)0b00000000); //use global inverter color
    layer_set_hidden(effect_layer_get_layer(s_battery_layer_fill), true);
    layer_add_child(main_window_layer, effect_layer_get_layer(s_battery_layer_fill));
  #endif
  
  
  
  // Date text
  Date_Layer = text_layer_create(GRect(5+X_OFFSET, 63+Y_OFFSET, 134 /* width */, 30 /* height */));
  text_layer_set_text_color(Date_Layer, textcolor);
  text_layer_set_background_color(Date_Layer, GColorClear );
  text_layer_set_font(Date_Layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(Date_Layer, GTextAlignmentCenter);
  layer_add_child(main_window_layer, text_layer_get_layer(Date_Layer));
  
  // Calendar Week
  cwLayer = text_layer_create(GRect(72+X_OFFSET, 135+Y_OFFSET, 64, 20)); //64 = label_width = 144-72-2*4 = display_width - display_width/2 - 2*Space
  text_layer_set_text_color(cwLayer, textcolor);
  text_layer_set_background_color(cwLayer, GColorClear );
  text_layer_set_font(cwLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(cwLayer, GTextAlignmentLeft);
  layer_add_child(main_window_layer, text_layer_get_layer(cwLayer));
  set_cwLayer_size();
  
  // Moon phase
  moonLayer_IMG = text_layer_create(GRect(51+X_OFFSET, 18+Y_OFFSET, 33, 33));
  text_layer_set_text_color(moonLayer_IMG, textcolor);
  text_layer_set_background_color(moonLayer_IMG, GColorClear);
  text_layer_set_font(moonLayer_IMG, pFontMoon);
  text_layer_set_text_alignment(moonLayer_IMG, GTextAlignmentCenter);
  layer_add_child(main_window_layer, text_layer_get_layer(moonLayer_IMG));
  
  // --- Weather Layers: ---
  
  // Create temperature Layer
  weather_layer_1_temp = text_layer_create(GRect(50+X_OFFSET, 10+Y_OFFSET, 94, 30));
  text_layer_set_background_color(weather_layer_1_temp, GColorClear);
  text_layer_set_text_color(weather_layer_1_temp, textcolor);
  text_layer_set_text_alignment(weather_layer_1_temp, GTextAlignmentRight);
  text_layer_set_text(weather_layer_1_temp, "---");
  text_layer_set_font(weather_layer_1_temp, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD)); //FONT_KEY_BITHAM_30_BLACK
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_1_temp));
  
  // Create location name Layer
  weather_layer_3_location = text_layer_create(GRect(0+X_OFFSET, -1+Y_OFFSET, 110, 17));
  text_layer_set_background_color(weather_layer_3_location, GColorClear);
  text_layer_set_text_color(weather_layer_3_location, textcolor);
  text_layer_set_text_alignment(weather_layer_3_location, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_3_location, "---" /*"Loading Weather ..."*/);
  text_layer_set_font(weather_layer_3_location, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_3_location));
  
  // Create last updated Layer
  weather_layer_4_last_update = text_layer_create(GRect(111+X_OFFSET, -1+Y_OFFSET, 33, 17));
  text_layer_set_background_color(weather_layer_4_last_update, GColorClear);
  text_layer_set_text_color(weather_layer_4_last_update, textcolor);
  text_layer_set_text_alignment(weather_layer_4_last_update, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_4_last_update, "---");
  text_layer_set_font(weather_layer_4_last_update, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_4_last_update));
  
  // Create String_1 Layer
  weather_layer_7_string_1 = text_layer_create(GRect(86+X_OFFSET, 54-15+Y_OFFSET, 144-86-2, 30)); //TODO
  text_layer_set_background_color(weather_layer_7_string_1, GColorClear);
  text_layer_set_text_color(weather_layer_7_string_1, textcolor);
  text_layer_set_text_alignment(weather_layer_7_string_1, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_7_string_1, "---\n---");
  text_layer_set_font(weather_layer_7_string_1, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_7_string_1));
  
  // Create String_2 Layer
  weather_layer_7_string_2 = text_layer_create(GRect(0+X_OFFSET, 50+Y_OFFSET, 84, 17)); //TODO
  text_layer_set_background_color(weather_layer_7_string_2, GColorClear);
  text_layer_set_text_color(weather_layer_7_string_2, textcolor);
  text_layer_set_text_alignment(weather_layer_7_string_2, GTextAlignmentCenter);
  text_layer_set_text(weather_layer_7_string_2, "--- / ---");
  text_layer_set_font(weather_layer_7_string_2, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	layer_add_child(main_window_layer, text_layer_get_layer(weather_layer_7_string_2));
  
  // Create TimeZone Layer
  text_TimeZone_layer = text_layer_create(GRect(5+X_OFFSET, 132+Y_OFFSET, 100, 20)); //TODO
  text_layer_set_background_color(text_TimeZone_layer, GColorClear);
  text_layer_set_text_color(text_TimeZone_layer, textcolor);
  text_layer_set_text_alignment(text_TimeZone_layer, GTextAlignmentLeft);
  text_layer_set_text(text_TimeZone_layer, " ");
  text_layer_set_font(text_TimeZone_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(main_window_layer, text_layer_get_layer(text_TimeZone_layer));
    
  // --- END ---
  
  apply_color_profile();
  
  DisplayData();
  
  // Avoids a blank screen on watch start.
  time_t now = time(NULL);
  tick_time = localtime(&now);
  handle_second_tick(tick_time, SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT);
  handle_battery(battery_state_service_peek());
  handle_bluetooth(bluetooth_connection_service_peek());
  
  // --- Register Event Handlers ---
  if (DisplaySeconds >= 2){
    SecOnShakingOn = 1;
    SecondsTimeoutCounter = 0;
  }
  if (DisplaySeconds)
    tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
  else
    tick_timer_service_subscribe(MINUTE_UNIT, &handle_second_tick);
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  accel_tap_service_subscribe(tap_handler);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  //APP_LOG(APP_LOG_LEVEL_INFO, "app_message_inbox_size_maximum()  = %d", (int)app_message_inbox_size_maximum());
  //APP_LOG(APP_LOG_LEVEL_INFO, "app_message_outbox_size_maximum() = %d", (int)app_message_outbox_size_maximum());
  #ifdef PBL_PLATFORM_APLITE
    //app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    app_message_open(200, 10); //in version 12.0, (200, 10) would be ok too. 500 just for security. Maybe 150 would also be OK. But not less!
  #else
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  #endif
  
  
  initDone = true;
  doUpdateWeather = false;
  
  if (AppFirstStart){
    AppFirstStart = 0;
    persist_write_int(KEY_DETECT_FIRST_START, AppFirstStart);
    doUpdateWeather = true;
  }
}

static void main_window_unload(Window *window) {
  
  SaveData();
  
  // --- Event Handlers ---
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  
  layer_destroy(background_paint_layer);
  
  layer_destroy(s_image_layer_hour_1);
  layer_destroy(s_image_layer_hour_2);
  layer_destroy(s_image_layer_minute_1);
  layer_destroy(s_image_layer_minute_2);
  layer_destroy(s_image_layer_second_1);
  layer_destroy(s_image_layer_second_2);
  
  
  #ifdef PBL_PLATFORM_APLITE //only on SDK 2.x
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