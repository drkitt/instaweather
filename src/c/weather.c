/*
Contains the weather module's implementation
*/

/* Included files */
// Weather module interface
#include "weather.h"
// Standard Pebble functions and types
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
    #define MIN(x, y) (((x) < (y)) ? (x) : (y))
    const int INBOX_SIZE = MIN(64, APP_MESSAGE_INBOX_SIZE_MINIMUM);
    const int OUTBOX_SIZE = MIN(4, APP_MESSAGE_OUTBOX_SIZE_MINIMUM);
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
    return persist_exists(TEMPERATURE_KEY) && persist_exists(CONDITIONS_KEY);
}

/*
Gets the temperature from persistent storage
    Returns: The temperature :]
        ...Or -1 if there's no temperature data saved
        (shoulda called saved_data_exists first!)
*/
int get_temperature(void) {
    return persist_exists(TEMPERATURE_KEY) ? persist_read_int(TEMPERATURE_KEY)
        : -1;
}

/*
Gets the conditions from persistent storage
Parameters:
    conditions_buffer: String that this function will write the weather
        conditions from persistent storage to. If no such data exists, it
        writes an empty string to this string instead
    conditions_buffer_size: Length of conditions_buffer, including the null
        terminator
*/
void get_conditions(char *conditions_buffer, int conditions_buffer_size) {
    if (persist_exists(CONDITIONS_KEY)) {
        persist_read_string(
            CONDITIONS_KEY, conditions_buffer, conditions_buffer_size);
    }
    else {
        conditions_buffer[0] = '\0';
    }
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
    const Tuple *temperature_tuple = dict_find(
        iterator, MESSAGE_KEY_TEMPERATURE);
    const Tuple *conditions_tuple = dict_find(
        iterator, MESSAGE_KEY_CONDITIONS);

    // If all data is available, store it
    if (temperature_tuple && conditions_tuple) {
        // Temperature
        const int temperature = (int)temperature_tuple->value->int32;
        persist_write_int(TEMPERATURE_KEY, temperature);

        // Conditions
        static char conditions_buffer[CONDITIONS_BUFFER_SIZE];
        snprintf(
            conditions_buffer,
            CONDITIONS_BUFFER_SIZE,
            "%s",
            conditions_tuple->value->cstring
        );
        persist_write_string(
            CONDITIONS_KEY, conditions_buffer);
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
