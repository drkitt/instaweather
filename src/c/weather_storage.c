/*
Contains the weather storage module's implementation
*/

/* Included files */
// Weather module interface
#include "weather_storage.h"
// Standard Pebble functions and types
#include "pebble.h"

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
int load_temperature(void) {
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
void load_conditions(char *conditions_buffer, int conditions_buffer_size) {
    if (persist_exists(CONDITIONS_KEY)) {
        persist_read_string(
            CONDITIONS_KEY, conditions_buffer, conditions_buffer_size);
    }
    else {
        // Like `return ""` but for nerds
        conditions_buffer[0] = '\0';
    }
}

/*
Saves the temperature to persistent storage
Parameters:
    temperature: The value to save
*/
void save_temperature(const int temperature) {
    persist_write_int(TEMPERATURE_KEY, temperature);
}

/*
Saves the conditions to parsistent storage
Parameters:
    conditions_buffer: String to write to persistent storage

    Shockingly, we don't have to provide the string length; the function we use
        here has a maximum length defined by the Pebble API
*/
void save_conditions(const char *conditions_buffer) {
    persist_write_string(CONDITIONS_KEY, conditions_buffer);
}
