#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <resp.h>

#define RESP_EQUAL(X, Y, Z)     (memcmp(X, Y, Z) == 0)
#define RESP_ATOI(X)            atoi((char*)X)
#define RESP_ELEMENT_AT(X, Y)   ((X->elements > Y) ? X->element[Y]->str : (unsigned char *)"")

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
