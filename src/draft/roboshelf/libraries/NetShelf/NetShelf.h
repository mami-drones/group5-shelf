#include "Arduino.h"
#include <PhyShelf.h>
#include <PipedStream.h>

#define SHELF_ONLINE 0
#define SHELF_OFFLINE 1
#define SHELF_PROTO_FAIL 2

class NetShelf
{
  public:
    NetShelf(PhyShelf shelf, PipedStream net);
    NetShelf(uint64_t id, PipedStream net);
    NetShelf(
      uint64_t id, 
      void (*readpacket)(void *buf,  uint8_t len), 
      void (*writepacket)(void *buf,  uint8_t len)
    );
    NetShelf(
      PhyShelf shelf, 
      void (*readpacket)(void *buf,  uint8_t len), 
      void (*writepacket)(void *buf,  uint8_t len)
    );
    void setReadCallback(void (*readpacket)(void *buf,  uint8_t len));
    void setWriteCallback(void (*writepacket)(void *buf,  uint8_t len));
    void begin();
    bool available();
    int  status();
    uint64_t getID();
    char* readID();
    bool setDeployReady(bool);
    bool dispence(uint64_t);
    bool dispencing();
    bool setDeployReady();
    bool makeBusy();
    void arrivalNotify();
    void requestItem(uint64_t);
    int* listContents();
};

