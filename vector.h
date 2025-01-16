#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x, y;
} vec2_t;

typedef struct {
    float x, y, z;
} vec3_t;

// 2D vector operations
float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_subtract(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t v, float scalar);
vec2_t vec2_div(vec2_t v, float scalar);
void normalize_2d(vec2_t* v);

// 3D vector operations
float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_subtract(vec3_t a, vec3_t b);
vec3_t vec3_mul(vec3_t v, float scalar);
vec3_t vec3_div(vec3_t v, float scalar);
vec3_t vec3_cross(vec3_t a, vec3_t b);
float vec3_dot(vec3_t a, vec3_t b);
void normalize_3d(vec3_t* v);

#endif
