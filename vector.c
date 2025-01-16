#include "vector.h"
#include <math.h>

// 2D vector functions
float vec2_length(vec2_t v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
    return (vec2_t){a.x + b.x, a.y + b.y};
}

vec2_t vec2_subtract(vec2_t a, vec2_t b) {
    return (vec2_t){a.x - b.x, a.y - b.y};
}

vec2_t vec2_mul(vec2_t v, float scalar) {
    return (vec2_t){v.x * scalar, v.y * scalar};
}

vec2_t vec2_div(vec2_t v, float scalar) {
    return (vec2_t){v.x / scalar, v.y / scalar};
}

void normalize_2d(vec2_t* v) {
    float length = vec2_length(*v);
    v->x /= length;
    v->y /= length;
}

// 3D vector functions
float vec3_length(vec3_t v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
    return (vec3_t){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3_t vec3_subtract(vec3_t a, vec3_t b) {
    return (vec3_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3_t vec3_mul(vec3_t v, float scalar) {
    return (vec3_t){v.x * scalar, v.y * scalar, v.z * scalar};
}

vec3_t vec3_div(vec3_t v, float scalar) {
    return (vec3_t){v.x / scalar, v.y / scalar, v.z / scalar};
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    return (vec3_t){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void normalize_3d(vec3_t* v) {
    float length = vec3_length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}
