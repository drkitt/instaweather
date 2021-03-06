/*
Contains the code for the background worker, which periodically fetches the
weather
*/

/* Included files */
// Functions and types for background workers
#include <pebble_worker.h>

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
    // Register with the tick timer service in order to update periodically
    tick_timer_service_subscribe(HOUR_UNIT, tick_handler);
}

/*
Deinitializes the worker
*/
static void deinit(void) {
    tick_timer_service_unsubscribe();
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
    APP_LOG(APP_LOG_LEVEL_INFO, "Background worker running tick handler");

    worker_launch_app();
}
