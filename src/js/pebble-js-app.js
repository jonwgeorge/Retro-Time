var invert = window.localStorage.getItem('invert') ? window.localStorage.getItem('invert') : 'off';
var chime = window.localStorage.getItem('chime') ? window.localStorage.getItem('chime') : 'off';
var units = window.localStorage.getItem('units') ? window.localStorage.getItem('units') : 'f';
var interval = window.localStorage.getItem('interval') ? window.localStorage.getItem('interval') : 900000;

Pebble.addEventListener("ready", function(e) {
    console.log("ready and up.");
    setInterval(function(){
        console.log("fetching weather for automatic location...");
        getLocation();
    },interval);
    getLocation();
});

Pebble.addEventListener('showConfiguration', function(e) {
    var url = "https://s3-us-west-2.amazonaws.com/s3.jonwgeorge.com/retro-dev.html?invert=" + invert + "&chime=" + chime + "&units=" + units + "&interval=" + interval;
	console.log("Pushing config webview...");
	Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
    
    window.localStorage.setItem('invert', configuration.invert);
    window.localStorage.setItem('chime', configuration.chime);
    window.localStorage.setItem('units', configuration.units);
    window.localStorage.setItem('interval', configuration.interval);
 
    //Send to Pebble, persist there
    Pebble.sendAppMessage(
      {"KEY_INVERT": configuration.invert},
      {"KEY_CHIME":  configuration.chime},
      {"KEY_UNITS": configuration.units},
      {"KEY_INTERVAL": configuration.interval},
      function(e) {
        console.log("settings delivered.");
      },
      function(e) {
        console.log("settings failed to deliver.");
      }
    )
});

Pebble.addEventListener('getWeather', function(e) {
  console.log("fetching weather for automatic location...");
  getLocation();
});

function getLocation()
{
  if (navigator.geolocation) {
      console.log("location success!");
      navigator.geolocation.getCurrentPosition(getWeather);
  } else {
    console.log("Can't get location.");
  }
}

function getWeather(position)
{
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" 
  	+ position.coords.latitude + "&lon=" + position.coords.longitude;
  var req = new XMLHttpRequest();
    req.open('GET', url, true);
    req.onload = function(e) {
      if (req.readyState == 4 && req.status == 200) {
        if(req.status == 200) {
          var response = JSON.parse(req.responseText);
            // console.log("Response: " + response);
          var kelvin = response.main.temp;
            // console.log("Kelvin: " + kelvin);
          if (units == 'f') {    
            var temperature = Math.floor(1.8 * (kelvin - 273) + 32);
            temperature = temperature.toString();
            temperature = temperature + "F";
          } else if (units == 'c') {
            var temperature = Math.floor(kelvin - 273);
            temperature = temperature.toString();
            temperature = temperature + "C";
          } else if (units == 'k') {
            var temperature = Math.floor(kelvin);
            temperature = temperature.toString();
            temperature = temperature + "K";
          }
            // console.log("Temperature: " + temperature);
          var icon = response.weather[0].icon.toString();
            // console.log("Icon: " + icon);
          var description = response.weather[0].description.toString();
            // console.log("Description: " + description);
          Pebble.sendAppMessage(
              {"KEY_TEMP": temperature},
              {"KEY_ICON": icon},
              {"KEY_DESCRIPTION": description},
              function(e) {
                console.log("Got weather from openweather.");
                console.log("temp = " + temperature + " description = " + description + " icon = " + icon);
              },
              function(e) {
                console.log("Weather failed!");
              }
            )
      } else { console.log("Error"); }
    }
  }
  req.send(null);
}