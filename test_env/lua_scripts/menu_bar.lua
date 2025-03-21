---@param mainframe EWS_Frame
function init_menubar(mainframe)

local menu_bar = EWS:MenuBar()
_menu_bar = menu_bar

------ File Menu
local file_menu = EWS:Menu("")
file_menu:append_item("NEW", "New", "Create new file")
file_menu:append_item("OPEN", "Open...", "Open world file")
file_menu:append_separator()
file_menu:append_item("SAVE", "Save", "Save world file")
file_menu:append_item("SAVE AS", "Save As...", "Save world file")
file_menu:append_item("SAVE AS COPY", "Save As copy...", "Save copy of world file")
file_menu:append_separator()

-- Recent Files Menu
_rf_menu = EWS:Menu("")
_recent_files_callback = callback(  "on_load_recent_file")

file_menu:append_menu("RECENT FILES", "Recent Files", _rf_menu, "Recent worked on files")
file_menu:append_separator()
file_menu:append_item("EXIT", "Exit", "Exit the application")
menu_bar:append(file_menu, "File")

mainframe:set_menu_bar(menu_bar)

mainframe:connect("", "EVT_CLOSE_WINDOW", callback(  "on_close"), "")
mainframe:connect("NEW", "EVT_COMMAND_MENU_SELECTED", callback(  "on_new"), "")
mainframe:connect("OPEN", "EVT_COMMAND_MENU_SELECTED", callback(  "on_open"), "")
mainframe:connect("SAVE", "EVT_COMMAND_MENU_SELECTED", callback(  "on_save"), "")
mainframe:connect("SAVE AS", "EVT_COMMAND_MENU_SELECTED", callback(  "on_saveas"), "")
mainframe:connect("SAVE AS COPY", "EVT_COMMAND_MENU_SELECTED", callback(  "on_saveascopy"), "")
mainframe:connect("EXIT", "EVT_COMMAND_MENU_SELECTED", callback(  "on_close"), "")

------ Layers Menu
local layers_menu = EWS:Menu("")

_disable_layer_menu = EWS:Menu("")
_hide_layer_menu = EWS:Menu("")
_unhide_layer_menu = EWS:Menu("")

layers_menu:append_item("ENABLE_ALL_LAYERS", "Enabled all layers", "Enabled all layers")
mainframe:connect("ENABLE_ALL_LAYERS", "EVT_COMMAND_MENU_SELECTED", callback(  "on_enable_all_layers"), "")

layers_menu:append_item("DISABLE_ALL_OTHER_LAYERS", "Disable layers", "Disable all layers but current one")
mainframe:connect("DISABLE_ALL_OTHER_LAYERS", "EVT_COMMAND_MENU_SELECTED", callback(  "on_disable_layers"),
  "")

layers_menu:append_menu("DISABLE_LAYERS", "Disable", _disable_layer_menu, "Disable layers")

layers_menu:append_separator()

menu_bar:append(layers_menu, "Layers")

------ Edit Menu
_edit_menu = EWS:Menu("")

_edit_menu:append_radio_item("TB WIDGET SELECT", "Select", "Select Unit")
_edit_menu:append_radio_item("TB WIDGET MOVE", "Move", "Select and Move")
_edit_menu:append_radio_item("TB WIDGET ROTATE", "Rotate", "Select and Rotate")

_edit_menu:append_item("SHOW_MOVE_TRANFORM_TYPE_IN",
  "Move transform type-in",
  "Opens the move transform type-in dialog")
mainframe:connect("SHOW_MOVE_TRANFORM_TYPE_IN", "EVT_COMMAND_MENU_SELECTED",
  callback(  "on_move_transform_type_in"), nil)

_edit_menu:append_item("SHOW_ROTATION_TRANFORM_TYPE_IN",
  "Rotate transform type-in",
  "Opens the rotate transform type-in dialog")
