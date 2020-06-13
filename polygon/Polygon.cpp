#include "Arduino.h"
#include "Polygon.h"


bool Poly::IsPointInPolygon(double lt, double lg)
{
    int i, j;
    bool c = false;
    j = 7;
    for (i = 0; i < 8; j = i++)
    {
        if ((((_Lt[i] <= lt) && (lt < _Lt[j])) 
                || ((_Lt[j] <= lt) && (lt < _Lt[i]))) 
                && (lg < (_Lg[j] - _Lg[i]) * (lt - _Lt[i]) 
                    / (_Lt[j] - _Lt[i]) + _Lg[i]))
        {

            c = !c;
        }
    }

    return c;
}