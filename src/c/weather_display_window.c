/*
Contains the implementation for the weather display window
*/

/* Included files */
// Weather display window interface
#include "weather_display_window.h"
// Weather saving and loading
#include "weather.h"

/* Static variables */
// Displays the temperature
static TextLayer *temperature_layer;

/* Static prototypes */
// Callback for loading the window
static void load(Window *window);
// Callback for unloading the window
static void unload(Window *window);

/*
Sets up a window and returns a pointer to it.
Note that this function doesn't add the new window to the window stack; the
caller must manage the returned window itself.
*/
Window *weather_display_window_create(void) {
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

    // Load or fetch weather data
    if (saved_data_exists()) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather exists");
        int temperature = get_temperature();
        char conditions_buffer[CONDITIONS_BUFFER_SIZE];
        get_conditions(conditions_buffer, CONDITIONS_BUFFER_SIZE);
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

    // Set up temperature display
    temperature_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
    text_layer_set_text(temperature_layer, "Hello world!");
    text_layer_set_text_alignment(temperature_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(temperature_layer));
}

/*
Called when the window unloads

Parameters:
    window: Pointer to the window-to-not-be
*/
static void unload(Window *window) {
    text_layer_destroy(temperature_layer);
}
