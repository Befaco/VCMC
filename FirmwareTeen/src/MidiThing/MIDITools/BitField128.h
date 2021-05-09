#ifndef __BITFIELD128_H__
#define __BITFIELD128_H__

#include <limits.h>
#include <string.h>

class BitField128{
private:
  uint8_t field[32]={0}; // 256 bits

public:
  void setbit128( uint8_t idx){
    field[idx / 8] |= 1u << (idx % 8);
  }

  void unsetbit128(uint8_t idx){
    field[idx / 8] &= ~(1u << (idx % 8));
  }

  void togglebit128(uint8_t idx){
    field[idx / 8] ^= 1u << (idx % 8);
  }

  bool isbitset128(uint8_t idx){
    return field[idx / 8] & (1u << (idx % 8));
  }

  void clearbitset128(){
    memset(field, 0, sizeof(field));
  }

};


#endif // __BITFIELD128_H__

