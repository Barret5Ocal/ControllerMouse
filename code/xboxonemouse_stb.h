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

    // TODO(barret): Pre-multiply the alpha
    uint32 *SourceRow = (uint32 *)Data;
    for(int32 Yi = 0;
        Yi < Y;
        ++Yi)
    {
        for(int32 Xi = 0;
            Xi < X;
            ++Xi)
        {
            uint32 C = *SourceRow;             
            real32 A = (real32)((C >> 24) & 0xFF);  
            real32 B = (real32)((C >> 16) & 0xFF);
            real32 G = (real32)((C >> 8) & 0xFF);
            real32 R = (real32)((C >> 0) & 0xFF);
            real32 NA = (A / 255.0f);

            B = B*NA;
            G = G*NA;
            R = R*NA;
            
            *SourceRow = (((uint32)(A + 0.5f) << 24) |
                          ((uint32)(R + 0.5f) << 16) |
                          ((uint32)(G + 0.5f) << 8) |
                          ((uint32)(B + 0.5f) << 0)); 

        }
    }
    
    Result.Width = X;
    Result.Height = Y;
    Result.Memory = Data; 
    Result.Pitch = Result.Width*BITMAP_BYTES_PER_PIXEL;
    return(Result);
}

#define XBOXONEMOUSE_STB_H
#endif

