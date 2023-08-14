#include "SevSegSource.h"

SevSeg sevseg;

void SevSegSourceSetup()
{
  byte numDigits = 4;
  byte digitPins[] = {2, 5, 6, 8};
  byte segmentPins[] = {3, 7, 12, 10, 9, 4, 13, 11};
  bool resistorsOnSegments = false;     // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false;        // Default 'false' is Recommended
  bool leadingZeros = false;            // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true;          // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
           updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

void SevSegSetNumber(int32_t numToShow)
{
  sevseg.setNumber(numToShow, -1, 0);
}

void SevSegSetChars(const char str[])
{
  sevseg.setChars(str);
}

void SevSegRefreshDisplay()
{
  sevseg.refreshDisplay();
}