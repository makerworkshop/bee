#include "Bee.h"
#include "resp.h"

Bee::Bee()
{
  this->cursor = 0;
}

void Bee::replyOK()
{
  this->Write((unsigned char *)RESP_OK, 5);
}

void Bee::replyError()
{
  this->Write((unsigned char *)RESP_ERROR, 6);
}

respObject* Bee::OnMessage(respObject *in)
{
  int pin;

  // Basic control commands.
  if (RESP_EQUAL(RESP_ELEMENT_AT(in, 0), "SET", 3)) {
    // SET {pin} {ON|OFF}
    pin = RESP_ATOI(RESP_ELEMENT_AT(in, 1));
    pinMode(pin, OUTPUT);
    if (RESP_EQUAL(RESP_ELEMENT_AT(in, 2), "ON", 2)) {
      digitalWrite(pin, HIGH);
    } else {
      digitalWrite(pin, LOW);
    }
    return createRespString(RESP_OBJECT_STATUS, "OK");
  } else if (RESP_EQUAL(RESP_ELEMENT_AT(in, 0), "GET", 3)) {
    // GET {pin}
    pin = RESP_ATOI(RESP_ELEMENT_AT(in, 1));
    pinMode(pin, INPUT);
    return createRespInteger(digitalRead(pin));
  } else if (RESP_EQUAL(RESP_ELEMENT_AT(in, 0), "READ", 4)) {
    // READ {pin}
    pin = RESP_ATOI(RESP_ELEMENT_AT(in, 1));
    pinMode(pin, INPUT);
    return createRespInteger(analogRead(pin));
  } else if (RESP_EQUAL(RESP_ELEMENT_AT(in, 0), "WRITE", 5)) {
    // WRITE {pin} {0-255 value}
    pin = RESP_ATOI(RESP_ELEMENT_AT(in, 1));
    pinMode(pin, OUTPUT);
    analogWrite(pin, RESP_ATOI(RESP_ELEMENT_AT(in, 2)));
    return createRespString(RESP_OBJECT_STATUS, "OK");
  }

  return NULL;
}

void Bee::Next()
{
  /* Listen for new data. */
  int i;
  char c;
  int ok;
  bool gotLine;

  respObject *in = NULL;
  respObject *out = NULL;

  gotLine = false;

  /* Should we clean the buffer? */
  if (this->cursor == 0) {
    for (i = 0; i < BUFLEN; i++) {
      this->buf[i] = 0;
    }
  }

  /* Reading from serial port. */
  for (gotLine = false; !gotLine && this->Read(&c); this->cursor++) {

    if (this->cursor >= BUFLEN) {
      this->replyError();
      this->cursor = 0;
      return;
    }

    this->buf[this->cursor] = c;

    if (this->cursor > 0) {
      if (this->buf[this->cursor] == '\n' && this->buf[this->cursor - 1] == '\r') {
        gotLine = true;
      }
    }
  }

  /* Processing line */
  if (gotLine) {
    ok = respDecode(&in, this->buf);

    if (ok > 0) {

      out = this->OnMessage(in);
      freeRespObject(in);

      if (out == NULL) {
        this->replyOK();
      } else {
        ok = respEncode(out, this->buf);
        if (ok > 0) {
          this->Write(this->buf, ok);
        } else {
          this->replyError();
        }
        freeRespObject(out);
      }

      this->cursor = 0;
    }
  }
}