mainframe:connect("SHOW_ROTATION_TRANFORM_TYPE_IN", "EVT_COMMAND_MENU_SELECTED",
  callback(  "on_rotate_transform_type_in"), nil)

_edit_menu:append_separator()

_edit_menu:append_item("SHOW_CAMERA_TRANFORM_TYPE_IN",
  "Camera transform type-in",
  "Opens the camera transform type-in dialog")
mainframe:connect("SHOW_CAMERA_TRANFORM_TYPE_IN", "EVT_COMMAND_MENU_SELECTED",
  callback(  "on_camera_transform_type_in"), nil)

_edit_menu:append_separator()

_coordinate_menu = EWS:Menu("")

_coordinate_menu:set_checked(_coordinate_system, true)

_edit_menu:append_menu("COORDINATE_MENU",
  "Reference Coordinate System\t(" .. ctrl_binding("toggle_coordinate_system") .. ")", _coordinate_menu,
  "Reference Coordinate System")

_edit_menu:append_separator()

_edit_menu:append_check_item("TB_SURFACE_MOVE", "Surface Move\t(" .. ctrl_binding("surface_move_toggle") .. ")",
  "Toggle surface move on and off")
_edit_menu:set_checked("TB_SURFACE_MOVE", _use_surface_move)
mainframe:connect("TB_SURFACE_MOVE", "EVT_COMMAND_MENU_SELECTED", callback(  "menu_toolbar_toggle"),
  { value = "_use_surface_move", menu = "_edit_menu", toolbar = "_toolbar" })

_edit_menu:append_check_item("TB_SNAPPOINTS", "Use Snappoints\t(" ..
ctrl_binding("use_snappoints_toggle") .. ")", "Toggle use of snappoints on and off")
_edit_menu:set_checked("TB_SNAPPOINTS", _use_snappoints)
mainframe:connect("TB_SNAPPOINTS", "EVT_COMMAND_MENU_SELECTED", callback(  "menu_toolbar_toggle"),
  { value = "_use_snappoints", menu = "_edit_menu", toolbar = "_toolbar" })

_edit_menu:append_separator()

_grid_sizes_menu = EWS:Menu("")
_grid_size= 250
_grid_sizes_menu:set_checked("TB_GRIDSIZE" .. _grid_size, true)

_edit_menu:append_menu("GRID_SIZES_MENU", "Grid sizes", _grid_sizes_menu, "Grid Sizes")

--	_edit_menu:append_separator()

_snap_rotations_menu = EWS:Menu("")
_snap_rotation = 45
_snap_rotations_menu:set_checked("TB_SNAPROTATION" .. _snap_rotation, true)

_edit_menu:append_menu("SNAP_ROTATION_MENU", "Snap Rotations", _snap_rotations_menu, "Snap Rotations")

_snap_rotations_axis_menu = EWS:Menu("")

_snap_rotations_axis_menu:append_radio_item("TB_SNAPROTATE_X", "X", "Snap rotate axis X")
_snap_rotations_axis_menu:append_radio_item("TB_SNAPROTATE_Y", "Y", "Snap rotate axis Y")
_snap_rotations_axis_menu:append_radio_item("TB_SNAPROTATE_Z", "Z", "Snap rotate axis Z")
if _snap_rotation_axis == "x" then
  _snap_rotations_axis_menu:set_checked("TB_SNAPROTATE_X", true)
elseif _snap_rotation_axis == "y" then
  _snap_rotations_axis_menu:set_checked("TB_SNAPROTATE_Y", true)
else
  _snap_rotations_axis_menu:set_checked("TB_SNAPROTATE_Z", true)
end

_edit_menu:append_menu("SNAP_ROTATION_AIXS_MENU",
  "Snap Rotation Axis\t(" .. ctrl_binding("change_snaprot_axis") .. ")", _snap_rotations_axis_menu,
  "Snap Rotation Axis")

menu_bar:append(_edit_menu, "Edit")

