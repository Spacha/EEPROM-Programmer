#pragma once

#define DATA_BITS 8

namespace spc
{
  typedef uint8_t Pin;
  
  struct PinConfig
  {
    Pin addrData;
    Pin addrClock;
    Pin addrLatch;
    Pin nChipEnable;
    Pin nOutputEnable;
    //Pin modeSelect;
    Pin data[DATA_BITS];
  };

  class EEPROMProgrammer
  {
  public:
    PinConfig pins;
  
    Programmer() { };
    
    void setPinConfig(PinConfig &pinConfig)
    {
      pins = pinConfig;
    };

    /**
     * Initialize programmer.
     */
    bool initialize()
    {
      pinMode(pins.addrData, OUTPUT);
      pinMode(pins.addrClock, OUTPUT);
      pinMode(pins.addrLatch, OUTPUT);
      pinMode(pins.nChipEnable, OUTPUT);
      pinMode(pins.nOutputEnable, OUTPUT);
      //pinMode(pins.modeSelect, OUTPUT);

      for (auto pin : pins.data)
        pinMode(pin, INPUT);

      return true;
    }

    /**
     * Read byte from address.
     */
    byte read(int16_t addr)
    {
      setAddress(addr);

      byte data = 0;
      for (int pinNum = 0; pinNum < DATA_BITS; pinNum++)
        data |= (digitalRead(pins.data[pinNum]) << pinNum);
      return data;
    }

    /**
     * Set address to the chip.
     */
    void setAddress(uint16_t addr, bool set = true)
    {
      shiftOut(pins.addrData, pins.addrClock, MSBFIRST, (addr >> 8) & 0xff);  // high byte
      shiftOut(pins.addrData, pins.addrClock, MSBFIRST, addr & 0xff);         // low byte

      if (set)
        pulse(pins.addrLatch);
    }

  protected:
    void pulse(Pin pin)
    {
      digitalWrite(pin, LOW);
      digitalWrite(pin, HIGH);
      delayMicroseconds(100);
      digitalWrite(pin, LOW);
    }
  };
};
