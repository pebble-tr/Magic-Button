#include <pebble.h>
#include "cl_progressbar.h"
#define DELAY 50
static Window* window;
static ProgressBarLayer *pb_layer;
static AppTimer *timer;
static TextLayer *text_layer;

static void timer_callback(void *context){
  //Add and/or wrap
  int new_progress = progressbar_layer_get_progress(pb_layer) + 1;
  if(new_progress > 100){
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
    text_layer_set_text(text_layer, "Everything is OK now");

    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(text_layer));

  }
  
  //Set new progress value
  progressbar_layer_set_progress(pb_layer, new_progress);
  //Repeat
  timer = app_timer_register(DELAY, (AppTimerCallback)timer_callback, NULL);
}

static void window_load(Window *window){
  //Add a new ProgressBarLayer
  pb_layer = progressbar_layer_create(GRect(5, 60, 134, 15));
  layer_add_child(window_get_root_layer(window), progressbar_layer_get_layer(pb_layer));
  //Register progression
  timer = app_timer_register(DELAY, (AppTimerCallback)timer_callback, NULL);
}

static void window_unload(Window *window){
  //Destroy ProgressBarLayer
  progressbar_layer_destroy(pb_layer);
}

static void init(){
  //Create Window
  window = window_create();
  WindowHandlers handlers = {
  .load = window_load,
  .unload = window_unload};
  window_set_window_handlers(window, (WindowHandlers) handlers);
  window_stack_push(window, true);
}

static void deinit(){
  //Destroy Window
  window_destroy(window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}
