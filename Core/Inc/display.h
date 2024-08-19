#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void
display_init      (void);

void
display_update    (uint16_t  *frameBuffer,
                   uint16_t   dispWidth,
                   uint16_t   dispHeight,
                   int16_t    rectX,
                   int16_t    rectY,
                   int16_t    rectWidth,
                   int16_t    rectHeight);

#ifdef __cplusplus
}
#endif

#endif /* __DISPLAY_H */
