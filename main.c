#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"
#include "matrix.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
uint32_t *color_buffer;
bool is_running = false;
int window_width = 800;
int window_height = 900;

triangle_t triangles_to_render[N_MESH_FACES];

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)
int previous_frame_time = 0;


int scaling_factor = 1100;

vec3_t cube_scale = {.x = 1, .y = 1, .z = 1};
vec3_t cube_translate = {.x = 1, .y = 1, .z = 7};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};
cube_scale.x += .01;
cube_translate.y += .009;
cube_translate.x += .03;
mat4_t scale_matrix = mat4_make_scale(cube_scale.x, cube_scale.y, cube_scale.z);
mat4_t rotation_matrix_x = mat4_make_rotation_x(cube_rotation.x);
mat4_t rotation_matrix_y = mat4_make_rotation_y(cube_rotation.y);
mat4_t rotation_matrix_z = mat4_make_rotation_z(cube_rotation.z);
mat4_t translate_matrix = mat4_make_translate(cube_translate.x, cube_translate.y, cube_translate.z);

bool initialize_windowing_system(void);
void process_keyboard_input(void);
void project_model(void);
void draw_rectangle(int x, int y, int width, int height, uint32_t color);
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void clear_color_buffer(uint32_t color);
void update_state(void);
void setup_memory_buffers(void);
void cleanUpWindowSystem(void);
void update(void);
void run_render_pipeline(void);

bool initialize_windowing_system(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL_Init() Failed\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow() Failed\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer() Failed\n");
        return false;
    }

    return true;
}

void process_keyboard_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        break;
    }
}

void project_model(void)
{
    vec3_t camera_position = {.x = 0, .y = 0, .z = 5};
    for (int i = 0; i < N_MESH_VERTICES; i++)
    {
        vec3_t point = mesh_vertices[i];

        vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
        transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);
        transformed_point.z -= camera_position.z;

        vec2_t projected_point = perspective_project_point(transformed_point);
        projected_points[i] = projected_point;
    }
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color)
{
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            color_buffer[(window_width * i) + j] = color;
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;

    while (1)
    {
        draw_rectangle(x1, y1, 1, 1, color);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void update_state(void)
{
    mat4_t scale_matrix = mat4_make_scale(cube_scale.x, cube_scale.y, cube_scale.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(cube_rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(cube_rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(cube_rotation.z);
    mat4_t translate_matrix = mat4_make_translate(cube_translate.x, cube_translate.y, cube_translate.z);

    mat4_t world_matrix = mat4_identity();
    world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
    world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
    world_matrix = mat4_mul_mat4(translate_matrix, world_matrix);
}

void setup_memory_buffers(void)
{
    color_buffer = (uint32_t *)malloc(window_width * window_height * sizeof(uint32_t));
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
}

void cleanUpWindowSystem(void)
{
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void run_render_pipeline(void)
{
    SDL_UpdateTexture(texture, NULL, color_buffer, (int)(window_width * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{

    is_running = initialize_windowing_system();
    setup_memory_buffers();

    while (is_running)
    {
        process_keyboard_input();
        update_state();
        run_render_pipeline();
        update();
    }

    cleanUpWindowSystem();
    return 0;
}