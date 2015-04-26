#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <resp.h>

#define RESP_TOKEN_LENGTH_AT(INPUT, INDEX)      ((INDEX < INPUT->elements) ? INPUT->element[INDEX]->len : 0)
#define RESP_TOKEN_AT(INPUT, INDEX)             ((INDEX < INPUT->elements) ? INPUT->element[INDEX]->str : (unsigned char *)"\0")
#define RESP_TOKEN_TO_INT(INPUT, INDEX)         atoi((char*) RESP_TOKEN_AT(INPUT, INDEX))
#define RESP_TOKEN_EQUALS(INPUT, INDEX, CONST)  (memcmp(RESP_TOKEN_AT(INPUT, INDEX), (unsigned char *)CONST, RESP_TOKEN_LENGTH_AT(INPUT, INDEX)) == 0)

#define RESP_OK     "+OK\r\n"
#define RESP_ERROR  "-ERR\r\n"

#define BUFLEN 128

class Bee {
  private:
    int cursor;
    unsigned char buf[BUFLEN];
    void replyOK();
    void replyError();

  public:
    Bee();
    ~Bee();

    void Next();

    virtual respObject* OnMessage(respObject*);
    virtual bool Open()=0;
    virtual bool Read(char *)=0;
    virtual bool Write(unsigned char *, int)=0;
    virtual bool Close()=0;
};
