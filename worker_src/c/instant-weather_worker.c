/*
Contains the code for the background worker, which periodically fetches the
weather
*/

/* Included files */
// Functions and types for background workers
#include <pebble_worker.h>

/* Initializes the worker */
static void init() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Background worker time >:)");
}

/* Deinitializes the worker */
static void deinit() {

}

/* Main entry point for the worker */
int main(void) {
    init();
    worker_event_loop();
    deinit();
}
