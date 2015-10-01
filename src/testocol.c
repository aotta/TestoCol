#include <pebble.h>
#include <num2words.h>

#define BUFFER_SIZE 86	

// keys for app message and storage
enum Settings {setting_ledcolorF = 1, 
							 setting_ledcolorB
						   };

static GColor Colore;
static GColor bkColore;

static enum SettingLedcolorF { lcf, ledf_count } ledcolorF;
static enum SettingLedcolorB { lcb, ledb_count } ledcolorB;


static AppSync app;
static uint8_t buffer[256];
  

static Layer *root_layer;

static struct CommonWordsData {
  TextLayer *label;
  Window *window;
  char buffer[BUFFER_SIZE];
} s_data;

static GFont fontbold;
static GFont fontnormal;


static void app_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void* context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "app error %d", app_message_error);
}

static void setdefcolor() {
//APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting colori default");
  Colore = GColorFromRGB(255,0, 0);
	bkColore = GColorFromRGB(0, 0, 0);	
}

static void update_time(struct tm* t) {
  time_to_words(t->tm_hour, t->tm_min, s_data.buffer, BUFFER_SIZE);
  
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Ora: %s", s_data.buffer);
	
	window_set_background_color(s_data.window, bkColore);
	text_layer_set_background_color(s_data.label, bkColore);
  text_layer_set_text_color(s_data.label, Colore);
	text_layer_set_overflow_mode(s_data.label,GTextOverflowModeTrailingEllipsis);
	// try also:GTextOverflowModeWordWrap 
	// try also: GTextOverflowModeTrailingEllipsis  
	// try also: GTextOverflowModeFill
	text_layer_set_text(s_data.label, s_data.buffer);
	//text_layer_set_text(s_data.label2, s_data.buffer2);
}

static void update_screen(void) {
	 time_t now = time(NULL); 
	 struct tm *t = localtime(&now);
   update_time(t);
}


static void tuple_changed_callback(const uint32_t key, const Tuple* tuple_new, const Tuple* tuple_old, void* context) {
  //  we know these values are uint8 format
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Tuple changed");
  int value = tuple_new->value->uint8;
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY: %lu  Value: %i",key,value);
  switch (key) {
 	  case setting_ledcolorF:
		  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting_ledcolorF: %i", value);
			ledcolorF = value;
			Colore.argb =  ((uint8_t)(0xC0|ledcolorF));
			break;
		case setting_ledcolorB:
    	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting_ledcolorB: %i", value);
		  ledcolorB = value;
			bkColore.argb = ((uint8_t)(0xC0|ledcolorB));
		  break;
	}
	if (gcolor_equal(Colore, bkColore)) {
				  setdefcolor();	
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Resetto i colori a default");
			}
	 
	update_screen();
}


static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
 	update_time(tick_time);
}

static void do_init(void) {
  
	
//  app communication
  Tuplet tuples[] = {
		TupletInteger(setting_ledcolorF, ledcolorF),
		TupletInteger(setting_ledcolorB, ledcolorB)
  };
  
	app_message_open(160, 160);
  app_sync_init(&app, buffer, sizeof(buffer), tuples, ARRAY_LENGTH(tuples),
                tuple_changed_callback, app_error_callback, NULL);
	
	
	s_data.window = window_create();
  const bool animated = true;
  window_stack_push(s_data.window, animated);

  window_set_background_color(s_data.window, bkColore);
  //fontbold = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
  fontnormal = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_42));
  root_layer = window_get_root_layer(s_data.window);
  GRect frame = layer_get_frame(root_layer);

  s_data.label = text_layer_create(GRect(8, 5, frame.size.w-8, frame.size.h-5));
  text_layer_set_background_color(s_data.label, bkColore);
  text_layer_set_text_color(s_data.label, Colore);
  text_layer_set_font(s_data.label, fontnormal);
  layer_add_child(root_layer, text_layer_get_layer(s_data.label));
	
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_time(t);

  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
}

static void do_deinit(void) {
  app_sync_deinit(&app);
	window_destroy(s_data.window);
  text_layer_destroy(s_data.label);
  fonts_unload_custom_font(fontbold);
  fonts_unload_custom_font(fontnormal);	
}

int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}


