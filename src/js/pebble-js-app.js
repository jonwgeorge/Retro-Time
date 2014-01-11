var setPebbleToken = 'MF6Z';

Pebble.addEventListener("ready",
  function(e) {
    console.log("JavaScript app ready and running!");
    
    var options = JSON.parse(window.localStorage.getItem("options"));
    
    Pebble.sendAppMessage(options,
			function(e) {
				console.log("Successfully sent options to Pebble");
			},
			function(e) {
				console.log("Failed to send options to Pebble.\nError: " + e.error.message);
			}
		);
    
    getLocation();
  }
);

Pebble.addEventListener('showConfiguration', function(e) {
	var options = JSON.parse(window.localStorage.getItem("options"));
        
        var url = "https://s3-us-west-2.amazonaws.com/s3.jonwgeorge.com/RetroTimeV2.html?" +
                          "invert=" + encodeURIComponent(options["0"]) +
                          "&chime=" + encodeURIComponent(options["1"]);
	
	console.log("Pushing config webview: " + url);
	
	Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
	if(e.response) {
		var options = JSON.parse(decodeURIComponent(e.response));
                
		window.localStorage.setItem("options", JSON.stringify(options));
                
		Pebble.sendAppMessage(options,
			function(e) {
				console.log("Successfully sent options to Pebble");
			},
			function(e) {
				console.log("Failed to send options to Pebble.\nError: " + e.error.message);
			}
		);
	}
});

Pebble.addEventListener('getWeather', function(e) {
  getLocation();
});

function getLocation()
{
  if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(getWeather);
  } else {
    console.log("Can't get location.");
  }
}

function getWeather(position)
{
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" 
  	+ position.coords.latitude + "&lon=" + position.coords.longitude;
  console.log(url);
  var req = new XMLHttpRequest();
    req.open('GET', url, true);
    req.onload = function(e) {
      if (req.readyState == 4 && req.status == 200) {
        if(req.status == 200) {
          var response = JSON.parse(req.responseText);
          var kelvin = response.main.temp;
          var temperature = Math.floor(1.8 * (kelvin - 273) + 32);
          console.log("Temperature: " + temperature);
          Pebble.sendAppMessage({ "temperature":temperature });
          console.log("Sent");
      } else { console.log("Error"); }
    }
  }
  req.send(null);
}