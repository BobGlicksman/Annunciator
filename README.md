# Annunciator
## Project Overview
The Annunciator is a device that subscribes to cloud events and plays pre-recorded audio clips that correspond to data in the event that it subscribes to. 

The Annunciator is intended to be used on a variety of projects that utilize Particle’s (Particle.io) cloud-based publish and subscribe mechanism.  This is an open source
project and this repository includes source code for the software and well as electronic and 2D CAD files for building the hardware.

Complete information about Annunciator operation, theory, construction and configuration and testing can be found in the document: 
  
  Documentation/Annunciator_User_Manual.pdf

## Brief Description
The Annunciator is a self-contained device, including:
- a laser cut enclosure.
- electronics, based upon a Particle Photon microcontroller and a DFRobotDFPlayer mini MP3 player.
- a 3 watt amplifier and an internal loudspeaker.
- Photon software.

The Annunciator subscribes to events that are published to the Particle Cloud.  The event data is parsed to determine the source (deviceID)
of the publication and it plays a voice clip associated with that source.  The voice clips are user defined -- recorded on a micro SD card that
is inserted into the DFRobotDFPlayer mini MP3 player.  The Annunciator flashes a backlit pushbutton when a new message is playing.  The button may be pressed
at any time to replay the last voice clip.

See the "Annunciator_User_Manual" for details.

## Contents of this repository.
### Root.
"Terms_of_Use_License_and_Disclaimer.pdf":  You must agree to these terms and conditions in order to use the material in this repository.

### Data/SD_card_data_MP3_folder.
This folder contains sample voice clip files that have been used on several of our projects.  The "Annumciator_User_Manual" describes how you can
record your own voice clips that are Annunciator compatible.

### Documentation.
This folder contains documentation for this project:
- "Annunciator_User_Manual":  complete instructions for building, understanding, operating and testing the Annunciator.
- "DFPlayer Mini Manual": the manufacturer's manual for the DFRobotDFPlayer mini MP3 player.

### Hardware
This folder contains hardware details, including:
- "Annunciator parts list.pdf": a parts list.
- "Photon adaptor mapping.pdf": a mapping analysis of the Photon pinout to the newer Photon 2
- "LaserCutEnclosure": Folder containing the 2D CAD file for the enclosure: "annunciator_1.svg"

### Software/PhotonSoftware/Annunciator_code
This folder contains source code and library files for the Annunciator.


