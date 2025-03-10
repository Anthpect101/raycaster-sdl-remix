#include <stdio.h>
#include "config.h"
#include "raycaster.h"
#include "player.h"

Vector3f viewplaneDir = {VIEWPLANE_DIR_X, VIEWPLANE_DIR_Y, 1};
float distFromViewplane;
Matrix3f counterClockwiseRotation = IDENTITY_M;
Matrix3f clockwiseRotation = IDENTITY_M;
RayTuple rays[VIEWPLANE_LENGTH];

void initializeRayDirections() {
    for(int i = 0; i < VIEWPLANE_LENGTH; i++) {
        Vector3f v1 = homogeneousVectorScale(&playerDir, distFromViewplane);
        Vector3f v2 = homogeneousVectorScale(&viewplaneDir, ((VIEWPLANE_LENGTH / 2) - i));
        Vector3f v3 = vectorSubtract(&v1, &v2);
        rays[i].hRay = normalizeVector(&v3);
        rays[i].vRay = normalizeVector(&v3);

        if (rayCastMode == ONLY_NORMALIZED) {
            rays[i].hRay = homogeneousVectorScale(&rays[i].hRay, 40);
            rays[i].vRay = homogeneousVectorScale(&rays[i].vRay, 40);
        }
    }
}

void extendRaysToFirstHit(RayTuple* rays) {
    for(int i = 0; i < VIEWPLANE_LENGTH; i++) {
        Vector3f perpVec = HOMOGENEOUS_V3;

        if(rays[i].vRay.x < 0) {
            perpVec.x = ((int)(playerPos.x / (float)WALL_SIZE)) * WALL_SIZE - playerPos.x;
        } else {
            perpVec.x = ((int)(playerPos.x / (float)WALL_SIZE)) * WALL_SIZE - playerPos.x + WALL_SIZE;
        }
        rays[i].vRay = homogeneousVectorScale(&rays[i].vRay, vectorDotProduct(&perpVec, &perpVec) / MAKE_FLOAT_NONZERO(vectorDotProduct(&perpVec, &rays[i].vRay)));

        perpVec.x = 0.0f;
        if(rays[i].hRay.y < 0) {
            perpVec.y = ((int)(playerPos.y / (float)WALL_SIZE)) * WALL_SIZE - playerPos.y;
        } else {
            perpVec.y = ((int)(playerPos.y / (float)WALL_SIZE)) * WALL_SIZE - playerPos.y + WALL_SIZE;
        }
        rays[i].hRay = homogeneousVectorScale(&rays[i].hRay, vectorDotProduct(&perpVec, &perpVec) / MAKE_FLOAT_NONZERO(vectorDotProduct(&perpVec, &rays[i].hRay)));
    }
}

Vector3f findVerticalRayStepVector(Vector3f* ray) {
    Vector3f stepVector = HOMOGENEOUS_V3;
    stepVector.x = (ray->x < 0) ? -1 * WALL_SIZE : WALL_SIZE;
    return homogeneousVectorScale(ray, vectorDotProduct(&stepVector, &stepVector) / MAKE_FLOAT_NONZERO(vectorDotProduct(&stepVector, ray)));
}

Vector3f findHorizontalRayStepVector(Vector3f* ray) {
    Vector3f stepVector = HOMOGENEOUS_V3;
    stepVector.y = (ray->y < 0) ? -1 * WALL_SIZE : WALL_SIZE;
    return homogeneousVectorScale(ray, vectorDotProduct(&stepVector, &stepVector) / MAKE_FLOAT_NONZERO(vectorDotProduct(&stepVector, ray)));
}

void raycast(RayTuple* rays) {
    for(int i = 0; i < VIEWPLANE_LENGTH; i++) {
        Vector3f vnorm = normalizeVector(&rays[i].vRay);
        Vector3f hnorm = normalizeVector(&rays[i].hRay);
        Vector3f vstep = findVerticalRayStepVector(&vnorm);
        Vector3f hstep = findHorizontalRayStepVector(&hnorm);
        Vector3f mapCoord;

        mapCoord = getTileCoordinateForVerticalRay(&rays[i].vRay);
        while(mapCoord.x >= 0 && mapCoord.y >= 0 && mapCoord.x < MAP_GRID_WIDTH && mapCoord.y < MAP_GRID_HEIGHT && MAP[(int)mapCoord.y][(int)mapCoord.x] < 1) {
            rays[i].vRay = vectorAdd(&rays[i].vRay, &vstep);
            mapCoord = getTileCoordinateForVerticalRay(&rays[i].vRay);
        }

        mapCoord = getTileCoordinateForHorizontalRay(&rays[i].hRay);
        while(mapCoord.x >= 0 && mapCoord.y >= 0 && mapCoord.x < MAP_GRID_WIDTH && mapCoord.y < MAP_GRID_HEIGHT && MAP[(int)mapCoord.y][(int)mapCoord.x] < 1) {
            rays[i].hRay = vectorAdd(&rays[i].hRay, &hstep);
            mapCoord = getTileCoordinateForHorizontalRay(&rays[i].hRay);
        }
    }
}

void updateRaycaster() {
    initializeRayDirections();
    if (rayCastMode == ONLY_NORMALIZED) return;
    extendRaysToFirstHit (rays);
    if (rayCastMode == ONLY_FIRST_HIT) return;
    raycast(rays);
}

Vector3f getTileCoordinateForVerticalRay(Vector3f* ray) {
    Vector3f pos = vectorAdd(&playerPos, ray);
    return (Vector3f){
        (int)(pos.x + ((ray->x < 0) ? (-1 * RAY_EPS) : (RAY_EPS))) / WALL_SIZE,
        (int)(pos.y + ((ray->y < 0) ? (-1 * EPS) : (EPS))) / WALL_SIZE
    };
}

Vector3f getTileCoordinateForHorizontalRay(Vector3f* ray) {
    Vector3f pos = vectorAdd(&playerPos, ray);
    return (Vector3f){
        (int)(pos.x + ((ray->x < 0) ? (-1 * EPS) : EPS)) / WALL_SIZE,
        (int)(pos.y + ((ray->y < 0) ? (-1 * RAY_EPS) : (RAY_EPS))) / WALL_SIZE
    };
}

void initRaycaster() {
    distFromViewplane = (WINDOW_WIDTH / 2.0f) / (float)(tan(FOV / 2.0f));

    counterClockwiseRotation[0][0] = cos(PLAYER_ROT_SPEED);
    counterClockwiseRotation[0][1] = -1.0f * sin(PLAYER_ROT_SPEED);
    counterClockwiseRotation[1][0] = sin(PLAYER_ROT_SPEED);
    counterClockwiseRotation[1][1] = cos(PLAYER_ROT_SPEED);

    clockwiseRotation[0][0] = cos(-1.0f * PLAYER_ROT_SPEED);
    clockwiseRotation[0][1] = -1.0f * sin(-1.0f * PLAYER_ROT_SPEED);
    clockwiseRotation[1][0] = sin(-1.0f * PLAYER_ROT_SPEED);
    clockwiseRotation[1][1] = cos(-1.0f * PLAYER_ROT_SPEED);
}