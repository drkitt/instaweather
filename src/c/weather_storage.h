/*
Contains the interface for the weather storage module, which takes care of
bringing weather data in and out of persistent storage.
*/

#ifndef WEATHER_STORAGE_H
#define WEATHER_STORAGE_H

/* Included files */
// Boolean type
#include <stdbool.h>

/* Constants */
// Key to read and write the temperature in persistent storage
#define TEMPERATURE_KEY 0
// Key to access the weather conditions in string form in peristent storage
#define CONDITIONS_BUFFER_KEY 1
// Key to access the id of the weather conditions in peristent storage
#define CONDITIONS_ID_KEY 2
// Size of a string stored in local storage
#define STORED_BUFFER_SIZE 32

/* Prototypes */
// Tells whether there's any saved weather data
bool saved_data_exists(void);
// Gets the temperature from persistent storage
int load_temperature(void);
// Gets the conditions buffer from persistent storage
void load_conditions_buffer(
    char *conditions_buffer, int conditions_buffer_size);
// Gets the conditions id from persistent storage
int load_conditions_id();
// Saves the temperature to persistent storage
void save_temperature(const int temperature);
// Saves the conditions buffer to persistent storage
void save_conditions_buffer(const char *conditions_buffer);
// Saves the conditions id to persistent storage
void save_conditions_id(const int conditions_id);

#endif
