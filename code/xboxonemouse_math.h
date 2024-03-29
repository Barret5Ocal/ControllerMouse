#if !defined(XBOXONEMOUSE_MATH_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Barret Gaylor $
   $Notice: (C) Copyright 2015 by Barret Gaylor. All Rights Reserved. $
   ======================================================================== */

union v2
{
    struct
    {
        real32 X;
        real32 Y;
    };

    real32 E[2];
};

union v4
{
    struct
    {
        real32 x, y, z, w;
    };
    struct
    {
        real32 r, g, b, a;
    };
    real32 E[4];
};

inline v2
V2(real32 X, real32 Y)
{
    v2 Result = {};

    Result.X = X;
    Result.Y = Y;

    return(Result);
}

inline v2
V2i(int32 X, int32 Y)
{
    v2 Result = {};

    Result.X = (real32)X;
    Result.Y = (real32)Y;

    return(Result);
}

inline v4
V4(real32 X, real32 Y, real32 Z, real32 W)
{
    v4 Result;

    Result.x = X;
    Result.y = Y;
    Result.z = Z;
    Result.w = W;

    return(Result);
}

inline v2
operator+(v2 A, v2 B)
{
    v2 Result = {};

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;

    return(Result);
}

inline v2 &
operator+=(v2 &A, v2 B)
{
   
    A = A + B;

    return(A);
}

inline v2
operator-(v2 A, v2 B)
{
    v2 Result = {};

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;

    return(Result);
}

inline v2
operator-(v2 A)
{
    v2 Result = {};

    Result.X = -A.X;
    Result.Y = -A.Y;

    return(Result);
}

inline v2 &
operator-=(v2 &A, v2 B)
{

    A = A - B;

    return(A);
}

inline v2
operator*(real32 A, v2 B)
{
    v2 Result = {};

    Result.X = A*B.X;
    Result.Y = A*B.Y;

    return(Result);
}

inline v2
operator*(v2 B, real32 A)
{
    v2 Result = {};

    Result.X = A*B.X;
    Result.Y = A*B.Y;

    return(Result);
}

inline v2 &
operator*=(v2 &B, real32 A)
{
    B = A * B;

    return(B);
}

inline v2
operator/(v2 B , real32 A)
{
    v2 Result = {};

    Result.X = A*B.X;
    Result.Y = A*B.Y;

    return(Result);
}

inline v4
operator*(real32 A, v4 B)
{
    v4 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;
    Result.z = A*B.z;
    Result.w = A*B.w;
    
    return(Result);
}

inline v4
operator*(v4 B, real32 A)
{
    v4 Result = A*B;

    return(Result);
}

inline v4 &
operator*=(v4 &B, real32 A)
{
    B = A * B;

    return(B);
}

inline v4
operator-(v4 A)
{
    v4 Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;
    Result.w = -A.w;

    return(Result);
}

inline v4
operator+(v4 A, v4 B)
{
    v4 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;
    Result.w = A.w + B.w;

    return(Result);
}

inline v4 &
operator+=(v4 &A, v4 B)
{
    A = A + B;

    return(A);
}

inline v4
operator-(v4 A, v4 B)
{
    v4 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    Result.w = A.w - B.w;

    return(Result);
}

inline real32
Square(real32 A)
{
    real32 Result = A*A;

    return(Result);
}

inline v2
Perp(v2 A)
{
    v2 Result = {-A.Y, A.X};
    return(Result);
}

inline real32
Inner(v2 A, v2 B)
{
    real32 Result = A.X*B.X + A.Y*B.Y;

    return(Result);
}

inline v2
Reflect(v2 A, v2 R)
{
    v2 Result = A - 2*Inner(A, R)*R;

    return(Result);
}

inline real32
LengthSq(v2 A)
{
    real32 Result = Inner(A, A);

    return(Result);
}

inline real32
Min(real32 A, real32 B)
{
    if(A <= B)
    {
        return(A);
    }
    else
    {
        return(B);
    }
    
}

inline v4
Lerp(v4 A, real32 t, v4 B)
{
    v4 Result = (1.0f - t)*A + t*B;

    return(Result);
}


#define XBOXONEMOUSE_MATH_H
#endif
