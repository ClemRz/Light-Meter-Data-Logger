/**
Copyright (c) 2018, Clément Ronzon
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of current-data-logger nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Wire.h>
#include <BH1750.h>
#include <SdFat.h>

#define MILLISEC                 1000
#define SEC                      1L
#define MINUTE                   (unsigned int) 60L*SEC
#define HOUR                     (unsigned int) 60L*MINUTE
#define DAY                      (unsigned long) 24L*HOUR
#define WEEK                     (unsigned long) 7L*DAY

// Pins:
#define SD_CS                    10             // SD chip select pin

// Settings
#define SAMPLE_INTERVAL          6*SEC*MILLISEC
#define FLUSH_INTERVAL           1*MINUTE*MILLISEC
#define READ_SAMPLES             20

// Global variables
SdFat SD;
SdFile _logfile;
uint32_t _syncTime =             0;
BH1750 _bh1750;
static unsigned long
  _sampleTimer =                 0,
  _flushTimer =                  0;

void setup(void) {
  Serial.begin(9600);
  Serial.println();
  initBH1750();
  initCard();
  initFile();
  writeHeaders();
  flush2SD();
}

void loop(void) {
  if (millis() - _sampleTimer > (unsigned long)SAMPLE_INTERVAL) {
    writeData(getRadiation());
    _sampleTimer = millis();
  }
  if (millis() - _flushTimer > (unsigned long)FLUSH_INTERVAL) {
    flush2SD();
    _flushTimer = millis();
  }
}
