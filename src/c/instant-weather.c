/*
Contains the code to be ran directly on the watch while the app is in focus
*/

/* Included files */
// Standard Pebble functions and types
#include <pebble.h>
// Weather-related functionality
#include "weather.h"

/* Static variables */
// Displays the entire app
static Window *s_window;
// Displays the temperature
static TextLayer *s_temp_layer;

/* Prototypes */
// Initializes the app
static void init(void);
// Cleans up after the app
static void deinit(void);
// Callback for loading the app's main window
static void main_window_load(Window *window);
// Callback for unloading the app's main window
static void main_window_unload(Window *window);

/*
Main entry point for the app
*/
int main(void) {
    init();

    APP_LOG(
        APP_LOG_LEVEL_DEBUG,
        "Done initializing, pushed window: %p",
        s_window
    );

    app_event_loop();
    deinit();
}

/*
Initializes the app by creating the main window and starting the background
worker
*/
static void init(void) {
    if (launch_reason() == APP_LAUNCH_WORKER) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Launched by worker");
    }
    else {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Launched somehow else");
    }

    // Create the main window
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload,
    });
    const bool animated = true;
    window_stack_push(s_window, animated);

    // Start the background worker if it's not running already
    AppWorkerResult result = app_worker_launch();
    APP_LOG(
        APP_LOG_LEVEL_DEBUG,
        "Background worker launch returned value %d",
        result
    );

    weather_init();
}

/*
Deinitializes the app by destroying the main window
*/
static void deinit(void) {
    window_destroy(s_window);
    weather_deinit();
}


/*
Called when the app's main window loads

Parameters:
    window: Pointer to the window-to-be
*/
static void main_window_load(Window *window) {
    // Get the main layer's boundsf
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // Set up temperature display
    s_temp_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
    text_layer_set_text(s_temp_layer, "Hello world!");
    text_layer_set_text_alignment(s_temp_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(s_temp_layer));
}

/*
Called when the app's main window unloads

Parameters:
    window: Pointer to the window-to-not-be
*/
static void main_window_unload(Window *window) {
    text_layer_destroy(s_temp_layer);
}
