#include <pebble.h>

/* Static variables */
// Displays the entire app
static Window *s_window;
// Displays the temperature
static TextLayer *s_temp_layer;

/*
Called when the app's main window loads

Parameters:
    window: Pointer to the window-to-be
*/
static void main_window_load(Window *window) {
    /* Local variables */
    // The window's main layer. Other elements (text, images, etc.) will have
    // their own layers that are children of this one.
    Layer *window_layer = window_get_root_layer(window);
    // The main layer's bounds
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

/* Initializes the app by creating the main window */
static void init(void) {
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload,
    });
    const bool animated = true;
    window_stack_push(s_window, animated);
}

/* Initializes the app by creating the main window */
static void deinit(void) {
    window_destroy(s_window);
}

/* Main entry point for the app */
int main(void) {
    init();

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

    app_event_loop();
    deinit();
}
