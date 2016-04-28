// Standard includes
<<<<<<< HEAD:src/retro-time.c
#include "pebble.h"
#ifdef PBL_COLOR
  #include "gcolor_definitions.h"
#endif
  
static int mConfigStyle = 1;               //0=WhiteOnBlack, 1=BlackOnWhite
static int mConfigBluetoothVibe = 1;       //0=off 1=on
static int mConfigHourlyVibe = 1;          //0=off 1=on
static int mConfigWeatherUnit = 1;         //1=Fahrenheit, 2=Celcius, 3=Kelvin
 
static int mTemperatureDegrees = 000;  //-999 to 999
static int currentTemp;
static char temperatureString[1024];
static char mWeatherDescription[20] = "";      //cloudy,sunny,rain,snow,etc.
static int mRefreshInterval = 900000;        //Time in milliseconds to refresh weather

enum {
  STYLE_KEY,
  BLUETOOTHVIBE_KEY,
  HOURLYVIBE_KEY,
  WEATHER_UNITS,
  WEATHER_TEMPERATURE_KEY,
  WEATHER_DESCRIPTION_KEY,
  REFRESH_INTERVAL_KEY
=======
#include <pebble.h>

enum {
  AKEY_NUMBER,
  AKEY_TEXT,
>>>>>>> d85fd0af28b5f066d3538588034dc47ff7c6ab75:src/main.c
};

// App-specific data
Window *window; // All apps must have at least one window
TextLayer *time_layer, *date_layer, *temp_layer, *battery_layer, *charge_layer, *connection_layer;
InverterLayer *inverter_layer;

// Function to set theme invert layer on or off
static void set_theme() {
  if (mConfigStyle == 1) {
    layer_set_hidden(inverter_layer_get_layer(inverter_layer), true);
  }
}

static void update_weather() {
  if (mTemperatureDegrees != currentTemp) {
    currentTemp = mTemperatureDegrees;
    switch( mConfigWeatherUnit ) 
    {
      case 1:
        snprintf(temperatureString, 1024, "%d\u00B0F", mTemperatureDegrees);
        text_layer_set_text(temp_layer, temperatureString);
      case 2:
        snprintf(temperatureString, 1204, "%d\u00B0C", mTemperatureDegrees);
        text_layer_set_text(temp_layer, temperatureString);
      case 3:
        snprintf(temperatureString, 1204, "%d\u00B0K", mTemperatureDegrees);
        text_layer_set_text(temp_layer, temperatureString);
      default :
        snprintf(temperatureString, 1204, "%d\u00B0F", mTemperatureDegrees);
        text_layer_set_text(temp_layer, temperatureString);
    }
  }
  
  //text_layer_set_text(temp_layer, mTemperatureDegrees);
}

// Function to handle battery level and charge status
static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "\uf004 \uf004 \uf004 \uf004 \uf0e7";
  static char charge_text[] = "   ";

  if (charge_state.charge_percent > 75) {
    snprintf(battery_text, sizeof(battery_text), "\uf004 \uf004 \uf004 \uf004");
  } else if (charge_state.charge_percent > 50) {
    snprintf(battery_text, sizeof(battery_text), "\uf004 \uf004 \uf004 \uf08a");
  } else if (charge_state.charge_percent > 25) {
    snprintf(battery_text, sizeof(battery_text), "\uf004 \uf004 \uf08a \uf08a");
  } else if (charge_state.charge_percent > 0) {
    snprintf(battery_text, sizeof(battery_text), "\uf004 \uf08a \uf08a \uf08a");
  } else {
    snprintf(battery_text, sizeof(battery_text), "\uf08a \uf08a \uf08a \uf08a");
  }
  
  if (charge_state.is_charging) {
    snprintf(charge_text, sizeof(charge_text), "\uf0e7");
  } else {
    snprintf(charge_text, sizeof(charge_text), "   ");
  }

  text_layer_set_text(battery_layer, battery_text);
  text_layer_set_text(charge_layer, charge_text);
}

// Called once per second
static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {
  static char time_text[] = "00:00 AM"; // Needs to be static because it's used by the system later.
  static char date_text[] = "Mon, Jan 31"; // Needs to be static because it's used by the system later.
  char * time_format;
  
  if(clock_is_24h_style()) {
	time_format = "%R";
  } else {
	time_format = "%I:%M %p";
  }
  
  strftime(time_text, sizeof(time_text), time_format, tick_time);
  text_layer_set_text(time_layer, time_text);
  
 // strftime(date_text, sizeof(date_text), "%a, %b %d", tick_time);
  strftime(date_text, sizeof(date_text), "%a, %b %d", tick_time);
  text_layer_set_text(date_layer, date_text);
  
  if (tick_time->tm_min == 0 && tick_time->tm_sec == 01 && mConfigHourlyVibe == 1) {
  	vibes_double_pulse();
  }

  handle_battery(battery_state_service_peek());
  update_weather();
}

