/*
 * main.c
 * Sets up a Window and a TextLayer.
 */

#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_text_layer;
static Layer *s_canvas_layer;
//static Layer *s_path_layer;
//static Layer *place1_layer;
static GFont s_font_40;
static char time_buffer[10];

static GDrawCommandImage *s_command_image;
// GPath describes the shape
//static GPath *s_path;
// static GPathInfo PATH_INFO = {
//   .num_points = 4,
//   .points = (GPoint []) {{120, 10}, {20, 46}, {40, 50}, {50, 115}}
// };

static void update_proc(Layer *layer, GContext *ctx) {
  // Set the color using RGB values


  // Draw the filled shape in above color
  //gpath_draw_filled(ctx, s_path);

  // Place image in the center of the Window
  GSize img_size = gdraw_command_image_get_bounds_size(s_command_image);
  GPoint origin = GPoint(72 - (img_size.w / 2), 84 - (img_size.h / 2));

  // If the image was loaded successfully...
  if (s_command_image) {
    // Draw it
    gdraw_command_image_draw(ctx, s_command_image, origin);
  }
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


   
  GRect bounds = layer_get_bounds(window_layer);

  // Load the image and check it was succcessful
  s_command_image = gdraw_command_image_create_with_resource(RESOURCE_ID_IMAGE_GATCHA);
  if (!s_command_image) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Image is NULL!");
  }

  // Create canvas Layer and set up the update procedure
  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, update_proc);
  layer_add_child(window_layer, s_canvas_layer);



  //Adding text layer to the window layer
  //layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  //layer_destroy(s_path_layer);
  //gpath_destroy(s_path);
  // Destroy the image
  layer_destroy(s_canvas_layer);
  gdraw_command_image_destroy(s_command_image);
  text_layer_destroy(s_text_layer);
}


static void init() {
  // Create main Window
  s_main_window = window_create();

  window_set_background_color(s_main_window, GColorJazzberryJam);

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
