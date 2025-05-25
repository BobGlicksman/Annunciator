/* ClipsList.h
*   This file defines the list of clips to play.
*
*   The index into the clipList[] array is the device ID - base device ID
*       contained in the message from the LoRa sensor.  The message is part of
*       the published event to the Particle cloud that the Annunciator subscribes to.
*
*   Change this file only if the list of MP3 files to play changes. Do not change
*       code in Annunciator_code.ino if all you are changing is the definition of which
*       MP3 clip to play 
*
*   (c) 2025; Team Practicle Projects, Bob Glicksman, Jim Schrempp
*       All rights reserved.
*/
#include "Particle.h"

const unsigned int BEGIN_DEV_NUM = 5;   // the device number reported if all ADR jumers are in

// This is the list of MP3 files to play based upon the numerical prefix of the file name
//  in the /MP/ folder on the mini SD card.

// 0: 21 Reception Desk
// 1: 22 Wood shop
// 2: 23 Cold Shop
// 3: 26 Lasers 3D
// 4: 27 Hot shop
// 5: 28 Electronics
// 6: 1 sensor 6 has no audio
// 7: 2 sensor 7 has no audio

unsigned int clipList[] = {21, 22, 23, 26, 27, 28, 1, 2}; // clips to play
