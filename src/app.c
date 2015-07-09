/*
 * main.c
 * Sets up a Window and a TextLayer.
 */

#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_text_layer;
static GFont s_font_30;


void handle_timechange(struct tm *tick_time, TimeUnits units_changed) {
  static char time_buffer[10];
  
  strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", tick_time);
  text_layer_set_text(s_text_layer, time_buffer);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_text_layer = text_layer_create(GRect(5, 70, window_bounds.size.w - 5, window_bounds.size.h));
  
  //setting the font
  s_font_30 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_nixie_30));
  text_layer_set_font(s_text_layer, s_font_30);
  
  //text align
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  
  //set the text of the text layer
  text_layer_set_text(s_text_layer, "Example TextLayer!");
  
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechange);
  //tick_timer_service_subscribe(TimeUnits tick_units, TickHandler handler);
  
  //Adding text layer to the window layer
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_text_layer);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
  text_layer_destroy(s_text_layer);
  fonts_unload_custom_font(s_font_30);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}