------ Group Menu
_group_menu = EWS:Menu("")

_group_menu:append_item("SELECT_GROUP_BY_NAME", "Select Group By Name", "Shows a list with all groups")

_group_menu:append_separator()

_group_menu:append_item("GROUP", "Group",
  "Create a group from the seleceted units")
_group_menu:append_item("UNGROUP", "Ungroup",
  "Removes the selected group")

_group_menu:append_separator()

_group_menu:append_check_item("DEBUG_DRAW_GROUPS", "Draw Groups", "Draws units to show that they belong to a group")
_group_menu:set_checked("DEBUG_DRAW_GROUPS", _debug_draw_groups)

_group_menu:append_separator()

_group_menu:append_item("SAVE_GROUP", "Save", "Saves a group to file")
_group_menu:append_item("LOAD_GROUP", "Load", "Loads a group from file")
_group_menu:append_item("SHOW_GROUP_PRESETS", "Group Presets", "Displays a list of group presets")

_group_menu:append_separator()

_group_menu:append_item("DUMP_GROUP", "Dump", "Dumps a group to mesh")

mainframe:connect("SELECT_GROUP_BY_NAME", "EVT_COMMAND_MENU_SELECTED",
  callback(  "on_select_group_by_name"), nil)

mainframe:connect("GROUP", "EVT_COMMAND_MENU_SELECTED", callback(  "group"), nil)
mainframe:connect("UNGROUP", "EVT_COMMAND_MENU_SELECTED", callback(  "ungroup"), nil)

mainframe:connect("DEBUG_DRAW_GROUPS", "EVT_COMMAND_MENU_SELECTED", callback(  "menu_toolbar_toggle"),
  { value = "_debug_draw_groups", menu = "_group_menu" })
--	mainframe:connect( "DEBUG_DRAW_GROUPS", "EVT_COMMAND_MENU_SELECTED", callback(   "toggle_info" ), { info_menu, "INFO OUTPUT" } )

mainframe:connect("SAVE_GROUP", "EVT_COMMAND_MENU_SELECTED", callback(  "save_group"), nil)
mainframe:connect("LOAD_GROUP", "EVT_COMMAND_MENU_SELECTED", callback(  "load_group"), nil)
mainframe:connect("SHOW_GROUP_PRESETS", "EVT_COMMAND_MENU_SELECTED", callback(  "show_group_presets"), nil)

mainframe:connect("DUMP_GROUP", "EVT_COMMAND_MENU_SELECTED", callback(  "dump_group"), nil)

menu_bar:append(_group_menu, "Group")

------ Mission Menu
_mission_menu = EWS:Menu("")

_mission_menu:append_item("RUN MISSION SIMULATION",
  "Run Mission Simulation", "Run Mission Simulation")
_mission_menu:append_item("RUN GAMEPLAY SIMULATION",
  "Run Gameplay Simulation", "Run Gameplay Simulation")

local difficulty_menu = EWS:Menu("")
difficulty_menu:set_checked(_mission_difficulty, true)
_mission_menu:append_menu("DIFFICULTY MENU", "Difficulty", difficulty_menu, "Difficulties")

local players_menu = EWS:Menu("")
players_menu:set_checked(_mission_player, true)
_mission_menu:append_menu("PLAYERS MENU", "Players", players_menu, "Players")

local platforms_menu = EWS:Menu("")
platforms_menu:set_checked(_mission_platform, true)
_mission_menu:append_menu("PLATFORMS MENU", "Platform", platforms_menu, "Platform")

_mission_menu:append_separator()

_mission_menu:append_check_item("MISSION_DEBUG_DRAW_AREAS", "Draw areas during simulation",
  "Will draw areas while running simulation")
_mission_menu:set_checked("MISSION_DEBUG_DRAW_AREAS", _simulation_debug_areas)

