#ifndef GFX_H
#define GFX_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#define RGBtoABGR(R,G,B)   (0xFF000000 | ((B) << 16) | ((G) << 8) | (R))

const char* gfxGetError();
int initGFX(char* title, unsigned int width, unsigned int height);
void* createTexture(unsigned int width, unsigned int height);
int destroyTexture(void* texture);
void displayFullscreenTexture(void* texture);
void destroyGFX();
void setDrawColor(int r, int g, int b, int a);
void drawLine(int x1, int y1, int x2, int y2);
void fillRect(int x, int y, int w, int h);
void drawRect(int x, int y, int w, int h);
void presentRenderer();
void clearRenderer();
Uint32* generateXorTexture(int size, int redmask, int greenmask, int bluemask);
Uint32* generateRedXorTexture(int size);
Uint32* generateGreenXorTexture(int size);
Uint32* generateBlueXorTexture(int size);
Uint32* generateGrayXorTexture(int size);

#endif /* GFX_H */