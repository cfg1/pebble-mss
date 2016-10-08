/*
var options = {
  "invert": invertSelect.selectedIndex,
  "light": light_select.selectedIndex,
  "display_sec": dsSelect.selectedIndex,
  "date_format": dfSelect.options[dfSelect.selectedIndex].value, //not working with %
  "date_format_index": dfSelect.selectedIndex, //only used for internal storage in browser
  "time_zone_info": timeZoneSelect.selectedIndex,
  "vibe_disconnect": vdSelect.selectedIndex,
  "vibe_full": vfSelect.selectedIndex,
  "vibe_hour": vhSelect.selectedIndex,

  "default_loc": defaultlocEdit.value,
  "autodetect_loc": autodetlocSelect.selectedIndex,
  "lang_id": owmlangEdit.value,
  "show_update_time": updateTimeSelect.selectedIndex,
  "weatherLine1": weatherLine1Select.selectedIndex,
  "weatherLine2": weatherLine2Select.selectedIndex,
  "weatherLine3": weatherLine3Select.selectedIndex,
  "weatherLine4": weatherLine4Select.selectedIndex,
  "weatherUpdateInt": parseInt(updIntSelect.value),

  "degree_f": tuSelect.selectedIndex,
  "speed_unit": suSelect.selectedIndex,
  "pressure_unit": puSelect.selectedIndex
};
*/


var CLOUDPEBBLE = 1;

var CLIMACON = {
  'cloud'            : '!',
  'cloud_day'        : '"',
  'cloud_night'      : '#',
  'rain'             : '$',
  'rain_day'         : '%',
  'rain_night'       : '&',
  'showers'          : "'",
  'showers_day'      : '(',
  'showers_night'    : ')',
  'downpour'         : '*',
  'downpour_day'     : '+',
  'downpour_night'   : ',',
  'drizzle'          : '-',
  'drizzle_day'      : '.',
  'drizzle_night'    : '/',
  'sleet'            : '0',
  'sleet_day'        : '1',
  'sleet_night'      : '2',
  'hail'             : '3',
  'hail_day'         : '4',
  'hail_night'       : '5',
  'flurries'         : '6',
  'flurries_day'     : '7',
  'flurries_night'   : '8',
  'snow'             : '9',
  'snow_day'         : ':',
  'snow_night'       : ';',
  'fog'              : '<',
  'fog_day'          : '=',
  'fog_night'        : '>',
  'haze'             : '?',
  'haze_day'         : '@',
  'haze_night'       : 'A',
  'wind'             : 'B',
  'wind_cloud'       : 'C',
  'wind_cloud_day'   : 'D',
  'wind_cloud_night' : 'E',
  'lightning'        : 'F',
  'lightning_day'    : 'G',
  'lightning_night'  : 'H',
// ---
  'sun'              : 'I',
   'set'             : 'J',
   'rise'            : 'K',
   'low'             : 'L',
   'lower'           : 'M',
  'moon'             : 'N',
   'new'             : 'O',
   'wax_cresc'       : 'P',
   'wax_quart'       : 'Q',
   'wax_gib'         : 'R',
   'full'            : 'S',
   'wane_cresc'      : 'T',
   'wane_quart'      : 'U',
   'wane_gib'        : 'V',
  'snowflake'        : 'W',
  'tornado'          : 'X',
  'thermometer'      : 'Y',
   'temp_low'        : 'Z',
   'temp_med-low'    : '[',
   'temp_med-high'   : "\\",
   'temp_high'       : ']',
   'temp_full'       : '^',
  'celsius'          : '`',
  'fahrenheit'       : '_',
  'compass'          : 'a',
   'north'           : 'b',
   'east'            : 'c',
   'south'           : 'd',
   'west'            : 'e',
  'umbrella'         : 'f',
  'sunglasses'       : 'g',
  'cloud_refresh'    : 'h',
  'cloud_up'         : 'i',
  'cloud_down'       : 'j'
};

