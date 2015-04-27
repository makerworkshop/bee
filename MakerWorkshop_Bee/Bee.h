/* Bee is a library that enables devices to communicate with other devices
 * using RESP encoded messages over different user-implemented communication
 * channels.
 * */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <resp.h>

// RESP_TOKEN_LENGTH_AT returns the length of the token at the given index.
#define RESP_TOKEN_LENGTH_AT(INPUT, INDEX)      ((INDEX < INPUT->elements) ? INPUT->element[INDEX]->len : 0)

// RESP_TOKEN_AT returns a pointer to the token at the given index.
#define RESP_TOKEN_AT(INPUT, INDEX)             ((INDEX < INPUT->elements) ? INPUT->element[INDEX]->str : (unsigned char *)"\0")

// RESP_TOKEN_TO_INT returns the integer value of the token at the given index.
#define RESP_TOKEN_TO_INT(INPUT, INDEX)         atoi((char*) RESP_TOKEN_AT(INPUT, INDEX))

// RESP_TOKEN_EQUALS returns true if the value of the token at the given index
#define RESP_TOKEN_EQUALS(INPUT, INDEX, CONST)  (memcmp(RESP_TOKEN_AT(INPUT, INDEX), (unsigned char *)CONST, RESP_TOKEN_LENGTH_AT(INPUT, INDEX)) == 0)

#define RESP_OK     "+OK\r\n"
#define RESP_ERROR  "-ERR\r\n"

#define BUFLEN      256

// Bee class defines virtual methods that can be satisfied by derived classes
// that want to communicate using RESP formatted messages.
class Bee {
private:
  int cursor;
  unsigned char buf[BUFLEN];
  void replyOK();
  void replyError();

public:
  Bee();

  // NextMessage attempts to read the next message, if the message is
  // incomplete, it will exit and return the control to the caller. Returns
  // true when a complete message was received, false otherwise.
  bool NextMessage();

  // Open attempts to configure the communication channel and to leave it ready
  // for reading and writing. Returns true on success, false otherwise.
  virtual bool Open()=0;

  // Read attempts to read a byte from the communication channel and to write
  // it to <b>. Returns true on success, false otherwise.
  virtual bool Read(char *b)=0;

  // Write attempts to write <len> bytes to the communication channel, the
  // source for the write is <buf>. Returns true on success, false otherwise.
  virtual bool Write(unsigned char *buf, int len)=0;

  // Close closes the communication channel. Returns true on success, false
  // otherwise.
  virtual bool Close()=0;

  // OnMessage executes predefined actions based on the following messages:
  //
  // SET {PIN NUMBER} {1|0}
  // SET performs a digital write to the given PIN. A value of 1 means HIGH
  // while 0 means LOW.
  //
  // GET {PIN NUMBER}
  // GET performs a digital read from the given PIN. Returns either 0 or 1 for
  // HIGH or LOW respectively.
  //
  // WRITE {PIN NUMBER} {0 - 255} WRITE performs an analog write to the given
  // PIN. Accepts a value between 0 and 255.
  //
  // READ {PIN NUMBER} READ performs an analog read from the given PIN. Returns
  // a value between 0 and 1023.
  virtual respObject* OnMessage(respObject*);
};
