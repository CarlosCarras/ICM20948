/*
  lsquaredc.h

  Copyright (C) 2014 Jan Rychter
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  Update: added extern "C"
*/

#ifndef LSQUAREDC_H
#define LSQUAREDC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define I2C_RESTART     1<<8    /* repeated start */
#define I2C_READ		2<<8    /* read a byte */

int i2c_open(uint8_t bus);

int i2c_send_sequence(int handle, uint16_t *sequence, uint32_t sequence_length, uint8_t *received_data);

int i2c_close(int handle);

#ifdef __cplusplus
}
#endif

#endif // LSQUAREDC_H
