#include "Arduino.h"
#include <PhyShelf.h>

#define SHELF_ONLINE 0
#define SHELF_OFFLINE 1
#define PROTO_FAIL 2

class NetShelf
{
  public:
    NetShelf(PhyShelf shelf, Stream net);
    void setWriteCallback(function writepacket);
    void setReadCallback(function readpacket);
    void arrivalNotify();
    void requestItem(unsigned long int item_id);
    int* listContents();
}

