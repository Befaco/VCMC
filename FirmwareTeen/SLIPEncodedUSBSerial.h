/*
Extends the Serial class to encode SLIP over serial
*/
#include "Arduino.h"

#ifndef SLIPEncodedUSBSerial_h
#define SLIPEncodedUSBSerial_h

/** @addtogroup GeneralUtil
 *  @{
 */

/**
 *  \file SLIPEncodedUSBSerial.h
 *  \brief Support for SLIP encoded message input/output
 */

#include <Stream.h>

#include <usb_serial.h>


class SLIPEncodedUSBSerial: public Stream{
	
private:
	enum erstate {CHAR, FIRSTEOT, SECONDEOT, SLIPESC } rstate;

    usb_serial_class* serial;
	
public:
	SLIPEncodedUSBSerial( usb_serial_class&		);

	int available();
	int read();
    int readBytes( uint8_t *buffer, size_t size);

	int peek();
	void flush();
	
	//same as Serial.begin
	void begin(unsigned long);
    //SLIP specific method which begins a transmitted packet
	void beginPacket();
	//SLIP specific method which ends a transmittedpacket
	void endPacket();
	// SLIP specific method which indicates that an EOT was received 
	bool endofPacket();

#if  defined(WIRING) || defined(BOARD_DEFS_H)
	void write(uint8_t b);
   void write(const uint8_t *buffer, size_t size);

#else
	//overrides the Stream's write function to encode SLIP
	size_t write(uint8_t b);
    size_t write(const uint8_t *buffer, size_t size);
	//using Print::write;	
#endif

};

/** @} */


#endif
