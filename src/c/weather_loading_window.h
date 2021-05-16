/*
Contains the interface for the window that displays a message while the weather
loads. Usually used when the app is launched by the background worker, but it
can also appear when the app is launched by the user and there's no saved
weather data
*/

#ifndef WEATHER_LOADING_H
#define WEATHER_LOADING_H

/* Included files */
// Standard Pebble functions and types
#include <pebble.h>

// Sets up a window and returns a pointer to it
Window *weather_loading_window_create(void);

#endif
