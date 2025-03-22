/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
// SYSTEM_THREAD(ENABLED);

#define RED_LED_PIN D5
#define GREEN_LED_PIN D6
#define STATUS_LED_PIN D7

// Include the mp3 player library
#include <DFRobotDFPlayerMini.h>

// Create an instance of the miniMP3 player
DFRobotDFPlayerMini miniMP3Player;

// setup() runs once, when the device is first turned on
void setup() {
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(STATUS_LED_PIN, OUTPUT);

    Serial.begin(9600);
    Serial1.begin(9600);

    miniMP3Player.begin(Serial1);

    miniMP3Player.volume(30);


    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);

    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(STATUS_LED_PIN, LOW);

    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(STATUS_LED_PIN, LOW);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
    for(int i = 11; i < 16; i++) {
        miniMP3Player.playMp3Folder(i);
        delay(5000);
    }
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(250);
    digitalWrite(GREEN_LED_PIN, LOW);   
    delay(250);

    digitalWrite(RED_LED_PIN, HIGH);
    delay(250);
    digitalWrite(RED_LED_PIN, LOW);   
    delay(1000);  
}
