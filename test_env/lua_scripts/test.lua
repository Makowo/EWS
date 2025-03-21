local frame = EWS:Frame("World Editor", Vector3(700, 100, 0), Vector3(1080, 800, 0),
  "CAPTION,CLOSE_BOX,MINIMIZE_BOX,MAXIMIZE_BOX,MAXIMIZE,SYSTEM_MENU,RESIZE_BORDER", nil)

init_menubar(frame)
init_toolbar(frame)


local frame_panel = EWS:Panel(frame, "", "")

-- not panel, custom class that extends wxWindow
local appwin = EWS:Panel(frame_panel, "", "")

local top_sizer = EWS:BoxSizer("VERTICAL")

local main_sizer = EWS:BoxSizer("HORIZONTAL")

local left_toolbar_sizer = EWS:BoxSizer("VERTICAL")
main_sizer:add(left_toolbar_sizer, 0, 0, "EXPAND")

local app_sizer = EWS:BoxSizer("VERTICAL")
main_sizer:add(app_sizer, 4, 0, "EXPAND")
app_sizer:add(appwin, 5, 0, "EXPAND")

top_sizer:add(main_sizer, 1, 0, "EXPAND")

frame_panel:set_sizer(top_sizer)

frame:set_icon("world_editor_16x16.png")

--- notebook test

local function ContinentPanel(splitterwindow)
  local panel = EWS:Panel(splitterwindow, "", "TAB_TRAVERSAL,ALWAYS_SHOW_SB")

  local panel_sizer = EWS:BoxSizer("VERTICAL")

  panel:set_sizer(panel_sizer)

  --local continentpanel = EWS:ScrolledWindow(panel, "", "VSCROLL,TAB_TRAVERSAL,SIMPLE_BORDER")
  local continentpanel = EWS:Panel(panel, "", "VSCROLL,TAB_TRAVERSAL,SIMPLE_BORDER")

  --continentpanel:set_scroll_rate(Vector3(0, 1, 0))
  --continentpanel:set_virtual_size_hints(Vector3(0, 0, 0), Vector3(1, -1, -1))
  --continentpanel:set_background_colour(255, 255, 255)
  --continentpanel:refresh()

  local self = ""

  local continentpanel_sizer = EWS:BoxSizer("VERTICAL")
  continentpanel:set_sizer(continentpanel_sizer)


  local toolbar_sizer = EWS:BoxSizer("VERTICAL")
  local tb = EWS:ToolBar(panel, "", "TB_FLAT,TB_NODIVIDER,TB_VERTICAL")

  tb:add_tool("CREATE_CONTINENT", "Create a new continent",
    CoreEWS.image_path("world_editor\\new_continent_16x16.png"), "Create a new continent")
  tb:connect("CREATE_CONTINENT", "EVT_COMMAND_MENU_SELECTED",
    callback(self, self, "create_continent_dialog"), nil)

  tb:add_tool("DELETE_CONTINENT", "Delete the current continent",
    CoreEWS.image_path("toolbar\\delete_16x16.png"), "Delete the current continent")
  tb:connect("DELETE_CONTINENT", "EVT_COMMAND_MENU_SELECTED", callback(self, self, "delete_continent"), nil)

  toolbar_sizer:add(tb, 1, 0, "EXPAND")

  tb:realize()

  local sizer = EWS:BoxSizer("HORIZONTAL")

  local _world_setting_toolbar = EWS:ToolBar(continentpanel, "", "TB_FLAT,TB_NODIVIDER")

  _world_setting_toolbar:add_tool("CREATE_WORLD_SETTING", "Create a world setting file",
    CoreEWS.image_path("world_editor\\continent\\create_world_setting_16x16.png"), "Create a world setting file")
  _world_setting_toolbar:connect("CREATE_WORLD_SETTING", "EVT_COMMAND_MENU_SELECTED",
    callback(self, self, "create_world_setting_dialog"), nil)

  _world_setting_toolbar:add_tool("OPEN_WORLD_SETTING", "Open a world setting file to edit",
    CoreEWS.image_path("toolbar\\open_16x16.png"), "Open a world setting file to edit")
  _world_setting_toolbar:connect("OPEN_WORLD_SETTING", "EVT_COMMAND_MENU_SELECTED",
    callback(self, self, "open_world_setting_dialog"), nil)

  sizer:add(_world_setting_toolbar, 0, 0, "EXPAND")

  local _world_setting_path = EWS:StaticText(continentpanel, "-", 0, "ALIGN_CENTRE")

  sizer:add(_world_setting_path, 1, 0, "ALIGN_CENTER_VERTICAL")

  local _right_world_setting_toolbar = EWS:ToolBar(continentpanel, "", "TB_FLAT,TB_NODIVIDER")

  _right_world_setting_toolbar:add_tool("CLEAR_SIMULATION_WORLD_SETTING",
    "Removes the world setting file when simulating", CoreEWS.image_path("toolbar\\delete_16x16.png"),
    "Removes the world setting file when simulating")
  _right_world_setting_toolbar:connect("CLEAR_SIMULATION_WORLD_SETTING", "EVT_COMMAND_MENU_SELECTED",
    callback(self, self, "remove_simulation_world_setting_dialog"), nil)

  _right_world_setting_toolbar:add_tool("SET_SIMULATION_WORLD_SETTING",
    "Set a world setting file to use when simulating", CoreEWS.image_path("toolbar\\open_16x16.png"),
    "Set a world setting file to use when simulating")
  _right_world_setting_toolbar:connect("SET_SIMULATION_WORLD_SETTING", "EVT_COMMAND_MENU_SELECTED",
    callback(self, self, "set_simulation_world_setting_dialog"), nil)

  sizer:add(_right_world_setting_toolbar, 0, 10, "EXPAND,RIGHT")

  _world_setting_toolbar:realize()
  _right_world_setting_toolbar:realize()

  continentpanel_sizer:add(sizer, 0, 0, "EXPAND")

  return panel
end

local splitterwindow = EWS:SplitterWindow(frame_panel, "", "")

local continents_panel = ContinentPanel(splitterwindow)

local notebook = EWS:Notebook(splitterwindow, "", "NB_TOP,NB_MULTILINE")

splitterwindow:split_horizontally(continents_panel, notebook, 256)


main_sizer:add(splitterwindow, 1, 0, "EXPAND")

local function add_page_test(notebook, name, select)
  local panel = EWS:Panel(notebook, "", "")
  notebook:add_page(panel, name, select)
end

add_page_test(notebook, "Brush", false)
add_page_test(notebook, "Sound", false)
add_page_test(notebook, "Mission", false)
add_page_test(notebook, "Environment", false)
add_page_test(notebook, "WorldCamera", false)
add_page_test(notebook, "Portals", false)
add_page_test(notebook, "Wires", false)
add_page_test(notebook, "Statics", false)
add_page_test(notebook, "Dynamics", false)
add_page_test(notebook, "Level Settings", false)
add_page_test(notebook, "Instances", false)

frame:set_visible(true)