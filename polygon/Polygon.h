#ifndef Polygon_h
#define Polygon_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Poly {
private:
  double _Lt[8] = {-26.883454, -26.884596, -26.880704,-26.881126,-26.878721,-26.878303,-26.875426,-26.874267};
  double _Lg[8] = {-48.659007, -48.658126,-48.651732,-48.651422,-48.647482,-48.647790,-48.642940,-48.643807};
public: 
  bool IsPointInPolygon(double lt, double lg);
};

#endif