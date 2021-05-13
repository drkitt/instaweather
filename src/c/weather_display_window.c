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
    LIGHT_RAIN,
    HEAVY_RAIN,
    SNOW,
    PARTLY_CLOUDY,
    SUNNY,
    NUM_CONDITIONS
} Condition;

/* Static constants */
// Maps the enum values above (implicitly converted to ints) to the
// corresponding icon
static const int ICONS[NUM_CONDITIONS] = {
    RESOURCE_ID_ICON_GENERIC_WEATHER,
    RESOURCE_ID_ICON_LIGHT_RAIN,
    RESOURCE_ID_ICON_HEAVY_RAIN,
    RESOURCE_ID_ICON_SNOW,
    RESOURCE_ID_ICON_PARTLY_CLOUDY,
    RESOURCE_ID_ICON_SUNNY
};

/* Static variables */
// Displays the temperature
static TextLayer *temperature_layer;
// Displays the conditions in text form
static TextLayer *conditions_text_layer;
// Icon for the conditions
static GDrawCommandImage *conditions_icon;
// Displays the conditions icon
static Layer *conditions_icon_layer;

/* Static prototypes */
// Callback for loading the window
static void load(Window *window);
// Callback for unloading the window
static void unload(Window *window);
// Callback for displaying the window
static void appear(Window *window);
// Callback for rendering the conditions layer
static void conditions_icon_layer_update(Layer *layer, GContext *context);
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
    // Otherwise, the window's appear method will do all the meaninful
    // initialization
}

/*
Called when the window unloads

Parameters:
    window: Pointer to the window-to-not-be
*/
static void unload(Window *window) {
    text_layer_destroy(temperature_layer);
    text_layer_destroy(conditions_text_layer);
    layer_destroy(conditions_icon_layer);
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
    const int temperature = load_temperature();
    static char conditions_text_buffer[STORED_BUFFER_SIZE];
    load_conditions_buffer(conditions_text_buffer, STORED_BUFFER_SIZE);
    const int conditions_id = load_conditions_id();
    const Condition condition = get_conditions(conditions_id);
    conditions_icon = gdraw_command_image_create_with_resource(ICONS[condition]);

    // Set up window
    Layer *window_layer = window_get_root_layer(window);
    const GRect bounds = layer_get_bounds(window_layer);
    window_set_background_color(window, GColorWhite);

    // Display conditions icon layer
    conditions_icon_layer = layer_create(
        GRect(0, 0, bounds.size.w, bounds.size.h));
    layer_set_update_proc(conditions_icon_layer, conditions_icon_layer_update);
    layer_add_child(window_layer, conditions_icon_layer);

    // Display conditions text layer
    conditions_text_layer = text_layer_create(GRect(0, 70, bounds.size.w, 20));
    text_layer_set_text_color(conditions_text_layer, GColorBlack);
    text_layer_set_background_color(conditions_text_layer, GColorClear);
    text_layer_set_text(conditions_text_layer, conditions_text_buffer);
    text_layer_set_text_alignment(conditions_text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(conditions_text_layer));

    // Display temperature_layer
    temperature_layer = text_layer_create(GRect(0, 100, bounds.size.w, 40));
    static char temperature_buffer[TEMPERATURE_BUFFER_SIZE];
    snprintf(temperature_buffer, TEMPERATURE_BUFFER_SIZE, "%d°", temperature);
    text_layer_set_text_color(temperature_layer, GColorBlack);
    text_layer_set_background_color(temperature_layer, GColorClear);
    text_layer_set_font(
        temperature_layer, fonts_get_system_font(FONT_KEY_LECO_38_BOLD_NUMBERS)
    );
    text_layer_set_text(temperature_layer, temperature_buffer);
    text_layer_set_text_alignment(temperature_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(temperature_layer));
}

/*
Called when the layer containing the conditions icon is rendered

Parameters:
    layer: The layer to be rendered
    context: The destination graphics context to draw into
*/
static void conditions_icon_layer_update(Layer *layer, GContext *context) {
    // Set the origin offset from the context for drawing the image
    GPoint origin = GPoint(47, 20);

    // Draw the GDrawCommandImage to the GContext
    gdraw_command_image_draw(context, conditions_icon, origin);
}

/*
// Gets the enum value for the current conditions

Parameters:
    id: Number from the API denoting the conditions
*/
static Condition get_conditions(int id) {
    Condition condition;
    if (id <= 299) {
        condition = HEAVY_RAIN;
    }
    else if (id <= 399) {
        condition = LIGHT_RAIN;
    }
    else if (id <= 599) {
        condition = HEAVY_RAIN;
    }
    else if (id <= 699) {
        condition = SNOW;
    }
    else if (id <= 799) {
        condition = GENERIC;
    }
    else if (id == 800) {
        condition = SUNNY;
    }
    else if (id <= 899) {
        condition = PARTLY_CLOUDY;
    }
    else {
        condition = GENERIC;
    }

    return condition;
}