var OWMclimacon= {
// Thunderstorm
  200 : CLIMACON['lightning'], // thunderstorm with light rain
  201 : CLIMACON['lightning'], // thunderstorm with rain
  202 : CLIMACON['lightning'], // thunderstorm with heavy rain
  210 : CLIMACON['lightning'], // light thunderstorm
  211 : CLIMACON['lightning'], // thunderstorm
  212 : CLIMACON['lightning'], // heavy thunderstorm
  221 : CLIMACON['lightning'], // ragged thunderstorm
  230 : CLIMACON['lightning'], // thunderstorm with light drizzle
  231 : CLIMACON['lightning'], // thunderstorm with drizzle
  232 : CLIMACON['lightning'], // thunderstorm with heavy drizzle
// Drizzle
  300 : CLIMACON['drizzle'], // light intensity drizzle
  301 : CLIMACON['drizzle'], // drizzle
  302 : CLIMACON['drizzle'], // heavy intensity drizzle
  310 : CLIMACON['drizzle'], // light intensity drizzle rain
  311 : CLIMACON['drizzle'], // drizzle rain
  312 : CLIMACON['drizzle'], // heavy intensity drizzle rain
  313 : CLIMACON['showers'], // shower rain and drizzle
  314 : CLIMACON['showers'], // heavy shower rain and drizzle
  321 : CLIMACON['showers'], // shower drizzle
// Rain
  500 : CLIMACON['rain'], // light rain
  501 : CLIMACON['rain'], // moderate rain
  502 : CLIMACON['downpour'], // heavy intensity rain
  503 : CLIMACON['downpour'], // very heavy rain
  504 : CLIMACON['downpour'], // extreme rain
  511 : CLIMACON['downpour'], // freezing rain
  520 : CLIMACON['showers'], // light intensity shower rain
  521 : CLIMACON['showers'], // shower rain
  522 : CLIMACON['showers'], // heavy intensity shower rain
  531 : CLIMACON['showers'], // ragged shower rain
// Snow
  600 : CLIMACON['snow'], // light snow
  601 : CLIMACON['snow'], // snow
  602 : CLIMACON['snow'], // heavy snow
  611 : CLIMACON['sleet'], // sleet
  612 : CLIMACON['sleet'], // shower sleet
  615 : CLIMACON['snow'], // light rain and snow
  616 : CLIMACON['snow'], // rain and snow
  620 : CLIMACON['snow'], // light shower snow
  621 : CLIMACON['snow'], // shower snow
  622 : CLIMACON['snow'], // heavy shower snow
// Atmosphere
  701 : CLIMACON['haze'], // mist
  711 : CLIMACON['haze'], // smoke
  721 : CLIMACON['haze'], // haze
  731 : CLIMACON['haze'], // Sand/Dust Whirls
  741 : CLIMACON['fog'], // Fog
  751 : CLIMACON['haze'], // sand
  761 : CLIMACON['haze'], // dust
  762 : CLIMACON['haze'], // VOLCANIC ASH
  771 : CLIMACON['wind'], // SQUALLS
  781 : CLIMACON['tornado'], // TORNADO
// Clouds
  800 : CLIMACON['sun'], // sky is clear
  801 : CLIMACON['cloud_day'], // few clouds
  802 : CLIMACON['cloud_day'], // scattered clouds
  803 : CLIMACON['cloud_day'], // broken clouds
  804 : CLIMACON['cloud'], // overcast clouds
// Extreme
  900 : CLIMACON['tornado'], // tornado
  901 : CLIMACON['tornado'], // tropical storm
  902 : CLIMACON['tornado'], // hurricane
  903 : CLIMACON['temp_low'], // cold
  904 : CLIMACON['temp_high'], // hot
  905 : CLIMACON['wind'], // windy
  906 : CLIMACON['hail'], // hail 
// Additional
  950 : CLIMACON['set'], // Setting
  951 : CLIMACON['sun'], // Calm
  952 : CLIMACON['sun'], // Light breeze
  953 : CLIMACON['sun'], // Gentle Breeze
  954 : CLIMACON['sun'], // Moderate breeze
  955 : CLIMACON['sun'], // Fresh Breeze
  956 : CLIMACON['wind'], // Strong breeze
  957 : CLIMACON['wind'], // High wind, near gale
  958 : CLIMACON['wind'], // Gale
  959 : CLIMACON['wind'], // Severe Gale
  960 : CLIMACON['lightning'], // Storm
  961 : CLIMACON['lightning'], // Violent Storm
  962 : CLIMACON['tornado'], // Hurricane 
};

