#ifndef SysExDef_h
#define SysExDef_h

/** @addtogroup SysEx
 *  @{
 */

/**
 *  \file SysExDef.h
 *  \brief SysEx structures and definitions
 */  


const unsigned SysEXPacketSize = 64;
const uint8_t SysExpacketDataLength = 55; ///< Limit to total 64 bytes SysEx message
/**
 * \brief SysEx Packet structure
 * 
 */
typedef struct
{
    byte sysExheader = 0xF0;           ///< Begin of SysEx
    byte Dev = 0x7D;                   // educational purpose
    byte ID[5];                        ///< Message ID (4 bytes ID encoded in MIDI 7-bit standard
    byte pCounter = 0;                 ///< Packet number
    byte pData[SysExpacketDataLength]={0}; ///< Data
    byte EOX = 0xF7;                   ///< End of SysEx
} GenericSysExPacket;

typedef struct
{
    byte sysExheader = 0xF0;          ///< Begin of SysEx
    byte Dev = 0x7D;                  // educational purpose
    byte ID = 0x32;                   ///< VCMC ID v2
    byte Type;                        ///< 0tttcccc ttt= Msg type cccc= channel
    byte Slot;                        ///< 0ssspppp sss=Port pppp=parameter
    byte Length;                      ///< Parameter Length (56 Max)
    byte pData[SysExpacketDataLength+1]={0}; ///< Data
    byte EOX = 0xF7;                   ///< End of SysEx
} VCMCSysExPacket;

typedef struct
{
    byte Slot;                 ///< 0ssspppp sss=Port pppp=parameter 
    byte bData1[5]; ///< Data
} VCMCSysExPar;

enum VCMCSysExType
{
  DUMPGENERAL=0,
  DUMPCHANNEL=1,
  INDPARSETUP=2,
  SYSEXCOMMAND=3
};
enum VCMCSysEXSlot
{
  GENSLOT=0,
  CVSLOT=1,
  FADERSLOT=2,
  GATESLOT=3
};

enum VCMCSysEXGlobalCommands
{
  NOP=0,
  SENDCURRENTCONFIG=1,
  RESETMODULE=2,
  FACTORYDEFAULT=3
};


/** @} */

#endif