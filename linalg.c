#include "linalg.h"

Vector3f vectorAdd(Vector3f* vec1, Vector3f* vec2) {
    return (Vector3f){vec1->x + vec2->x, vec1->y + vec2->y, 1};
}

Vector3f vectorSubtract(Vector3f* vec1, Vector3f* vec2) {
    return (Vector3f){vec1->x - vec2->x, vec1->y - vec2->y, 1};
}

Vector3f homogeneousVectorScale(Vector3f* vec, float scalar) {
    return (Vector3f){vec->x * scalar, vec->y * scalar, 1};
}

Vector3f normalizeVector(Vector3f* vec) {
    return homogeneousVectorScale(vec, 1.0f / homogeneousVectorMagnitude(vec));
}

Vector3f vectorProjection(Vector3f* vec1, Vector3f* vec2) {
    Vector3f pvec = normalizeVector(vec2);
    return homogeneousVectorScale(&pvec, vectorDotProduct(&pvec, vec1));
}

float homogeneousVectorMagnitude(Vector3f* vec) {
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

float vectorDotProduct(Vector3f* vec1, Vector3f* vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y;
}

void matrixVectorMultiply(Matrix3f* mat, Vector3f* vec) {
    Vector3f newVec = {
        (*mat)[0][0] * vec->x + (*mat)[0][1] * vec->y + (*mat)[0][2] * vec->z,
        (*mat)[1][0] * vec->x + (*mat)[1][1] * vec->y + (*mat)[1][2] * vec->z,
        (*mat)[2][0] * vec->x + (*mat)[2][1] * vec->y + (*mat)[2][2] * vec->z
    };
    *vec = newVec;
}

void matrixMatrixMultiply(Matrix3f* mat1, Matrix3f* mat2) {
    Matrix3f retMat;
    for(int row = 0; row < 3; row++) {
        for(int col = 0; col < 3; col++) {
            retMat[row][col] = (*mat1)[row][0] * (*mat2)[0][col] +
                               (*mat1)[row][1] * (*mat2)[1][col] +
                               (*mat1)[row][2] * (*mat2)[2][col];
        }
    }
    matrix3fCopy(mat1, &retMat);
}

void matrix3fCopy(Matrix3f* dst, Matrix3f* src) {
    for(int row = 0; row < 3; row++)
        for(int col = 0; col < 3; col++)
            (*dst)[row][col] = (*src)[row][col];
}