var OWM_DEFAULT_API_KEY = "1b5b37a3117bb6acd583d662fdbb24c7";

var configuration = {
  invert: 0,
  colored_temp: 1,
  light: 1,
  display_sec: 1,
  date_format: "%a, %d.%m.%Y",
  date_format_index: 2,
  time_zone_info: 2,
  
  vibe_disconnect: 1,
  vibe_full: 0,
  vibe_hour: 0,
  
  default_loc: "Berlin",
  autodetect_loc: 1,
  lang_id: "en",
  show_update_time: 0,
  moon_phase: 0,
  weatherLine1: 5,
  weatherLine2: 2,
  weatherLine3: 3,
  weatherLine4: 4,
  weatherUpdateInt: 20,
  
  degree_f: 0,
  speed_unit: 0,
  pressure_unit: 0, 
  
  OWM_API_KEY: OWM_DEFAULT_API_KEY
};

var last_detected_loc = {
  lat: 0.0,
  lon: 0.0
}

var ForecastDataJSON;
var WeatherDataJSON;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  console.log("locationSuccess Begin");
  //TODO: save loc:
  window.localStorage.last_location = JSON.stringify(last_detected_loc);
  SendToPebble(pos, 0);
}

function locationError(err) {
  console.log("locationError Begin");
  console.log('location error (' + err.code + '): ' + err.message);
  console.log("Error requesting location! Using Default.");
  //TODO: load loc:
  
  if (window.localStorage.getItem("last_location")){
    console.log("read last location start");
    last_location = JSON.parse(window.localStorage.configuration);
    //var test = JSON.parse(window.localStorage.configuration);
    console.log("loaded last location = " + JSON.stringify(last_location, null, 2));
    console.log("read last location finished");
    last_detected_loc.lat = last_location.lat;
    last_detected_loc.lon = last_location.lon;
  } else {
    console.log("error reading last location from localStorage");
  }
  
  if ((last_detected_loc.lat === 0.0) && (last_detected_loc.lon === 0.0)){
    var pos = {
      coords: {latitude: 0, longitude: 0}
    };
    SendToPebble(pos, 1);
  } else {
    var pos = {
      coords: {latitude: last_detected_loc.lat, longitude: last_detected_loc.lon}
    };
    SendToPebble(pos, 0);
  }
}

