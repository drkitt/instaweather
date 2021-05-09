/*
Contains the interface for the window that displays the weather
*/

/* Included files */
// Standard Pebble functions and types
#include <pebble.h>

/* Constants */
// Size of the temperature in string form (allows for a negative sign, three
// digits for the value, the degree symbol, and the null terminator)
#define TEMPERATURE_BUFFER_SIZE 6

// Sets up a window and returns a pointer to it
Window *weather_display_window_create(void);
