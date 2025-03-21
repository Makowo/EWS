core:import("CoreDatabaseManager")
core:import("CoreWorldDefinition")
core:import("CoreFreeFlight")
require("core/lib/utils/dev/ews/CoreEWSDeprecated")
require("lib/units/props/Ladder")
require("lib/units/props/ZipLine")
require("lib/managers/GroupAIManager")
require("core/lib/utils/dev/editor/worldholder")

dofile("mods/ews/ews/ews.lua")
log("CoreSetup_post requiring EWS")

EWS_Log("CoreSetup_post hook")

local function make_lua_class_hook(classname, functionname, new_function)
  local old_func = _G[classname][functionname]
  _G[classname][functionname] = function(...)
    return new_function(old_func, ...)
  end
end

local function log_function_call_in_class(classname, functionname)
  local hook_func = function(old_func, ...)
    EWS_Log("[" .. classname .. ":" .. functionname .. "] Begin " .. functionname)
    local ret = old_func(...)
    EWS_Log("[" .. classname .. ":" .. functionname .. "] End " .. functionname)

    return ret
  end
  make_lua_class_hook(classname, functionname, hook_func)
end

make_lua_class_hook("CoreEditor", "_project_init_layer_classes", function(old_func, self)
  self:add_layer("Ai", CoreAiLayer.AiLayer) -- only WorldEditor adds this, but CoreEditor always assumes it exists
end)

make_lua_class_hook("CoreEditor", "_save_unit_stats", function(old_func, self, dir)
end)

make_lua_class_hook("WorldHolder", "create_world", function(old_func, self, world, layer, offset)
  EWS_Log("[WorldHolder:create_world] self._definition: " .. tostring(self._definition))
  local return_val = old_func(self, world, layer, offset)
  EWS_Log("[WorldHolder:create_world] create_world: " .. world .. ", " .. layer .. ", " .. offset)

  if type(return_val) == "nil" then
    EWS_Log("[WorldHolder:create_world] return value for layer " .. layer .. " was nil. returning empty table")
    return_val = {}
    if layer == "portal" then
      return_val.portals = {}
    end
  end
  return return_val
end)

make_lua_class_hook("WorldHolder", "_worldfile_generation", function(old_func, self, file_type, file_path)
  local ret = old_func(self, file_type, file_path)
  EWS_Log("[WorldHolder:_worldfile_generation] _worldfile_generation: " .. file_type .. ", " .. file_path .. " (ret: " .. ret .. ")")
  return ret
end)

_G.cat_print = function(category, to_print)
  EWS_Log("[" .. category .. "] " .. to_print)
end

local CoreSetup___init = CoreSetup___init or CoreSetup.__init
local CoreSetup_init = CoreSetup_init or CoreSetup.init
function CoreSetup:init()
  --World:load("core/levels/editor/editor", false)
  --EWS_Log("World test " .. tostring(World:get_object(Idstring("ref"))))

  --test:new()
  EWS_Log("EWS mod: running coresetup: __init")
  --self.__init()
  CoreSetup_init(self)
end

Global = Global or {}
local function create_ews_frame()
  if Global.frame ~= nil then
    return
  end
  local frame_resolution = SystemInfo:desktop_resolution()
  local appwin_resolution = Vector3(frame_resolution.x * 0.75, frame_resolution.y * 0.75, 0)
  local frame = EWS:Frame("World Editor", Vector3(0, 0, 0), frame_resolution,
    "CAPTION,CLOSE_BOX,MINIMIZE_BOX,MAXIMIZE_BOX,MAXIMIZE,SYSTEM_MENU,RESIZE_BORDER")

  frame:set_icon(CoreEWS.image_path("world_editor_16x16.png"))

  local frame_panel = EWS:Panel(frame, "", "")
  local appwin = EWS:AppWindow(frame_panel, appwin_resolution, "SUNKEN_BORDER")

  appwin:set_max_size(Vector3(-1, -1, 0))

  Global.application_window = appwin
  Global.frame = frame

  appwin:connect("EVT_LEAVE_WINDOW", callback(nil, _G, "leaving_window"))
  appwin:connect("EVT_ENTER_WINDOW", callback(nil, _G, "entering_window"))
  appwin:connect("EVT_KILL_FOCUS", callback(nil, _G, "kill_focus"))
  appwin:connect("EVT_ACTIVATE", callback(nil, _G, "on_activate"))
  Application:set_ews_window(appwin)

  local top_sizer = EWS:BoxSizer("VERTICAL")
  local main_sizer = EWS:BoxSizer("HORIZONTAL")
  local left_toolbar_sizer = EWS:BoxSizer("VERTICAL")

  main_sizer:add(left_toolbar_sizer, 0, 0, "EXPAND")

  local app_sizer = EWS:BoxSizer("VERTICAL")

  main_sizer:add(app_sizer, 4, 0, "EXPAND")
  app_sizer:add(appwin, 5, 0, "EXPAND")
  top_sizer:add(main_sizer, 1, 0, "EXPAND")
  frame_panel:set_sizer(top_sizer)

  Global.main_sizer = main_sizer
  Global.v_sizer = app_sizer
  Global.frame_panel = frame_panel
  Global.left_toolbar_sizer = left_toolbar_sizer
end
local function convert_binary_xml_to_generic_xml(input_path, output_path)
  local in_file = SystemFS:open(input_path, "r")

  local loaded = ScriptSerializer:from_binary(in_file:read())
  in_file:close()

  local out_file = SystemFS:open(output_path, "w")

  out_file:puts(ScriptSerializer:to_generic_xml(loaded))
  out_file:close()
