require("core/lib/system/coresystem")
require("core/lib/units/editor/coremissionelement")
require("core/lib/utils/dev/editor/coreeditor")

dofile("mods/ews/editor_patches/engine_function_overrides.lua")

--__classes["Application"].debug = function(print)
--  EWS_Log(print)
--end

--SystemFS = {}
--setmetatable(SystemFS, SystemFS)
--SystemFS.__index = function(self, ...)
--  EWS_Log("Attempt to call SystemFS:", ...)
--end


--PackageManager:set_resource_loaded_clbk(Idstring("world"), function(type, name, package_name)
--  EWS_Log("[PackageManager] Resource Loaded: " .. name:s() .. "." .. type:s() .. " in package: " .. package_name:s())
--end)
dofile("mods/ews/asset_additions.lua")
dofile("mods/ews/ews/ews.lua")