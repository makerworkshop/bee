# Bee

Bee is a library that enables devices (commonly Arduino) to communicate with
other devices using human-readable [RESP][1] encoded messages over different
user-implemented communication channels such as serial port, bluetooth or
Wi-Fi.

Bee could be awesome if you're willing to give up from 16 (average) up to 256
bytes of precious RAM only for exchanging messages, that could be the case for
devices with simple sensors that just need to report sensor data to another
device that is going to interprete them (see [Hive][3]).

If you're looking for a more mature (and not human-readable) protocol you
should try [Firmata][2] first.

## How to install (on Arduino)

Download the latest [.ZIP package][4] and follow the official instructions on
[importing a .ZIP library][5].

## License

> Copyright (c) 2015 José Carlos Nieto, https://menteslibres.net/xiam
>
> Permission is hereby granted, free of charge, to any person obtaining
> a copy of this software and associated documentation files (the
> "Software"), to deal in the Software without restriction, including
> without limitation the rights to use, copy, modify, merge, publish,
> distribute, sublicense, and/or sell copies of the Software, and to
> permit persons to whom the Software is furnished to do so, subject to
> the following conditions:
>
> The above copyright notice and this permission notice shall be
> included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
> EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
> MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
> NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
> LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
> OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
> WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[1]: http://redis.io/topics/protocol
[2]: http://www.arduino.cc/en/Reference/Firmata
[3]: https://github.com/makerworkshop/hive
[4]: https://github.com/makerworkshop/bee/archive/master.zip
[5]: http://www.arduino.cc/en/guide/libraries
