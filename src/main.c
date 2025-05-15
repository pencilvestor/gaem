#include "kit.h"

int main(void) {
  kit_Context *ctx = kit_create("hi", 320, 200, KIT_SCALE2X);
  while (kit_step(ctx, NULL)) {
    kit_draw_text(ctx, KIT_WHITE, "Hello world!", 10, 10);
  }
  kit_destroy(ctx);
  return 0;
}
