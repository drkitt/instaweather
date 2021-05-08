/*
Contains the interface for the weather fetch module, which - you won't believe
this - fetches the weather! Fetches it from the internet using the watch's
bluetooth connection with the phone, specifically.
*/

#ifndef WEATHER_FETCH_H
#define WEATHER_FETCH_H

/* Included files */
// Standard Pebble functions and types
#include <pebble.h>

/* Constants */
// Size of a string transmitted over bluetooth
#define TRANSMISSION_BUFFER_SIZE 32

/* Types */
/*
Pointer to function to be called when the weather has been fetched

Parameters:
    temperature: The temperature at the user's current location
    conditions_buffer: String containing the weather conditions
    conditions_buffer_size: Length of conditions_buffer, including the null
        terminator
*/
typedef void (*OnFetched)(
    int temperature, char *conditions_buffer, int conditions_buffer_size
);

// Fetches the weather (amazing!)
void fetch_weather(Window *window, OnFetched p_callback);

#endif /* end of include guard: WEATHER_FETCH_H */
