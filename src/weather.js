



//[HANDY] pebble-app.js:?: cfg_watchface_8_8__0.0/pebble-js-app.js:264 Configuration window returned: 
//{"invert":0,
//"light":1,
//"display_sec":1,
//"date_format":"_d._m._Y",
//"date_format_index":3,"degree_f":0,"speed_unit":1,"vibe_disconnect":1,"vibe_full":0,
//"default_loc":"London",
//"autodetect_loc":0}

var configuration = {
  invert: 0,
  light: 1,
  display_sec: 1,
  date_format: "%a, %d.%m.",
  date_format_index: 1,
  vibe_disconnect: 0,
  vibe_full: 0,
  degree_f: 0,
  speed_unit: 0,
  default_loc: "Berlin",
  autodetect_loc: 1,
  weatherLine1: 1,
  weatherLine2: 2,
  weatherUpdateInt: 15
};

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
  SendToPebble(pos, 0);
}

function locationError(err) {
  console.log("locationError Begin");
  console.log('location error (' + err.code + '): ' + err.message);
  console.log("Error requesting location! Using Default.");
  var pos = {
    coords: {latitude: 0, longitude: 0}
  };
  SendToPebble(pos, 1);
}

function SendToPebble(pos, use_default) {
  var url;
  var url_forecast;
  // Construct URL
  console.log("conf.auto_loc = " + configuration.autodetect_loc);
  if ((use_default === 0) && (configuration.autodetect_loc)){
    var multiplier = 100;
    var pos_lat = Math.round(multiplier*pos.coords.latitude)/multiplier;
    var pos_lon = Math.round(multiplier*pos.coords.longitude)/multiplier;
    console.log("pos_lat = " + pos_lat);
    console.log("pos_lon = " + pos_lon);
    url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
        pos_lat + "&lon=" + pos_lon;
    url_forecast = "http://api.openweathermap.org/data/2.5/forecast?lat=" +
        pos_lat + "&lon=" + pos_lon;
  } else {
    console.log("conf.def_loc = " + configuration.default_loc);
    var city_name_req = configuration.default_loc;
    // Construct URL
    url = "http://api.openweathermap.org/data/2.5/weather?q=" + city_name_req;
    url_forecast = "http://api.openweathermap.org/data/2.5/forecast?q=" + city_name_req;
  }
  
  console.log("Weather URL = " + url);
  console.log("Weather Forecast URL = " + url_forecast);
  
  var utc_offset = new Date().getTimezoneOffset() * 60;
  
  xhrRequest(url_forecast, 'GET', 
    function(responseText) {
      ForecastDataJSON = JSON.parse(responseText);
      xhrRequest(url, 'GET', 
        function(responseText) {
          WeatherDataJSON = JSON.parse(responseText);
          
          
          //---------------------------------------------------------------------------------------------------
          
          // Temperature in Kelvin requires adjustment
          var temperature = Math.round((WeatherDataJSON.main.temp - 273.15));
          console.log("Temperature is " + temperature);
          var temp_min = Math.round((WeatherDataJSON.main.temp_min - 273.15));
          console.log("Temp. MIN is " + temp_min);
          var temp_max = Math.round((WeatherDataJSON.main.temp_max - 273.15));
          console.log("Temp. MAX is " + temp_max);
              
        
          // Conditions
          var conditions = WeatherDataJSON.weather[0].main;
          console.log("Conditions are " + conditions);
              
              
          var pressure = Math.round(WeatherDataJSON.main.pressure);
          console.log("Pressure is " + pressure);
              
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
              
              
          console.log("sunrise unix = "+WeatherDataJSON.sys.sunrise);
          console.log("sunset  unix = "+WeatherDataJSON.sys.sunset);
          var sunrise = timeConverter(Math.round(WeatherDataJSON.sys.sunrise));
          var sunset  = timeConverter(Math.round(WeatherDataJSON.sys.sunset));
          console.log("sunrise = " + sunrise);
          console.log("sunset =  " + sunset);
        
          // Location:
          var location_name = WeatherDataJSON.name;
          if (use_default){
            location_name = location_name + "*";
          }
          console.log("City name is " + location_name);
          console.log("LATITUDE  is " + pos.coords.latitude);
          console.log("LONGITUDE is " + pos.coords.longitude);
        
          // TIME:
          console.log("UTC Offset is " + utc_offset);
          
          // Get Min/Max temp. from forecast:
          console.log("forecast_list has " + ForecastDataJSON.cnt + " elements");
          var Forecast = {
            TempMin: 10000, // in Kelvin
            TempMax:     0  // in Kelvin
          };
          var i;
          for (i = 0; i < Math.min(ForecastDataJSON.cnt, 8); i++) { // 8 entries means 24 hours for 3 hour forecast
            console.log("forecast_list[" + i + "].dt_text = "+ForecastDataJSON.list[i].dt_txt+"; T = " + (ForecastDataJSON.list[i].main.temp - 273.15) + " C");
            Forecast.TempMin = Math.min(ForecastDataJSON.list[i].main.temp, Forecast.TempMin);
            Forecast.TempMax = Math.max(ForecastDataJSON.list[i].main.temp, Forecast.TempMax);
          }
          console.log("ForecastTempMin = "+Forecast.TempMin);
          console.log("ForecastTempMax = "+Forecast.TempMax);
              
          var weather_Line_1 = "";
          var weather_Line_2 = "";
          
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
              weather_Line_1 = pressure + " hPa";
              break;
            case 5:
              if ((Forecast.TempMin == 10000) || (Forecast.TempMax === 0)){
                weather_Line_1 = " --/-- ";
              } else {
                if (configuration.degree_f){
                  weather_Line_1 = Math.round((Forecast.TempMax-273.15)*1.8+32) + "°/" + Math.round((Forecast.TempMin-273.15)*1.8+32) + "°F";
                } else {
                  weather_Line_1 = Math.round((Forecast.TempMax-273.15)) + "°/" + Math.round((Forecast.TempMin-273.15)) + "°C";
                }
              }
              break;
          }
          
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
              weather_Line_2 = pressure + " hPa";
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
          
        
          var weather_string_1 = weather_Line_1 + "\n" + weather_Line_2;
          console.log("weather_string_1 is: \n" + weather_string_1+'\n');
          var weather_string_2 = weather_Line_1 + "\n" + weather_Line_2; //TODO: what should be on this string?
          console.log("weather_string_2 is: \n" + weather_string_1+'\n');
        
        
          // Assemble dictionary using our keys
          var dictionary = {
            "KEY_LOCATION_NAME": location_name,
            "KEY_LOCATION_LAT": Math.round(pos.coords.latitude*1000000),
            "KEY_LOCATION_LON": Math.round(pos.coords.longitude*1000000),
            "KEY_WEATHER_TEMP": temperature,
            "KEY_WEATHER_STRING_1": weather_string_1,
            "KEY_WEATHER_STRING_2": weather_string_2,
            "KEY_TIME_UTC_OFFSET": utc_offset,
            "KEY_TIME_ZONE_NAME": getTimeZone(),
            "KEY_SUN_RISE": sunrise,
            "KEY_SUN_SET": sunset
          };
        
          // Send to Pebble
          Pebble.sendAppMessage(dictionary,
                                function(e) {
                                  console.log("Weather info sent to Pebble successfully!");
                                },
                                function(e) {
                                  console.log("Error sending weather info to Pebble!");
                                }
                               );
          var date = new Date();
          console.log("Time is " + date);
          
          
          //---------------------------------------------------------------------------------------------------
        }
      );
    }
  );
  
  
  
  //var WeatherData = getWeatherDataJSON(url);
  //var ForecastData = getWeatherDataJSON(url_forecast);
  //var WeatherDataJSON = JSON.parse(WeatherData);
  //var ForecastDataJSON = JSON.parse(ForecastData);

  // Send request to OpenWeatherMap

  
  
  
  // ------------ TESTS: --------------
  // Construct URL
  /*
  url = "http://api.openweathermap.org/data/2.5/forecast?q=Berlin";
  
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      
      console.log("Liste hat " + json.cnt + " Elemente");
      for (i = 0; i < json.cnt; i++) {
        console.log("list[" + i + "].dt_text = "+json.list[i].dt_txt+"; T = " + (json.list[i].main.temp - 273.15) + " C");
      }
    }
  );
  */
}

