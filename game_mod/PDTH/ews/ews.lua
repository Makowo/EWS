--require("core/lib/utils/coreclass")
core:import("CoreClass")

EWS_Log("EWS module loading")

EWS_UserSettings = EWS_UserSettings or {
  DISABLE_SUNKENBORDER_ON_APPWINDOW = true
}

EWS_Log("Registering EWS class functions")

function EWS_LuaSided_CHelper_Print_Stack()
  EWS_Log(Application:stack())
end

---@class EWS_BaseComponent
EWS_BaseComponent = EWS_BaseComponent or class()


function EWS_BaseComponent:init()
  -- setup stuff for garbage collection here. __gc for tables doesn't exist in Lua 5.1, so it's either keep track of everything in a huge list or use a bunch of hacks to create a userdata object that garbage collects the table
  
  if type(self.internal_c_obj) ~= "userdata" then
    EWS_Log("[EWS_BaseComponent:init] Lua stack error: " .. tostring(self.internal_c_obj))
    EWS_Log(Application:stack())
  end

  self.garbage_collection_object = newproxy(true)

  local ews_obj = self

  --[[getmetatable(self.garbage_collection_object).__gc = function(self)
    if ews_obj.parent then
    else
      -- causes crash in development :(
      --EWS_Delete_Object(ews_obj.internal_c_obj)
    end
  end]]--
end

function EWS_BaseComponent:connect(id_or_event, event_or_callback, callback_or_unk1, unk1)
  local id = ""
  local event = ""
  local callback = nil


  local extra_data = nil

  if type(event_or_callback) == "string" then
    -- :connect("ELEMENT_ID", "EVT_NAME", callback, nil)
    id = id_or_event
    event = event_or_callback
    callback = callback_or_unk1
    extra_data = unk1
  else
    -- :connect("EVT_NAME", callback, nil)
    event = id_or_event
    callback = event_or_callback
    extra_data = callback_or_unk1
  end

  local callback_wrapper = function(cb, edata)
    return function(event_id_str)
      local evt_tbl = {}
      evt_tbl.id = event_id_str
      evt_tbl.get_id = function(self) return self.id end
      evt_tbl.skip = function(self) end -- implement event skipping
      if type(edata) ~= "nil" then
        cb(edata, evt_tbl)
      else
        cb(evt_tbl)
      end
    end
  end

  EWS_EventHandler_Connect(self.internal_c_obj, id, event, callback, callback_wrapper(callback, extra_data))
  --EWS_Log("[EWS_Window:connect] " .. tostring(id) .. ", " .. tostring(event) .. ", " .. tostring(callback))
end

function EWS_BaseComponent:disconnect(...) end

---@class EWS_Window: EWS_BaseComponent
EWS_Window = EWS_Window or class(EWS_BaseComponent)

---@return EWS_Window
function EWS_Window:parent()
  return self.parent_obj
end

function EWS_Window:refresh()
  EWS_Window_Refresh(self.internal_c_obj)
end

function EWS_Window:update()
  EWS_Window_Update(self.internal_c_obj)
end

function EWS_Window:layout()
  EWS_Window_Layout(self.internal_c_obj)
end

function EWS_Window:destroy()
  EWS_Window_Destroy(self.internal_c_obj)
end

function EWS_Window:destroy_children()
  EWS_Window_DestroyChildren(self.internal_c_obj)
end

function EWS_Window:freeze()
  EWS_Window_Freeze(self.internal_c_obj)
end

function EWS_Window:thaw()
  EWS_Window_Thaw(self.internal_c_obj)
end

---@param new_visible boolean
function EWS_Window:set_visible(new_visible)
  -- CAUSES ERRORS IN PDTH?
  EWS_Window_Set_Visible(self.internal_c_obj, new_visible)
end

---@param sizer EWS_Sizer
function EWS_Window:set_sizer(sizer)
  --EWS_Panel_SetSizer(self.internal_c_obj, sizer.internal_c_obj)
  EWS_Window_Set_Sizer(self.internal_c_obj, sizer.internal_c_obj)
end

function EWS_Window:fit()
  EWS_Window_Fit(self.internal_c_obj)
end

function EWS_Window:fit_inside()
  EWS_Window_FitInside(self.internal_c_obj)
end

---@return Vector3
function EWS_Window:background_colour() end

---@param r_or_name number|string can be the red value or a string containing the name of a colour
---@param g number
---@param b number
function EWS_Window:set_background_colour(r_or_name, g, b)
  local is_colour_name = type(r_or_name) == "string"
end

---@param new_size Vector3
function EWS_Window:set_size(new_size)
  EWS_Window_SetSize(self.internal_c_obj, new_size)
end

---@return Vector3
function EWS_Window:get_size()
  local x, y = EWS_Window_GetSize(self.internal_c_obj)
  return Vector3(x, y, 0)
end

---@param new_size Vector3
function EWS_Window:set_min_size(new_size)
  EWS_Window_SetMinSize(self.internal_c_obj, new_size)
end

---@return Vector3
function EWS_Window:get_min_size()
  local x, y = EWS_Window_GetMinSize(self.internal_c_obj)
  return Vector3(x, y, 0)
end

---@param new_size Vector3
function EWS_Window:set_max_size(new_size)
  EWS_Window_SetMaxSize(self.internal_c_obj, new_size)
end

---@return Vector3
function EWS_Window:get_best_size()
end

---@return Vector3
function EWS_Window:get_effective_min_size()
end

---@param position Vector3
function EWS_Window:set_position(position)
end

---@return Vector3
function EWS_Window:get_position()
  local x, y = EWS_Window_GetPosition(self.internal_c_obj)
  return Vector3(x, y, 0)
end

function EWS_Window:center(direction)
end

---@param enabled boolean
function EWS_Window:set_enabled(enabled)
  EWS_Window_SetEnabled(self.internal_c_obj, enabled)
end

function EWS_Window:enabled()
  return EWS_Window_Enabled(self.internal_c_obj)
end

function EWS_Window:set_font_size(size) end

function EWS_Window:set_font_family(font_family) end

function EWS_Window:set_font_style(style) end

function EWS_Window:set_font_weight(weight) end

function EWS_Window:set_font_underline(underline) end

function EWS_Window:set_font_face(face) end

function EWS_Window:set_own_font_size(size) end

function EWS_Window:set_own_font_family(font_family) end

function EWS_Window:set_own_font_style(style) end

function EWS_Window:set_own_font_weight(weight) end

function EWS_Window:set_own_font_underline(underline) end

function EWS_Window:set_own_font_face(face) end

function EWS_Window:set_tool_tip(tooltip) end

---@param menu EWS_Menu
---@param position Vector3
---@return EWS_Window
function EWS_Window:popup_menu(menu, position) end

---Gets keyboard focus
function EWS_Window:set_focus()
  EWS_Window_SetFocus(self.internal_c_obj)
end

---@return bool
---Returns true if the window is in focus
function EWS_Window:in_focus()
  return EWS_Window_InFocus(self.internal_c_obj)
end

---Sets the virtual size of the window
function EWS_Window:set_virtual_size(size) end

function EWS_Window:set_virtual_size_hints(min_size, max_size) end

function EWS_Window:toggle_style(style) end

-- TopLevelWindow functions, only Frames and Dialogs should call these
---@param title string
function EWS_Window:set_title(title)
  EWS_TopLevelWindow_SetTitle(self.internal_c_obj, title)
end
---@return boolean
function EWS_Window:is_active()
  return EWS_TopLevelWindow_IsActive(self.internal_c_obj)
end
---@return boolean
function EWS_Window:is_maximized()
  return EWS_TopLevelWindow_IsMaximized(self.internal_c_obj)
end

----- appwindow.lua
---Special Diesel Engine class, used to render the game viewport

---@class EWS_AppWindow
EWS_AppWindow = EWS_AppWindow or class(EWS_Window)

function EWS_Window:init(parent, size, style)
  self.parent_obj = parent
  self.size = size
  self.style = style

  if EWS_UserSettings.DISABLE_SUNKENBORDER_ON_APPWINDOW then
    style = ""
  end

  self.internal_c_obj = EWS_New_AppWindow(parent.internal_c_obj, size, style)

  EWS_BaseComponent.init(self)
end

----- frame.lua
--- Frame

---@class EWS_Frame: EWS_Window
EWS_Frame = EWS_Frame or class(EWS_Window)

function EWS_Frame:init(title, start_position, size, style, parent)
  self.title = title
  self.start_pos = start_position
  self.size = size
  self.style = style
  self.parent_obj = parent
  if parent == nil then
    self.has_parent = false
  else
    self.has_parent = true
    parent = parent.internal_c_obj
  end

  self.internal_c_obj = EWS_New_Frame(title, start_position, size, style, parent)

  EWS_BaseComponent.init(self)
end

---@param menubar EWS_MenuBar
function EWS_Frame:set_menu_bar(menubar)
  EWS_Frame_Set_MenuBar(self.internal_c_obj, menubar.internal_c_obj)
end

---@param toolbar EWS_ToolBar
function EWS_Frame:set_tool_bar(toolbar)
  EWS_Frame_Set_ToolBar(self.internal_c_obj, toolbar.internal_c_obj)
end

---@param icon string
function EWS_Frame:set_icon(icon)
  EWS_Frame_Set_Icon(self.internal_c_obj, icon)
end

---@param status_bar EWS_StatusBar
function EWS_Frame:set_status_bar(status_bar)
  EWS_Frame_Set_StatusBar(self.internal_c_obj, status_bar.internal_c_obj)
end

function EWS_Frame:is_iconized()
  return EWS_Frame_IsIconized(self.internal_c_obj)
end

--- Dialog

---@class EWS_Dialog: EWS_Window
EWS_Dialog = EWS_Dialog or class(EWS_Window)

-- This function has two forms, one that takes 4 arguments and one that takes 6
function EWS_Dialog:init(parent, title, unknown, style_or_position, size, style_)
  local p = parent
  if p ~= nil then
    p = parent.internal_c_obj
  end

  local simple = true -- assume simple, 4 argument, form

  if type(style_or_position) == "string" then
    simple = true
  else
    simple = false
  end

  self.parent_obj = p

  if simple then
    self.internal_c_obj = EWS_New_Dialog_Simple(p, title, style_or_position)
  else
    self.internal_c_obj = EWS_New_Dialog_Complex(p, title, style_or_position, size, style_)
  end

  EWS_BaseComponent.init(self)
end

function EWS_Dialog:show_modal()
  return EWS_Dialog_ShowModal(self.internal_c_obj)
end
function EWS_Dialog:end_modal(code)
  EWS_Dialog_EndModal(self.internal_c_obj, code)
end

function EWS_Dialog:set_icon(icon_path)
  EWS_Dialog_SetIcon(self.internal_c_obj, icon_path)
end

function EWS_Dialog:visible()
  return EWS_Dialog_Visible(self.internal_c_obj)
end

----- menu.lua
--- Menu

---@class EWS_Menu
EWS_Menu = EWS_Menu or class(EWS_BaseComponent)

---@param style string style, Bringer of Worlds uses an empty string
function EWS_Menu:init(style)
  self.internal_c_obj = EWS_New_Menu(style)

  EWS_BaseComponent.init(self)
end

function EWS_Menu:append_item(id, label, description)
  EWS_Menu_AppendItem(self.internal_c_obj, id, label, description)
end

function EWS_Menu:append_separator()
  EWS_Menu_AppendSeparator(self.internal_c_obj)
end

---@param id string
---@param name string
---@param menu EWS_Menu menu to append
---@param description string
function EWS_Menu:append_menu(id, name, menu, description)
  EWS_Menu_AppendMenu(self.internal_c_obj, id, name, menu.internal_c_obj, description)
end

---@param id string
---@param name string
---@param description string
function EWS_Menu:append_radio_item(id, name, description)
  EWS_Menu_AppendRadioItem(self.internal_c_obj, id, name, description)
end

---@param id string
---@param name string
---@param description string
function EWS_Menu:append_check_item(id, name, description)
  EWS_Menu_AppendCheckItem(self.internal_c_obj, id, name, description)
end

---@param item_id string id of the menu item
---@param checked boolean
function EWS_Menu:set_checked(item_id, checked)
  EWS_Menu_SetChecked(self.internal_c_obj, item_id, checked)
end

---@param item_id string
function EWS_Menu:is_checked(item_id)
  return EWS_Menu_IsChecked(self.internal_c_obj, item_id)
end

function EWS_Menu:clear()
  EWS_Menu_Clear(self.internal_c_obj)
end

----- menubar.lua
--- MenuBar

---@class EWS_MenuBar
EWS_MenuBar = EWS_MenuBar or class(EWS_Window)

function EWS_MenuBar:init()
  self.internal_c_obj = EWS_New_MenuBar()

  EWS_BaseComponent.init(self)
end

---@param menu EWS_Menu
---@param name string
function EWS_MenuBar:append(menu, name)
  EWS_MenuBar_Append(self.internal_c_obj, menu.internal_c_obj, name)
end

----- messagedialog.lua
--- MessageDialog

---@class EWS_MessageDialog
EWS_MessageDialog = EWS_MessageDialog or class(EWS_Dialog)

function EWS_MessageDialog:init(parent, message, caption, style)
  self.parent_obj = parent
  self.message = message
  self.caption = caption
  self.style = style

  self.internal_c_obj = EWS_New_MessageDialog(parent.internal_c_obj, message, caption, style)

  EWS_BaseComponent.init(self)
end

----- notebook.lua
--- Notebook

---@class EWS_Notebook: EWS_Window
EWS_Notebook = EWS_Notebook or class(EWS_Window)

function EWS_Notebook:init(parent, name, style)
  self.parent_obj = parent
  self.name = name
  self.style = style

  self.pages = {}

  self.internal_c_obj = EWS_New_Notebook(parent.internal_c_obj, name, style)

  EWS_BaseComponent.init(self)
end

---@param panel EWS_Window should usually be a EWS_Panel
---@param name string
---@param select boolean also called start_page in coreeditor.lua
function EWS_Notebook:add_page(panel, name, select)
  table.insert(self.pages, panel)
  EWS_Notebook_Add_Page(self.internal_c_obj, panel.internal_c_obj, name, select)
end

---@param index number 0 based index into pages
function EWS_Notebook:get_page(index)
  if type(index) == "nil" then
    return nil
  end
  if index < #self.pages then
    return self.pages[index + 1] -- avoid calling into C
  end
  return nil
end
function EWS_Notebook:get_current_page()
  local idx = EWS_Notebook_GetCurrentPage(self.internal_c_obj) -- idx is already adjusted for lua table indexing
  return self.pages[idx]
end

function EWS_Notebook:get_page_count()
  return EWS_Notebook_GetPageCount(self.internal_c_obj)
end
function EWS_Notebook:set_page_text(index, text)
  return EWS_Notebook_SetPageText(self.internal_c_obj, index, text)
end
function EWS_Notebook:get_page_text(index)
  return EWS_Notebook_GetPageText(self.internal_c_obj, index)
end
function EWS_Notebook:fit()
  EWS_Notebook_Fit(self.internal_c_obj)
end
function EWS_Notebook:set_page(index)
  EWS_Notebook_SetPage(self.internal_c_obj, index)
end

----- panel.lua
--- Panel

---@class EWS_Panel: EWS_Window
EWS_Panel = EWS_Panel or class(EWS_Window)

---@param parent EWS_Window
---@param id string
---@param style string
function EWS_Panel:init(parent, id, style)
  self.parent_obj = parent
  self.id = id
  self.style = style

  local p = nil
  if parent ~= nil then
    p = parent.internal_c_obj
  end

  self.internal_c_obj = EWS_New_Panel(p, id, style)

  EWS_BaseComponent.init(self)
end

----- sizer.lua
--- BoxSizer

---@class EWS_Sizer
EWS_Sizer = class(EWS_BaseComponent)

function EWS_Sizer:init()
  self.is_sizer = true
end

-- proportion and border could be wrong
---@param to_add any
---@param proportion number
---@param border number
---@param flags string
function EWS_Sizer:add(to_add, proportion, border, flags)
  if to_add == nil then
    return
  end
  if to_add.is_sizer then
    EWS_Sizer_Add_SizerObj(self.internal_c_obj, to_add.internal_c_obj, proportion, border, flags)
  else
    EWS_Sizer_Add_WindowObj(self.internal_c_obj, to_add.internal_c_obj, proportion, border, flags)
  end
end

function EWS_Sizer:add_spacer(unk1, unk2, unk3, unk4)
  EWS_Log("[EWS_Sizer:add_spacer] call to unimplemented function")
end
function EWS_Sizer:add_stretch_spacer(prop)
  EWS_Sizer_AddStretchSpacer(self.internal_c_obj, prop)
end

---@class EWS_BoxSizer:EWS_Sizer
EWS_BoxSizer = EWS_BoxSizer or class(EWS_Sizer)

function EWS_BoxSizer:init(orient)
  self.is_sizer = true
  self.orient = orient

  self.internal_c_obj = EWS_New_BoxSizer(orient)

  EWS_BaseComponent.init(self)
end

---@class EWS_StaticBoxSizer: EWS_BoxSizer
EWS_StaticBoxSizer = EWS_StaticBoxSizer or class(EWS_BoxSizer)

function EWS_StaticBoxSizer:init(win, orient, label)
  self.is_sizer = true
  self.orient = orient

  if type(label) == "nil" then
    label = ""
  end

  self.internal_c_obj = EWS_New_StaticBoxSizer(win.internal_c_obj, orient, label)

  EWS_BaseComponent.init(self)
end

----- splitterwindow.lua
--- SplitterWindow

---@class EWS_SplitterWindow: EWS_Window
EWS_SplitterWindow = EWS_SplitterWindow or class(EWS_Window)

function EWS_SplitterWindow:init(parent, name, style)
  self.parent_obj = parent
  self.name = name or ""
  self.style = style or ""

  self.internal_c_obj = EWS_New_SplitterWindow(parent.internal_c_obj, self.name, self.style)

  EWS_BaseComponent.init(self)
end

---@param window1 EWS_Window
---@param window2 EWS_Window
---@param sashposition number
function EWS_SplitterWindow:split_horizontally(window1, window2, sashposition)
  EWS_SplitterWindow_Split_Horizontally(self.internal_c_obj, window1.internal_c_obj, window2.internal_c_obj, sashposition)
end

---@param window1 EWS_Window
---@param window2 EWS_Window
---@param sashposition number
function EWS_SplitterWindow:split_vertically(window1, window2, sashposition)
  EWS_SplitterWindow_Split_Vertically(self.internal_c_obj, window1.internal_c_obj, window2.internal_c_obj, sashposition)
end

function EWS_SplitterWindow:set_minimum_pane_size(size)
  EWS_SplitterWindow_SetMinimumPaneSize(self.internal_c_obj, size)
end

----- statictext.lua
--- StaticText

---@class EWS_StaticText: EWS_Window
EWS_StaticText = EWS_StaticText or class(EWS_Window)

function EWS_StaticText:init(parent, text, unknown, style)
  self.parent_obj = parent
  self.text = text
  self.unknown = unknown
  self.style = style

  self.internal_c_obj = EWS_New_StaticText(parent.internal_c_obj, text, unknown, style)

  EWS_BaseComponent.init(self)
end

function EWS_StaticText:set_value(text)
  EWS_StaticText_SetValue(self.internal_c_obj, text)
end

function EWS_StaticText:set_label(label)
  EWS_StaticText_SetLabel(self.internal_c_obj, label)
end

----- toolbar.lua
--- ToolBar

---@class EWS_ToolBar: EWS_Window
EWS_ToolBar = EWS_ToolBar or class(EWS_Window)

---@param id string
---@param name string
---@param image_path string
---@param description string
function EWS_ToolBar:add_tool(id, name, image_path, description)
  if description == nil then
    description = ""
  end
  EWS_ToolBar_AddTool(self.internal_c_obj, id, name, image_path, description)
end

function EWS_ToolBar:add_check_tool(id, name, image_path, description)
  if description == nil then
    description = ""
  end
  EWS_ToolBar_AddCheckTool(self.internal_c_obj, id, name, image_path, description)
end

function EWS_ToolBar:add_radio_tool(id, name, icon_path, description)
  if description == nil then
    description = ""
  end
  EWS_ToolBar_AddRadioTool(self.internal_c_obj, id, name, icon_path, description)
end

function EWS_ToolBar:add_separator()
  EWS_ToolBar_AddSeparator(self.internal_c_obj)
end

---@param control EWS_Control
function EWS_ToolBar:add_control(control)
  EWS_ToolBar_AddControl(self.internal_c_obj, control.internal_c_obj)
end

---Gets the state of a check tool
function EWS_ToolBar:tool_state(id)
  return EWS_ToolBar_GetToolState(self.internal_c_obj, id)
end

---Sets the state of a check tool
function EWS_ToolBar:set_tool_state(id, state)
  EWS_ToolBar_SetToolState(self.internal_c_obj, id, state)
end

function EWS_ToolBar:set_tool_enabled(id, enabled)
end

function EWS_ToolBar:init(parent, name, style)
  self.parent_obj = parent
  self.name = name
  self.style = style

  self.internal_c_obj = EWS_New_ToolBar(parent.internal_c_obj, name, style)

  EWS_BaseComponent.init(self)
end

function EWS_ToolBar:realize()
  EWS_ToolBar_Realize(self.internal_c_obj)
end

--- TextCtrl

---@class EWS_Control: EWS_Window
EWS_Control = EWS_Control or class(EWS_Window)

---@class EWS_TextCtrl: EWS_Control
EWS_TextCtrl = EWS_TextCtrl or class(EWS_Control)

function EWS_TextCtrl:init(parent, text, unknown, style)
  self.parent_obj = parent
  
  if type(style) == "nil" then
    style = ""
  end

  self.internal_c_obj = EWS_New_TextCtrl(parent.internal_c_obj, text, style)

  EWS_BaseComponent.init(self)
end

--- Begin TextEntry funcs, maintain parity with ComboBox
function EWS_TextCtrl:get_last_position()
  return EWS_TextCtrl_GetLastPosition(self.internal_c_obj)
end

function EWS_TextCtrl:get_value()
  return EWS_TextCtrl_GetValue(self.internal_c_obj)
end

function EWS_TextCtrl:set_value(value)
  EWS_TextCtrl_SetValue(self.internal_c_obj, value)
end

function EWS_TextCtrl:append(append_text)
  EWS_TextCtrl_AppendText(self.internal_c_obj, append_text)
end

function EWS_TextCtrl:clear()
  EWS_TextCtrl_Clear(self.internal_c_obj)
end
--- End TextEntry

--- what?
function EWS_TextCtrl:change_value(value)
  self:set_value(tostring(value))
end

function EWS_TextCtrl:show_position(position)
  EWS_TextCtrl_ShowPosition(self.internal_c_obj, position)
end
function EWS_TextCtrl:set_default_style_colour(colour)
end
function EWS_TextCtrl:set_default_style_font_weight(font_weight)
end

--- ComboBox

---@class EWS_ComboBox: EWS_Control
EWS_ComboBox = EWS_ComboBox or class(EWS_Control)

function EWS_ComboBox:init(parent, unk1, unk2, style)
  self.parent_obj = parent

  self.internal_c_obj = EWS_New_ComboBox(parent.internal_c_obj, unk1, unk2, style)

  EWS_BaseComponent.init(self)
end

--- Begin TextEntry funcs, maintain parity with TextCtrl
function EWS_ComboBox:get_last_position()
  return EWS_ComboBox_GetLastPosition(self.internal_c_obj)
end

function EWS_ComboBox:get_value()
  return EWS_ComboBox_GetValue(self.internal_c_obj)
end

function EWS_ComboBox:set_value(value)
  EWS_ComboBox_SetValue(self.internal_c_obj, value)
end

function EWS_ComboBox:append(append_text)
  EWS_ComboBox_Append(self.internal_c_obj, append_text)
end

function EWS_ComboBox:clear()
  EWS_ComboBox_Clear(self.internal_c_obj)
end
--- End TextEntry

---- Buttons

---@class EWS_AnyButton: EWS_Control
EWS_AnyButton = EWS_AnyButton or class(EWS_Control)

---@class EWS_Button: EWS_AnyButton
EWS_Button = EWS_Button or class(EWS_AnyButton)

function EWS_Button:init(parent, label, unknown, style)
  self.parent_obj = parent

  if type(style) == "nil" then
    style = ""
  end

  self.internal_c_obj = EWS_New_Button(parent.internal_c_obj, label, style)

  EWS_BaseComponent.init(self)
end

---@class EWS_ToggleButton: EWS_AnyButton
EWS_ToggleButton = EWS_ToggleButton or class(EWS_AnyButton)

function EWS_ToggleButton:init(parent, label, unknown, style)
  self.parent_obj = parent

  self.internal_c_obj = EWS_New_ToggleButton(parent.internal_c_obj, label, style)

  EWS_BaseComponent.init(self)
end

function EWS_ToggleButton:set_value(value)
  EWS_ToggleButton_SetValue(self.internal_c_obj, value)
end

function EWS_ToggleButton:get_value()
  return EWS_ToggleButton_GetValue(self.internal_c_obj)
end


---@class EWS_BitmapButton: EWS_Button
EWS_BitmapButton = EWS_BitmapButton or class(EWS_Button)

function EWS_BitmapButton:init(parent, image_path, unknown, style)
  self.parent_obj = parent

  self.internal_c_obj = EWS_New_BitmapButton(parent.internal_c_obj, image_path, style)

  EWS_BaseComponent.init(self)
end

--- CheckBox

---@class EWS_CheckBox: EWS_Control
EWS_CheckBox = EWS_CheckBox or class(EWS_Control)

function EWS_CheckBox:init(parent, name, unk1, style)
  self.parent_obj = parent

  if type(name) == "nil" then
    name = ""
  end
  if type(style) == "nil" then
    style = ""
  end

  self.internal_c_obj = EWS_New_CheckBox(parent.internal_c_obj, name, style)

  EWS_BaseComponent.init(self)
end

function EWS_CheckBox:set_value(value)
  EWS_CheckBox_SetValue(self.internal_c_obj, value)
end

function EWS_CheckBox:get_value(value)
  return EWS_CheckBox_GetValue(self.internal_c_obj)
end

--- SpinCtrl

---@class EWS_SpinCtrl: EWS_Control
EWS_SpinCtrl = EWS_SpinCtrl or class(EWS_Control)

function EWS_SpinCtrl:init(parent, value, unk, style)
  self.parent_obj = parent

  self.internal_c_obj = EWS_New_SpinCtrl(parent.internal_c_obj, style)

  EWS_BaseComponent.init(self)

  self:set_value(value)
end

---@param value string|number
function EWS_SpinCtrl:set_value(value)
  EWS_SpinCtrl_SetValue(self.internal_c_obj, value)
end

function EWS_SpinCtrl:get_value()
  return EWS_SpinCtrl_GetValue(self.internal_c_obj)
end

---@param min number
---@param max number
function EWS_SpinCtrl:set_range(min, max)
  EWS_SpinCtrl_SetRange(self.internal_c_obj, min, max)
end

--- TreeCtrl

---@class EWS_TreeCtrl: EWS_Control
EWS_TreeCtrl = EWS_TreeCtrl or class(EWS_Control)

function EWS_TreeCtrl:init(parent, id, style)
  self.parent_obj = parent

  self.internal_c_obj = EWS_New_TreeCtrl(parent.internal_c_obj, id, style)

  EWS_BaseComponent.init(self)
end

--- SpinButton

---@class EWS_SpinButton: EWS_Control
EWS_SpinButton = EWS_SpinButton or class(EWS_Control)

function EWS_SpinButton:init(parent, id, style)
  self.parent_obj = parent

  self.internal_c_obj = EWS_New_SpinButton(parent.internal_c_obj, id, style)

  EWS_BaseComponent.init(self)
end

--- ScrolledWindow

---@class EWS_ScrolledWindow: EWS_Panel
EWS_ScrolledWindow = EWS_ScrolledWindow or class(EWS_Panel)

function EWS_ScrolledWindow:init(parent, name, style)
  self.parent_obj = parent

  if type(style) == "nil" then
    style = ""
  end

  self.internal_c_obj = EWS_New_ScrolledWindow(parent.internal_c_obj, name, style)

  EWS_BaseComponent.init(self)
end

---@param scroll_rate Vector3
function EWS_ScrolledWindow:set_scroll_rate(scroll_rate)
  EWS_ScrolledWindow_SetScrollRate(self.internal_c_obj, scroll_rate)
end

--- FileDialog

---@class EWS_FileDialog: EWS_Dialog
EWS_FileDialog = EWS_FileDialog or class(EWS_Dialog)

function EWS_FileDialog:init(parent, title, start_path, default_file, type_selection_restrictions, style)

  self.parent_obj = parent

  self.internal_c_obj = EWS_New_FileDialog(parent.internal_c_obj, title, start_path, default_file, type_selection_restrictions, style)

  EWS_BaseComponent.init(self)
end

function EWS_FileDialog:get_path()
  return EWS_FileDialog_GetPath(self.internal_c_obj)
end
function EWS_FileDialog:get_directory()
  return EWS_FileDialog_GetDirectory(self.internal_c_obj)
end

--- StatusBar

---@class EWS_StatusBar
EWS_StatusBar = EWS_StatusBar or class(EWS_Control)

---@param parent EWS_Window
function EWS_StatusBar:init(parent, unk1, unk2)
  self.parent_obj = parent

  self.internal_c_obj = EWS_New_StatusBar(parent.internal_c_obj, unk1, unk2)

  EWS_BaseComponent.init(self)
end

function EWS_StatusBar:set_status_text(text)
  EWS_StatusBar_SetStatusText(self.internal_c_obj, text)
end

--- ProgressDialog

---@class EWS_ProgressDialog
EWS_ProgressDialog = EWS_ProgressDialog or class(EWS_Dialog)

function EWS_ProgressDialog:destroy() end

function EWS_ProgressDialog:init(parent, title, message, maximum, style)
  self.parent_obj = parent

  EWS_Log("[EWS_ProgressDialog:init] parent: " .. tostring(parent))
  EWS_Log("[EWS_ProgressDialog:init] title: " .. tostring(title))
  EWS_Log("[EWS_ProgressDialog:init] message: " .. tostring(message))
  EWS_Log("[EWS_ProgressDialog:init] maximum: " .. tostring(maximum))
  EWS_Log("[EWS_ProgressDialog:init] style: " .. tostring(style))

  --self.internal_c_obj = EWS_New_ProgressDialog(parent.internal_c_obj, title, message, maximum, style)

  EWS_Log("[EWS_ProgressDialog:init] internal_c_obj: " .. tostring(self.internal_c_obj))

  EWS_BaseComponent.init(self)
end

---@param value number
---@param new_message string optional
function EWS_ProgressDialog:update_bar(value, new_message)
  --EWS_ProgressDialog_UpdateBar(self.internal_c_obj, value, new_message)
end

--- RadioButton

---@class EWS_RadioButon: EWS_Control
EWS_RadioButton = EWS_RadioButton or class(EWS_Control)

function EWS_RadioButton:init(parent, name, group, style)
  self.parent_obj = parent

  self.internal_c_obj = EWS_New_RadioButton(parent.internal_c_obj, name, group, style)

  EWS_BaseComponent.init(self)
end

function EWS_RadioButton:set_value()
  
end
function EWS_RadioButton:get_value()
  return false
end

------ MOD DEVELOPERS LOOK HERE


if EWS_clazz == nil or EWS == nil then
  ---@class EWS_clazz
  EWS_clazz = EWS_class or class()
  function EWS_clazz:init()
  end


  --- Funcs for Diesel

  ---@param title string Title for the frame
  ---@param start_position Vector3 Start pos for the frame
  ---@param size Vector3 Size of the frame
  ---@param style string comma (`,`) separated list of the wxWidgets styles
  ---@param parent EWS_Frame can be not supplied
  ---@return EWS_Frame
  function EWS_clazz:Frame(title, start_position, size, style, parent)
    return EWS_Frame:new(title, start_position, size, style, parent or nil)
  end

  ---@param orient string "HORIZONTAL" or "VERTICAL"
  ---@return EWS_BoxSizer
  function EWS_clazz:BoxSizer(orient)
    return EWS_BoxSizer:new(orient)
  end

  ---@return EWS_Dialog
  function EWS_clazz:Dialog(...)
    return EWS_Dialog:new(...)
  end

  ---@return EWS_MessageDialog
  function EWS_clazz:MessageDialog(parent, message, caption, style)
    return EWS_MessageDialog:new(parent, message, caption, style)
  end

  ---@return EWS_MenuBar
  function EWS_clazz:MenuBar()
    return EWS_MenuBar:new()
  end

  ---@return EWS_Menu
  function EWS_clazz:Menu(style)
    return EWS_Menu:new(style)
  end

  ---@return EWS_Panel
  function EWS_clazz:Panel(parent, id, style)
    return EWS_Panel:new(parent, id, style)
  end

  ---@return EWS_ToolBar
  function EWS_clazz:ToolBar(parent, id, style)
    return EWS_ToolBar:new(parent, id, style)
  end

  ---@return EWS_Notebook
  ---@param parent EWS_Window
  ---@param id string
  ---@param style string
  function EWS_clazz:Notebook(parent, id, style)
    return EWS_Notebook:new(parent, id, style)
  end

  ---@return EWS_SplitterWindow
  ---@param parent EWS_Window
  ---@param id string
  ---@param style string
  function EWS_clazz:SplitterWindow(parent, id, style)
    return EWS_SplitterWindow:new(parent, id, style)
  end

  ---@return EWS_StaticText
  ---@param parent EWS_Window
  ---@param text string
  ---@param unknown number
  ---@param style string
  function EWS_clazz:StaticText(parent, text, unknown, style)
    return EWS_StaticText:new(parent, text, unknown, style)
  end

  ---@return EWS_AppWindow
  ---@param parent EWS_Window
  ---@param size Vector3
  ---@param style string Comma separated list of styles
  function EWS_clazz:AppWindow(parent, size, style)
    return EWS_AppWindow:new(parent, size, style)
  end

  ---@return EWS_TextCtrl
  function EWS_clazz:TextCtrl(parent, text, unknown, style)
    return EWS_TextCtrl:new(parent, text, unknown, style)
  end

  ---@return EWS_Button
  function EWS_clazz:Button(parent, label, unknown, style)
    return EWS_Button:new(parent, label, unknown, style)
  end

  ---@return EWS_ScrolledWindow
  function EWS_clazz:ScrolledWindow(parent, name, style)
    return EWS_ScrolledWindow:new(parent, name, style)
  end

  ---@return EWS_StatusBar
  function EWS_clazz:StatusBar(parent, unknown, unknown2)
    return EWS_StatusBar:new(parent, unknown, unknown2)
  end

  ---@return EWS_ProgressDialog
  function EWS_clazz:ProgressDialog(parent, title, message, maximum, style)
    return EWS_ProgressDialog:new(parent, title, message, maximum, style)
  end

  ---@return EWS_ComboBox
  function EWS_clazz:ComboBox(parent, unk1, unk2, style)
    return EWS_ComboBox:new(parent, unk1, unk2, style)
  end

  ---@return EWS_CheckBox
  function EWS_clazz:CheckBox(parent, name, unk1, style)
    return EWS_CheckBox:new(parent, name, unk1, style)
  end

  ---@return EWS_StaticBoxSizer
  function EWS_clazz:StaticBoxSizer(win, orient, title)
    return EWS_StaticBoxSizer:new(win, orient, title)
  end

  ---@return EWS_SpinCtrl
  function EWS_clazz:SpinCtrl(parent, value, unk, style)
    return EWS_SpinCtrl:new(parent, value, unk, style)
  end

  ---@return EWS_ToggleButton
  ---Used only in the continents panel
  function EWS_clazz:ToggleButton(parent, label, unknown, style)
    return EWS_ToggleButton:new(parent, label, unknown, style)
  end

  ---@return EWS_FileDialog
  function EWS_clazz:FileDialog(parent, title, start_path, default_file, type_selection_restrictions, style)
    return EWS_FileDialog:new(parent, title, start_path, default_file, type_selection_restrictions, style)
  end

  ---@return EWS_BitmapButton
  function EWS_clazz:BitmapButton(parent, image_path, unknown, style)
    return EWS_BitmapButton:new(parent, image_path, unknown, style)
  end
  
  ---@return EWS_SpinButton
  function EWS_clazz:SpinButton(parent, id, style)
    return EWS_SpinButton:new(parent, id, style)
  end

  ---@return EWS_TreeCtrl
  function EWS_clazz:TreeCtrl(parent, id, style)
    return EWS_TreeCtrl:new(parent, id, style)
  end

  ---@return EWS_RadioButon
  function EWS_clazz:RadioButton(parent, name, group, style)
    return EWS_RadioButton:new(parent, name, group, style)
  end
  function EWS_clazz:ListBox(parent, name, style)
    local fake = EWS:Panel(parent, name, "")
    fake.clear = function(self) end
    fake.append = function(self, name) end
    fake.select_index = function(self, index) end
    fake.nr_items = function(self) return 0 end

    fake.selected_index = function(self) return -1 end
    fake.deselect_index = function(self, index) end
    fake.get_string = function(self, index) return "" end
    --EWS_Log("EWS:ListBox: " .. name)
    return fake
  end
  function EWS_clazz:ListCtrl(parent, name, style)
    local fake = EWS:Panel(parent, name, "")
    fake.clear_all = function(self) end
    fake.append_column = function(self, column_name) end
    fake.set_item_data = function(self, index, name) end
    fake.autosize_column = function(self, unk) end
    fake.delete_all_items = function(self) end
    --EWS_Log("EWS_ListCtrl: " .. name)
    return fake
  end
  function EWS_clazz:Slider(parent, value, minimum_value, maximum_value, id, style_maybe)
    local fake = EWS:Panel(parent, "fakename", "")
    fake.set_value = function(self, value) end
    --EWS_Log("EWS:Slider")
    return fake
  end

function EWS_clazz:message_box(parent, msg, title_maybe, style, position)
  local messagebox = EWS:MessageDialog(parent, msg, title_maybe, style)
  messagebox:show_modal()
end

-- Non UI functions
function EWS_clazz:system_file_exists(path)
  return EWS_Lua_SystemFileExists(path)
end

-- Mondo monkey
function EWS_clazz:launch_url(url)
  EWS_Log("[EWS:launch_url] Launching URL: " .. url)
  EWS_Lua_LaunchURL(url)
end

function EWS_clazz:name_to_key_code(...)
  return 0
end

---@param parent EWS_Window
---@param message string
---@param caption string
---@param default_value string
---@param start_pos Vector3
---@param centre boolean
function EWS_clazz:get_text_from_user(parent, message, caption, default_value, start_pos, centre)
  if type(parent) ~= "nil" then
    parent = parent.internal_c_obj
  end
  return EWS_Lua_GetTextFromUser(parent, message, caption, default_value, start_pos, centre)
end

---@type EWS_clazz
_G.EWS = _G.EWS or EWS_clazz:new()

--log(EWS_clazz:Frame("test.txt"))
--setmetatable(EWS, EWS)
end


local EWS_clazzz = __classes["EWS"]
function EWS_clazzz:ColorSpectrum(parent, ...)
  local fake = EWS:Panel(parent, "COLORSPECTRUM_FAKE_PANEL", "")
  return fake
end
function EWS_clazzz:ColorSlider(parent, ...)
  local fake = EWS:Panel(parent, "COLORSPECTRUM_FAKE_PANEL", "")
  return fake
end
function EWS_clazzz:StaticLine(parent, name, style)
  local fake = EWS:StaticText(parent, "", 0, style)
  return fake
end
