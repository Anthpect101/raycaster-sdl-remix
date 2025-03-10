#include <stdlib.h>
#include "config.h"
#include "renderer.h"
#include "raycaster.h"
#include "player.h"

float calculateDrawHeight(float rayLength) {
    return distFromViewplane * WALL_SIZE / rayLength;
}

void drawUntexturedStrip(int x, float wallYStart, float length, Uint32 ABGRColor, char darken) {
    if(wallYStart < 0) wallYStart = 0;

    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        if(y < wallYStart) {
            screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = CEILING_COLOR;
        } else if(y > (wallYStart + length)) {
            screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = FLOOR_COLOR;
        } else {
            screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = (darken) ? ABGRColor : DARKEN_COLOR(ABGRColor);
        }
    }
}

void drawTexturedStrip(int x, float wallYStart, float length, int textureX, Uint32* texture, char darken) {
    if(wallYStart < 0) wallYStart = 0;

    for(int y = 0; y < WINDOW_HEIGHT; y++) {
        float d = y - (WINDOW_HEIGHT / 2.0f) + length / 2.0f;
        float ty = d * (float)(TEXTURE_SIZE - EPS) / length;

        if(y < wallYStart) {
            screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = CEILING_COLOR;
        } else if(y > (wallYStart + length)) {
            screenBuffer[XY_TO_SCREEN_INDEX(x, y )] = FLOOR_COLOR;
        } else {
            Uint32 color = texture[XY_TO_TEXTURE_INDEX(textureX, (int)ty)];
            if(darken) color = DARKEN_COLOR(color);
            screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = color;
        }
    }
}

int getTextureColumnNumberForRay(Vector3f* ray, RayType rtype) {
    Vector3f rayHitPos = vectorAdd(&playerPos, ray);
    if(rtype == HORIZONTAL_RAY) {
        return (ray->y < 0) ? (int)rayHitPos.x % TEXTURE_SIZE : TEXTURE_SIZE - 1 - ((int)rayHitPos.x % TEXTURE_SIZE);
    } else {
        return (ray->x > 0) ? (int)rayHitPos.y % TEXTURE_SIZE : TEXTURE_SIZE - 1 - ((int)rayHitPos.y % TEXTURE_SIZE);
    }
}

float getUndistortedRayLength(Vector3f* ray) {
    Vector3f undistortedRay;
    Vector3f proj = vectorProjection(ray, &viewplaneDir);
    undistortedRay = vectorSubtract(ray, &proj);
    return homogeneousVectorMagnitude(&undistortedRay);
}

void renderProjectedScene() {
    if (slowRenderMode) {
        for(int x = 0; x < WINDOW_WIDTH; x++)
            for(int y = 0; y < WINDOW_HEIGHT; y++)
                screenBuffer[(WINDOW_WIDTH * y) + x] = 0xFFFFFFFF;
    }

    for(int i = 0; i < WINDOW_WIDTH; i++) {
        int textureX = 0;
        int mapx, mapy;
        float drawLength;
        RayType rtype;
        Vector3f ray;

        if(homogeneousVectorMagnitude(&rays[i].hRay) < homogeneousVectorMagnitude(&rays[i].vRay)) {
            ray = rays[i].hRay;
            rtype = HORIZONTAL_RAY;
            Vector3f coords = getTileCoordinateForHorizontalRay(&ray);
            mapx = coords.x;
            mapy = coords.y;
        } else {
            ray = rays[i].vRay;
            rtype = VERTICAL_RAY;
            Vector3f coords = getTileCoordinateForVerticalRay(&ray);
            mapx = coords.x;
            mapy = coords.y;
        }

        if(textureMode) textureX = getTextureColumnNumberForRay(&ray, rtype);
        drawLength = distortion ? calculateDrawHeight(homogeneousVectorMagnitude(&ray)) : calculateDrawHeight(getUndistortedRayLength(&ray));

        if(textureMode) {
            int texnum = MAP[mapy][mapx];
            if(texnum < 1 || texnum > 4) texnum = 4;
            drawTexturedStrip(i, (WINDOW_HEIGHT / 2.0f) - (drawLength / 2.0f), drawLength, textureX, TEXTURES[texnum - 1], rtype == HORIZONTAL_RAY);
        } else {
            int color = MAP[mapy][mapx];
            if(color < 1 || color > 4) color = 4;
            drawUntexturedStrip(i, (WINDOW_HEIGHT / 2.0f) - (drawLength / 2.0f), drawLength, COLORS[color - 1], rtype == HORIZONTAL_RAY);
        }

        if (slowRenderMode) {
            clearRenderer();
            displayFullscreenTexture(screenBuffer);
            SDL_Delay(2);
        }
    }

    if (slowRenderMode) slowRenderMode = 0;
    clearRenderer();
    displayFullscreenTexture(screenBuffer);
}