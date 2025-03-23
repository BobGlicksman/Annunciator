/* 
 * Aunnciator:  Software to subscribe to an event and play an associated clip.
 *  
 * 
 */

#define VERSION "0.5" // This is still an in-process development version

// NOTE:  MUST USE PARTICLE OS VERSION 3.0.0 OWING TO BUGS IN MINI MP3 PLAYER LIBRARY.
  //    Specifically, some functions have non-void return value declared but no return statement.
  //    OS versions above 3.0.0 won't treat this as a warning anymore; build will fail.

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
// SYSTEM_THREAD(ENABLED);

// Defined constants for the program
#define RED_LED_PIN D5
#define GREEN_LED_PIN D6
#define STATUS_LED_PIN D7
#define BUSY_PIN D2
#define BUTTON_PIN A3

const unsigned long BUSY_WAIT = 1000UL;  // Busy pin wait time
const unsigned long DEBOUNCE_TIME = 10UL;  // time for button debouncing

const uint8_t FIRST_CLIP_NUM = 11; // just for testing
const uint8_t LAST_CLIP_NUM = 15;  // just for testing

// Global Variables
  // define main state variable states
enum StateVariable {
    idle,
    triggered,
    clipWaiting,
    clipPlaying,
    clipComplete,
    clipEnd,
    paused
};

  // Other globals
int relativeVolumeControl = 100;  // value between 0 and 100 (%); Preset to 100%
int currentClip = 0;    // number of the last clip played
bool newClip2Play = false;  // set to true to indicate that there is a new clip to play
bool greenLEDFlash = false; // set to true to start the green LED flashing; false to stop it.
String version = VERSION;

// Include the mp3 player library
#include <DFRobotDFPlayerMini.h>

// Create an instance of the miniMP3 player
DFRobotDFPlayerMini miniMP3Player;

// Cloud function to set the master volume level
int setVolume(String volumeControl) {
    int volume = volumeControl.toInt();
    if (volume < 0) {
        relativeVolumeControl = 0;
    } else if (volume > 100) {
        relativeVolumeControl = 100;
    } else {
        relativeVolumeControl = volume;
    }
    
    return 0;
}   // end of setVolume()

// Cloud function to play a specified clip
int playClip(String clipNumber) {
    uint8_t clipNum = clipNumber.toInt();
    if(clipNum < FIRST_CLIP_NUM) {
        currentClip = FIRST_CLIP_NUM;
    } else if(clipNum > LAST_CLIP_NUM) {
        currentClip = LAST_CLIP_NUM;
    } else {
        currentClip = clipNum;
    }
    
    newClip2Play = true;    // play the clip
    return 0;

}   // end of playClip()

// function to flash the green LED rapidly (called from a non-blocking loop())
void flashLED() {
    #define BLINK_TIME 100  // turn on and off every 100 ms
    static unsigned long timeInState = millis();
    static bool onOff = false;

    if(greenLEDFlash == false) {    // do not flash the green LED; turn it off
        digitalWrite(GREEN_LED_PIN, LOW);
    } else {    // flash the green LED
        if((millis() - timeInState) > BLINK_TIME) { // time to toggle the LED state
            if(onOff == false) {
                digitalWrite(GREEN_LED_PIN, HIGH);
                onOff = true;
            } else {
                digitalWrite(GREEN_LED_PIN, LOW);
                onOff = false;
            }
            timeInState = millis();
        }

    }
}   // end of flashLED()

// setup() runs once, when the device is first turned on
void setup() {
    // Photon pin definitions
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(STATUS_LED_PIN, OUTPUT);
    pinMode(BUSY_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Cloud variables and funtions
    Particle.variable("Firmware Version", version);
    Particle.variable("Master Volume Control", relativeVolumeControl);
    Particle.variable("Current Clip Number", currentClip);

    Particle.function("Set Master Volume Level", setVolume); // call to set master volume level
    Particle.function("Play Clip", playClip);

    // initialize serial ports and mini MP3 player
    Serial.begin(9600);
    Serial1.begin(9600);
    miniMP3Player.begin(Serial1);
    miniMP3Player.volume(30);       // set the max volume - it may be changed when playing a clip

    // set initial state of green LED
    digitalWrite(GREEN_LED_PIN, LOW);

    // signal end of setup
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(STATUS_LED_PIN, LOW);

    digitalWrite(RED_LED_PIN, HIGH);    // indicate powered, connected to WiFi and ready

}   // end of setup()

// loop() runs over and over again, as quickly as it can execute.
void loop() {
    static unsigned long busyTime = millis();
    static StateVariable state = idle; // begin in the idle state

    // flash the green LED in non-blocking manner
    flashLED();

    // state machine to play a clip and flash LED
    switch(state) {
        case idle:  // wait for event or other trigger
            if (digitalRead(BUSY_PIN) == HIGH)  {  // make sure MP3 player is ready
                if(newClip2Play == true) {   // we must play the new clip
                    greenLEDFlash = true; // signal a new value
                    busyTime = millis();    // startr the time for the next state
                    state = triggered;
                } else {
                    state = idle;
                } 
                
            } else {    // stay in idle state until MP3 player busy pin is low
                state = idle;
            }
            break;

        case triggered: // we must start playing the clip -- light (flash) green LED
            // keep the green LED lit (flashing) for a bit, and then start a clip playing
            if((millis() - busyTime) < BUSY_WAIT) { // stay in this state until time to play clip
                state = triggered;
            } else {    // it is time to play the clip
                // set the volume
                float vol = 30 * ((float)relativeVolumeControl/100.0);
                miniMP3Player.volume((int)vol);

                // play the designated clip
                miniMP3Player.playMp3Folder(currentClip);
                state = clipWaiting;
            }
            break;

        case clipWaiting:   // wait fop MP3 player busy to asset (low)
            if(digitalRead(BUSY_PIN) == HIGH) {  // waiting for the clip to begin playing
                state = clipWaiting;
            } else {    // clip is now playing
                state = clipPlaying;
            }
            break;

        case clipPlaying:   // clip is playing, wait for busy to unassert (playing complete)
            if(digitalRead(BUSY_PIN) == LOW) {  // clip is still playing
                state = clipPlaying;
            } else {    // clip complete - start time for keeping green LED on (flashing)
                busyTime = millis();
                state = clipComplete;
            }
            break;

        case clipComplete:  // clip has finished, keep flashing the GREEN LED for a while
            if((millis() - busyTime) < BUSY_WAIT) { // keep green LED high (flashing)
                state = clipComplete;
            } else { // turn green LED off (stop flashing)
                greenLEDFlash = false;
                state = clipEnd;
            }
            break;

        case clipEnd:   // flag that no clip is active
            newClip2Play = false; // XXX can't command replay until here -- should this be sooner??
            state = idle;
            break;

        default:    // the next state is idle
            newClip2Play = false;
            state = idle;
            break;

    } // end state machine

    // test the replay button and replay the current clip as needed

    // refresh flashing the green LED



}   // end of loop()

// Code needed:
//  1. subscribe to the event.  Parse the sensor number and map it to proper clip number to play.
//  2. add in a button.  Process and debounce button press and play the current clip.


