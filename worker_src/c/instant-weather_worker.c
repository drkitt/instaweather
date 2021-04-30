/*
Contains the code for the background worker, which periodically fetches the
weather
*/

/* Included files */
// Functions and types for background workers
#include <pebble_worker.h>

// Temporary stuff to test out peristent storage
static int counter = 0;
typedef struct TEST {
    int incrementer;
    int decrementer;
} test_struct;
test_struct *test;

/* Prototypes */
// Initializes the worker
static void init(void);
// Deinitializes the worker
static void deinit(void);
// Updates weather info
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);

/*
Main entry point for the worker
*/
int main(void) {
    init();
    worker_event_loop();
    deinit();
}

/*
Initializes the worker
*/
static void init(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Background worker time >:)");

    // Register with the tick timer service in order to update periodically
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

/* Deinitializes the worker */
static void deinit(void) {

}

/*
Called periodically according to the tick timer service. Updates wheather
info.
Parameters (which are mostly just here to satisfy the tick timer service's
interface):
    tick_time: Contains the current time
    units_changed: Constant denoting which unit has changed (hour, minute, or
    second).
*/
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "hey what's up it's the background worker");

    if (persist_exists(0)) {
        counter = persist_read_int(0);
    }
    else {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "No persistent data for counter found");
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", counter);
    counter++;
    persist_write_int(0, counter);

    test = malloc(sizeof(test_struct));
    if (persist_exists(1)) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Reading struct (current value: %d, %d)", test->incrementer, test->decrementer);
        persist_read_data(1, test, sizeof(test_struct));
    }
    else {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "No persistent data for test_struct found");
        test->incrementer = 0;
        test->decrementer = 0;
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d, %d", test->incrementer, test->decrementer);
    test->incrementer++;
    test->decrementer--;
    persist_write_data(1, test, sizeof(test_struct));
    
    worker_launch_app();
}
