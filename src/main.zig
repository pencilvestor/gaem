const engine = @import("engine");
const render = engine.render;
const window = engine.window;

const WHITE: u32 = 0xFFFFFFFF;

pub fn main() void {
    const ctx = window.create("howdy", 320, 200, 1);
    defer window.destroy(ctx);

    while (window.step(ctx, null)) {
        _ = render.draw_text(ctx, @bitCast(WHITE), "Howdy, Partner!", 10, 10);
    }
}
