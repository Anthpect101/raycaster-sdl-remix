#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "config.h"
#include "linalg.h"

#define RAY_EPS   (WALL_SIZE / 3.0f)

typedef struct {
    Vector3f vRay;
    Vector3f hRay;
} RayTuple;

extern Vector3f viewplaneDir;
extern float distFromViewplane;
extern Matrix3f counterClockwiseRotation;
extern Matrix3f clockwiseRotation;
extern RayTuple rays[VIEWPLANE_LENGTH];

void initializeRayDirections();
void extendRaysToFirstHit(RayTuple* rays);
Vector3f findVerticalRayStepVector(Vector3f* ray);
Vector3f findHorizontalRayStepVector(Vector3f* ray);
void raycast(RayTuple* rays);
Vector3f getTileCoordinateForVerticalRay(Vector3f* ray);
Vector3f getTileCoordinateForHorizontalRay(Vector3f* ray);
void updateRaycaster();
void initRaycaster();

#endif /* RAYCASTER_H */