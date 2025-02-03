#ifndef CONFIG_H
#define CONFIG_H

#include "gfx.h"
#include "linalg.h"

/* Special settings */
#define ONLY_NORMALIZED 1
#define ONLY_FIRST_HIT 2
extern char slowRenderMode;
extern char rayCastMode;

/* Misc. constants */
#define FALSE 0
#define TRUE  1
#define PI    3.14159265358979323846
#define EPS   0.00000001

/* Floating point operations */
#define MAKE_FLOAT_NONZERO(A)  ((fabs((A)) < EPS) ? EPS : A)

/* Window parameters */
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

/* Raycaster parameters */
#define TEXTURE_SIZE           64
#define WALL_SIZE              64
#define HUD_MAP_SIZE           WINDOW_HEIGHT
#define FOV                    (PI / 3.0f)
#define PLAYER_MOVEMENT_SPEED  5.0f
#define PLAYER_ROT_SPEED       ((3.0f * (PI)) / 180.0f)
#define PLAYER_SIZE            20

/* Projection parameters */
#define VIEWPLANE_LENGTH  WINDOW_WIDTH
#define VIEWPLANE_DIR_X  -1
#define VIEWPLANE_DIR_Y   0
#define PLAYER_DIR_X      0
#define PLAYER_DIR_Y      1
#define PLAYER_START_X    (2.5f * WALL_SIZE)
#define PLAYER_START_Y    (2.5f * WALL_SIZE)

/* Map constants */
#define MAP_GRID_WIDTH    10
#define MAP_GRID_HEIGHT   10
#define MAP_PIXEL_WIDTH   (MAP_GRID_WIDTH * WALL_SIZE)
#define MAP_PIXEL_HEIGHT  (MAP_GRID_HEIGHT * WALL_SIZE)

/* Map wall types */
#define P            -1
#define R             1
#define G             2
#define B             3
#define W             4

#define CEILING_COLOR  RGBtoABGR(0x65, 0x65, 0x65)
#define FLOOR_COLOR    RGBtoABGR(0xAA, 0xAA, 0xAA)

/* Globals */
extern const short MAP[MAP_GRID_HEIGHT][MAP_GRID_WIDTH];
extern char distortion;
extern char textureMode;
extern Uint32* screenBuffer;
extern const Uint32 COLORS[];
extern Uint32* TEXTURES[];

#endif /* CONFIG_H */