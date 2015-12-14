
#include "..\include\cspriteslope.h"

cSpriteSlope::cSpriteSlope(int pixels) {
    // TODO (Bastiaan#1#): int needs to be a 8 bit int, this object is created many times. So keeping it small is best. At this time i cant find the UINT8 datatype.

    // Initialise data object
    slopevalue = new int*[pixels];
    for(int i=0; i < pixels; i++)
        slopevalue[i] = 0;
}

cSpriteSlope::~cSpriteSlope() {
}
