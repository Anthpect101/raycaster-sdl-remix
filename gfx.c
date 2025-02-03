#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

#define TEX_TAG 0x55AA
char errstr[256];

typedef struct ManagedTexture_ ManagedTexture_;
struct ManagedTexture_ {
    void* pixelData;
    SDL_Texture* texture;
    Uint32 pitch;
    ManagedTexture_* next;
    ManagedTexture_* prev;
    Uint16 magicTag;
};
ManagedTexture_* managedTextures = NULL;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
unsigned int screenWidth  = -1;
unsigned int screenHeight = -1;

const char* gfxGetError() {
    return errstr;
}

void gfxSetError(char* str, char showSdlErr) {
    if(showSdlErr)
        sprintf(errstr, "%s: %s", str, SDL_GetError());
    else
        sprintf(errstr, "%s", str);
}

int initGFX(char* title, unsigned int width, unsigned int height) {
    if(window || renderer) return 0;

    screenWidth = width;
    screenHeight = height;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        gfxSetError("Error initializing SDL", 1);
        return 0;
    }

    window = SDL_CreateWindow(title, 50, 50, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        gfxSetError("Could not create SDL window", 1);
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        gfxSetError("Could not create SDL renderer", 1);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    SDL_SetWindowTitle(window, title);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return 1;
}

void* createTexture(unsigned int width, unsigned int height) {
    if(!width || !height || !renderer) {
        gfxSetError("SDL window has not been initialized yet", 0);
        return NULL;
    }

    ManagedTexture_* newmtex = malloc(sizeof(ManagedTexture_));
    if(!newmtex) return NULL;

    newmtex->pitch = width * sizeof(Uint32);
    newmtex->next = NULL;
    newmtex->prev = NULL;
    newmtex->magicTag  = TEX_TAG;

    newmtex->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(!(newmtex->texture)) {
        free(newmtex);
        gfxSetError("Could not create texture", 1);
        return NULL;
    }

    Uint32* data = malloc((sizeof(Uint32) * width * height) + sizeof(ManagedTexture_*));
    if(!data) {
        free(newmtex);
        return NULL;
    }

    *((ManagedTexture_**)data) = newmtex;
    newmtex->pixelData = ((ManagedTexture_**)data) + 1;

    if(managedTextures) {
        newmtex->next = managedTextures;
        managedTextures->prev = newmtex;
    }
    managedTextures = newmtex;

    return newmtex->pixelData;
}

int destroyTexture(void* ptr) {
    ManagedTexture_* mtex = *(((ManagedTexture_**)ptr) - 1);
    if(mtex->magicTag != TEX_TAG) {
        gfxSetError("Not a valid texture pointer", 0);
        return 0;
    }

    mtex->magicTag = 0;
    free(((ManagedTexture_**)ptr) - 1);
    SDL_DestroyTexture(mtex->texture);

    if(mtex->prev) mtex->prev->next = mtex->next;
    if(mtex->next) mtex->next->prev = mtex->prev;
    if(managedTextures == mtex) managedTextures = mtex->next;
    free(mtex);

    return 1;
}

void displayFullscreenTexture(void* texture) {
    if(!window || !renderer) {
        gfxSetError("SDL window has not been initialized yet", 0);
        return;
    }

    ManagedTexture_* mtex = *(((ManagedTexture_**)texture) - 1);
    if(mtex->magicTag != TEX_TAG) {
        gfxSetError("Not a valid texture pointer", 0);
        return;
    }

    SDL_UpdateTexture(mtex->texture, NULL, mtex->pixelData, mtex->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, mtex->texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void destroyGFX() {
    while(managedTextures) destroyTexture(managedTextures->pixelData);

    if(window && renderer) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

void setDrawColor(int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void fillRect(int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderDrawRect(renderer, &rect);
}

void presentRenderer() {
    SDL_RenderPresent(renderer);
}

void clearRenderer() {
    SDL_RenderClear(renderer);
}

Uint32* generateXorTexture(int size, int redmask, int greenmask, int bluemask) {
    Uint32* texture = createTexture(size, size);
    if (!texture) return NULL;

    for(int x = 0; x < size; x++)
        for(int y = 0; y < size; y++)
            texture[(size * y) + x] = RGBtoABGR((int)((x ^ y) * (256.0f / size)) & redmask, 
                                                     (int)((x ^ y) * (256.0f / size)) & greenmask, 
                                                     (int)((x ^ y) * (256.0f / size)) & bluemask);
    return texture;
}

Uint32* generateRedXorTexture(int size) {
    return generateXorTexture(size, 0xFF, 0x00, 0x00);
}

Uint32* generateGreenXorTexture(int size) {
    return generateXorTexture(size, 0x00, 0xFF, 0x00);
}

Uint32* generateBlueXorTexture(int size) {
    return generateXorTexture(size, 0x00, 0x00, 0xFF);
}

Uint32* generateGrayXorTexture(int size) {
    return generateXorTexture(size, 0xFF, 0xFF, 0xFF);
}