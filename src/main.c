#include "raylib.h"
#include <stdint.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 720
#define SCALE 10

typedef float f32;
typedef uint16_t u16;

typedef struct {
  Vector2 coord;
  char *name;
  Color color;
} position_on_map;

#define TOTAL_PLACES 5
position_on_map places[TOTAL_PLACES] = {
    {.coord.x = 27, .coord.y = 36, .name = "Supermarket", .color = BLUE},
    {.coord.x = 42, .coord.y = 20, .name = "Pub", .color = GREEN},
    {.coord.x = 70, .coord.y = 39, .name = "Basketball Court", .color = ORANGE},
    {.coord.x = 52, .coord.y = 60, .name = "Diner", .color = BROWN},
};

f32 sqr(f32 n) { return n * n; }
f32 set_position(f32 n) { return n * SCALE; }
f32 get_position(f32 n) { return n / SCALE; }

position_on_map *search_by_proximity(Vector2 coordinate, f32 dmax,
                                     u16 *out_qty_found);

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib - Points of Interest");
  SetTargetFPS(60);

  Vector2 position = {.x = (f32)SCREEN_WIDTH / 2, .y = (f32)SCREEN_HEIGHT / 2};

  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_L))
      position.x += 2.0f;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_H))
      position.x -= 2.0f;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_K))
      position.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_J))
      position.y += 2.0f;

    Vector2 my_position = {.x = get_position(position.x),
                           .y = get_position(position.y)};

    u16 qty_found = 0;
    f32 distance_max = 10;

    position_on_map *nearby_places =
        search_by_proximity(my_position, distance_max, &qty_found);

    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int i = 0; i < qty_found; i++) {
      DrawText(TextFormat("Nearby places: %s", nearby_places[i].name), 10,
               10 + (i * 20), 20, DARKGRAY);
    }
    free(nearby_places);

    DrawCircleV(position, 15, RED);
    DrawText(TextFormat("You (%0.2f, %0.2f)", my_position.x, my_position.y),
             (u16)set_position(my_position.x) - 50,
             (u16)set_position(my_position.y) - 50, 20, BLACK);
    DrawCircleLines(position.x, position.y, 100, RED);

    for (int i = 0; i < TOTAL_PLACES; i++) {
      Vector2 formatPosition = {.x = set_position(places[i].coord.x),
                                .y = set_position(places[i].coord.y)};

      DrawCircleV(formatPosition, 15, places[i].color);
      DrawText(TextFormat("%s (%0.2f, %0.2f)", places[i].name,
                          places[i].coord.x, places[i].coord.y),
               (u16)set_position(places[i].coord.x) - 50,
               (u16)set_position(places[i].coord.y) - 50, 20, BLACK);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

position_on_map *search_by_proximity(Vector2 coordinate, f32 dmax,
                                     u16 *out_qty_found) {
  position_on_map *results =
      (position_on_map *)malloc(TOTAL_PLACES * sizeof(position_on_map));
  int count = 0;

  f32 dmax_sq = sqr(dmax);

  for (int i = 0; i < TOTAL_PLACES; i++) {
    f32 dist_q = sqr(places[i].coord.x - coordinate.x) +
                 sqr(places[i].coord.y - coordinate.y);

    if (dist_q <= dmax_sq) {
      results[count] = places[i];
      count++;
    }
  }

  *out_qty_found = count;
  return results;
}
