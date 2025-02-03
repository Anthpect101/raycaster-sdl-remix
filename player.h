#ifndef PLAYER_H
#define PLAYER_H

#include "linalg.h"

extern Vector3f playerPos;
extern Vector3f playerDir;

extern char movingForward;
extern char movingBack;
extern char turningLeft;
extern char turningRight;
extern char playerIsRunning;

void initPlayer();
void updatePlayer();
void movePlayer(float dx, float dy);
int clipMovement(float dx, float dy);

#endif /* PLAYER_H */