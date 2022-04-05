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
    Pin modeProgram;
    Pin modeErase;
    Pin data[DATA_BITS];
  };

  enum Mode { MODE_READ, MODE_PROGRAM, MODE_ERASE };

  class EEPROMProgrammer
  {
  public:
    PinConfig pins;
    Mode mode;
  
    EEPROMProgrammer() { };
    
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
      pinMode(pins.modeProgram, OUTPUT);
      pinMode(pins.modeErase, OUTPUT);

      digitalWrite(pins.addrData, LOW);
      digitalWrite(pins.addrClock, LOW);
      digitalWrite(pins.addrLatch, LOW);

      setAddress(0);  // reset the shift register
      setMode(MODE_READ);
      enableChip(true);

      for (auto pin : pins.data)
        pinMode(pin, INPUT);

      return true;
    }

    /**
     * Read a byte from an address.
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
     * Write abyte to an address.
     * 
     * NOTE: If writing multiple bytes, keep the mode as PROGRAM
     *       (same for data pin modes) without changing them
     *       between each byte!
     */
    void write(int16_t addr, byte data)
    {
      /**
       * 1. Disable chip (nCE = HIGH)
       * 2. Set programming mode (nOE = 12V).
       * 3. Set address to the shift register and latch it out.
       * 4. Set the desired data to the data pins.
       * 5. Pulse nCE LOW.
       */
      enableChip(false);
      setMode(MODE_PROGRAM);
      setAddress(addr);

      for (int pinNum = 0; pinNum < DATA_BITS; pinNum++)
        digitalWrite(pins.data[pinNum], (data >> pinNum & 1));

      pulseLow(pins.nChipEnable);
      setMode(MODE_READ);
    }

    /**
     * Set programmer mode (MODE_READ | MODE_PROGRAM | MODE_ERASE).
     */
    void setMode(Mode mode)
    {
      switch (mode)
      {
        case MODE_READ:
          digitalWrite(pins.modeErase, LOW);
          digitalWrite(pins.modeProgram, LOW);
          digitalWrite(pins.nOutputEnable, LOW);
          break;
        case MODE_PROGRAM:
          digitalWrite(pins.modeErase, LOW);
          digitalWrite(pins.modeProgram, HIGH);
          digitalWrite(pins.nOutputEnable, HIGH);
          break;
        case MODE_ERASE:
          digitalWrite(pins.modeErase, HIGH);
          digitalWrite(pins.modeProgram, LOW);
          digitalWrite(pins.nOutputEnable, HIGH);
          break;
        default:
          break;
      }

      mode = mode;
    }

    /**
     * Enable or disable chip.
     */
    void enableChip(bool enable = true)
    {
      digitalWrite(pins.nChipEnable, enable ? LOW : HIGH);
    }

  protected:
    /**
     * Set address to the chip.
     */
    void setAddress(uint16_t addr, bool set = true)
    {
      shiftOut(pins.addrData, pins.addrClock, MSBFIRST, (addr >> 8) & 0xff);  // high byte
      shiftOut(pins.addrData, pins.addrClock, MSBFIRST, addr & 0xff);         // low byte

      if (set)
        pulseHigh(pins.addrLatch);
    }

    /**
     * Pulse given pin to HIGH.
     */
    void pulseHigh(Pin pin)
    {
      digitalWrite(pin, LOW);
      digitalWrite(pin, HIGH);
      delayMicroseconds(1);
      digitalWrite(pin, LOW);
    }

    /**
     * Pulse given pin to LOW.
     */
    void pulseLow(Pin pin)
    {
      digitalWrite(pin, HIGH);
      digitalWrite(pin, LOW);
      delayMicroseconds(1);
      digitalWrite(pin, HIGH);
    }
  };
};
