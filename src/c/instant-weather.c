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
    // Due to the way the background weather storage works, the execution here
    // can take radically different paths based on how the app was launched.
    if (launch_reason() == APP_LAUNCH_WORKER) {
        // If the app was launched by the background worker, we want to fetch
        // new weather data and exit as quickly as possible, while informing the
        // user about what it's doing.
        // (note that I originally planned to have the background worker fetch
        // the data, but the Pebble C API turned out to be very strict about
        // letting background processes communicate with the phone! Go figure.)
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Launched by worker");
    }
    else {
        // If the app was launched by the user, just display the weather :)
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Launched somehow else");

        if (saved_data_exists()) {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather exists");
            int temperature = persist_read_int(TEMPERATURE_KEY);
            char conditions_buffer[CONDITIONS_BUFFER_SIZE];
            persist_read_string(
                CONDITIONS_KEY, conditions_buffer, CONDITIONS_BUFFER_SIZE);
            APP_LOG(
                APP_LOG_LEVEL_DEBUG,
                "Temperature is %d and conditions are %s. Isn't that neat?",
                temperature,
                conditions_buffer
            );
        }
        else {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather exists... not!");
        }
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
    const AppWorkerResult result = app_worker_launch();
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
    const GRect bounds = layer_get_bounds(window_layer);

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
