# VCMC v1.3 / CV Thing v1.3 

This code is made for Befaco's VCMC module. A voltage controlled MIDI controller.
Find further information [here](https://www.befaco.org/vcmc-2/)
This code will work in any hardware revision from PCB v1

The code will also run with CV Thing module. See below install options for details. Find further information about CV Thing [here](https://www.befaco.org/CV_thing/)

If you are updating version, it´s MANDATORY to do a Factory callibration (Read Below). Also it is recommended a factory reset and save. Also keep in mind that old saves will not be valid in the new version.


## Changelog/ Bug Fixes:

### v1.3
- Chord mode implemented.
- Aftertouch implemented.
- Improved compatibility with iConnectivity devices (But disabled OSC)

*Chord mode idea and original code was implemented by Tony Fleig.

### v1.2.3
- Bug fix: POLY mode now works in VCMC when cv and fader is set to sum.   

### v1.2.2
- Clock save bug fix.
- Memory cleanup.

### v1.2.1
- CV Delay bug fixed.
- Other minor bug fixes.

### v1.2
- New Polyphonic mode added.
- Changed save structure. Now there are four save spaces available.
- 14 bit CC implemented.
- Web configurer available [here.](https://www.befaco.org/VCMCconfig/) 

### v1.1
- Controls now can be named. List of common names is available as well as a list of user defined names.
- Velocity now applies to notes sent via Gates.
- Clock and ST/SP can be mapped to Aux.
- Changed clock handling.
- Changed Sysex structure to improve compatibility with MACOS 
- Panic can be mapped to gates
- RTC initialization removed from mk20dx128.c to avoid startup issues on high loaded systems (applied to Bin file)
- Default mapping fixed to match manual and Bitwig script.
- Gates are no longer visualized in the screen as redundant.
- Some minor visual tweaks on menus.
- Improved stability on menus.Now configuring during performance is not disturbing usability.
- Calibration ranges change depending in input range.
- Calibration is not resetted when changing Input range.
- In Note mode now you can choose the time for the Note OFF
- Fader readings are smoother now.
- NRPNs  values can be set now in decimal format.
- Fixed code to improve SPI screen compatibility.


## Dependencies:

[Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

[Adafruit SSD1306 Display](https://github.com/adafruit/Adafruit_SSD1306)

[Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)

[TimerOne](http://playground.arduino.cc/Code/Timer1)

[Menu class](https://forum.arduino.cc/index.php?topic=353045.0)

[Midi Library](https://github.com/FortySevenEffects/arduino_midi_library)

[Bounce2](https://github.com/thomasfredericks/Bounce2)

[ClickEncoder](https://github.com/0xPIT/encoder)


## Uploading the firmware from hex file

If you are updating your module from a previous version, data structure will not be compatible and you will loose all saved data.


- Get latest firmware file from Bin folder. Make sure you clon the repository, as downloading just the file won't work. Follow instructions from Readme.md file there.

- Install Teensy uploader app found [here](https://www.pjrc.com/teensy/loader.html)

- Connect VCMC to your computer using a USB cable.

- RunTeensy uploader and load hex file. Press teensy button to begin transfer... Make sure Module is plugged in.

If you are getting  File too long error message in teensy loader, it might be due to the app not detecting what version of Teensy it is connected to.
This is a known issue with teensy and it is OS dependant. Pressing the button on the teensy (back of the module) will override this message and begin the upload. 

## Uploading the firmware from source code

If you are updating from a previous version, data structure will not be compatible and you will loose all saved data.

1. Download Arduino IDE
Go to the official [Arduino website](https://www.arduino.cc/en/Main/Software) and download the latest version of Arduino IDE for your operating system and install it.

2. Download Teensyduino
Go to the official [Teensy site](https://www.pjrc.com/teensy/teensyduino.html) and follow the instructions to install the add-on.
Make sure you have version 1.53.

3. Edit LCD Library

  * Browse in your Arduino folder to hardware/teensy/avr/libraries/Adafruit_SSD1306

  * Open file Adafruit_SSD1306.h and search for line:

    `//#define SSD1306_128_64`

  * Uncomment it (and comment the currently uncommented one):

    `#define SSD1306_128_64`

  (Note that recent versions of this library no longer require this #define, so we might be able to remove this step in the future.)

4. Download the firmware 
  Clone or download this repository. Go to https://github.com/Befaco/VCMC and press "clone or download" to download the repository to your computer. Extract the zip file and copy "firmwareteen" folder to your sketchbook folder. 

5. Install the libraries
  Download the libraries and install them following the instructions from this link https://www.arduino.cc/en/Guide/Libraries

6. Connecting the module
  Connect teensy or the module to the computer using USB connector.  If you are programming with Teensy connected to the module, please make sure the module is powered on. 

7. In  Arduino software Tools > USB Type submenu  choose **MIDI**

8. Upgrading
  Press "Open" and search the file "Firmwareteen.ino" located on the "VCMC" folder. 
  Select Teensy 3.2/3.1 as your board, choose the relevant port, Speed at 96 MHz, and configure the USB type as USB+Serial.. Then hit upload

If everything goes well you should see a "Done uploading" message in a few seconds. If something happens during the upgrade, check your Arduino settings following the instructions of this link https://www.arduino.cc/en/Guide/Troubleshooting

## Uploading code to CV Thing.
Upload process is the same for CV Thing. But you will need to uncomment two lines from the code:

- In **Definitions.h** uncomment line 62:

//#define CVTHING 1

## Screen incosistencies.

We have been facing inconsistencies with screen providers. Differnet batches had different horizontal lines offset. 
If you are facing offset problems, there is a setup in Definitions.h lines 68 - 70 Possible values are  0x00, 0xFF, 0x01:


	const uint8_t SCREEN_OFFSET = 0xFF; /// Screen offset (Adjust based on screen manufacturer, lot, ...) Possible values 0x00 0xFF 0x01



	const uint8_t SCREEN_OFFSET = 0x00; /// Screen offset (Adjust based on screen manufacturer, lot, ...) Possible values 0x00 0xFF 0x01



Change the value acordingly.

Binary files have been created for all known screen batches.

## Disabling RTC
In some heavy loaded, power hungry, systems teensy might not boot up. 
We found that disabling RTC from mk20dx128.c file solved the issue. If this happend to you, comment RTC code from the file and compile.

## Factory callibration
When updating your firmware you will need to do a factory callibration. This must be done with trusted voltage sources so hardware will behave as expected.
To enter this callibration go to global settings > Credits then hold encoder button for a couple of seconds. when you are returned to global menu, factory cal should appear in the menu.


- Calibrating Faders.
Move all faders to maximum position until all values stabilize. Then move all of them to zero. Press end to store the values

- Calibrating Cvs.
We will be entering to a two points calibration screen. You will be prompted to send 2v and 8 volts, so they will be stored and correcting any eventual offset and non linearity in your CV source.
Make sure your CV source is precise enough. We suggest to measure with a three digits mutimeter.
If your voltage source is not able to provide 8v, you can change input range in CV 1 to +5 and try again. It should prompt you to send 1v and 4v.

 - Calibrating Aux.
We will be entering to a two points calibration screen. You will be prompted to send 2v and 8 volts, so they will be stored and correcting any eventual offset and non linearity in your CV source.
Make sure your CV source is precise enough. We suggest to measure with a three digits mutimeter. 

If your voltage source is not able to provide 8v, you can change input range in CV 1 to +5 and try again. It should prompt you to send 1v and 4v.


- Fader Filter.
Fader values are filtered in order to avoid unstable values. The higher the filter the better valu stability, but the fader response will be slower.
Find your preffered value. Our tests find that 50 - 65 are good values. going up to 75 - 100 will be a bit too much. 

## Credits.

Coded By Sergio Retamero (sergioretamero@gmail.com)