mainframe:connect("MISSION_DEBUG_DRAW_AREAS", "EVT_COMMAND_MENU_SELECTED",
  callback(  "menu_toolbar_toggle"), { value = "_simulation_debug_areas", menu = "_mission_menu" })

_mission_menu:append_separator()

_mission_menu:append_item("CONNECT_SLAVE", "Connect Slave System", "Full Client Sync")

mainframe:connect("RUN MISSION SIMULATION", "EVT_COMMAND_MENU_SELECTED",
  callback(  "run_simulation_callback"), true)
mainframe:connect("RUN GAMEPLAY SIMULATION", "EVT_COMMAND_MENU_SELECTED",
  callback(  "run_simulation_callback"), false)
mainframe:connect("CONNECT_SLAVE", "EVT_COMMAND_MENU_SELECTED", callback(  "connect_slave"), nil)

menu_bar:append(_mission_menu, "Mission")

------ View Menu
local view_menu = EWS:Menu("")

-- Info Menu
local info_menu = EWS:Menu("")
info_menu:append_check_item("INFO OUTPUT", "Show output", "Toggle output")
info_menu:set_checked("INFO OUTPUT", true)
mainframe:connect("INFO OUTPUT", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_info"),
  { info_menu, "INFO OUTPUT" })

info_menu:append_check_item("INFO UNIT", "Show unit info", "Toggle unit info")
info_menu:set_checked("INFO UNIT", true)
mainframe:connect("INFO UNIT", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_info"),
  { info_menu, "INFO UNIT" })

view_menu:append_menu("INFO MENU", "Info", info_menu, "Info views")

view_menu:append_item("SHOW_NEWS", "News...", "Show the news list")
mainframe:connect("SHOW_NEWS", "EVT_COMMAND_MENU_SELECTED", callback(  "show_news"), nil)

view_menu:append_separator()
view_menu:append_item("LIGHTING", "Deffered Lighting", "Change visualization to Deffered Lighting")
view_menu:append_item("ALBEDO", "Albedo", "Change visualization to Albedo")
view_menu:append_item("NORMAL", "Normal", "Change visualization to Normal")
--view_menu:append_item( "REFLECTION", "Reflection", "Change visualization to Reflection" )
view_menu:append_item("SPECULAR", "Specular", "Change visualization to Specular")
view_menu:append_item("GLOSSINESS", "Glossiness", "Change visualization to Glossiness")
view_menu:append_item("DEPTH", "Depth", "Change visualization to Depth")
view_menu:append_separator()

local post_processor_effects_menu = EWS:Menu("")

for _, effect in ipairs({ "empty", "default" }) do
  post_processor_effects_menu:append_radio_item(effect, effect, "")
  mainframe:connect(effect, "EVT_COMMAND_MENU_SELECTED", callback(  "on_post_processor_effect"), effect)
end

post_processor_effects_menu:set_checked("empty", true)

view_menu:append_menu("POST_PROCESSOR_MENU", "Post processor effects", post_processor_effects_menu,
  "Post processor effects")

view_menu:append_separator()
view_menu:append_check_item("LOCK_1280_720", "Lock 1280x720",
  "Toggle lock resolution of application window to/from 1280x720")
view_menu:set_checked("LOCK_1280_720", _appwin_fixed_resolution and true or false)
view_menu:append_separator()
view_menu:append_check_item("ORTHOGRAPHIC", "Orthographic",
  "Toggle Orthographic")
view_menu:append_separator()
view_menu:append_check_item("USE LIGHT", "Use Light", "Turn head light on / off")
view_menu:append_check_item("SHOW CENTER", "Show Center",
  "Show Center on / off")
view_menu:set_checked("SHOW CENTER", _show_center)
view_menu:append_check_item("SHOW CAMERA INFO", "Show Camera info",
  "Show Camera info on / off")
