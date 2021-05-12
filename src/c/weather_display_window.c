/*
Contains the implementation for the weather display window
*/

/* Included files */
// Weather display window interface
#include "weather_display_window.h"
// Weather saving and loading
#include "weather_storage.h"
// Fetching the weather in case we don't have any saved
#include "weather_loading_window.h"

/* Types */
// Possible values for the weather conditions
typedef enum {
    GENERIC,
    HEAVY_RAIN,
    HEAVY_SNOW,
    LIGHT_RAIN,
    LIGHT_SNOW,
    PARTLY_CLOUDY,
    SUNNY,
    NUM_CONDITIONS
} Condition;

/* Static variables */
// Displays the temperature
static TextLayer *temperature_layer;
// Icon for the conditions
static GDrawCommandImage *conditions_icon;
// Displays the conditions icon
static Layer *conditions_layer;

/* Static prototypes */
// Callback for loading the window
static void load(Window *window);
// Callback for unloading the window
static void unload(Window *window);
// Callback for displaying the window
static void appear(Window *window);
// Callback for rendering the conditions layer
static void conditions_layer_update(Layer *layer, GContext *context);
// Gets the enum value for the current conditions
static Condition get_conditions(int id);

/*
Sets up a window and returns a pointer to it.
Note that this function doesn't add the new window to the window stack; the
caller must manage the returned window itself.
*/
Window *weather_display_window_create(void) {
    Window *new_window = window_create();
    window_set_window_handlers(new_window, (WindowHandlers) {
        .load = load,
        .unload = unload,
        .appear = appear,
    });

    // Test code for icon drawing - hopefully this can eventually go in the appear function to allow for conditional rendering based on the conditions
    conditions_icon = gdraw_command_image_create_with_resource(RESOURCE_ID_ICON_GENERIC_WEATHER);

    return new_window;
}

/*
Called when the window loads

Parameters:
    window: Pointer to the window-to-be
*/
static void load(Window *window) {
    // Load or fetch weather data if necessary
    if (!saved_data_exists()) {
        APP_LOG(APP_LOG_LEVEL_INFO, "No saved data found");
        Window *loading_window = weather_loading_window_create();
        window_stack_push(loading_window, true);
    }
}

/*
Called when the window unloads

Parameters:
    window: Pointer to the window-to-not-be
*/
static void unload(Window *window) {
    text_layer_destroy(temperature_layer);
    layer_destroy(conditions_layer);
    gdraw_command_image_destroy(conditions_icon);
}

/*
Called when the window (re)appears on screen

Parameters:
    window: The new window
*/
static void appear(Window *window) {
    // Since the load function brings up the loading window if there's no saved
    // weather data (and said data is never deleted), if we get to this point
    // then we can assume that saved weather exists
    int temperature = load_temperature();
    char conditions_buffer[STORED_BUFFER_SIZE];
    load_conditions_buffer(conditions_buffer, STORED_BUFFER_SIZE);

    // Set up temperature layer
    Layer *window_layer = window_get_root_layer(window);
    const GRect bounds = layer_get_bounds(window_layer);
    temperature_layer = text_layer_create(GRect(0, 90, bounds.size.w, 40));
    static char temperature_buffer[TEMPERATURE_BUFFER_SIZE];
    snprintf(temperature_buffer, TEMPERATURE_BUFFER_SIZE, "%d°", temperature);

    // Display temperature
    window_set_background_color(window, GColorWhite);
    text_layer_set_text_color(temperature_layer, GColorBlack);
    text_layer_set_background_color(temperature_layer, GColorClear);
    text_layer_set_font(
        temperature_layer, fonts_get_system_font(FONT_KEY_LECO_38_BOLD_NUMBERS)
    );
    text_layer_set_text(temperature_layer, temperature_buffer);
    text_layer_set_text_alignment(temperature_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(temperature_layer));

    // Set up conditions layer
    conditions_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
    layer_set_update_proc(conditions_layer, conditions_layer_update);
    layer_add_child(window_layer, conditions_layer);

    // Can't wait to remove this
    APP_LOG(
        APP_LOG_LEVEL_INFO,
        "Temperature is %d and conditions are %s. Isn't that neat?",
        temperature,
        conditions_buffer
    );
}

/*
Called when the layer containing the conditions icon is rendered

Parameters:
    layer: The layer to be rendered
    context: The destination graphics context to draw into
*/
static void conditions_layer_update(Layer *layer, GContext *context) {
    // Set the origin offset from the context for drawing the image
    GPoint origin = GPoint(47, 30);

    // Draw the GDrawCommandImage to the GContext
    gdraw_command_image_draw(context, conditions_icon, origin);
}

/*
// Gets the enum value for the current conditions

Parameters:
    id: Number denoting the conditions
*/
static Condition get_conditions(int id) {
    return GENERIC;
}
