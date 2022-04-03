namespace spc
{
  typedef uint8_t Pin;
  
  struct PinConfig
  {
    Pin addrDataPin;
    Pin addrClockPin;
    Pin addrLatchPin;
    Pin nChipEnablePin;
    Pin nOutputEnablePin;
    //Pin modePin;
    Pin dataPins[8];
  };
  
  /*
  typedef struct
  {
    Pin pin0;
    Pin pin1;
    Pin pin2;
    Pin pin3;
    Pin pin4;
    Pin pin5;
    Pin pin6;
    Pin pin7;
  } port_t;
  */
  
  class EEPROMProgrammer
  {
  public:
    PinConfig pinConfig;
  
    Programmer() { };
    
    void setPinConfig(PinConfig &pinConfig) {
      this->pinConfig = pinConfig;
    };
  };
};