function SendToPebble(pos, use_default) {
  var url;
  var url_forecast;
  
  var multiplier = 10000;
  var pos_lat = Math.round(multiplier*pos.coords.latitude)/multiplier;
  var pos_lon = Math.round(multiplier*pos.coords.longitude)/multiplier;
  console.log("pos_lat = " + pos_lat);
  console.log("pos_lon = " + pos_lon);
  
  // Construct URL
  console.log("conf.auto_loc = " + configuration.autodetect_loc);
  //configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY; //TODO
  if (typeof configuration.OWM_API_KEY === 'string' || configuration.OWM_API_KEY instanceof String){
    if (configuration.OWM_API_KEY == "default"){
      configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
    }
    if (String(configuration.OWM_API_KEY).length < 20){
      configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
    }
  } else {
    configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
  }
  console.log("Using OWM API KEY: "+configuration.OWM_API_KEY);
  if ((use_default === 0) && (configuration.autodetect_loc)){
    url = "http://api.openweathermap.org/data/2.5/weather?APPID=" + configuration.OWM_API_KEY + "&lat=" +
        pos_lat + "&lon=" + pos_lon + "&lang=" + configuration.lang_id;
    url_forecast = "http://api.openweathermap.org/data/2.5/forecast?APPID=" + configuration.OWM_API_KEY + "&lat=" +
        pos_lat + "&lon=" + pos_lon + "&lang=" + configuration.lang_id;
  } else {
    console.log("conf.def_loc = " + configuration.default_loc);
    var city_name_req = configuration.default_loc;
    // Construct URL
    url = "http://api.openweathermap.org/data/2.5/weather?APPID=" + configuration.OWM_API_KEY + "&q=" + city_name_req + "&lang=" + configuration.lang_id;
    url_forecast = "http://api.openweathermap.org/data/2.5/forecast?APPID=" + configuration.OWM_API_KEY + "&q=" + city_name_req + "&lang=" + configuration.lang_id;
  }
  
  console.log("Weather URL = " + url);
  console.log("Weather Forecast URL = " + url_forecast);
  
  var utc_offset = new Date().getTimezoneOffset() * 60;
  
  xhrRequest(url_forecast, 'GET', 
    function(responseText) {
      var ForecastDataJSON_error = 0;
      try {
        ForecastDataJSON = JSON.parse(responseText);
        console.log("successfully parsed returned text of weather forecast.");
      } catch (e){
        ForecastDataJSON_error = 1;
        console.log("could not parse returned text of weather forecast: " + e);
      }
        
      xhrRequest(url, 'GET', 
        function(responseText) {
          var WeatherDataJSON_error = 0;
          var WeatherDataJSON_error_str = "";
          try {
            WeatherDataJSON = JSON.parse(responseText);
            console.log("successfully parsed returned text of weather data.");
          } catch (e){
            WeatherDataJSON_error = 1;
            WeatherDataJSON_error_str = responseText;
            console.log("could not parse returned text of weather data: " + e);
          }
          
          
          //---------------------------------------------------------------------------------------------------
          
          if (!WeatherDataJSON_error){
            
            // Temperature in Kelvin requires adjustment
            var temperature = Math.round((WeatherDataJSON.main.temp - 273.15));
            console.log("Temperature is " + temperature);
            var temp_min = Math.round((WeatherDataJSON.main.temp_min - 273.15));
            console.log("Temp. MIN is " + temp_min);
            var temp_max = Math.round((WeatherDataJSON.main.temp_max - 273.15));
            console.log("Temp. MAX is " + temp_max);
                
          
            // Conditions
            var conditions = WeatherDataJSON.weather[0].description;
            console.log("Conditions are " + conditions);
            
            var conditions_icon = OWMclimacon[WeatherDataJSON.weather[0].id].charCodeAt(0);
            console.log("Conditions icon is " + conditions_icon);
                
                
            var pressure = Math.round(WeatherDataJSON.main.pressure);
            var pressure_unit = "hPa";
            switch (configuration.pressure_unit){
              case 1: 
                pressure = Math.round(pressure/1.333);
                pressure_unit = "mmHg";
                break;
              case 2:
                pressure = Math.round(pressure/33.86389*100)/100;
                pressure_unit = "inHg";
                break;
            }
            console.log("Pressure is " + pressure + " " + pressure_unit);
                
            var humidity = Math.round(WeatherDataJSON.main.humidity);
            console.log("Humidity is " + humidity);
                
            var speed_unit_conversion_factor = 1;
            if (configuration.speed_unit === 0){
              speed_unit_conversion_factor = 3.6; //m/s -> km/h
            } else if (configuration.speed_unit == 1){
              speed_unit_conversion_factor = 2.236; //m/s -> mph
            }
            var wind_speed = WeatherDataJSON.wind.speed*speed_unit_conversion_factor;
            if (wind_speed < 10){
              wind_speed = Math.round(wind_speed*10)/10;
            } else {
              wind_speed = Math.round(wind_speed);
            }
            var wind_speed_unit = "m/s";
            if (configuration.speed_unit === 0) wind_speed_unit = "km/h";
            if (configuration.speed_unit == 1) wind_speed_unit = "mph";
            console.log("Wind Speed is " + wind_speed + " " + wind_speed_unit);
                
            var sunrise_unix = WeatherDataJSON.sys.sunrise;
            var sunset_unix  = WeatherDataJSON.sys.sunset;
            console.log("sunrise unix = "+sunrise_unix);
            console.log("sunset  unix = "+sunset_unix);
            var sunrise = timeConverter(Math.round(sunrise_unix));
            var sunset  = timeConverter(Math.round(sunset_unix));
            console.log("sunrise = " + sunrise);
            console.log("sunset =  " + sunset);
            //sunrise_unix = sunrise_unix - utc_offset;
            //sunset_unix  = sunset_unix  - utc_offset;
            
            var time_of_last_data = Number(WeatherDataJSON.dt);
            console.log("time_of_last_data = "+time_of_last_data);
          
            // Location:
            var location_name = WeatherDataJSON.name;
            var warn_location = 0;
            if ((configuration.autodetect_loc) && (use_default)){ //tried autodection of location, but could not get the lat long values from phone, so used default location set by the user.
              warn_location = 2;
              console.log("Tried autodection of location, but could not get the lat long values from phone.");
            } 
            if ((configuration.autodetect_loc === 0) && (use_default === 0)){ //detected location, but used user input
              console.log("Could autodect location, but used user input instead.");
              console.log(" |lat_autodetect - user_lat| = " + Math.abs(WeatherDataJSON.coord.lat - pos_lat));
              console.log(" |lon_autodetect - user_lon| = " + Math.abs(WeatherDataJSON.coord.lon - pos_lon));
              console.log("pos_lat                   = " + pos_lat);
              console.log("WeatherDataJSON.coord.lat = " + WeatherDataJSON.coord.lat);
              console.log("pos_lon                   = " + pos_lon);
              console.log("WeatherDataJSON.coord.lon = " + WeatherDataJSON.coord.lon);
              if ((Math.abs(WeatherDataJSON.coord.lat - pos_lat) > 0.3) && (Math.abs(WeatherDataJSON.coord.lon - pos_lon) > 0.5)){
                console.log("User input location differs from autodeteced location.");
                warn_location = 1;
              }
            }
            console.log("City name is " + location_name);
            console.log("LATITUDE  is " + pos.coords.latitude);
            console.log("LONGITUDE is " + pos.coords.longitude);
            console.log("warn_location = " + warn_location);
          
            // TIME:
            console.log("UTC Offset is " + utc_offset);
            
            // Get Min/Max temp. from forecast:
            var Forecast = {
              TempMin: 10000, // in Kelvin
              TempMax:     0  // in Kelvin
            };
            if (!ForecastDataJSON_error){
              console.log("forecast_list has " + ForecastDataJSON.cnt + " elements");
              var i;
              for (i = 0; i < Math.min(ForecastDataJSON.cnt, 8); i++) { // 8 entries means 24 hours for 3 hour forecast
                console.log("forecast_list[" + i + "].dt_text = "+ForecastDataJSON.list[i].dt_txt+"; T = " + (ForecastDataJSON.list[i].main.temp - 273.15) + " C");
                Forecast.TempMin = Math.min(ForecastDataJSON.list[i].main.temp, Forecast.TempMin);
                Forecast.TempMax = Math.max(ForecastDataJSON.list[i].main.temp, Forecast.TempMax);
              }
            }
            console.log("ForecastTempMin = "+Forecast.TempMin);
            console.log("ForecastTempMax = "+Forecast.TempMax);
            
            
                
            var weather_Line_1 = "";
            var weather_Line_2 = "";
            var weather_Line_3 = "";
            var weather_Line_4 = "";
            
            
            switch (configuration.weatherLine1){
              case 1:
                weather_Line_1 = conditions;
                break;
              case 2:
                weather_Line_1 = wind_speed + " " + wind_speed_unit;
                break;
              case 3:
                weather_Line_1 = humidity + " %";
                break;
              case 4:
                weather_Line_1 = pressure + " " + pressure_unit;
                break;
              case 5:
                if ((Forecast.TempMin == 10000) || (Forecast.TempMax === 0)){
                  weather_Line_1 = " --/-- ";
                } else {
                  if (configuration.degree_f){
                    weather_Line_1 = Math.round((Forecast.TempMax-273.15)*1.8+32) + "°/" + Math.round((Forecast.TempMin-273.15)*1.8+32) + "°F";
                  } else {
                    weather_Line_1 = Math.round((Forecast.TempMax-273.15)) + "°/" + Math.round((Forecast.TempMin-273.15)) + "°C";
                    //weather_Line_1 = Math.round((Forecast.TempMax-273.15)) + "/" + Math.round((Forecast.TempMin-273.15)) + "C";
                  }
                }
                break;
            }
            console.log("weather_Line_1 = " + (weather_Line_1.replace('°', ' ')).replace('°', ' '));
            
            
            
            switch (configuration.weatherLine2){
              case 1:
                weather_Line_2 = conditions;
                break;
              case 2:
                weather_Line_2 = wind_speed + " " + wind_speed_unit;
                break;
              case 3:
                weather_Line_2 = humidity + " %";
                break;
              case 4:
                weather_Line_2 = pressure + " " + pressure_unit;
                break;
              case 5:
                if ((Forecast.TempMin == 10000) || (Forecast.TempMax === 0)){
                  weather_Line_2 = " --/-- ";
                } else {
                  if (configuration.degree_f){
                    weather_Line_2 = Math.round((Forecast.TempMax-273.15)*1.8+32) + "°/" + Math.round((Forecast.TempMin-273.15)*1.8+32) + "°F";
                  } else {
                    weather_Line_2 = Math.round((Forecast.TempMax-273.15)) + "°/" + Math.round((Forecast.TempMin-273.15)) + "°C";
                  }
                }
                break;
            }
            console.log("weather_Line_2 = " + (weather_Line_2.replace('°', ' ')).replace('°', ' '));
            
            
            
            switch (configuration.weatherLine3){
              case 1:
                weather_Line_3 = conditions;
                break;
              case 2:
                weather_Line_3 = wind_speed + " " + wind_speed_unit;
                break;
              case 3:
                weather_Line_3 = humidity + " %";
                break;
              case 4:
                weather_Line_3 = pressure + " " + pressure_unit;
                break;
              case 5:
                if ((Forecast.TempMin == 10000) || (Forecast.TempMax === 0)){
                  weather_Line_3 = " --/-- ";
                } else {
                  if (configuration.degree_f){
                    weather_Line_3 = Math.round((Forecast.TempMax-273.15)*1.8+32) + "°/" + Math.round((Forecast.TempMin-273.15)*1.8+32) + "°F";
                  } else {
                    weather_Line_3 = Math.round((Forecast.TempMax-273.15)) + "°/" + Math.round((Forecast.TempMin-273.15)) + "°C";
                  }
                }
                break;
            }
            console.log("weather_Line_3 = " + (weather_Line_3.replace('°', ' ')).replace('°', ' '));
            
            
            
            switch (configuration.weatherLine4){
              case 1:
                weather_Line_4 = conditions;
                break;
              case 2:
                weather_Line_4 = wind_speed + " " + wind_speed_unit;
                break;
              case 3:
                weather_Line_4 = humidity + " %";
                break;
              case 4:
                weather_Line_4 = pressure + " " + pressure_unit;
                break;
              case 5:
                if ((Forecast.TempMin == 10000) || (Forecast.TempMax === 0)){
                  weather_Line_4 = " --/-- ";
                } else {
                  if (configuration.degree_f){
                    weather_Line_4 = Math.round((Forecast.TempMax-273.15)*1.8+32) + "°/" + Math.round((Forecast.TempMin-273.15)*1.8+32) + "°F";
                  } else {
                    weather_Line_4 = Math.round((Forecast.TempMax-273.15)) + "°/" + Math.round((Forecast.TempMin-273.15)) + "°C";
                  }
                }
                break;
            }
            console.log("weather_Line_4 = " + (weather_Line_4.replace('°', ' ')).replace('°', ' '));
            
            
            
          
            var weather_string_1 = weather_Line_1 + "\n" + weather_Line_2;
            console.log("weather_string_1 is: \n" + (weather_string_1.replace('°', ' ')).replace('°', ' ') +'\n');
            var weather_string_2 = weather_Line_3 + " / " + weather_Line_4; //TODO: what should be on this string?
            if (configuration.weatherLine3 === 0) weather_string_2 = weather_Line_4;
            if (configuration.weatherLine4 === 0) weather_string_2 = weather_Line_3;
            console.log("weather_string_2 is: \n" + (weather_string_2.replace('°', ' ')).replace('°', ' ') +'\n');
          
            if (CLOUDPEBBLE) {
              weather_string_1 = (weather_string_1.replace('°', '__')).replace('°', '__');
              weather_string_2 = (weather_string_2.replace('°', '__')).replace('°', '__');
            }
            
            if (configuration.autodetect_loc == 2) warn_location = 0;
            
            // Assemble dictionary using our keys
            var dictionary = {
              "KEY_LOCATION_NAME": location_name,
              "KEY_LOCATION_LAT": Math.round(pos.coords.latitude*1000000),
              "KEY_LOCATION_LON": Math.round(pos.coords.longitude*1000000),
              "KEY_WEATHER_TEMP": temperature,
              "KEY_WEATHER_STRING_1": weather_string_1,
              "KEY_WEATHER_STRING_2": weather_string_2,
              "KEY_WEATHER_ICON": conditions_icon,
              "KEY_TIME_UTC_OFFSET": utc_offset,
              "KEY_TIME_ZONE_NAME": getTimeZone(),
              "KEY_SUN_RISE": sunrise,
              "KEY_SUN_SET": sunset,
              "KEY_SUN_RISE_UNIX": sunrise_unix,
              "KEY_SUN_SET_UNIX": sunset_unix, //both converted to local time zone
              "KEY_WEATHER_DATA_TIME": time_of_last_data,
              "KEY_WARN_LOCATION": warn_location
            };
          
            // Send to Pebble
            
            console.log("Sending Weather Info to Pebble ...");
            Pebble.sendAppMessage(dictionary,
                                  function(e) {
                                    console.log("Weather info sent to Pebble successfully!");
                                  },
                                  function(e) {
                                    console.log("Error sending weather info to Pebble!");
                                  }
                                 );
            
            //var dictionary2 = {
              /*"KEY_LOCATION_NAME": location_name,
              "KEY_LOCATION_LAT": Math.round(pos.coords.latitude*1000000),
              "KEY_LOCATION_LON": Math.round(pos.coords.longitude*1000000),
              "KEY_WEATHER_TEMP": temperature,
              */ //"KEY_WEATHER_STRING_1": weather_string_1,
            //  "KEY_WEATHER_STRING_2": weather_string_2
            /*,
              "KEY_TIME_UTC_OFFSET": utc_offset,
              "KEY_TIME_ZONE_NAME": getTimeZone(),
              "KEY_SUN_RISE": sunrise,
              "KEY_SUN_SET": sunset*/
            //};
            /*
            console.log("Sending Weather Info to Pebble (2) ...");
            Pebble.sendAppMessage(dictionary2,
                                  function(e) {
                                    console.log("Weather info sent to Pebble successfully!");
                                  },
                                  function(e) {
                                    console.log("Error sending weather info to Pebble!");
                                  }
                                 );
              */                   
          } else { //end: if (!WeatherDataJSON_error)
            
            var weather_string_1 = WeatherDataJSON_error_str;
            var weather_string_2 = "E01: OWM Data error.";
            
            // Assemble dictionary using our keys
            var dictionary = {
              "KEY_WEATHER_STRING_1": weather_string_1,
              "KEY_WEATHER_STRING_2": weather_string_2,
              "KEY_TIME_UTC_OFFSET": utc_offset,
              "KEY_TIME_ZONE_NAME": getTimeZone()
            };
            
            console.log("Sending Error Message to Pebble:");
            console.log("  "+weather_string_2+":");
            console.log("  '"+weather_string_1+"'");
            Pebble.sendAppMessage(dictionary,
                                  function(e) {
                                    console.log("Error message sent to Pebble successfully!");
                                  },
                                  function(e) {
                                    console.log("Error sending Error message to Pebble!");
                                  }
                                 );
          }
          var date = new Date();
          console.log("Time is " + date);
          
          ForecastDataJSON = {};
          WeatherDataJSON  = {};
          
          
          //---------------------------------------------------------------------------------------------------
        }
      );
    }
  );
}