/*
function getWeatherDataJSON(url){
  var returnText = "";
  xhrRequest(url, 'GET', 
    function(responseText) {
      returnText = responseText;
    }
  );
  return returnText;
}
*/

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
  
  console.log("getWeather Begin");
  var options = {
    enableHighAccuracy: false,
    timeout: 5000,
    maximumAge: 0
  };
  navigator.geolocation.getCurrentPosition(    //could also use navigator.geolocation.watchPosition() ?
    locationSuccess,
    locationError,
    options
  );
  console.log("getWeather End");
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    //var i;
    //for (i=0; i<12; i++) console.log(pad(i));
    
    // Get the initial weather
    getWeather();
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
    //Pebble.openURL("https://dl.dropboxusercontent.com/u/10824180/pebble%20config%20pages/sdktut9-config.html"); //original link from tutorial
    //Pebble.openURL("https://www.dropbox.com/s/mzfrbbp8wlvkp4a/pebble_cfg_watchface_config.html"); // not working
    //Pebble.openURL("https://googledrive.com/host/0B3ivuMdwFLKzfnRGRFRHaXdJbGVRd0FsUElteEVybVZhSHBjM3YzQWRwa0loYUVqaG1JaWM/pebble_cfg_watchface_config.html");
    
    //Pebble.openURL("https://googledrive.com/host/0B3ivuMdwFLKzfnRGRFRHaXdJbGVRd0FsUElteEVybVZhSHBjM3YzQWRwa0loYUVqaG1JaWM/pebble_mss_config_url_test.html");
    
    /* --> */ Pebble.openURL("https://googledrive.com/host/0B3ivuMdwFLKzfnRGRFRHaXdJbGVRd0FsUElteEVybVZhSHBjM3YzQWRwa0loYUVqaG1JaWM/pebble_mss_config.html");
    
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
      
   
      //Send to Pebble, persist there
      var InvertColors = 0;
      if (configuration.invert == "1") InvertColors = 1;
      console.log("InvertColors    = " + InvertColors);
      var LightOn = configuration.light;
      //if (configuration.light == "1") LightOn = 1;
      //if (configuration.light == "2") LightOn = 2;
      console.log("LightOn         = " + LightOn);
      var DisplaySeconds = 0;
      if (configuration.display_sec == "1") DisplaySeconds = 1;
      console.log("DisplaySeconds  = " + DisplaySeconds);
      
      var date_format_str = configuration.date_format; //"%a, %m.%d.%y";
      console.log(" DEBUG = " + date_format_str);
      /*if (configuration.date_format == 0){
        date_format_str = "%a, %d.%m.";
      } else if (configuration.date_format == 1){
        date_format_str = "%a, %d/%m";
      } else if (configuration.date_format == 2){
        date_format_str = "%a, %m/%d";
      }*/
      /*
      var valuetxtarr = date_format_str.val();
      console.log(" DEBUG = " + valuetxtarr);
      date_format_str = valuetxtarr.replace('_', '%');
      */
      date_format_str = date_format_str.split('_').join('%');
      console.log(" DEBUG = " + date_format_str);
      
      
      console.log("vibe_disconnect = " + configuration.vibe_disconnect);
      console.log("vibe_full       = " + configuration.vibe_full);
      console.log("degree_f        = " + configuration.degree_f);
      console.log("date_format     = " + configuration.date_format + "; date_format_str = " + date_format_str);
      
      
      Pebble.sendAppMessage(
        {
          "KEY_SET_INVERT_COLOR": InvertColors,
          "KEY_SET_LIGHT_ON": LightOn,
          "KEY_SET_DISPLAY_SEC": DisplaySeconds,
          "KEY_SET_VIBE_DISC": configuration.vibe_disconnect,
          "KEY_SET_VIBE_FULL": configuration.vibe_full,
          "KEY_SET_DEGREE_F": configuration.degree_f,
          "KEY_SET_DATE_FORMAT": date_format_str,
          "KEY_WEATHER_UPDATE_INT": configuration.weatherUpdateInt
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

