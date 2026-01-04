local post_hooks = {}
local pre_hooks = {}

local function post_hook(game_file, mod_file)
  if not post_hooks[game_file] then
    post_hooks[game_file] = {}
  end
  table.insert(post_hooks[game_file], mod_file)
end

local function pre_hook(game_file, mod_file)
  if pre_hooks[game_file] == nil then
    pre_hooks[game_file] = {}
  end
  table.insert(pre_hooks[game_file], mod_file)
end


local old_require = require

require = function(modname)
  local new_modname = string.lower(modname)

  if pre_hooks[new_modname] then
    for i, file in pairs(pre_hooks[new_modname]) do
      dofile("./mods/ews/" .. file)
    end
  end

  local ret = nil
    ret = old_require(modname)
  
  if post_hooks[new_modname] then
    for i, file in pairs(post_hooks[new_modname]) do
      dofile("./mods/ews/" .. file)
    end
  end
  
  return ret
end

pre_hook("lib/entry", "editor_patches/entry_pre.lua")
post_hook("core/lib/system/coremodule", "editor_patches/coremodule_post.lua")
post_hook("core/lib/setups/coresetup", "editor_patches/coresetup_post.lua")
post_hook("core/lib/utils/dev/editor/coreeditor", "editor_patches/coreeditor_post.lua")