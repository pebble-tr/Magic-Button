#include <pebble.h>
#include "cl_progressbar.h"
#define DELAY 50

static Window *window;
static TextLayer *text_layer;
static ProgressBarLayer *pb_layer;
static AppTimer *timer;
GBitmap *past_bitmap;
BitmapLayer *past_layer;

//static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "OK");
//}

static void timer_callback(ClickRecognizerRef recognizer, void *context){
  //Add and/or wrap
  int new_progress = progressbar_layer_get_progress(pb_layer) + 1;
  if(new_progress > 100){
    Layer *window_layer = window_get_root_layer(window);

    text_layer = text_layer_create(GRect(3,90,140,15));
    text_layer_set_text(text_layer, "Everything is OK now!!!");

    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(text_layer));

  }
 //Set new progress value
  progressbar_layer_set_progress(pb_layer, new_progress);
  //Repeat
  timer = app_timer_register(DELAY, (AppTimerCallback)timer_callback, NULL);
}

static void click_config_provider(void *context) {
  //Add a new ProgressBarLayer
  pb_layer = progressbar_layer_create(GRect(5, 60, 134, 15));
  layer_add_child(window_get_root_layer(window), progressbar_layer_get_layer(pb_layer));
  //Register progression
  //timer = app_timer_register(DELAY, (AppTimerCallback)timer_callback, NULL);

  window_single_click_subscribe(BUTTON_ID_UP, timer_callback);
}

static void window_load(Window *window) {
  //Layer *window = window_get_root_layer(window);
  text_layer = text_layer_create(GRect(0, 100, 144, 168));
  past_bitmap = gbitmap_create_with_resource(RESOURCE_ID_OK);
  past_layer = bitmap_layer_create(GRect(3,3,144,50));

  bitmap_layer_set_bitmap(past_layer, past_bitmap);
  //layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(past_layer));

  //Add a new ProgressBarLayer
  //pb_layer = progressbar_layer_create(GRect(5, 60, 134, 15));
  //layer_add_child(window_get_root_layer(window), progressbar_layer_get_layer(pb_layer));
  //Register progression
  //timer = app_timer_register(DELAY, (AppTimerCallback)timer_callback, NULL);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  gbitmap_destroy(past_bitmap);
  bitmap_layer_destroy(past_layer);
  progressbar_layer_destroy(pb_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
}

static void deinit(void) {
  window_destroy(window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

