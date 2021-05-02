/*
Contains the code to be ran directly on the watch while the app is in focus
*/

/* Included files */
// Standard Pebble functions and types
#include <pebble.h>
// Weather saving and loading
#include "weather.h"
// Displaying the weather
#include "weather_display_window.h"

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

    APP_LOG(
        APP_LOG_LEVEL_DEBUG,
        "Done initializing, pushed window: %p",
        main_window
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
    }

    // Create the main window
    main_window = weather_display_window_create();
    const bool animated = true;
    window_stack_push(main_window, animated);

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
    window_destroy(main_window);
    weather_deinit();
}
