/*
Contains the weather module's implementation
*/

/* Included files */
// Weather module interface
#include "weather.h"
// Pebble standard library
#include "pebble.h"

/* Static prototypes */
// Callback for receiving an AppMessage
static void inbox_received_callback(
    DictionaryIterator *iterator, void *context);
// Callback for dropping an AppMessage
static void inbox_dropped_callback(AppMessageResult reason, void *context);
// Callback for sending an AppMessage
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
// Callback for failing to send an AppMessage
static void outbox_failed_callback(
    DictionaryIterator *iterator, AppMessageResult reason, void *context);

/* Types */
// Contains all of the info to be stored in and retrieved from local storage
typedef struct {
    int temperature;
} stored_data;

/*
Sets up the weather module
*/
void weather_init(void) {
    // Register callbacks
    app_message_register_inbox_received(inbox_received_callback);
    // Open AppMessage (this is best done immediately after registering the
    // inbox receive callback, to make sure that we don't miss anything)
    const int INBOX_SIZE = 128;
    const int OUTBOX_SIZE = 128;
    app_message_open(INBOX_SIZE, OUTBOX_SIZE);
    // Finish registering callbacks
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);
}

/*
Sets down the weather module
*/
void weather_deinit(void) {

}

/*
Tells whether there's any saved weather data
*/
bool saved_data_exists(void) {
    // It'll be like this until the persistence stuff is in the main branch
    return false;
}


/*
Callback for receiving an AppMessage
Parameters:
    iterator: Points to the contents of the received message
    context: Pointer to application data from when the callback was registered
*/
static void inbox_received_callback(
    DictionaryIterator *iterator, void *context) {

    // Use the given iterator to load the data from the dictionary to the tuples
    Tuple *temperature_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
    Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);

    // If all data is available, store it
    if (temperature_tuple && conditions_tuple) {
        const int temperature = (int)temperature_tuple->value->int32;
        static char conditions_buffer[32];
        snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);

        APP_LOG(APP_LOG_LEVEL_INFO, "%d %s", temperature, conditions_buffer);
    }
}

/*
Callback for dropping an AppMessage
Parameters:
reason: Why the message was dropped
context: Pointer to application data from when the callback was registered
*/
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

/*
Callback for sending an AppMessage
Parameters:
iterator: Points to the contents of the sent message
context: Pointer to application data from when the callback was registered
*/
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

/*
Callback for failing to send an AppMessage
Parameters:
iterator: Points to the contents of the non-sent message
reason: Why the message didn't send
context: Pointer to application data from when the callback was registered
*/
static void outbox_failed_callback(
    DictionaryIterator *iterator, AppMessageResult reason, void *context) {

    APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}
