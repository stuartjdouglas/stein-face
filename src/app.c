/*
 * main.c
 * Sets up a Window and a TextLayer.
 */

#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_text_layer;
static Layer *s_path_layer;
//static Layer *place1_layer;
static GFont s_font_40;
static char time_buffer[10];


// GPath describes the shape
static GPath *s_path;
static GPathInfo PATH_INFO = {
  .num_points = 4,
  .points = (GPoint []) {{120, 10}, {20, 46}, {40, 50}, {50, 115}}
};

static void layer_update_proc(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorWhite);

  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_path);
}


void handle_timechange(struct tm *tick_time, TimeUnits units_changed) { 
  strftime(time_buffer, sizeof(time_buffer), "%H:%M", tick_time);
  text_layer_set_text(s_text_layer, time_buffer);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  GRect time_layer_bounds = GRect(5, 115, window_bounds.size.w - 5, window_bounds.size.h);
  s_text_layer = text_layer_create(time_layer_bounds);
  
//  GRect object_layer_bounds = GRect(5, 5, window_bounds.size.w - 5, window_bounds.size.h);
//  place1_layer = text_layer_create(object_layer_bounds);
  
  //setting the font
  s_font_40 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_nixie_45));
  text_layer_set_font(s_text_layer, s_font_40);
  
  //text align
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  
  //set the text of the text layer
  text_layer_set_text(s_text_layer, "00:00:00");
  
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechange);
  //tick_timer_service_subscribe(TimeUnits tick_units, TickHandler handler);
  
  
  // Create GPath object
  s_path = gpath_create(&PATH_INFO);

  // Create Layer that the path will be drawn on
  s_path_layer = layer_create(window_bounds);
  layer_set_update_proc(s_path_layer, layer_update_proc);
  layer_add_child(window_layer, s_path_layer);
  
  
  
  //setting background colour
  
  #ifdef PBL_COLOR
    window_set_background_color(s_main_window, GColorBlack);
    text_layer_set_background_color(s_text_layer, GColorBlack);
    text_layer_set_text_color(s_text_layer, GColorOrange);
//layer_set(place1_layer, GColorCeleste);
  
  #else
    window_set_background_color(s_main_window, GColorBlack);
  text_layer_set_text_color(s_text_layer, GColorBlack);
  #endif
  
  //Adding text layer to the window layer
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  layer_destroy(s_path_layer);
  gpath_destroy(s_path);
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
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}