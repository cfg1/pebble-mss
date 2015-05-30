//NOTE: longitude is positive for East and negative for West
//TODO: read this from internet/geolocation and use this only for default values.
// BERLIN:
// 52.519444°, 13.406667°
#define LATITUDE   52.519444
#define LONGITUDE  13.406667
#define TIMEZONE   +1

#define DAY_NAME_LANGUAGE   DAY_NAME_GERMAN // Valid values: DAY_NAME_ENGLISH, DAY_NAME_GERMAN
#define TRANSLATION_CW "KW%V" 							// Translation for the calendar week (e.g. "CW%V")

//(un-)comment to change initial settings
#define INVERT_COLORS 0
#define LIGHT_ON 1

#define WEATHER_UPDATE_INTERVAL_MINUTE 15

#define DISPLAY_SECONDS 1


const char *DAY_NAME_GERMAN[] = {
	"SON",
	"MON",
	"DIE",
	"MIT",
	"DON",
	"FRE",
	"SAM"
};

const char *DAY_NAME_ENGLISH[] = {
	"SUN",
	"MON",
	"TUE",
	"WED",
	"THU",
	"FRE",
	"SAT"
};