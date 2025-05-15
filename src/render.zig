const c = @cImport({
    @cInclude("kit.h");
});

pub const draw_text = c.kit_draw_text;
