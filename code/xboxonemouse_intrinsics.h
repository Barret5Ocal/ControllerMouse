#if !defined(XBOXONEMOUSE_INTRINSICS_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Barret Gaylor $
   $Notice: (C) Copyright 2015 by Barret Gaylor. All Rights Reserved. $
   ======================================================================== */

#define XBOXONEMOUSE_INTRINSICS_H

#include "math.h"


inline int32
RoundReal32ToInt32(real32 Real32)
{
    
    int32 Result = (int32)roundf(Real32);
    return(Result);
    
}

inline int32
RoundReal32ToUInt32(real32 Real32)
{
    uint32 Result = (uint32)roundf(Real32);
    return(Result);
}

inline int32
FloorReal32ToInt32(real32 A)
{
    int32 Result = (int32)floorf(A);

    return(Result);
}

inline int32
CeilReal32ToInt32(real32 A)
{
    int32 Result = (int32)ceilf(A);

    return(Result);
}

#endif
