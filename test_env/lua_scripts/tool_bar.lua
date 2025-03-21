CoreEWS = {}
function CoreEWS.image_path(path) return path end

icons_path = ""

function init_toolbar(frame)
  _toolbar = EWS:ToolBar(frame, "", "TB_FLAT,TB_NODIVIDER")
  frame:set_tool_bar(_toolbar)

  _toolbar:add_tool("TB NEW WORLD", "New World", CoreEWS.image_path("toolbar/new_16x16.png"), "New World")
  _toolbar:connect("TB NEW WORLD", "EVT_COMMAND_MENU_SELECTED", callback("on_new"), "")

  _toolbar:add_tool("TB OPEN WORLD", "Open World", CoreEWS.image_path("toolbar/open_16x16.png"), "Open World")
  _toolbar:connect("TB OPEN WORLD", "EVT_COMMAND_MENU_SELECTED", callback("on_open"), "")

  _toolbar:add_tool("TB SAVE WORLD", "Save World", CoreEWS.image_path("toolbar/save_16x16.png"), "Save World")
  _toolbar:connect("TB SAVE WORLD", "EVT_COMMAND_MENU_SELECTED", callback("on_save"), "")

  _toolbar:add_separator()

  _toolbar:add_tool("TB SELECT BY NAME", "Select by name", icons_path .. "select_by_name.bmp", "Select by name")
  _toolbar:connect("TB SELECT BY NAME", "EVT_COMMAND_MENU_SELECTED", callback("on_select_by_name"), "")

  _toolbar:add_tool("TB PREVIEW UNITS", "Browse and preview all availible units", icons_path .. "browse_units.bmp",
    "Browse and preview all availible units")
  _toolbar:connect("TB PREVIEW UNITS", "EVT_COMMAND_MENU_SELECTED", callback("on_unit_tree_browser"), "")

  _toolbar:add_tool("TB GLOBAL SELECT UNITS", "Global select unit", icons_path .. "global_select_unit.bmp",
    "Global select unit")
  _toolbar:connect("TB GLOBAL SELECT UNITS", "EVT_COMMAND_MENU_SELECTED",
    callback("on_global_select_unit"), "")

  _toolbar:add_tool("TB_UNHIDE_BY_NAME", "Unhide by name",
    CoreEWS.image_path("world_editor/unhide_by_name_16x16.png"), "Opens the unhide by name dialog")
  _toolbar:connect("TB_UNHIDE_BY_NAME", "EVT_COMMAND_MENU_SELECTED", callback("on_unhide_by_name"), "")

  _toolbar:add_tool("TB UNIT DEBUG LIST", "Unit debug list", icons_path .. "unit_list.bmp", "Unit debug list")
  _toolbar:connect("TB UNIT DEBUG LIST", "EVT_COMMAND_MENU_SELECTED", callback("on_unit_list"), "")

  _toolbar:add_separator()

  _toolbar:add_tool("TB_SELECT_GROUP_BY_NAME", "Select group by name", icons_path .. "select_group_by_name.bmp",
    "Select group by name")
  _toolbar:connect("TB_SELECT_GROUP_BY_NAME", "EVT_COMMAND_MENU_SELECTED",
    callback("on_select_group_by_name"), "")

  _toolbar:add_separator()

  _toolbar:add_radio_tool("TB WIDGET SELECT", "Select Unit", icons_path .. "widget_select.bmp", "Select Unit")
  frame:connect("TB WIDGET SELECT", "EVT_COMMAND_MENU_SELECTED", callback("set_widget"),
    { select = true, move = false, rotate = false })
  _toolbar:add_radio_tool("TB WIDGET MOVE", "Select and Move", icons_path .. "widget_move.bmp", "Select and Move")
  frame:connect("TB WIDGET MOVE", "EVT_COMMAND_MENU_SELECTED", callback("set_widget"),
    { select = false, move = true, rotate = false })
  frame:connect("TB WIDGET MOVE", "EVT_COMMAND_TOOL_RCLICKED", callback("on_move_transform_type_in"),
    nil)
  _toolbar:add_radio_tool("TB WIDGET ROTATE", "Select and Rotate", icons_path .. "widget_rotation.bmp",
    "Select and Rotate")
  frame:connect("TB WIDGET ROTATE", "EVT_COMMAND_MENU_SELECTED", callback("set_widget"),
    { select = false, move = false, rotate = true })
  frame:connect("TB WIDGET ROTATE", "EVT_COMMAND_TOOL_RCLICKED",
    callback("on_rotate_transform_type_in"), nil)

  --build_ref_coordinate_system()

  _toolbar:add_check_tool("TB_SURFACE_MOVE", "Surface move",
    CoreEWS.image_path("world_editor/surface_move_16x16.png"), "Toggle surface move on and off")
  _toolbar:set_tool_state("TB_SURFACE_MOVE", _use_surface_move)
  _toolbar:connect("TB_SURFACE_MOVE", "EVT_COMMAND_MENU_SELECTED", callback("toolbar_toggle"),
    { value = "_use_surface_move", menu = "_edit_menu" })
  _ews_triggers = callback("toolbar_toggle_trg",
    { value = "_use_surface_move", id = "TB_SURFACE_MOVE", menu = "_edit_menu" })

  _toolbar:add_check_tool("TB_SNAPPOINTS", "Use Snappoints",
    CoreEWS.image_path("world_editor/snappoints_16x16.png"), "Toggle use of snappoints on and off")
  _toolbar:set_tool_state("TB_SNAPPOINTS", _use_snappoints)
  _toolbar:connect("TB_SNAPPOINTS", "EVT_COMMAND_MENU_SELECTED", callback("toolbar_toggle"),
    { value = "_use_snappoints", menu = "_edit_menu" })
  _ews_triggers = callback("toolbar_toggle_trg",
    { value = "_use_snappoints", id = "TB_SNAPPOINTS", menu = "_edit_menu" })

  _toolbar:add_separator()

  _toolbar:add_check_tool("TB_USING_GROUPS", "Using groups (" .. ctrl_binding("using_group_toggle") .. ")",
    CoreEWS.image_path("world_editor/using_groups_16x16.png"), "Toggle using groups on and off")
  _toolbar:connect("TB_USING_GROUPS", "EVT_COMMAND_MENU_SELECTED", callback("toolbar_toggle"),
    { value = "_using_groups" })
  _ews_triggers = callback("toolbar_toggle_trg",
    { value = "_using_groups", id = "TB_USING_GROUPS" })

  _toolbar:add_separator()

  --build_grid_sizes(icons_path)

  _toolbar:add_check_tool("TB_LAYER_DRAW_GRID", "Draw grid",
    CoreEWS.image_path("world_editor/toggle_draw_grid_16x16.png"), "Toggle draw grid on and off")
  _toolbar:set_tool_state("TB_LAYER_DRAW_GRID", _layer_draw_grid)
  _toolbar:connect("TB_LAYER_DRAW_GRID", "EVT_COMMAND_MENU_SELECTED", callback("toolbar_toggle"),
    { value = "_layer_draw_grid", toolbar = "_toolbar" })

  _toolbar:add_separator()

  --build_snap_rotations()

  _toolbar:add_radio_tool("TB_SNAPROTATE_X", "Snap rotate axis X",
    CoreEWS.image_path("world_editor/snap_rotation_x_16x16.png"), "Snap rotate axis X")
  frame:connect("TB_SNAPROTATE_X", "EVT_COMMAND_MENU_SELECTED", callback("set_snap_rotation_axis"),
    { axis = "x" })
  _toolbar:add_radio_tool("TB_SNAPROTATE_Y", "Snap rotate axis Y",
    CoreEWS.image_path("world_editor/snap_rotation_y_16x16.png"), "Snap rotate axis Y")
  frame:connect("TB_SNAPROTATE_Y", "EVT_COMMAND_MENU_SELECTED", callback("set_snap_rotation_axis"),
    { axis = "y" })
  _toolbar:add_radio_tool("TB_SNAPROTATE_Z", "Snap rotate axis Z",
    CoreEWS.image_path("world_editor/snap_rotation_z_16x16.png"), "Snap rotate axis Z")
  frame:connect("TB_SNAPROTATE_Z", "EVT_COMMAND_MENU_SELECTED", callback("set_snap_rotation_axis"),
    { axis = "z" })
  if _snap_rotation_axis == "x" then
    _toolbar:set_tool_state("TB_SNAPROTATE_X", true)
  elseif _snap_rotation_axis == "y" then
    _toolbar:set_tool_state("TB_SNAPROTATE_Y", true)
  else
    _toolbar:set_tool_state("TB_SNAPROTATE_Z", true)
  end

  --_ews_triggers["change_snaprot_axis"] = callback(  "change_snaprot_axis",
  --  { value = "_coordinate_system", combobox = _ref_coordinate_system, t = "_coordinate_systems", menu = self
  --  ._coordinate_menu })

  --build_rotation_speed()

  _toolbar:add_separator()

  _toolbar:add_tool("TB_GENERATE_SELECTED_PROJECTION_LIGHT", "Generate selected projection light",
    CoreEWS.image_path("world_editor/generate_one_projection_16x16.png"), "Generate selected projection light")
  _toolbar:add_tool("TB_GENERATE_ALL_PROJECTION_LIGHT", "Generate all projection light",
    CoreEWS.image_path("world_editor/generate_all_projection_16x16.png"), "Generate all projection light")
  _toolbar:connect("TB_GENERATE_SELECTED_PROJECTION_LIGHT", "EVT_COMMAND_MENU_SELECTED",
    callback("create_projection_light"), "selected")
  _toolbar:connect("TB_GENERATE_ALL_PROJECTION_LIGHT", "EVT_COMMAND_MENU_SELECTED",
    callback("create_projection_light"), "all")

  _toolbar:add_separator()

  local s =
  'Burton Leon Reynolds, Jr.[2] (born February 11, 1936) is an Oscar-nominated and Emmy Award-winning American actor.'
  _toolbar:add_tool("TB_HELP", "Burt Reynolds guide to using the editor", icons_path .. "burt.bmp", s)
  _toolbar:connect("TB_HELP", "EVT_COMMAND_MENU_SELECTED", callback("on_using_the_editor"), "")
end