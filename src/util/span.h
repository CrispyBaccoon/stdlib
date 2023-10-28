#pragma once

#include "types.h"

struct span {
  byte *v;
  usize size;

  usize size_bytes() {
    return sizeof(v);
  }

  byte* ptr() { return this->v; }

  /* T& operator[](int i) { */
  /*   if (i > this->size) { */
  /*     // FIXME: proper error handling */
  /*     return v[0]; */
  /*   } */
  /**/
  /*   return v[i]; */
  /* } */
};

#define span_wrap(_s, _v) _s.v = &_v
#define span_new(_v) { &_v, sizeof(_v) }
