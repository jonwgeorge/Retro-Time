var invert = localStorage.getItem('invert') ? localStorage.getItem('invert') : 1;
var bluetooth = localStorage.getItem('bluetooth') ? localStorage.getItem('bluetooth') : 1;
var chime = localStorage.getItem('chime') ? localStorage.getItem('chime') : 1;
var units = localStorage.getItem('units') ? localStorage.getItem('units') : 1;
var interval = localStorage.getItem('interval') ? localStorage.getItem('interval') : 900000;

console.log(invert + ", " + bluetooth + ", " + chime + ", " + units + ", " + interval);

Pebble.addEventListener("ready", function(e) {
    console.log("Retro Time is running!");
    getLocation();
});

Pebble.addEventListener('showConfiguration', function(e) {
  var invert = localStorage.getItem('invert') ? localStorage.getItem('invert') : 1;
  var bluetooth = localStorage.getItem('bluetooth') ? localStorage.getItem('bluetooth') : 1;
  var chime = localStorage.getItem('chime') ? localStorage.getItem('chime') : 1;
  var units = localStorage.getItem('units') ? localStorage.getItem('units') : 1;
  var interval = localStorage.getItem('interval') ? localStorage.getItem('interval') : 900000;
  
  var url = "https://s3-us-west-2.amazonaws.com/s3.jonwgeorge.com/retro-dev.html?invert=" + invert + "&bluetooth=" + bluetooth + "&chime=" + chime + "&units=" + units + "&interval=" + interval;
	console.log("Pushing config webview..." + url);
	Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function(e) {
    //Get JSON dictionary
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log("Configuration window returned: " + JSON.stringify(configuration));
    
    localStorage.setItem('invert', configuration.invert);
    localStorage.setItem('bluetooth', configuration.bluetooth);
    localStorage.setItem('chime', configuration.chime);
    localStorage.setItem('units', configuration.units);
    localStorage.setItem('interval', configuration.interval);
 
    //Send to Pebble, persist there
    Pebble.sendAppMessage(
      {
        'STYLE_KEY': parseInt(configuration.invert),
        'BLUETOOTHVIBE_KEY': parseInt(configuration.bluetooth),
        'HOURLYVIBE_KEY': parseInt(configuration.chime),
        'WEATHER_UNITS': parseInt(configuration.units),
        'WEATHER_REFRESH_INTERVAL_KEY': parseInt(configuration.interval)
      },
      function(e) {
        console.log('Successfully delivered settings with transactionId=' + e.data.transactionId);
      },
      function(e) {
        console.log('Unable to deliver settings with transactionId=' + e.data.transactionId + ' Error is: ' + e.error.message);
      }
    );
});

Pebble.addEventListener('getWeather', function(e) {
  console.log("Fetching weather for automatic location...");
  getLocation();
});

function getLocation()
{
  if (navigator.geolocation) {
      console.log("Location success!");
      navigator.geolocation.getCurrentPosition(getWeather);
  } else {
    console.log("Can't get location.");
  }
}

function getWeather(position)
{
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" + position.coords.latitude + "&lon=" + position.coords.longitude;
  console.log(url);
  var req = new XMLHttpRequest();
    req.open('GET', url, true);
    req.onload = function(e) {
      if (req.readyState == 4 && req.status == 200) {
        if(req.status == 200) {
          var response = JSON.parse(req.responseText);
            // console.log("Response: " + response);
          var kelvin = response.main.temp;
            // console.log("Kelvin: " + kelvin);
          
          var temperature;
          var units = localStorage.getItem('units');
          switch(units) {
            case 1:
              temperature = Math.floor(1.8 * (kelvin - 273) + 32);
              break;
            case 2:
              temperature = Math.floor(kelvin - 273);
              break;
            case 3:
              temperature = Math.floor(kelvin);
              break;
            default:
              temperature = Math.floor(1.8 * (kelvin - 273) + 32);
          }
          
          var description = response.weather[0].main.toString();
          
          console.log("Temperature: " + temperature + ", Description: " + description);
          
          Pebble.sendAppMessage(
            {
              'WEATHER_TEMPERATURE_KEY': parseInt(temperature),
              'WEATHER_DESCRIPTION_KEY': description
            },
            function(e) {
              console.log('Successfully delivered weather with transactionId=' + e.data.transactionId);
            },
            function(e) {
              console.log('Unable to deliver weather with transactionId=' + e.data.transactionId + ' Error is: ' + e.error.message);
            }
          );
      } else { console.log("No response from openweather..."); }
    }
  };
  req.send(null);
}