/*
Contains the interface for the window that displays a message while the weather
loads
*/

/* Included files */
// Standard Pebble functions and types
#include <pebble.h>

// Sets up a window and returns a pointer to it
Window *weather_loading_window_create(void);
