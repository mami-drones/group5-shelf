
/*
Скетч для загрузки на устройства, связывающиеся с полкой.

Имитирует робота, который соединяется с единственной полкой №2,
сообщает о приезде, и запрашивает вещь с определенного этажа.
*/


// BEGIN Robot behaviour
#define ROBOT_SHELF_ARRIVING 1
#define ROBOT_SHELF_PARKING 2
#define ROBOT_SHELF_GRABBING 3
#define ROBOT_SHELF_CALLDISPENCE 4
#define ROBOT_SHELF_LEAVING 5
#define ROBOT_SCRAM 6
int mode = ROBOT_SCRAM; // robot interaction state
unsigned int cell=3; // shelf cell
uint64_t shelf_id=2; 


// Radio
#include <SPI.h>
#include <RF24.h>
RF24 radio(9,10);
const uint8_t pipe_shelf[6] = {'1', 'S', 'H', 'L', 'F'};
const uint8_t pipe_robot[6] = {'1', 'R', 'O', 'B', 'O'};


// Support functions
void serial_setup();
void radio_setup(RF24 *cl_radio, const uint8_t* pipe_w, const uint8_t* pipe_r);
void rf24write(const void *buf, uint8_t len);
void rf24read(void *buf,  uint8_t len);


// Shelf
#include <NetShelf.h>
NetShelf shelf(shelf_id, &rf24read, &rf24write);


void setup() 
{
  serial_setup();
  radio_setup(&radio, pipe_robot, pipe_shelf);
  
  shelf.begin();
  while (!shelf.available() && shelf.status()!=SHELF_ONLINE)
  {
    switch (shelf.status())
    {
      case SHELF_OFFLINE:
        Serial.println("Shelf is not available in the network. ");
        break;
      case SHELF_PROTO_FAIL:
        Serial.println("Protocol faillure. ");
    }
    delay(2000);
  }
  Serial.print("Connected to shelf ");
  Serial.print(shelf.readID());
  Serial.println(".");
  mode = ROBOT_SHELF_ARRIVING;
}

//
void loop() 
{
  bool arrived=false, parked=false;
  if (mode == ROBOT_SHELF_ARRIVING)
  // robot goes to the shelf
  {
    // your code here: moving to the shelf
    if (arrived)
    {
      mode = ROBOT_SHELF_PARKING;
    }
  }
  else if (mode == ROBOT_SHELF_PARKING)
  // robot park near the shelf, ready to take item
  {
    if (shelf.makeBusy())
    {
      //your code here: parking to the shelf.
      if (parked)
      {
        mode = ROBOT_SHELF_GRABBING;
        shelf.setDeployReady(true);
      }
    }
    else 
    {
      //oops, someone uses our right now shelf 
      //or maybe forgets to release the shell
    }
  }
  else if (mode == ROBOT_SHELF_CALLDISPENCE)
  // robot requests item
  {
    // robot requests item from cell 3 
    shelf.dispence(3);
    mode = ROBOT_SHELF_GRABBING;
  }
  else if (mode == ROBOT_SHELF_GRABBING) 
  {
    // wait until dispencing finished.
    do 
    {
      delay(1000);
    } while (shelf.dispencing());
    mode = ROBOT_SHELF_LEAVING;
  }
  else if (mode == ROBOT_SHELF_LEAVING) 
  // robot leaves shelf
  {
    
  }
  else if (mode == ROBOT_SCRAM)
  // other labour not related to the shelf.
  {
    // your code here: robot goes away futher from shelf 
    // doing it's own business (ex: moving item to pay desk)
  }
}






// SUPPORT FUNCTIONS

void serial_setup()
{
  Serial.begin(115200);
  while(!Serial)
  {
    delay(100);
  }
}


void radio_setup(RF24 *cl_radio, uint8_t *pipe_w, uint8_t *pipe_r)
{
  cl_radio->begin();
  cl_radio->openWritingPipe(pipe_w);
  cl_radio->openReadingPipe(1, pipe_r);
}

void rf24write(void *buf, uint8_t len)
{
  radio.write(buf, len);
}


void rf24read(void *buf,  uint8_t len)
{
  radio.read(buf, len);
}