function getTimeZone() {
    return /\((.*)\)/.exec(new Date().toString())[1];
}



function timeConverter(UNIX_timestamp){
  
  var a = new Date(UNIX_timestamp*1000);
  var year = a.getFullYear();
  var month = pad(a.getMonth()+1);
  var date = pad(a.getDate());
  var hour = pad(a.getHours());
  var min = pad(a.getMinutes());
  var sec = pad(a.getSeconds());
  var time = hour + ':' + min;
  console.log("timeConverter: "+year+" "+month+" "+date+" "+hour+" "+min+" "+sec);
  
  /*
  var t = new Date(UNIX_timestamp*1000);
  var log = t.format("dd.mm.yyyy hh:MM:ss");
  console.log("timeConverter: "+log);
  var time = t.format("hh:MM");
  */
  return time;
}

function pad(input) {
    //var BASE = "00";
    //return input ? BASE.substr(0, 2 - Math.ceil(input / 10)) + input : BASE;
  if (input < 10){
    return ('0' + input);
  } else return input;
}



function getWeather() {
  
  
  if (window.localStorage.getItem("configuration")){
    console.log("read config start");
    configuration = JSON.parse(window.localStorage.configuration);
    //var test = JSON.parse(window.localStorage.configuration);
    console.log("loaded config = " + JSON.stringify(configuration, null, 2));
    console.log("read config finished");
  } else {
    console.log("error reading config from localStorage");
  }
  
  //console.log("reading config from localStorage REMOVED!!!");
  
  console.log("getWeather Begin");
  var options = {
    enableHighAccuracy: false,
    timeout: 10000,
    maximumAge: 0
  };
  if (configuration.autodetect_loc == 2){
    var pos = {
      coords: {latitude: 0, longitude: 0}
    };
    SendToPebble(pos, 1);
  } else {
    navigator.geolocation.getCurrentPosition(    //could also use navigator.geolocation.watchPosition() ?
      locationSuccess,
      locationError,
      options
    );
  }
  console.log("getWeather End");
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    //var i;
    //for (i=0; i<12; i++) console.log(pad(i));
    
    // Get the initial weather
    //getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWeather();
  }
);

