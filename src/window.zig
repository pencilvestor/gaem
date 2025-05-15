const c = @cImport({
    @cInclude("kit.h");
});

pub const Context = c.kit_Context;
pub const create = c.kit_create;
pub const destroy = c.kit_destroy;
pub const step = c.kit_step;
