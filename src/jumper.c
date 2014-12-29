#include "pebble.h"
#define ACCEL_STEP_MS 50

static Window *window;

static GRect window_frame;
static TextLayer *text_layer;
static int i;
static int prevx = -1, prevprevx = -1, x = -1;
static uint64_t prevtime = 0;
static int jumptime = 1;



static AppTimer *timer;

static void timer_callback(void *data) {
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
  accel_service_peek(&accel);
  char* str;
  prevprevx = prevx;
  prevx = x;
  x = accel.x;
  if(prevprevx != -1)
    {
      if(prevx >= 1400 && prevprevx - prevx >= 0 && x - prevx <= 0)
      {
        i++;
        if(prevtime != 0)
          {
           jumptime = accel.timestamp - prevtime;
          }
        prevtime = accel.timestamp;
      }
  }
  
  str = malloc(50);
  
  if(jumptime > 1)
    {
  snprintf(str, 50, "%d\n%d jmp/min", i, 60000 / jumptime);
  }
  else
    {
    snprintf(str, 50, "%d\n [X] jmp/min", i);
  }
  //app_log(APP_LOG_LEVEL_DEBUG,"", 0, "%d;%d;%d" , accel.x, accel.y, accel.z);
  text_layer_set_text(text_layer, str);
  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
  free(str);
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
   i=0;
  jumptime = 1;
 }

void config_provider(void *context) {
   window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
 }

static void init(void) {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorWhite);
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  text_layer = text_layer_create((GRect){ .origin = { 0, 30 }, .size = bounds.size });
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  accel_data_service_subscribe(0, NULL);
  window_set_click_config_provider(window, config_provider);
  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

static void deinit(void) {
  accel_data_service_unsubscribe();
  text_layer_destroy(text_layer);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
