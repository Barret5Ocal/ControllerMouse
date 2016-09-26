#if !defined(XBOXONEMOUSE_STB_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Barret Gaylor $
   $Notice: (C) Copyright 2015 by Barret Gaylor. All Rights Reserved. $
   ======================================================================== */
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "../../stb/stb_image.h"


internal loaded_bitmap
STBLoadMap(char *Filename)
{
    loaded_bitmap Result = {};
    int X,Y,N;
    unsigned char *Data = stbi_load(Filename, &X, &Y, &N, 4);

    Result.Width = X;
    Result.Height = Y;
    Result.Memory = Data; 
    Result.Pitch = Result.Width*BITMAP_BYTES_PER_PIXEL;
    return(Result);
}

#define XBOXONEMOUSE_STB_H
#endif

