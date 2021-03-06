/*
This code will be ran on the phone. It fetches data from the OpenWeatherMap API
and sends it to the watch
*/

// Used to communicate with OpenWeatherMap. Since this is a small-scale project,
// I'm hard-coding my own API key, but a nice "version 2" feature would be to
// let a user supply their own key.
const MY_API_KEY = '9a7868c2beb4f4d09ad6d9fe3c00bdc5';

/*
Simplifies the process of sending an XMLHttpRequest to OpenWeatherMap

Parameters:
    url: The API enpoint to send a request to
    type: The type of request
    callback: Function to call when we receive a response
*/
function xhrRequest(url, type, callback) {
    const xhr = new XMLHttpRequest();
    xhr.onload = function() {
        callback(this.responseText);
    };
    xhr.open(type, url);
    xhr.send();
}

/*
Fetches weather from the API when the user's location is known

Parameters:
    pos: User location
*/
function locationSuccess(pos) {
    // Construct URL
    const url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
        pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=' +
        MY_API_KEY;

    // Request weather information
    xhrRequest(url, 'GET',
        function(responseText) {
            // responseText contains a JSON object with weather info
            console.log('Response: ' + responseText);
            const json = JSON.parse(responseText);

            // Temperature in Kelvin requires adjustment
            const temperature = Math.round(json.main.temp - 273.15);
            console.log('Temperature is ' + temperature);

            // Conditions
            const conditions = json.weather[0].description;
            const conditions_sentence_case = conditions.charAt(0).toUpperCase()
                + conditions.slice(1);
            const conditions_id = json.weather[0].id;
            console.log('Conditions: ' + conditions_sentence_case);
            console.log('Conditions id: ' + conditions_id);

            // Assemble dictionary using our keys
            const dictionary = {
                'TEMPERATURE': temperature,
                'CONDITIONS': conditions_sentence_case,
                'CONDITIONS_ID': conditions_id,
            };

            // Send to Pebble
            Pebble.sendAppMessage(dictionary,
                function(e) {
                    console.log('Weather info sent to Pebble successfully!');
                },
                function(e) {
                    console.log('Error sending weather info to Pebble!');
                }
            );
        }
    );
}

/*
Called when something goes wrong in finding the user's location

Parameters:
err: The error that occurred
*/
function locationError(err) {
    console.log('Error requesting location!');
}

/*
Gets weather information
*/
function getWeather() {
    navigator.geolocation.getCurrentPosition(
        locationSuccess,
        locationError,
        { timeout: 15000, maximumAge: 60000 }
    );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
    function(e) {
        console.log('PebbleKit JS is ready!');

        Pebble.sendAppMessage({},
            function(e) {
                console.log('Initial message sent to Pebble successfully!');
            },
            function(e) {
                console.log('No response for initial message');
            }
        );
    }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
    function(e) {
        console.log('AppMessage received!');
        getWeather();
    }
);
