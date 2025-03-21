
--[[
0 = none
1 = error
2 = warn
3 = info
4 = debug
]]
local APPLICATION_LOG_LEVEL = 0

--Global.load_start_menu = false
__classes["Application"].editor = function()
  return true
end
-- returns true if EWS is present
__classes["Application"].ews_enabled = function()
  return true
end
__classes["Application"].production_build = function()
  return false
end
---@param appwindow EWS_AppWindow
__classes["Application"].set_ews_window = function(self, appwindow)
  EWS_SetEWSWindow(appwindow.internal_c_obj)
  local size = appwindow:get_size()
  Application:set_mode(size.x, size.y, false, 60, false, false)
end
__classes["Application"].throw_exception = function(self, throw_exception)
  EWS_Log("[Application:throw_exception] " .. throw_exception)
end


local PackageManager_load = __classes["PackageManager"].load
__classes["PackageManager"].load = function(self, package)
  
  -- loading core/packages/editor crashes the game
  --if package ~= "core/packages/editor" and package ~= "packages/game_base" then
  if package ~= "packages/game_base" then
    EWS_Log("Loading package " .. package)
    return PackageManager_load(self, package)
  end
end

__classes["PackageManager"].editor_load = function(self, package)
  return PackageManager:load(package)
end

__classes["PackageManager"].editor_load_script_data = function(self, type, name)
  EWS_Log("[PackageManager:editor_load_script_data] editor_load_script_data: " .. type:s() .. " " .. name:s())

  type = type:id() -- enforce idstrings for arguments
  name = name:id()

  if type:s() == "environment" then
    return PackageManager:script_data(type, Idstring("core/environments/default"):id())
  end

  --[[local possible_unpacked_path = "./assets/" .. name:s() .. "." .. type:s()
  if SystemFS:exists(possible_unpacked_path) then
    EWS_Log("[PackageManager:editor_load_script_data] raw xml version exists in assets folder, loading that instead. (" .. possible_unpacked_path .. ")")
    local f = SystemFS:open(possible_unpacked_path)
    local deserialised = ScriptSerializer:from_generic_xml(f:read())
    f:close()
    EWS_Log(dump(deserialised))
    return deserialised
  end]]--

  --[[if DB:has(type, name) then
    EWS_Log("[PackageManager:editor_load_script_data] DB:has file, loading as binary xml")
    --local file = DB:open(type, name)

    --local script_data = ScriptSerializer:from_binary(file:read())
    --file:close()

    EWS_Log("[PackageManager:editor_load_script_data] typeof type: " .. typeof(type) .. " typeof name: " .. typeof(name))

    local script_data = PackageManager_script_data(self, type, name)
    --EWS_Log("ScriptSerializer:from_binary (DB:open():read() " .. name:s() .. "." .. type:s() .. "): " .. typeof(script_data))
    EWS_Log(dump(script_data))
    return script_data
  end]]--

  local script_data = PackageManager:script_data(type, name)--PackageManager_script_data(self, type, name)

  --EWS_Log(dump(script_data))
  return script_data
end

__classes["PackageManager"].editor_unit_data = function(self, name)
  return PackageManager:unit_data(name)
end

__classes["PackageManager"].editor_reload = function(self, type, name) end
__classes["PackageManager"].editor_reload_node = function(self, node, type, name) end

--- requires SoundbanksInfo.xml to work properly :(
__classes["SoundDevice"].sound_banks = function(self)
  return {} -- returns a table of all existing sound banks
end
__classes["SoundDevice"].events = function(self, soundbank_name)
  return {} -- returns a table of all events belonging to a soundbank name
end
__classes["SoundDevice"].effects = function(self)
  return {} -- returns a table of all environmental effects
end

local World_load = __classes["World"].load
__classes["World"].load = function(self, level, bool_val)
  if level == "core/levels/editor/editor" then
    level = "core/levels/zone"
  end
  --level = "core/levels/editor/editor"
  log("Loading level: " .. tostring(level))
  World_load(self, level, bool_val)
end

local World_spawn_unit = __classes["World"].spawn_unit
__classes["World"].spawn_unit = function(self, path, position)
  EWS_Log("[World:spawn_unit] spawning unit: " .. path:s())
  --EWS_Log(Application:stack())
  return World_spawn_unit(self, path, position)
end

local World_find_units_quick = __classes["World"].find_units_quick
__classes["World"].find_units_quick = function(self, unk1)
  local units = World_find_units_quick(self, unk1)
  EWS_Log("[World:find_units_quick] find_units_quick: " .. unk1)

  for _, u in ipairs(units) do
    local u_meta = getmetatable(u)
    --u_meta.author = function() return Idstring("fake author") end
    --u_meta.used_texture_names = function() return {} end
    --u_meta.get_objects_by_type = function(type) return {} end
    --u_meta.nr_models = function() return 0 end
    --u_meta.geometry_memory_use = function() return 0 end
  end
  return units
end

MassUnitManager = MassUnitManager or {}
__classes["MassUnitManager"] = __classes["MassUnitManager"] or {}

--setmetatable(MassUnitManager, __classes["MassUnitManager"])

-- excluded in non-production builds, implementation exists in Lead and Gold though (thanks Fatshark)
__classes["MassUnitManager"].save = function(self, path)
end

-- excluded in non-production builds, implementation exists for the datacompiler module in Lead and Gold, but no implementation exists for Application:data_compile
__classes["Application"].data_compile = function(self, t)
end

__classes["MainDB"].reload = function(self)
end

local Input_create_virtual_controller = __classes["Input"].create_virtual_controller
__classes["Input"].create_virtual_controller = function(self, name)
  EWS_Log("[Input:create_virtual_controller] creating virtual controller: " .. tostring(name))
  local controller = Input_create_virtual_controller(self, name)
  return controller
end

local EWS_Log_real = EWS_Log
EWS_Log = function(...)
  local msg = ""
  for _, data in ipairs({ ... }) do
    msg = msg .. tostring(data)
  end
  EWS_Log_real(msg)
end

__classes["Application"].debug = function(self, ...)
  local msg = ""
  for _, data in ipairs({ ... }) do
    msg = msg .. tostring(data)
  end
  --EWS_Log("Application:debug " .. msg)
end
__classes["Application"].error = function(self, ...)
  local msg = ""
  for _, data in ipairs({ ... }) do
    msg = msg .. tostring(data) .. " "
  end
  if APPLICATION_LOG_LEVEL > 0 then
    EWS_Log("[Application:error] " .. msg)
  end
end
__classes["Application"].warn = function(self, ...)
  local msg = ""
  for _, data in ipairs({ ... }) do
    msg = msg .. tostring(data)
  end
  if APPLICATION_LOG_LEVEL > 1 then
    EWS_Log("Application:warn " .. msg)
  end
end
__classes["Application"].info = function(self, ...)
  local msg = ""
  for _, data in ipairs({ ... }) do
    msg = msg .. tostring(data)
  end
  if APPLICATION_LOG_LEVEL > 2 then
    EWS_Log("Application:info " .. msg)
  end
end
