var setPebbleToken = 'MF6Z';

Pebble.addEventListener("ready",
  function(e) {
    console.log("JavaScript app ready and running!");
    
    /* getLocation(); */
  }
);

Pebble.addEventListener('appmessage', function(e) {
  key = e.payload.action;
  if (typeof(key) != 'undefined') {
    var settings = localStorage.getItem(setPebbleToken);
    if (typeof(settings) == 'string') {
      try {
        Pebble.sendAppMessage(JSON.parse(settings));
        console.log("AppMessage: " + settings);
      } catch (e) {
      }
    }
    var request = new XMLHttpRequest();
    request.open('GET', 'http://x.SetPebble.com/api/' + setPebbleToken + '/' + Pebble.getAccountToken(), true);
    request.onload = function(e) {
      if (request.readyState == 4)
        if (request.status == 200)
          try {
            // Pebble.sendAppMessage(JSON.parse(request.responseText));
            Pebble.sendAppMessage(JSON.parse(request.responseText),
  				function(e) {
   				 console.log("Successfully delivered message with transactionId="
     				 + e.data.transactionId);
  				},
  				function(e) {
   				 console.log("Unable to deliver message with transactionId="
   				   + e.data.transactionId
   				   + " Error is: " + e.error.message);
  				}
			);
            console.log(request.responseText);
          } catch (e) {
          }
    }
    request.send(null);
  }
});
Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('https://s3-us-west-2.amazonaws.com/s3.jonwgeorge.com/retro-config.html');
});
Pebble.addEventListener('webviewclosed', function(e) {
  if ((typeof(e.response) == 'string') && (e.response.length > 0)) {
    try {
      // Pebble.sendAppMessage(JSON.parse(e.response));
      Pebble.sendAppMessage(JSON.parse(e.response),
  		function(e) {
   		 console.log("Successfully delivered message with transactionId="
     		 + e.data.transactionId);
  		},
  		function(e) {
   		 console.log("Unable to deliver message with transactionId="
   		   + e.data.transactionId
   		   + " Error is: " + e.error.message);
  		}
	  );
      localStorage.setItem(setPebbleToken, e.response);
      console.log("Webview Closed: " + e.response);
    } catch(e) {
    }
  }
});

/* function getLocation()
{
  if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(showPosition);
      navigator.geolocation.getCurrentPosition(getWeather);
  } else {
    console.log("Can't get location.");
  }
}
  
function showPosition(position)
{
  console.log("Latitude: " + position.coords.latitude + "Longitude: " + position.coords.longitude);
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
          var temperature = response.main.temp;
          var icon = response.main.icon;
          Pebble.sendAppMessage({ "icon":icon, "temperature":temperature + "\u00B0C"});
          console.log("Sent");
      } else { console.log("Error"); }
    }
  }
  req.send(null);
} */