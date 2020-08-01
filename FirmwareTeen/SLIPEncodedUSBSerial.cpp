
/////////////////Original file begins

#include "SLIPEncodedUSBSerial.h"

/** @addtogroup GeneralUtil
 *  @{
 */

/**
 *  \file SLIPEncodedUSBSerial.cpp
 *  \brief Support for SLIP encoded message input/output
 */

#ifdef USEOSC
/*
 CONSTRUCTOR
 */
//instantiate with the tranmission layer


//USB Serials
SLIPEncodedUSBSerial::SLIPEncodedUSBSerial(  usb_serial_class &s){
	serial = &s;
	rstate = CHAR;
}

static const uint8_t eot = 0300;
static const uint8_t slipesc = 0333;
static const uint8_t slipescend = 0334;
static const uint8_t slipescesc = 0335;
/*
 SERIAL METHODS
 */

bool SLIPEncodedUSBSerial::endofPacket()
{
	if(rstate == SECONDEOT)
	{
		rstate = CHAR;
		return true;
	}
	if (rstate==FIRSTEOT)
	{
        if(serial->available())
        {
            uint8_t c =serial->peek();
            if(c==eot)
            {
                serial->read(); // throw it on the floor
            }
        }
		rstate = CHAR;
		return true;
	}
	return false;
}
int SLIPEncodedUSBSerial::available(){
back:
	int cnt = serial->available();

	if(cnt==0)
		return 0;
	if(rstate==CHAR)
	{
		uint8_t c =serial->peek();
		if(c==slipesc)
		{
			rstate = SLIPESC;
			serial->read(); // throw it on the floor
			goto back;
		}
		else if( c==eot)
		{
			rstate = FIRSTEOT;
			serial->read(); // throw it on the floor
			goto back;
		}
		return 1; // we may have more but this is the only sure bet
	}
	else if(rstate==SLIPESC)
		return 1;
	else if(rstate==FIRSTEOT)
	{
		if(serial->peek()==eot)
		{
			rstate = SECONDEOT;
			serial->read(); // throw it on the floor
			return 0;
		}
		rstate = CHAR;
	}else if (rstate==SECONDEOT) {
		rstate = CHAR;
	}

	return 0;

}

//reads a byte from the buffer
int SLIPEncodedUSBSerial::read(){
back:
	uint8_t c = serial->read();
	if(rstate==CHAR)
	{
		if(c==slipesc)
		{
			rstate=SLIPESC;
			goto back;
		}
		else if(c==eot){

			return -1; // xxx this is an error
		}

		return c;
	}
	else
	if(rstate==SLIPESC)
	{
		rstate=CHAR;
		if(c==slipescend)
			return eot;
		else if(c==slipescesc)
			return slipesc;
			else {
				// insert some error code here
				return -1;
			}

	}
	else
		return -1;
}
//#ifdef FUTUREDEVELOPMENT
int SLIPEncodedUSBSerial::readBytes( uint8_t *buffer, size_t size)
{
    int count = 0;
    while(!endofPacket() && available() && (size>0))
    {
        int c = read();
        if(c>=0)
        {
            *buffer++ = c;
            ++count;
            --size;

        }
        else
            break;
    }
    return count;
}
//#endif

// as close as we can get to correct behavior
int SLIPEncodedUSBSerial::peek(){
	uint8_t c = serial->peek();
	if(rstate==SLIPESC)
	{
		if(c==slipescend)
			return eot;
		else if(c==slipescesc)
			return slipesc;
	}
	return c;
}

//the arduino and wiring libraries have different return types for the write function
#if defined(WIRING) || defined(BOARD_DEFS_H)

//encode SLIP
 void SLIPEncodedUSBSerial::write(uint8_t b){
	if(b == eot){
		serial->write(slipesc);
		return serial->write(slipescend);
	} else if(b==slipesc) {
		serial->write(slipesc);
		return serial->write(slipescesc);
	} else {
		return serial->write(b);
	}
}

void SLIPEncodedUSBSerial::write(const uint8_t *buffer, size_t size)
{
        while(size--)
            write(*buffer++);
}

#else
//encode SLIP
size_t SLIPEncodedUSBSerial::write(uint8_t b){
	if(b == eot){
		serial->write(slipesc);
		return serial->write(slipescend);
	} else if(b==slipesc) {
		serial->write(slipesc);
		return serial->write(slipescesc);
	} else {
		return serial->write(b);
	}
}
size_t SLIPEncodedUSBSerial::write(const uint8_t *buffer, size_t size)
{
    size_t result=0;
    while(size--)
        result = write(*buffer++); return result;
}
#endif

void SLIPEncodedUSBSerial::begin(unsigned long baudrate){
	serial->begin(baudrate);
}
//SLIP specific method which begins a transmitted packet
void SLIPEncodedUSBSerial::beginPacket() { 	serial->write(eot); }

//signify the end of the packet with an EOT
void SLIPEncodedUSBSerial::endPacket(){
	serial->write(eot);
#if defined(CORE_TEENSY)
    serial->send_now();
#endif
}

void SLIPEncodedUSBSerial::flush(){
	serial->flush();
}

/////////////////End orginal file

/************************************************************************************
 SLIP ENCODING
 ************************************************************************************/
#define MAXSLIPBUFSIZE 1024
char serialBuffer[MAXSLIPBUFSIZE+1];
int serPointer = 0;
void SerialSLIPReceived(void);

/*
byte eot = 192;
byte slipesc = 219;
byte slipescend = 220;
byte slipescesc = 221;
*/
byte previousByte;

boolean serialBufferAdd(byte add)
{
	serialBuffer[serPointer++] = add;
	if( serPointer > MAXSLIPBUFSIZE){
		serPointer = 0; // BUFFER OVERRUN !!
		return false;
	}
	return true;
}


void slipDecode(byte incoming) {
  byte previous = previousByte;
  previousByte = incoming;
  //if the previous was the escape char
  if (previous == slipesc) {
    //if this one is the esc eot
    if (incoming==slipescend) {
      serialBufferAdd(eot);
    } else if (incoming==slipescesc) {
      serialBufferAdd(slipesc);
    }
  } else if (incoming==eot) {
    //if it's the eot
    //send off the packet
    SerialSLIPReceived();
  } else if (incoming != slipesc) {
    serialBufferAdd(incoming);
  }
}

void slipEncode(byte incoming) {
  if (incoming == eot) {
    Serial.write(slipesc);
    Serial.write(slipescend);
  } else if (incoming==slipesc) {
    Serial.write(slipesc);
    Serial.write(slipescesc);
  } else {
    Serial.write(incoming);
  }
}

// Gloal func
/*
 void serialEvent(void) {
 //decode the message
  while (Serial .available () > 0) {
    slipDecode((byte)Serial.read());
	#ifdef DEBUGMODE
	SetMessageText("Serial received");
	#endif

  }
}
*/
void SerialSLIPReceived(void){
  if( serPointer < 1) return;

	#ifdef DEBUGMODE
	SetMessageText("Slip received");
	#endif

  //oscMsg = oscDataReady(); Read the OSC message

  //clear the buffer
  serPointer = 0;
}
#endif
/** @} */
