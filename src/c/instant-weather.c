/*
Contains the code to be ran directly on the watch while the app is in focus
*/

/* Included files */
// Standard Pebble functions and types
#include <pebble.h>
// Displaying the weather
#include "weather_display_window.h"
// Loading screen
#include "weather_loading_window.h"

/* Static variables */
// Displays the entire app
static Window *main_window;

/* Prototypes */
// Initializes the app
static void init(void);
// Cleans up after the app
static void deinit(void);

/*
Main entry point for the app
*/
int main(void) {
    init();
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
        APP_LOG(APP_LOG_LEVEL_INFO, "Launched by worker");
        main_window = weather_loading_window_create();
    }
    else {
        // If the app was launched by the user, just display the weather :)
        APP_LOG(
            APP_LOG_LEVEL_INFO,
            "Launched somehow other than by worker (most likely by user)"
        );
        main_window = weather_display_window_create();
    }

    // Create the main window
    window_stack_push(main_window, true);

    // Start the background worker if it's not running already
    const AppWorkerResult result = app_worker_launch();
    APP_LOG(
        APP_LOG_LEVEL_INFO,
        "Background worker launch returned value %d",
        result
    );
}

/*
Deinitializes the app by destroying the main window
*/
static void deinit(void) {
    window_destroy(main_window);
}