view_menu:set_checked("SHOW CAMERA INFO", _show_camera_position)
view_menu:append_check_item("SHOW_MARKER_BALL", "Show Marker", "Show Marker on / off")
view_menu:set_checked("SHOW_MARKER_BALL", _layer_draw_marker)

menu_bar:append(view_menu, "View")

mainframe:connect("LIGHTING", "EVT_COMMAND_MENU_SELECTED", callback(  "change_visualization"),
  "deferred_lighting")
mainframe:connect("ALBEDO", "EVT_COMMAND_MENU_SELECTED", callback(  "change_visualization"),
  "albedo_visualization")
mainframe:connect("NORMAL", "EVT_COMMAND_MENU_SELECTED", callback(  "change_visualization"),
  "normal_visualization")
-- mainframe:connect( "REFLECTION", "EVT_COMMAND_MENU_SELECTED", callback(   "change_visualization" ), "reflection_visualization" )
mainframe:connect("SPECULAR", "EVT_COMMAND_MENU_SELECTED", callback(  "change_visualization"),
  "specular_visualization")
mainframe:connect("GLOSSINESS", "EVT_COMMAND_MENU_SELECTED", callback(  "change_visualization"),
  "glossiness_visualization")
mainframe:connect("DEPTH", "EVT_COMMAND_MENU_SELECTED", callback(  "change_visualization"),
  "depth_visualization")
mainframe:connect("LOCK_1280_720", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_lock_1280_720"),
  { view_menu, "LOCK_1280_720" })
mainframe:connect("ORTHOGRAPHIC", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_orthographic"),
  { view_menu, "ORTHOGRAPHIC" })
mainframe:connect("USE LIGHT", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_light"),
  { view_menu, "USE LIGHT" })
mainframe:connect("SHOW CENTER", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_show_center"),
  { view_menu, "SHOW CENTER" })
mainframe:connect("SHOW CAMERA INFO", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_show_camera_info"),
  { view_menu, "SHOW CAMERA INFO" })
mainframe:connect("SHOW_MARKER_BALL", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_draw_marker_ball"),
  { view_menu, "SHOW_MARKER_BALL" })

------ Hide/Unhide Menu
local hide_menu = EWS:Menu("")

hide_menu:append_item("UNHIDE_BY_NAME", "Unhide by name", "Unhide units by name")
hide_menu:append_item("UNHIDE_ALL", "Unhide all", "Unhide all units")
hide_menu:append_item("HIDE_SELECTED", "Hide selected",
  "Hide selected units")
hide_menu:append_item("HIDE_UNSELECTED", "Hide unselected", "Hide unselected units")

mainframe:connect("UNHIDE_BY_NAME", "EVT_COMMAND_MENU_SELECTED", callback(  "on_unhide_by_name"), nil)
mainframe:connect("UNHIDE_ALL", "EVT_COMMAND_MENU_SELECTED", callback(  "on_unhide_all"), nil)
mainframe:connect("HIDE_SELECTED", "EVT_COMMAND_MENU_SELECTED", callback(  "on_hide_selected"), nil)
mainframe:connect("HIDE_UNSELECTED", "EVT_COMMAND_MENU_SELECTED", callback(  "on_hide_unselected"), nil)

hide_menu:append_separator()

hide_menu:append_item("HIDE_CURRENT_LAYERS", "Hide current layer", "Hide current layer")
hide_menu:append_item("HIDE_ALL_LAYERS", "Hide other layers", "Hide all layers but current")
hide_menu:append_menu("HIDE_LAYERS", "Hide", _hide_layer_menu, "Hide layers")
hide_menu:append_menu("UNHIDE_HIDE_LAYERS", "Unhide", _unhide_layer_menu, "Unhide layers")

mainframe:connect("HIDE_CURRENT_LAYERS", "EVT_COMMAND_MENU_SELECTED", callback(  "on_hide_current_layer"),
  nil)
mainframe:connect("HIDE_ALL_LAYERS", "EVT_COMMAND_MENU_SELECTED", callback(  "on_hide_all_layers"), nil)

