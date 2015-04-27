#include "Bee.h"
#include "resp.h"

// Bee constructor initializes variables that are used to determine the state
// within Next.
Bee::Bee()
{
  this->cursor = 0;
}

// replyOK writes a status message with value RESP_OK to the connection
// channel.
void Bee::replyOK()
{
  this->Write((unsigned char *)RESP_OK, 5);
}

// replyError writes a status message with value RESP_ERROR to the connection
// channel.
void Bee::replyError()
{
  this->Write((unsigned char *)RESP_ERROR, 6);
}

// OnMessage implements default actions when receiving messages.
respObject* Bee::OnMessage(respObject *in)
{
  int pin;

  if (RESP_TOKEN_EQUALS(in, 0, "SET")) {
    // SET {pin} {ON|OFF}
    pin = RESP_TOKEN_TO_INT(in, 1);
    pinMode(pin, OUTPUT);
    if (RESP_TOKEN_EQUALS(in, 2, "1")) {
      digitalWrite(pin, HIGH);
    } else {
      digitalWrite(pin, LOW);
    }
    // Returns status OK.
    return createRespString(RESP_OBJECT_STATUS, "OK");
  } else if (RESP_TOKEN_EQUALS(in, 0, "GET")) {
    // GET {pin}
    pin = RESP_TOKEN_TO_INT(in, 1);
    pinMode(pin, INPUT);
    // Returns integer.
    return createRespInteger(digitalRead(pin));
  } else if (RESP_TOKEN_EQUALS(in, 0, "READ")) {
    // READ {pin}
    pin = RESP_TOKEN_TO_INT(in, 1);
    pinMode(pin, INPUT);
    // Returns integer.
    return createRespInteger(analogRead(pin));
  } else if (RESP_TOKEN_EQUALS(in, 0, "WRITE")) {
    // WRITE {pin} {0-255 value}
    pin = RESP_TOKEN_TO_INT(in, 1);
    pinMode(pin, OUTPUT);
    analogWrite(pin, RESP_TOKEN_TO_INT(in, 2));
    // Returns status OK.
    return createRespString(RESP_OBJECT_STATUS, "OK");
  }

  // If nothing matched, return NULL, a NULL value will be catched by Next and
  // transformed into an error.
  return NULL;
}

// NextMessage creates a RESP message with consecutive calls to Read.
bool Bee::NextMessage()
{
  int i, ok;
  char c;
  bool gotLine;

  respObject *in = NULL;
  respObject *out = NULL;

  gotLine = false;

  // Cleaning initial buffer.
  if (this->cursor == 0) {
    for (i = 0; i < BUFLEN; i++) {
      this->buf[i] = 0;
    }
  }

  // Reading one byte at a time and advancing cursor.
  for (gotLine = false; !gotLine && this->Read(&c); this->cursor++) {

    if (this->cursor >= BUFLEN) {
      this->replyError();
      this->cursor = 0;
      return false;
    }

    this->buf[this->cursor] = c;

    if (this->cursor > 0) {
      if (this->buf[this->cursor] == '\n' && this->buf[this->cursor - 1] == '\r') {
        gotLine = true;
      }
    }
  }

  // Processing line.
  if (gotLine) {
    // Attempt to decode message.
    ok = respDecode(&in, this->buf);

    if (ok > 0) {

      out = this->OnMessage(in);
      freeRespObject(in);

      if (out == NULL) {
        this->replyError();
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
      return true;
    }
  }

  return false;
}
