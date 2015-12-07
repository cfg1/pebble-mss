//NOTE: longitude is positive for East and negative for West
//TODO: read this from internet/geolocation and use this only for default values.
// BERLIN:
// 52.519444°, 13.406667°
#define LATITUDE   52.519444
#define LONGITUDE  13.406667
#define TIMEZONE   +1

// Translation for the calendar week (e.g. "CW%V")
#define TRANSLATION_CW_DE "KW%V" 							
#define TRANSLATION_CW_EN "CW%V" 							
#define TRANSLATION_CW_FR "S.%V"

//(un-)comment to change initial settings
#ifdef PBL_COLOR
  #define INVERT_COLORS 3 //2 means colors on Basalt, and white on black on Aplite
#else
  #define INVERT_COLORS 0
#endif
#define LIGHT_ON      1
#define VIBE_ON_DISC 0 //disconnect
#define VIBE_ON_FULL 0
#define VIBE_ON_HOUR 0
#define DEGREE_F     0
#define DATE_FORMAT  "%a, %d.%m."

#define WEATHER_UPDATE_INTERVAL_MINUTE 30

#define DISPLAY_SECONDS 5
  
// uncomment this to show all colors of the temperature
//#define ITERATE_TEMP

// uncomment to use a different method to get HH:MM:SS:
#define GET_TIME_FROM_STRING