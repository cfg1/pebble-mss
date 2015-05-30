

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
  // Construct URL
  if (use_default === 0){
    var multiplier = 100;
    var pos_lat = Math.round(multiplier*pos.coords.latitude)/multiplier;
    var pos_lon = Math.round(multiplier*pos.coords.longitude)/multiplier;
    console.log("pos_lat = " + pos_lat);
    console.log("pos_lon = " + pos_lon);
    url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
        pos_lat + "&lon=" + pos_lon;
  } else {
    var city_name_req = "Berlin";
    // Construct URL
    url = "http://api.openweathermap.org/data/2.5/weather?q=" + city_name_req;
  }
  
  var utc_offset = new Date().getTimezoneOffset() * 60;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round((json.main.temp - 273.15));
      console.log("Temperature is " + temperature);
      var temp_min = Math.round((json.main.temp_min - 273.15));
      console.log("Temp. MIN is " + temp_min);
      var temp_max = Math.round((json.main.temp_max - 273.15));
      console.log("Temp. MAX is " + temp_max);
      

      // Conditions
      var conditions = json.weather[0].main;
      console.log("Conditions are " + conditions);
      
      
      var pressure = Math.round(json.main.pressure);
      console.log("Pressure is " + pressure);
      
      var humidity = Math.round(json.main.humidity);
      console.log("Humidity is " + humidity);
      
      var wind_speed = Math.round(json.wind.speed*3.6);
      console.log("Wind Speed is " + wind_speed);
      
      
      console.log("sunrise unix = "+json.sys.sunrise);
      console.log("sunset  unix = "+json.sys.sunset);
      var sunrise = timeConverter(Math.round(json.sys.sunrise));
      var sunset  = timeConverter(Math.round(json.sys.sunset));
      console.log("sunrise = " + sunrise);
      console.log("sunset =  " + sunset);
      
      // Location:
      var location_name = json.name;
      if (use_default){
        location_name = location_name + " (default)";
      }
      console.log("City name is " + location_name);
      console.log("LATITUDE  is " + pos.coords.latitude);
      console.log("LONGITUDE is " + pos.coords.longitude);
      
      // TIME:
      console.log("UTC Offset is " + utc_offset);
      
      
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_LOCATION_NAME": location_name,
        "KEY_LOCATION_LAT": Math.round(pos.coords.latitude*1000000),
        "KEY_LOCATION_LON": Math.round(pos.coords.longitude*1000000),
        "KEY_WEATHER_TEMP": temperature,
        "KEY_WEATHER_TEMP_MIN": temp_min,
        "KEY_WEATHER_TEMP_MAX": temp_max,
        "KEY_WEATHER_PRESSURE": pressure,
        "KEY_WEATHER_WIND_SPEED": wind_speed,
        "KEY_WEATHER_CONDITIONS": conditions,
        "KEY_WEATHER_HUMIDITY": humidity,
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
    }
  );
  
  
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
    var BASE = "00";
    return input ? BASE.substr(0, 2 - Math.ceil(input / 10)) + input : BASE;
}



function getWeather() {
  console.log("getWeather Begin");
  var options = {
    enableHighAccuracy: false,
    timeout: 5000,
    maximumAge: 0
  };
  navigator.geolocation.getCurrentPosition(
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
    //Pebble.openURL("https://dl.dropboxusercontent.com/u/10824180/pebble%20config%20pages/sdktut9-config.html"); //original link
    
    //Pebble.openURL("www.googledrive.com/host/0B3ivuMdwFLKzR2d3Sjg1YUpSU2s");
    Pebble.openURL("https://googledrive.com/host/0B3ivuMdwFLKzfnRGRFRHaXdJbGVRd0FsUElteEVybVZhSHBjM3YzQWRwa0loYUVqaG1JaWM/pebble_cfg_watchface_config.html");
    
    //Pebble.openURL("https://www.dropbox.com/s/mzfrbbp8wlvkp4a/pebble_cfg_watchface_config.html"); // not working
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
 
    //Send to Pebble, persist there
    var InvertColors = 0;
    if (configuration.invert == "1") InvertColors = 1;
    console.log("InvertColors = " + InvertColors);
    var LightOn = configuration.light;
    //if (configuration.light == "1") LightOn = 1;
    //if (configuration.light == "2") LightOn = 2;
    console.log("LightOn = " + LightOn);
    var DisplaySeconds = 0;
    if (configuration.display_sec == "1") DisplaySeconds = 1;
    console.log("DisplaySeconds = " + DisplaySeconds);
    Pebble.sendAppMessage(
      {
        "KEY_SET_INVERT_COLOR": InvertColors,
        "KEY_SET_LIGHT_ON": LightOn,
        "KEY_SET_DISPLAY_SEC": DisplaySeconds
        
      },
      function(e) {
        console.log("Settings data transfered successfully.");
      },
      function(e) {
        console.log("Settings feedback failed!");
      }
    );
  }
);

