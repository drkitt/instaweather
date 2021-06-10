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

/* Constants */
// Time between attempted fetches
#define TIMEOUT 1000 * SECONDS_PER_MINUTE

/* Static variables */
// Displays a status message to the user
static TextLayer *status_layer;
// Keeps track of how much time has passed since starting the weather fetch and
// retries the fetch if necessary
static AppTimer *retry_timer;

/* Static prototypes */
// Callback for loading the window
static void load(Window *window);
// Callback for unloading the window
static void unload(Window *window);
// Callback for recieving weather info
static void on_fetch(
    Window *window,
    const int temperature,
    const char *conditions_buffer,
    const int conditions_id
);
// Callback for updating the app glance
static void update_app_glance(
    AppGlanceReloadSession *session, size_t limit, void *context);
// Callback for retrying the weather fetch
static void on_retry(void *data);

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
    window_set_background_color(window, GColorBlack);
    status_layer = text_layer_create(GRect(0, 36, bounds.size.w, 60));
    text_layer_set_text_color(status_layer, GColorWhite);
    text_layer_set_background_color(status_layer, GColorClear);
    text_layer_set_text(status_layer,
        "Getting updated weather info.\n\nPlease wait..."
    );
    text_layer_set_text_alignment(status_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(status_layer));
    text_layer_enable_screen_text_flow_and_paging(status_layer, 0);

    // The "weather loading" part
    fetch_weather(window, on_fetch);
    // If we don't hear back soon enough, try again
    retry_timer = app_timer_register(TIMEOUT, on_retry, window);
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
    conditions_id: Integer denoting the conditions
*/
static void on_fetch(
    Window *window,
    const int temperature,
    const char *conditions_buffer,
    const int conditions_id
) {
    // Save the loaded data
    save_temperature(temperature);
    save_conditions_buffer(conditions_buffer);
    save_conditions_id(conditions_id);

    // Also display it on the app launcher
    char app_glance_buffer[STORED_BUFFER_SIZE];
    snprintf(
        app_glance_buffer,
        STORED_BUFFER_SIZE,
        "%d°C / %s",
        temperature,
        conditions_buffer
    );
    app_glance_reload(update_app_glance, app_glance_buffer);

    // Self-destruct
    app_timer_cancel(retry_timer);
    window_stack_remove(window, true);
}

/*
Called when updating the app glance, which is a text field below the app icon

Parameters:
    session: Pointer to object used to tell whether we can currently write
        the app glance
    limit: The number of entries that can be added to the app glance
    context: Pointer to application data from when the callback was registered.
        In this case, it contains the string to write to the glance.
*/
static void update_app_glance(
    AppGlanceReloadSession *session, size_t limit, void *context) {

    // Avoid writing to app glance if that functionality is unsupported on the
    // watch
    if (limit < 1) return;

    // Cast the context to a string
    const char *message = context;

    // Create the AppGlanceSlice
    const AppGlanceSlice entry = (AppGlanceSlice) {
        .layout = {
            .subtitle_template_string = message
        },
        // Expire the glance after an hour. Since the weather info is updated
        // hourly, this being blank is a clue to the user that something has
        // gone wrong!
        .expiration_time = time(NULL) + SECONDS_PER_HOUR
    };

    // Add the slice and check the result
    const AppGlanceResult result = app_glance_add_slice(session, entry);
    if (result != APP_GLANCE_RESULT_SUCCESS) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "AppGlance error: %d", result);
    }
}

/*
Called when the weather fetch hasn't worked for a minute

Parameters:
    data: Data specified when the callback registered. In this case, it should
        be a pointer to the loading window.
*/
static void on_retry(void *data) {
    // Check whether we need to retry. If the topmost window is the loading
    // window, that means that the weather data hasn't been successfully
    // fetched.
    if (window_stack_get_top_window() == data) {
        text_layer_set_text(status_layer,
            "Couldn't update weather info.\n"
            "Trying again...\n"
            "\n"
            "You may need to close this app\n"
            "and restart the Pebble app on your phone."
        );
        fetch_weather(data, on_fetch);
    }
}
