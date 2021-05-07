/*
Contains the interface for the weather module, which fetches the weather using
the phone's internet connection and saves it to persistent storage.
Generally used by the background worker, but may be used by the foreground app
if there's no weather info saved.
*/

#ifndef WEATHER_H
#define WEATHER_H

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
int get_temperature(void);

// Gets the conditions from persistent storage
void get_conditions(char *conditions_buffer, int conditions_buffer_size);

#endif /* end of include guard: WEATHER_H */