hide_menu:append_separator()

hide_menu:append_item("HIDE HELPERS", "Hide Helpers", "Hide Helpers")
hide_menu:append_item("UNHIDE HELPERS", "Unhide Helpers", "Unhide Helpers")

hide_menu:append_separator()

hide_menu:append_check_item("RENDER_EFFECTS", "Render Effects", "Toggle rendering of effects on and off")
hide_menu:set_checked("RENDER_EFFECTS", true)

mainframe:connect("HIDE HELPERS", "EVT_COMMAND_MENU_SELECTED", callback(  "on_hide_helper_units"), false)
mainframe:connect("UNHIDE HELPERS", "EVT_COMMAND_MENU_SELECTED", callback(  "on_hide_helper_units"), true)
mainframe:connect("RENDER_EFFECTS", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_render_effects"),
  { hide_menu, "RENDER_EFFECTS" })

menu_bar:append(hide_menu, "Hide/Unhide")

------ Advanced Menu
local advanced_menu = EWS:Menu("")

advanced_menu:append_item("CONFIGURATION", "Configuration...",
  "Configuration")
advanced_menu:append_item("CONTROLLER_BINDINGS", "Show controller bindings...", "Show controller bindings")
advanced_menu:append_separator()
advanced_menu:append_check_item("EXPERT MODE", "Expert Mode",
  "Toggle expert mode")
advanced_menu:set_checked("EXPERT MODE", false)
advanced_menu:append_separator()
advanced_menu:append_item("SELECT UNIT", "Global select unit...",
  "Global select unit")
advanced_menu:append_item("SELECT BY NAME", "Select by name...",
  "Select by name")

menu_bar:append(advanced_menu, "Advanced")

mainframe:connect("CONFIGURATION", "EVT_COMMAND_MENU_SELECTED", callback(  "on_configuration"), "")
mainframe:connect("CONTROLLER_BINDINGS", "EVT_COMMAND_MENU_SELECTED", callback(  "on_controller_bindings"),
  "")
mainframe:connect("EXPERT MODE", "EVT_COMMAND_MENU_SELECTED", callback(  "on_expert_mode"),
  { advanced_menu, "EXPERT MODE" })
mainframe:connect("SELECT UNIT", "EVT_COMMAND_MENU_SELECTED", callback(  "on_global_select_unit"), "")
mainframe:connect("SELECT BY NAME", "EVT_COMMAND_MENU_SELECTED", callback(  "on_select_by_name"), "")

------ Unit menu
local unit_menu = EWS:Menu("")
unit_menu:append_item("EDIT_UNIT_DIALOG", "Edit Unit..",
  "Show/hide the Edit Unit dialog.")
unit_menu:append_item("RELOAD UNIT", "Reload Unit", "Reload Unit")
unit_menu:append_item("RELOAD UNIT QUICK", "Reload Unit Quick", "Reload Unit Quick")

menu_bar:append(unit_menu, "Unit")

