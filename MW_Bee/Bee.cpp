#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <resp.c>

#define EQUALS(X, Y, Z) memcmp(X, Y, Z) == 0
#define STR2INT(X) atoi((char*) X)
#define ELEMENT_STR(X, Y) ((X->elements > Y) ? X->element[Y]->str : (unsigned char *)"")

#define BUFLEN 512

class Bee {
  private:
    int cursor;
    unsigned char buf[BUFLEN];

  public:
    Bee();
    ~Bee();

    void Step();

    virtual bool Open()=0;
    virtual bool Read(char *)=0;
    virtual bool Write(unsigned char *, int)=0;
    virtual bool Close()=0;
};

Bee::Bee()
{
  this->cursor = 0;
}

void Bee::Step()
{
  /* Listen for new data. */
  int i;
  char *c;
  int ok;
  bool gotLine;

  respObject *in;
  respObject *out;

  gotLine = false;

  /* Should we clean the buffer? */
  if (this->cursor == 0) {
    for (i = 0; i < BUFLEN; i++) {
      this->buf[i] = 0;
    }
  }

  /* Reading from serial port. */
  while (this->Read(c)) {
    this->buf[this->cursor] = *c;

    if (this->cursor > 0) {
      if (this->buf[this->cursor] == '\n' && this->buf[this->cursor - 1] == '\r') {
        gotLine = true;
        this->cursor++;
        break;
      }
    }

    this->cursor++;
  }

  /* Processing line */
  if (gotLine) {
    ok = respDecode(&in, this->buf);

    if (ok > 0) {

      int pin;
      int value;

      out = NULL;

      if (EQUALS(ELEMENT_STR(in, 0), "SET", 3)) {
        pin = STR2INT(ELEMENT_STR(in, 1));
        pinMode(pin, OUTPUT);
        if (EQUALS(ELEMENT_STR(in, 2), "ON", 2)) {
          digitalWrite(pin, HIGH);
        } else {
          digitalWrite(pin, LOW);
        }
      } else if (EQUALS(ELEMENT_STR(in, 0), "GET", 3)) {
        pin = STR2INT(ELEMENT_STR(in, 1));
        pinMode(pin, INPUT);
        out = createRespInteger(digitalRead(pin));
      } else if  (EQUALS(ELEMENT_STR(in, 0), "READ", 4)) {
        pin = STR2INT(ELEMENT_STR(in, 1));
        pinMode(pin, INPUT);
        out = createRespInteger(analogRead(pin));
      } else if (EQUALS(ELEMENT_STR(in, 0), "WRITE", 5)) {
        pin = STR2INT(ELEMENT_STR(in, 1));
        pinMode(pin, OUTPUT);

        value = STR2INT(ELEMENT_STR(in, 2));
        analogWrite(pin, value);
      } else {
        out = createRespString(RESP_OBJECT_ERROR, "UNKNOWN_COMMAND\0");
      }

      /* Send ackowledgement */
      if (out == NULL) {
        this->Write((unsigned char *)"+OK\r\n", 5);
      } else {
        ok = respEncode(out, this->buf);
        if (ok > 0) {
          this->Write(this->buf, ok);
        } else {
          this->Write((unsigned char *)"-ERROR\r\n", 8);
        }
      }

      freeRespObject(in);
      freeRespObject(out);

      this->cursor = 0;
    }

  }

}

