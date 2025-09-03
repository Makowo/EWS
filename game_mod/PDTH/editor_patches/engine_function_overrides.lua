EWS_Log("engine_function_overrides")

__classes["Application"].editor = function()
  return true
end
__classes["Application"].ews_enabled = function()
  return true
end

__classes["MassUnitManager"] = __classes["MassUnitManager"] or {}

__classes["MassUnitManager"].save = function(self, path)end
__classes["MassUnitManager"].data_compile = function(self, t)end
__classes["MainDB"].reload = function(self)
  -- call DB::load again in C++
end

__classes["Application"].set_ews_window = function(self, appwindow)
end

--__classes["PackageManager"].loaded = function(self, package)return true end

local function should_stop_package_load(name)
  if name == "packages/level_debug" then
    return true
  end
  return false
end

local PackageManager_load = __classes["PackageManager"].load
__classes["PackageManager"].load = function(self, package)
  EWS_Log("[PackageManager:load] loading package: " .. package)
  if not should_stop_package_load(package) then
    PackageManager_load(self, package)
  end
end

__classes["PackageManager"].editor_package = function(self, ...) EWS_Log("[PackageManager:editor_package] called") end
--[[__classes["PackageManager"].editor_load = function(self, package, or_path)
  EWS_Log("[PackageManager:editor_load] loading package: " .. package)
  if not should_stop_package_load(package) then
    PackageManager_load(self, package)
  end
end]]--
__classes["PackageManager"].editor_load = function(self, type, name)
  EWS_Log("[PackageManager:editor_load] " .. type:s() .. " " .. name:s())

  --return PackageManager:resource(type, name)
  return {}
end
__classes["PackageManager"].editor_reload = function(self, ...) EWS_Log("[PackageManager:editor_reload] called") end
__classes["PackageManager"].editor_reload_node = function(self, ...) EWS_Log("[PackageManager:editor_reload_node] called") end
__classes["PackageManager"].editor_unit_data = function(self, ...) EWS_Log("[PackageManager:editor_unit_data] called") end

__classes["PackageManager"].editor_load_script_data = function(self, type_, name)
  EWS_Log("[PackageManager:editor_load_script_data] " .. name:s() .. "." .. type_:s())

  return PackageManager:script_data(type_, name)
end

__classes["SoundDevice"].sound_banks = function(self) return {} end
__classes["SoundDevice"].events = function(self, bankname) return {} end
__classes["SoundDevice"].switch_groups = function(self, bankname) return {} end
__classes["SoundDevice"].switches = function(self, switchgroup, bankname) return {} end
__classes["SoundDevice"].rtpcs = function(self, bankname) return {} end
__classes["SoundDevice"].triggers = function(self, bankname) return {} end
__classes["SoundDevice"].effects = function(self) return {} end

__classes["SystemFS"] = __classes["SystemFS"] or {}
SystemFS = __classes["SystemFS"]

function SystemFS:exists(filename)
  return false
end
function SystemFS:make_dir(directory)
  EWS_Log("[SystemFS:make_dir] " .. directory)
end

function SystemFS:open(path, flags)
  EWS_Log("[SystemFS:open] " .. path .. ", " .. flags)
  local ret = {}
  ret.read = function(flags) return "" end
  ret.puts = function(value) end
  ret.close = function() end
  return ret
end

function SystemFS:close(handle)
end

local DB_has = __classes["MainDB"].has
__classes["MainDB"].has = function(self, type, name)
  if type == "index" then
    return false
  end
  if type == "controller" then
    if name == "core/lib/utils/dev/editor/xml/default_controller" or name == "lib/utils/dev/editor/xml/project_controller" or name == "lib/utils/dev/editor/xml/custom_controller" then
      return false
    end
  end
  if type == "xml" and name == "lib/utils/dev/editor/xml/editor" then -- _editor_settings_path
    return false
  end
  if type == "editor_layout" and name == "lib/utils/dev/editor/xml/editor_layout" then
    return false
  end
	if name == "lib/utils/dev/editor/xml/version" then
    return false
  end
	if name == "lib/utils/dev/editor/xml/editor_configuration" then
    return false
  end
	if name == "lib/utils/dev/editor/xml/editor_edit_setting_values" then
    return false
  end

  return DB_has(self, type, name)
end