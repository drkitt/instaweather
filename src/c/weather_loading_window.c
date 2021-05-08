/*
Contains the implementation for the weather display window.
This window opens up a connection to the Pebble phone app, loads the weather,
then closes.
*/

/* Included files */
// Weather loading window interface
#include "weather_loading_window.h"
// Weather fetching
#include "weather_fetch.h"
// Weather saving and loading
#include "weather_storage.h"

/* Static variables */
// Displays a status message to the user
static TextLayer *status_layer;

/* Static prototypes */
// Callback for loading the window
static void load(Window *window);
// Callback for unloading the window
static void unload(Window *window);
// Callback for recieving weather info
static void on_fetch(
    int temperature, char *conditions_buffer, int conditions_buffer_size
);

/*
Sets up a window and returns a pointer to it.
Note that this function doesn't add the new window to the window stack; the
caller must manage the returned window itself.
*/
Window *weather_loading_window_create(void) {
    Window *new_window = window_create();
    window_set_window_handlers(new_window, (WindowHandlers) {
        .load = load,
        .unload = unload,
    });

    return new_window;
}

/*
Called when the window loads

Parameters:
    window: Pointer to the window-to-be
*/
static void load(Window *window) {
    // Get the main layer's bounds
    Layer *window_layer = window_get_root_layer(window);
    const GRect bounds = layer_get_bounds(window_layer);

    // Set up status message
    status_layer = text_layer_create(GRect(0, 36, bounds.size.w, 60));
    text_layer_set_text(status_layer,
        "Getting updated weather info.\n\nPlease wait..."
    );
    text_layer_set_text_alignment(status_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(status_layer));
    text_layer_enable_screen_text_flow_and_paging(status_layer, 0);

    // The "weather loading" part
    fetch_weather(window, on_fetch);
}

/*
Called when the window unloads

Parameters:
    window: Pointer to the window-to-not-be
*/
static void unload(Window *window) {
    text_layer_destroy(status_layer);
}

/*
Called when the weather info has arrived from the internet. Saves the incoming
weather info to local storage and then closes this window

Parameters:
    temperature: The temperature at the user's current location
    conditions_buffer: String containing the weather conditions
    conditions_buffer_size: Length of conditions_buffer, including the null
        terminator
*/
static void on_fetch(
    int temperature, char *conditions_buffer, int conditions_buffer_size
) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "I caught it!");
}