Pebble.addEventListener("showConfiguration",
  function(e) {
    //Load the remote config page
    
    // Pebble.openURL("https://googledrive.com/host/0B3ivuMdwFLKzfnRGRFRHaXdJbGVRd0FsUElteEVybVZhSHBjM3YzQWRwa0loYUVqaG1JaWM/pebble_m7s_config_v15.3.html");
	  Pebble.openURL("https://rawgit.com/cfg1/pebble-mss/master/pebble_m7s_config_v15.5.html");
    
    //TODO: send some usefull values to the settings page (e. g. location, battery staistics etc.) by adding ?xxx to the URL
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    configuration = JSON.parse(decodeURIComponent(e.response));
    var configuration_str = JSON.stringify(configuration);
    console.log("Configuration window returned: " + JSON.stringify(configuration, null, 2));

    if (configuration_str.charAt(0) == "{" && configuration_str.slice(-1) == "}" && configuration_str.length > 5) {
    
      window.localStorage.configuration = JSON.stringify(configuration);
      
      
      if (typeof configuration.OWM_API_KEY === 'string' || configuration.OWM_API_KEY instanceof String){
        if (configuration.OWM_API_KEY == "default"){
          configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
        }
        if (String(configuration.OWM_API_KEY).length < 10){
          configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
        }
      } else {
        configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
      }
      
   
      //Send to Pebble, persist there
      var InvertColors = configuration.invert;
      console.log("DEBUG: InvertColors    = " + InvertColors);
      var LightOn = configuration.light;
      console.log("DEBUG: LightOn         = " + LightOn);
      var DisplaySeconds = configuration.display_sec;
      console.log("DEBUG: DisplaySeconds  = " + DisplaySeconds);
      
      var date_format_str = configuration.date_format; //"%a, %m.%d.%Y";
      date_format_str = date_format_str.split('_').join('%');
      console.log("DEBUG: date_format     = " + configuration.date_format + "; date_format_str = " + date_format_str);
      
      Pebble.sendAppMessage(
        {
          "KEY_SET_INVERT_COLOR": InvertColors,
          "KEY_SET_LIGHT_ON": LightOn,
          "KEY_SET_DISPLAY_SEC": DisplaySeconds,
          "KEY_SET_VIBE_DISC": configuration.vibe_disconnect,
          "KEY_SET_COLORED_TMP": configuration.colored_temp,
          "KEY_SET_VIBE_HOUR": configuration.vibe_hour,
          "KEY_SET_DEGREE_F": configuration.degree_f,
          "KEY_SET_DATE_FORMAT": date_format_str,
          "KEY_WEATHER_UPDATE_INT": configuration.weatherUpdateInt,
          "KEY_SET_TZ_FORMAT": configuration.time_zone_info,
          "KEY_SET_HEALTH": configuration.health_info,
          "KEY_SET_UPDATE_TIME": configuration.show_update_time,
          "KEY_SET_MOON_PHASE": configuration.moon_phase
        },
        function(e) {
          console.log("Settings data transfered successfully.");
        },
        function(e) {
          console.log("Settings feedback failed!");
        }
      );
    } else {
      console.log("Settings page cancelled.");
    }
  }
);