end
local function convert_generic_xml_to_binary_xml(input_path, output_path)
  local in_file = SystemFS:open(input_path, "r")

  local loaded = ScriptSerializer:from_generic_xml(in_file:read())
  in_file:close()

  local out_file = SystemFS:open(output_path, "w")

  out_file:puts(ScriptSerializer:to_binary(loaded))
  out_file:close()
end

log_function_call_in_class("CoreEditor", "draw_grid")

-- class functions from core/lib/utils/dev/tools/coreprefhud from PAYDAY 2 PS3
PrefHud = PrefHud or class()
function PrefHud:init() end
function PrefHud:remove_counter(name) end
function PrefHud:remove_all_counters() end
function PrefHud:add_counter(name, sort, min, mid, max, precision, inv, inv_colors, call_str) end
function PrefHud:load_config() end
function PrefHud:build_gui() end
function PrefHud:show() end
function PrefHud:hide() end
function PrefHud:toggle() end
function PrefHud:update_bars(t, dt) end
function PrefHud:update_keys() end
function PrefHud:update(t, dt) end
function PrefHud:paused_update(t, dt) self:update(t, dt) end
function PrefHud:destroy() end


function PrefHud:toggle()
  if managers.editor then
    managers.editor:output("Performance Hud is unavailable in non-production versions of Diesel (missing profiling functions)")
  end
end

function CoreSetup:__init()
	--PackageManager:set_streaming_enabled(true)
  --convert_binary_xml_to_generic_xml("./assets/core/world.world", "./assets/core/world_.world")
  --convert_binary_xml_to_generic_xml("./mods/ews/assets/levels/editor/editor.scene", "./mods/ews/assets/levels/editor/editor.scene.xml")

  --convert_binary_xml_to_generic_xml("./assets/units/upd_candy/weapons/wpn_gre_gold_bar/gold/wpn_gre_gold_bar.sequence_manager", "./assets/units/upd_candy/weapons/wpn_gre_gold_bar/gold/wpn_gre_gold_bar.xml")

  --convert_generic_xml_to_binary_xml("./assets/test_level/world.world", "./assets/test_level/world.bworld")

  EWS_Log("CoreSetup:__init")
  --self:__pre_init()


  CoreSetup___init(self)

  --managers.freeflight = CoreFreeFlight.FreeFlight:new(self.__gsm, managers.viewport, managers.controller)

  managers.database = CoreDatabaseManager.DatabaseManager:new()

  create_ews_frame()

  --managers.enemy = EnemyManager:new()
  --managers.groupai = managers.groupai or GroupAIManager:new()

  managers.prefhud = PrefHud:new() -- fix errors caused by the performance toolbar item binding an event to PrefHud:toggle

  managers.editor = CoreEditor:new(self.__gsm)
  --managers.editor = WorldEditor:new(self.__gsm)
  --Global.test_editor = managers.editor
  --managers.editor = (rawget(_G, "WorldEditor") or rawget(_G, "CoreEditor")):new(self.__gsm)

  managers.editor:toggle()

  PackageManager:load("world_init")
  PackageManager:load("packages/zone_germany")
  PackageManager:load("levels/vanilla/germany_camp/world")

  EWS_Log("PackageManager:loaded (world_init): " .. tostring(PackageManager:loaded(Idstring("world_init"))))
  EWS_Log("PackageManager:loaded (packages/zone_germany): " .. tostring(PackageManager:loaded(Idstring("packages/zone_germany"))))
  EWS_Log("PackageManager:loaded (levels/vanilla/germany_camp/world): " .. tostring(PackageManager:loaded(Idstring("levels/vanilla/germany_camp/world"))))

  EWS_Log("DB:has (levels/vanilla/germany_camp/world.world): " .. tostring(DB:has("world", "levels/vanilla/germany_camp/world")))

  --[[
	managers.editor._world_holder = WorldHolder:new({
		file_path = nil,
		file_type = "world",
		file_path = "levels/vanilla/germany_camp/world",
	})]]

	--local status = managers.editor._world_holder:status()

  --EWS_Log("[EWS level load test] status: " .. status)
  --]]

  --managers.editor:do_save("X:/SteamLibrary/steamapps/common/RAID World War II/assets/test_level/world.world", "X:/SteamLibrary/steamapps/common/RAID World War II/assets/test_level/")
  --managers.editor:load_level("levels/vanilla/germany_camp/", "levels/vanilla/germany_camp/world.world")
  managers.editor:load_level("levels/vanilla/forest_gumpy/", "levels/vanilla/forest_gumpy/world.world")

  --managers.editor:on_unit_tree_browser()
  --managers.editor:load_level("assets/levels/upg_005/fury_railway", "assets/levels/upg_005/fury_railway/world.world")

  --Application:set_mode(1280, 720, false, 60, false, false)

  --self.test_world_editor = CoreEditor:new(self.__gsm)

  --self.test_world_editor:open()

end

local CoreSetup__make_entrypoint = CoreSetup__make_entrypoint or CoreSetup.make_entrypoint

function CoreSetup:make_entrypoint()
  log("make_entrypoint")
  CoreSetup__make_entrypoint(self)
end

local CoreSetup____update = CoreSetup____update or CoreSetup.__update
function CoreSetup:__update(t, dt)
  if managers.editor then
    if Input:keyboard():down(Idstring("t")) then
      EWS_Log("t pressed, toggling mixed input mode")
      managers.editor:toggle_mixed_input_mode()
    end

    managers.editor:update(t, dt)
  end

  CoreSetup____update(self, t, dt)
end