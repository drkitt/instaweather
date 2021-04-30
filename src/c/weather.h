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

/* Prototypes */
// Sets up the weather module
void weather_init(void);

// Sets down the weather module
void weather_deinit(void);

// Tells whether there's any saved weather data
bool saved_data_exists(void);

#endif /* end of include guard: WEATHER_H */
