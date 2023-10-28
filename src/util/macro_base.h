#pragma once

#define BREAK() __builtin_trap()

#define STMNT(S) do{ S }while(0)

#define ASSERT(c) STMNT( if (!(c)){ BREAK(); } )

// -- math --

#define Min(a, b) (((a)<b))?(a):(b)
#define Max(a, b) (((a)>b))?(a):(b)

#define Clamp(a, x, b) (((x)<(a))?(a):\
                        ((b)<(x))?(b):(x))

#define ClampTop(a, b) Min(a, b)
#define ClampBot(a, b) Max(a, b)