mainframe:connect("EDIT_UNIT_DIALOG", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_edit_unit_dialog"), "")
mainframe:connect("RELOAD UNIT", "EVT_COMMAND_MENU_SELECTED", callback(  "on_reload_unit"), "")
mainframe:connect("RELOAD UNIT QUICK", "EVT_COMMAND_MENU_SELECTED", callback(  "on_reload_unit", true), "")

------ Profiler Menu
_profiler_menu = EWS:Menu("")

_profiler_menu:append_item("PROFILER_MAIN", "Main", "")
_profiler_menu:append_item("PROFILER_MEM", "Mem", "")
_profiler_menu:append_item("PROFILER_D3D", "D3d", "")
_profiler_menu:append_item("PROFILER_UNIT_PROFILER", "Unit Profiler", "")

menu_bar:append(_profiler_menu, "Profiler")

mainframe:connect("PROFILER_MAIN", "EVT_COMMAND_MENU_SELECTED", callback(  "on_profiler_main"), "")
mainframe:connect("PROFILER_MEM", "EVT_COMMAND_MENU_SELECTED", callback(  "on_profiler_mem"), "")
mainframe:connect("PROFILER_D3D", "EVT_COMMAND_MENU_SELECTED", callback(  "on_profiler_d3d"), "")
mainframe:connect("PROFILER_UNIT_PROFILER", "EVT_COMMAND_MENU_SELECTED",
  callback(  "on_profiler_unit_profiler"), "")

------ Debug Menu
_debug_menu = EWS:Menu("")

_debug_menu:append_item("ADD_WORKVIEW", "Add workview", "Adds a workview based on camera position and rotation")
_debug_menu:append_item("SHOW_WORKVIEW", "Show workviews", "Shows the workviews dialog")

_debug_menu:append_separator()

_debug_menu:append_item("CHECK DUALITY", "Check Unit Duality",
  "Goes through all units and checks if any of the same share position and rotation")

_debug_menu:append_separator()

_debug_menu:append_item("TB_MAKE_SCREENSHOT", "Capture Screenshot\t" ..
ctrl_menu_binding("capture_screenshot"), "Choose this to create a screenshot")
_debug_menu:append_check_item("TB_DRAW_OCCLUDERS", "Draw Occluders", "Toggle debug draw of occluder objects")

mainframe:connect("ADD_WORKVIEW", "EVT_COMMAND_MENU_SELECTED", callback(  "on_add_workview"), "")
mainframe:connect("SHOW_WORKVIEW", "EVT_COMMAND_MENU_SELECTED", callback(  "on_show_workview"), "")
mainframe:connect("CHECK DUALITY", "EVT_COMMAND_MENU_SELECTED", callback(  "on_check_duality"), "")
mainframe:connect("TB_MAKE_SCREENSHOT", "EVT_COMMAND_MENU_SELECTED", callback(  "on_make_screenshot"), "")
mainframe:connect("TB_DRAW_OCCLUDERS", "EVT_COMMAND_MENU_SELECTED", callback(  "menu_toolbar_toggle"),
  { value = "_draw_occluders", menu = "_debug_menu", toolbar = "_left_upper_toolbar" })
--	mainframe:connect( "TB_DRAW_OCCLUDERS", "EVT_COMMAND_MENU_SELECTED", callback(   "toggle_draw_occluders" ), { _debug_menu, "DRAW OCCLUDERS" } )

menu_bar:append(_debug_menu, "Debug")

------ Toolbox Menu
if (managers and managers.toolhub) then
  menu_bar:append(managers.toolhub:get_tool_menu(mainframe), "Toolhub")
end

------ Window Menu
local window_menu = EWS:Menu("")

window_menu:append_check_item("SHOW MARKERS", "Show Markers", "Toggle markers")
window_menu:set_checked("SHOW MARKERS", _show_markers)
mainframe:connect("SHOW MARKERS", "EVT_COMMAND_MENU_SELECTED", callback(  "toggle_show_markers"),
  { window_menu, "SHOW MARKERS" })

menu_bar:append(window_menu, "Window")

------ Help Menu
local help_menu = EWS:Menu("")

help_menu:append_item("HELP", "Problem solver", "Open help")
help_menu:append_item("USING THE EDITOR", "Using the editor", "Open using the editor")
help_menu:append_separator()
help_menu:append_item("ABOUT", "About...", "Hello and whatnot")

menu_bar:append(help_menu, "Help")

mainframe:connect("HELP", "EVT_COMMAND_MENU_SELECTED", callback(  "on_help"), "")
mainframe:connect("USING THE EDITOR", "EVT_COMMAND_MENU_SELECTED", callback(  "on_using_the_editor"), "")
mainframe:connect("ABOUT", "EVT_COMMAND_MENU_SELECTED", callback(  "on_about"), "")

end