static void handle_bluetooth(bool connected) {
  text_layer_set_text(connection_layer, connected ? "\uf09e" : "\uf071");
  
  if (!connected && mConfigBluetoothVibe == 1) {
    vibes_long_pulse();
  }
}

<<<<<<< HEAD:src/retro-time.c
void out_sent_handler(DictionaryIterator *sent, void *context) {
  
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   // outgoing message failed
}

=======
>>>>>>> d85fd0af28b5f066d3538588034dc47ff7c6ab75:src/main.c
void in_received_handler(DictionaryIterator *received, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings received...");
  // Check for fields you expect to receive
<<<<<<< HEAD:src/retro-time.c
  Tuple *invert_tuple = dict_find(received, STYLE_KEY);
  Tuple *bluetooth_tuple = dict_find(received, BLUETOOTHVIBE_KEY);
  Tuple *chime_tuple = dict_find(received, HOURLYVIBE_KEY);
  Tuple *temp_tuple = dict_find(received, WEATHER_TEMPERATURE_KEY);
  Tuple *units_tuple = dict_find(received, WEATHER_UNITS);
  Tuple *description_tuple = dict_find(received, WEATHER_DESCRIPTION_KEY);
  Tuple *interval_tuple = dict_find(received, REFRESH_INTERVAL_KEY);
  
  // Act on the found fields received
  if (invert_tuple) {
    persist_write_int(STYLE_KEY, invert_tuple->value->uint16);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Invert received...");
  }
  
  if (bluetooth_tuple) {
    persist_write_int(BLUETOOTHVIBE_KEY, bluetooth_tuple->value->uint16);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Bluetooth received...");
  }
  
  if (chime_tuple) {    
    persist_write_int(HOURLYVIBE_KEY, chime_tuple->value->uint16);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Chime received...");
  }
  
  if (temp_tuple) {    
    persist_write_int(WEATHER_TEMPERATURE_KEY, temp_tuple->value->uint32);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Temp received...");
  }
  
  if (units_tuple) {
    persist_write_int(WEATHER_UNITS, units_tuple->value->uint16);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Units received...");
  }
    
  if (description_tuple) {    
    persist_write_string(WEATHER_DESCRIPTION_KEY, description_tuple->value->cstring);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Description received...");
  }
  
  if (interval_tuple) {
    persist_write_int(REFRESH_INTERVAL_KEY, interval_tuple->value->uint16);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Interval received...");
=======
  Tuple *text_tuple = dict_find(received, AKEY_TEXT);

  // Act on the found fields received
  if (text_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Text: %s", text_tuple->value->cstring);
>>>>>>> d85fd0af28b5f066d3538588034dc47ff7c6ab75:src/main.c
  }
  
  // Update static variables and reset theme if needed
  mConfigStyle = persist_read_int(STYLE_KEY);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ConfigStyle: %d", mConfigStyle);
  mConfigBluetoothVibe = persist_read_int(BLUETOOTHVIBE_KEY);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ConfigBluetoothVibe: %d", mConfigBluetoothVibe);
  mConfigHourlyVibe = persist_read_int(HOURLYVIBE_KEY);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ConfigHourlyVibe: %d", mConfigHourlyVibe);
  mTemperatureDegrees = persist_read_int(WEATHER_TEMPERATURE_KEY);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Temperature: %d", mTemperatureDegrees);
  mConfigWeatherUnit = persist_read_int(WEATHER_UNITS);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ConfigWeatherUnit: %d", mConfigWeatherUnit);
  persist_read_string(WEATHER_DESCRIPTION_KEY, mWeatherDescription, 20);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Description: %s", mWeatherDescription);
  mRefreshInterval = persist_read_int(WEATHER_DESCRIPTION_KEY);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ConfigRefreshInterval: %d", mRefreshInterval);
  
}

void in_dropped_handler(AppMessageResult reason, void *context) {
   // incoming message dropped
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Incoming message dropped");
}

void out_sent_handler(DictionaryIterator *sent, void *context) {
   // outgoing message was delivered
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   // outgoing message failed
}

// Handle the start-up of the app
static void do_init(void) {

  // Create our app's base window
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_stack_push(window, true);

  Layer *root_layer = window_get_root_layer(window);
  GRect frame = layer_get_frame(root_layer);
  GFont awesome_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AWESOME_12));
  GFont minecraft_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MINECRAFTIA_24));
  GFont minecraft_font_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MINECRAFTIA_16));

<<<<<<< HEAD:src/retro-time.c
  // Init the text layer used to show the time
  time_layer = text_layer_create(GRect(0, 115, frame.size.w /* width */, 34/* height */));
=======
  // Init Layers
  time_layer = text_layer_create(GRect(0, 63, frame.size.w /* width */, 34/* height */));
  date_layer = text_layer_create(GRect(0, 93, frame.size.w /* width */, 34/* height */));
  connection_layer = text_layer_create(GRect(-2, 2, /* width */ frame.size.w, 34 /* height */));
  battery_layer = text_layer_create(GRect(2, 2, /* width */ frame.size.w, 34 /* height */));
  charge_layer = text_layer_create(GRect(64, 2, /* width */ frame.size.w, 34 /* height */));
  
  // Set Colors, if required
