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
// Key to access the weather conditions in peristent storage
#define CONDITIONS_KEY 1
// Size of a string stored in local storage
#define STORED_BUFFER_SIZE 32

// Tells whether there's any saved weather data
bool saved_data_exists(void);

// Gets the temperature from persistent storage
int load_temperature(void);

// Gets the conditions from persistent storage
void load_conditions(char *conditions_buffer, int conditions_buffer_size);

// Saves the temperature to persistent storage
void save_temperature(const int temperature);

// Saves the conditions to persistent storage
void save_conditions(const char *conditions_buffer);

#endif /* end of include guard: WEATHER_STORAGE_H */
