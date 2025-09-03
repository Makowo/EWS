


local function create_ews_frame()
	managers.global_texture = CoreGTextureManager.GTextureManager:new()
	local frame_resolution = SystemInfo:desktop_resolution()
	local appwin_resolution = Vector3(frame_resolution.x * 0.75, frame_resolution.y * 0.75, 0)
	local frame = EWS:Frame("World Editor", Vector3(0, 0, 0), frame_resolution, "CAPTION,CLOSE_BOX,MINIMIZE_BOX,MAXIMIZE_BOX,MAXIMIZE,SYSTEM_MENU,RESIZE_BORDER")
	frame:set_icon(CoreEWS.image_path("world_editor_16x16.png"))
	local frame_panel = EWS:Panel(frame, "", "")
	local appwin = EWS:AppWindow(frame_panel, appwin_resolution, "SUNKEN_BORDER")
	appwin:set_max_size(Vector3(-1, -1, 0))
	appwin:connect("EVT_LEAVE_WINDOW", callback(nil, _G, "leaving_window"))
	appwin:connect("EVT_ENTER_WINDOW", callback(nil, _G, "entering_window"))
	appwin:connect("EVT_KILL_FOCUS", callback(nil, _G, "kill_focus"))
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
	Global.frame = frame
	Global.frame_panel = frame_panel
	Global.application_window = appwin
	Global.left_toolbar_sizer = left_toolbar_sizer
end

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

function CoreEditor:_project_init_layer_classes()
  self:add_layer("Ai", CoreAiLayer.AiLayer)
end

local CoreSetup___init = CoreSetup.__init
function CoreSetup:__init()
  self:__pre_init()
  managers.prefhud = PrefHud:new()
  CoreSetup___init(self)

  --managers.editor = CoreEditor:new(self.__gsm)

  --Global.frame:set_visible(true)
  managers.editor:pickup_tool()
  --EWS_Log("[CoreSetup:__init] managers.editor " .. tostring(managers.editor))

  --PackageManager:load("packages/level_suburbia")
end