>>>>>>> d85fd0af28b5f066d3538588034dc47ff7c6ab75:src/main.c
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_background_color(time_layer, GColorClear);
  
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_background_color(date_layer, GColorClear);
  
  text_layer_set_background_color(connection_layer, GColorClear);
  text_layer_set_background_color(battery_layer, GColorClear);
  text_layer_set_background_color(charge_layer, GColorClear);
  
    
  #ifdef PBL_COLOR
    text_layer_set_text_color(connection_layer, GColorVividCerulean);
    text_layer_set_text_color(battery_layer, GColorSunsetOrange);
    text_layer_set_text_color(charge_layer, GColorIcterine);
  #else
    text_layer_set_text_color(connection_layer, GColorWhite);
    text_layer_set_text_color(battery_layer, GColorWhite);
    text_layer_set_text_color(charge_layer, GColorWhite);
  #endif
    
  // Init the text layer used to show the time
  text_layer_set_font(time_layer, minecraft_font);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_text(time_layer, " : ");
  
<<<<<<< HEAD:src/retro-time.c
  // Init the text layer used to show the date
  date_layer = text_layer_create(GRect(0, 145, frame.size.w /* width */, 34/* height */));
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_background_color(date_layer, GColorClear);
=======
  // Init the text layer used to show the date  
>>>>>>> d85fd0af28b5f066d3538588034dc47ff7c6ab75:src/main.c
  text_layer_set_font(date_layer, minecraft_font_small);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_text(date_layer, "Mon, Jan 31");
  
  // Init the text layer used to show the temperature
  temp_layer = text_layer_create(GRect(0, 90, frame.size.w /* width */, 34/* height */));
  text_layer_set_text_color(temp_layer, GColorWhite);
  text_layer_set_background_color(temp_layer, GColorClear);
  text_layer_set_font(temp_layer, minecraft_font_small);
  text_layer_set_text_alignment(temp_layer, GTextAlignmentCenter);
  text_layer_set_text(temp_layer, "999°F");

  // Init the text layer used to show bluetooth connection
  text_layer_set_font(connection_layer, awesome_font);
  text_layer_set_text_alignment(connection_layer, GTextAlignmentRight);
  text_layer_set_text(connection_layer, "\uf09e");

  // Init the text layer used to show the battery percentage
<<<<<<< HEAD:src/retro-time.c
  battery_layer = text_layer_create(GRect(2, 2, /* width */ frame.size.w, 34 /* height */));
  #ifdef PBL_COLOR
    text_layer_set_text_color(battery_layer, GColorRed);
  #else
    text_layer_set_text_color(battery_layer, GColorWhite);
  #endif
  text_layer_set_background_color(battery_layer, GColorClear);
=======
>>>>>>> d85fd0af28b5f066d3538588034dc47ff7c6ab75:src/main.c
  text_layer_set_font(battery_layer, awesome_font);
  text_layer_set_text_alignment(battery_layer, GTextAlignmentLeft);
  text_layer_set_text(battery_layer, "\uf004 \uf004 \uf004 \uf004");
  
  // Init the text layer used to show the battery charge state
  text_layer_set_font(charge_layer, awesome_font);
  text_layer_set_text_alignment(charge_layer, GTextAlignmentLeft);
  text_layer_set_text(charge_layer, "\uf004 \uf004 \uf004 \uf004");
  
  // Create the inverter layer
  inverter_layer = inverter_layer_create(frame);

  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  handle_minute_tick(current_time, SECOND_UNIT);

  tick_timer_service_subscribe(SECOND_UNIT, &handle_minute_tick);
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_sent(out_sent_handler);
  app_message_register_outbox_failed(out_failed_handler);
  
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);
  
  APP_LOG(APP_LOG_LEVEL_INFO, "SETTING THEME");
  set_theme();

  layer_add_child(root_layer, text_layer_get_layer(time_layer));
  layer_add_child(root_layer, text_layer_get_layer(date_layer));
  layer_add_child(root_layer, text_layer_get_layer(temp_layer));
  layer_add_child(root_layer, text_layer_get_layer(connection_layer));
  layer_add_child(root_layer, text_layer_get_layer(battery_layer));
  layer_add_child(root_layer, text_layer_get_layer(charge_layer));
  layer_add_child(root_layer, inverter_layer_get_layer(inverter_layer));
}

static void do_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(temp_layer);
  text_layer_destroy(connection_layer);
  text_layer_destroy(battery_layer);
  text_layer_destroy(charge_layer);
  inverter_layer_destroy(inverter_layer);
  window_destroy(window);
}


// The main event/run loop for our app
int main(void) {
  do_init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  app_event_loop();
  do_deinit();
}