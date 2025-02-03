#ifndef RENDERER_H
#define RENDERER_H

#include "gfx.h"
#include "linalg.h"

#define XY_TO_SCREEN_INDEX(X, Y)   (((Y) * WINDOW_WIDTH) + (X))
#define XY_TO_TEXTURE_INDEX(X, Y)   (((Y) * TEXTURE_SIZE) + (X))
#define DARKEN_COLOR(C)     ((((C) >> 1) & 0x7F7F7F7F) | 0xFF000000)

typedef enum {HORIZONTAL_RAY, VERTICAL_RAY} RayType;

float calculateDrawHeight(float rayLength);
void drawTexturedStrip(int x, float wallYStart, float length, int textureX, Uint32* texture, char darken);
void drawUntexturedStrip(int x, float wallYStart, float length, Uint32 ABGRColor, char darken);
int getTextureColumnNumberForRay(Vector3f* ray, RayType rtype);
float getUndistortedRayLength(Vector3f* ray);
void renderProjectedScene();

#endif /* RENDERER_H */