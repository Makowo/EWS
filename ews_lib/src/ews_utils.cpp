#include "ews_utils.h"

#include <wx/wx.h>

#include "wxwidgets_includes.h"

#include <sstream>

#include <filesystem>


int do_nothing(lua_State* L) { return 0; }

enum
{
  wxRICHTEXT_FIELD_STYLE_COMPOSITE = 0x01,
  wxRICHTEXT_FIELD_STYLE_RECTANGLE = 0x02,
  wxRICHTEXT_FIELD_STYLE_NO_BORDER = 0x04,
  wxRICHTEXT_FIELD_STYLE_START_TAG = 0x08,
  wxRICHTEXT_FIELD_STYLE_END_TAG = 0x10
};

Vector3 *get_vec3_from_arg(lua_State *L, int top)
{
  Vector3 *vec = (Vector3 *)lua_touserdata(L, top);
  return vec;
}

const wxColour* get_wxcolour_from_name(std::string name) {
  if(name == "BLACK")
    return wxBLACK;
  if (name == "BLUE")
    return wxBLUE;
  if (name == "CYAN")
    return wxCYAN;
  if (name == "GREEN")
    return wxGREEN;
  if (name == "YELLOW")
    return wxYELLOW;
  if (name == "LIGHT GREY")
    return wxLIGHT_GREY;
  if (name == "RED")
    return wxRED;
  if (name == "WHITE")
    return wxWHITE;

  static wxColour TransparentColour = wxColour(0, 0, 0, wxALPHA_TRANSPARENT);
  // Diesel likely doesn't have this colour, here so mod creators have the option
  if (name == "TransparentColour") return &TransparentColour;

  return wxYELLOW;
}

std::string resolve_icon_path(std::string path) {
  if (std::filesystem::exists(path))
    return path;

  return "./assets/core/lib/utils/dev/ews/images/error_16x16.png";
}

wxImage get_wximage_from_path(std::string path) {
  DISABLE_IMAGE_LOAD_WARNING
  wxImage img;
  if (path.ends_with(".png"))
    img.LoadFile(wxString(path), wxBITMAP_TYPE_PNG);
  if (path.ends_with(".bmp"))
    img.LoadFile(wxString(path), wxBITMAP_TYPE_BMP);

  return img;
}

int get_style_from_merged_list(const std::string& merged_list)
{
  std::vector<std::string> list;

  std::stringstream sstream(merged_list);

  std::string s;
  while(getline(sstream, s, ',')) {
    list.push_back(s);
  }

  return get_style_from_list(list);
}

static const char* replace_deprecated_events(const char* name_cstr) {
  if (!strcmp(name_cstr, "EVT_COMMAND_THREAD")) {
    return "EVT_THREAD";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_BUTTON_CLICKED")) {
    return "EVT_BUTTON";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_CHECKBOX_CLICKED")) {
    return "EVT_CHECKBOX";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_CHOICE_SELECTED")) {
    return "EVT_CHOICE";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_LISTBOX_SELECTED")) {
    return "EVT_LISTBOX";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_LISTBOX_DOUBLECLICKED")) {
    return "EVT_LISTBOX_DCLICK";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_CHECKLISTBOX_TOGGLED")) {
    return "EVT_CHECKLISTBOX";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_MENU_SELECTED")) {
    return "EVT_MENU";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_TOOL_CLICKED")) {
    return "EVT_TOOL";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_SLIDER_UPDATED")) {
    return "EVT_SLIDER";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_RADIOBOX_SELECTED")) {
    return "EVT_RADIOBOX";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_RADIOBUTTON_SELECTED")) {
    return "EVT_RADIOBUTTON";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_SCROLLBAR_UPDATED")) {
    return "EVT_SCROLLBAR";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_VLBOX_SELECTED")) {
    return "EVT_VLBOX";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_COMBOBOX_SELECTED")) {
    return "EVT_COMBOBOX";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_TOOL_RCLICKED")) {
    return "EVT_TOOL_RCLICKED";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_TOOL_DROPDOWN_CLICKED")) {
    return "EVT_TOOL_DROPDOWN";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_TOOL_ENTER")) {
    return "EVT_TOOL_ENTER";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_COMBOBOX_DROPDOWN")) {
    return "EVT_COMBOBOX_DROPDOWN";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_COMBOBOX_CLOSEUP")) {
    return "EVT_COMBOBOX_CLOSEUP";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_TEXT_COPY")) {
    return "EVT_TEXT_COPY";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_TEXT_CUT")) {
    return "EVT_TEXT_CUT";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_TEXT_PASTE")) {
    return "EVT_TEXT_PASTE";
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_TEXT_UPDATED")) {
    return "EVT_TEXT";
  }
  return name_cstr;
}

static int get_event_by_name(const char* name) {
  const char* name_cstr = replace_deprecated_events(name);
  if (!strcmp(name_cstr, "EVT_CALENDAR_SEL_CHANGED"))
  {
    return wxEVT_CALENDAR_SEL_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_CALENDAR_PAGE_CHANGED"))
  {
    return wxEVT_CALENDAR_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_CALENDAR_DOUBLECLICKED"))
  {
    return wxEVT_CALENDAR_DOUBLECLICKED;
  }
  if (!strcmp(name_cstr, "EVT_CALENDAR_WEEKDAY_CLICKED"))
  {
    return wxEVT_CALENDAR_WEEKDAY_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_CALENDAR_WEEK_CLICKED"))
  {
    return wxEVT_CALENDAR_WEEK_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_CHOICEBOOK_PAGE_CHANGED"))
  {
    return wxEVT_CHOICEBOOK_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_CHOICEBOOK_PAGE_CHANGING"))
  {
    return wxEVT_CHOICEBOOK_PAGE_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_COLOURPICKER_CHANGED"))
  {
    return wxEVT_COLOURPICKER_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_COLOURPICKER_CURRENT_CHANGED"))
  {
    return wxEVT_COLOURPICKER_CURRENT_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_COLOURPICKER_DIALOG_CANCELLED"))
  {
    return wxEVT_COLOURPICKER_DIALOG_CANCELLED;
  }
  if (!strcmp(name_cstr, "EVT_COLLAPSIBLEPANE_CHANGED"))
  {
    return wxEVT_COLLAPSIBLEPANE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_COLOUR_CHANGED"))
  {
    return wxEVT_COLOUR_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_SELECTION_CHANGED"))
  {
    return wxEVT_DATAVIEW_SELECTION_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_ACTIVATED"))
  {
    return wxEVT_DATAVIEW_ITEM_ACTIVATED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_COLLAPSING"))
  {
    return wxEVT_DATAVIEW_ITEM_COLLAPSING;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_COLLAPSED"))
  {
    return wxEVT_DATAVIEW_ITEM_COLLAPSED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_EXPANDING"))
  {
    return wxEVT_DATAVIEW_ITEM_EXPANDING;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_EXPANDED"))
  {
    return wxEVT_DATAVIEW_ITEM_EXPANDED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_START_EDITING"))
  {
    return wxEVT_DATAVIEW_ITEM_START_EDITING;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_EDITING_STARTED"))
  {
    return wxEVT_DATAVIEW_ITEM_EDITING_STARTED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_EDITING_DONE"))
  {
    return wxEVT_DATAVIEW_ITEM_EDITING_DONE;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_VALUE_CHANGED"))
  {
    return wxEVT_DATAVIEW_ITEM_VALUE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_CONTEXT_MENU"))
  {
    return wxEVT_DATAVIEW_ITEM_CONTEXT_MENU;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_COLUMN_HEADER_CLICK"))
  {
    return wxEVT_DATAVIEW_COLUMN_HEADER_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_COLUMN_HEADER_RIGHT_CLICK"))
  {
    return wxEVT_DATAVIEW_COLUMN_HEADER_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_COLUMN_SORTED"))
  {
    return wxEVT_DATAVIEW_COLUMN_SORTED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_COLUMN_REORDERED"))
  {
    return wxEVT_DATAVIEW_COLUMN_REORDERED;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_CACHE_HINT"))
  {
    return wxEVT_DATAVIEW_CACHE_HINT;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_BEGIN_DRAG"))
  {
    return wxEVT_DATAVIEW_ITEM_BEGIN_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_DROP_POSSIBLE"))
  {
    return wxEVT_DATAVIEW_ITEM_DROP_POSSIBLE;
  }
  if (!strcmp(name_cstr, "EVT_DATAVIEW_ITEM_DROP"))
  {
    return wxEVT_DATAVIEW_ITEM_DROP;
  }
  if (!strcmp(name_cstr, "EVT_DATE_CHANGED"))
  {
    return wxEVT_DATE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_TIME_CHANGED"))
  {
    return wxEVT_TIME_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_DIRCTRL_SELECTIONCHANGED"))
  {
    return wxEVT_DIRCTRL_SELECTIONCHANGED;
  }
  if (!strcmp(name_cstr, "EVT_DIRCTRL_FILEACTIVATED"))
  {
    return wxEVT_DIRCTRL_FILEACTIVATED;
  }
  if (!strcmp(name_cstr, "EVT_NULL"))
  {
    return wxEVT_NULL;
  }
  if (!strcmp(name_cstr, "EVT_ANY"))
  {
    return wxEVT_ANY;
  }
  if (!strcmp(name_cstr, "EVT_BUTTON"))
  {
    return wxEVT_BUTTON;
  }
  if (!strcmp(name_cstr, "EVT_CHECKBOX"))
  {
    return wxEVT_CHECKBOX;
  }
  if (!strcmp(name_cstr, "EVT_CHOICE"))
  {
    return wxEVT_CHOICE;
  }
  if (!strcmp(name_cstr, "EVT_LISTBOX"))
  {
    return wxEVT_LISTBOX;
  }
  if (!strcmp(name_cstr, "EVT_LISTBOX_DCLICK"))
  {
    return wxEVT_LISTBOX_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_CHECKLISTBOX"))
  {
    return wxEVT_CHECKLISTBOX;
  }
  if (!strcmp(name_cstr, "EVT_MENU"))
  {
    return wxEVT_MENU;
  }
  if (!strcmp(name_cstr, "EVT_SLIDER"))
  {
    return wxEVT_SLIDER;
  }
  if (!strcmp(name_cstr, "EVT_RADIOBOX"))
  {
    return wxEVT_RADIOBOX;
  }
  if (!strcmp(name_cstr, "EVT_RADIOBUTTON"))
  {
    return wxEVT_RADIOBUTTON;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLBAR"))
  {
    return wxEVT_SCROLLBAR;
  }
  if (!strcmp(name_cstr, "EVT_VLBOX"))
  {
    return wxEVT_VLBOX;
  }
  if (!strcmp(name_cstr, "EVT_COMBOBOX"))
  {
    return wxEVT_COMBOBOX;
  }
  if (!strcmp(name_cstr, "EVT_TOOL_RCLICKED"))
  {
    return wxEVT_TOOL_RCLICKED;
  }
  if (!strcmp(name_cstr, "EVT_TOOL_DROPDOWN"))
  {
    return wxEVT_TOOL_DROPDOWN;
  }
  if (!strcmp(name_cstr, "EVT_TOOL_ENTER"))
  {
    return wxEVT_TOOL_ENTER;
  }
  if (!strcmp(name_cstr, "EVT_COMBOBOX_DROPDOWN"))
  {
    return wxEVT_COMBOBOX_DROPDOWN;
  }
  if (!strcmp(name_cstr, "EVT_COMBOBOX_CLOSEUP"))
  {
    return wxEVT_COMBOBOX_CLOSEUP;
  }
  if (!strcmp(name_cstr, "EVT_THREAD"))
  {
    return wxEVT_THREAD;
  }
  if (!strcmp(name_cstr, "EVT_LEFT_DOWN"))
  {
    return wxEVT_LEFT_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_LEFT_UP"))
  {
    return wxEVT_LEFT_UP;
  }
  if (!strcmp(name_cstr, "EVT_MIDDLE_DOWN"))
  {
    return wxEVT_MIDDLE_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_MIDDLE_UP"))
  {
    return wxEVT_MIDDLE_UP;
  }
  if (!strcmp(name_cstr, "EVT_RIGHT_DOWN"))
  {
    return wxEVT_RIGHT_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_RIGHT_UP"))
  {
    return wxEVT_RIGHT_UP;
  }
  if (!strcmp(name_cstr, "EVT_MOTION"))
  {
    return wxEVT_MOTION;
  }
  if (!strcmp(name_cstr, "EVT_ENTER_WINDOW"))
  {
    return wxEVT_ENTER_WINDOW;
  }
  if (!strcmp(name_cstr, "EVT_LEAVE_WINDOW"))
  {
    return wxEVT_LEAVE_WINDOW;
  }
  if (!strcmp(name_cstr, "EVT_LEFT_DCLICK"))
  {
    return wxEVT_LEFT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_MIDDLE_DCLICK"))
  {
    return wxEVT_MIDDLE_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_RIGHT_DCLICK"))
  {
    return wxEVT_RIGHT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_SET_FOCUS"))
  {
    return wxEVT_SET_FOCUS;
  }
  if (!strcmp(name_cstr, "EVT_KILL_FOCUS"))
  {
    return wxEVT_KILL_FOCUS;
  }
  if (!strcmp(name_cstr, "EVT_CHILD_FOCUS"))
  {
    return wxEVT_CHILD_FOCUS;
  }
  if (!strcmp(name_cstr, "EVT_MOUSEWHEEL"))
  {
    return wxEVT_MOUSEWHEEL;
  }
  if (!strcmp(name_cstr, "EVT_MAGNIFY"))
  {
    return wxEVT_MAGNIFY;
  }
  if (!strcmp(name_cstr, "EVT_AUX1_DOWN"))
  {
    return wxEVT_AUX1_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_AUX1_UP"))
  {
    return wxEVT_AUX1_UP;
  }
  if (!strcmp(name_cstr, "EVT_AUX1_DCLICK"))
  {
    return wxEVT_AUX1_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_AUX2_DOWN"))
  {
    return wxEVT_AUX2_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_AUX2_UP"))
  {
    return wxEVT_AUX2_UP;
  }
  if (!strcmp(name_cstr, "EVT_AUX2_DCLICK"))
  {
    return wxEVT_AUX2_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_CHAR"))
  {
    return wxEVT_CHAR;
  }
  if (!strcmp(name_cstr, "EVT_CHAR_HOOK"))
  {
    return wxEVT_CHAR_HOOK;
  }
  if (!strcmp(name_cstr, "EVT_NAVIGATION_KEY"))
  {
    return wxEVT_NAVIGATION_KEY;
  }
  if (!strcmp(name_cstr, "EVT_KEY_DOWN"))
  {
    return wxEVT_KEY_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_KEY_UP"))
  {
    return wxEVT_KEY_UP;
  }
  if (!strcmp(name_cstr, "EVT_HOTKEY"))
  {
    return wxEVT_HOTKEY;
  }
  if (!strcmp(name_cstr, "EVT_SET_CURSOR"))
  {
    return wxEVT_SET_CURSOR;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_TOP"))
  {
    return wxEVT_SCROLL_TOP;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_BOTTOM"))
  {
    return wxEVT_SCROLL_BOTTOM;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_LINEUP"))
  {
    return wxEVT_SCROLL_LINEUP;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_LINEDOWN"))
  {
    return wxEVT_SCROLL_LINEDOWN;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_PAGEUP"))
  {
    return wxEVT_SCROLL_PAGEUP;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_PAGEDOWN"))
  {
    return wxEVT_SCROLL_PAGEDOWN;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_THUMBTRACK"))
  {
    return wxEVT_SCROLL_THUMBTRACK;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_THUMBRELEASE"))
  {
    return wxEVT_SCROLL_THUMBRELEASE;
  }
  if (!strcmp(name_cstr, "EVT_SCROLL_CHANGED"))
  {
    return wxEVT_SCROLL_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_SPIN_UP"))
  {
    return wxEVT_SPIN_UP;
  }
  if (!strcmp(name_cstr, "EVT_SPIN_DOWN"))
  {
    return wxEVT_SPIN_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_SPIN"))
  {
    return wxEVT_SPIN;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLWIN_TOP"))
  {
    return wxEVT_SCROLLWIN_TOP;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLWIN_BOTTOM"))
  {
    return wxEVT_SCROLLWIN_BOTTOM;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLWIN_LINEUP"))
  {
    return wxEVT_SCROLLWIN_LINEUP;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLWIN_LINEDOWN"))
  {
    return wxEVT_SCROLLWIN_LINEDOWN;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLWIN_PAGEUP"))
  {
    return wxEVT_SCROLLWIN_PAGEUP;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLWIN_PAGEDOWN"))
  {
    return wxEVT_SCROLLWIN_PAGEDOWN;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLWIN_THUMBTRACK"))
  {
    return wxEVT_SCROLLWIN_THUMBTRACK;
  }
  if (!strcmp(name_cstr, "EVT_SCROLLWIN_THUMBRELEASE"))
  {
    return wxEVT_SCROLLWIN_THUMBRELEASE;
  }
  if (!strcmp(name_cstr, "EVT_GESTURE_PAN"))
  {
    return wxEVT_GESTURE_PAN;
  }
  if (!strcmp(name_cstr, "EVT_GESTURE_ZOOM"))
  {
    return wxEVT_GESTURE_ZOOM;
  }
  if (!strcmp(name_cstr, "EVT_GESTURE_ROTATE"))
  {
    return wxEVT_GESTURE_ROTATE;
  }
  if (!strcmp(name_cstr, "EVT_TWO_FINGER_TAP"))
  {
    return wxEVT_TWO_FINGER_TAP;
  }
  if (!strcmp(name_cstr, "EVT_LONG_PRESS"))
  {
    return wxEVT_LONG_PRESS;
  }
  if (!strcmp(name_cstr, "EVT_PRESS_AND_TAP"))
  {
    return wxEVT_PRESS_AND_TAP;
  }
  if (!strcmp(name_cstr, "EVT_SIZE"))
  {
    return wxEVT_SIZE;
  }
  if (!strcmp(name_cstr, "EVT_MOVE"))
  {
    return wxEVT_MOVE;
  }
  if (!strcmp(name_cstr, "EVT_CLOSE_WINDOW"))
  {
    return wxEVT_CLOSE_WINDOW;
  }
  if (!strcmp(name_cstr, "EVT_END_SESSION"))
  {
    return wxEVT_END_SESSION;
  }
  if (!strcmp(name_cstr, "EVT_QUERY_END_SESSION"))
  {
    return wxEVT_QUERY_END_SESSION;
  }
  if (!strcmp(name_cstr, "EVT_ACTIVATE_APP"))
  {
    return wxEVT_ACTIVATE_APP;
  }
  if (!strcmp(name_cstr, "EVT_ACTIVATE"))
  {
    return wxEVT_ACTIVATE;
  }
  if (!strcmp(name_cstr, "EVT_CREATE"))
  {
    return wxEVT_CREATE;
  }
  if (!strcmp(name_cstr, "EVT_DESTROY"))
  {
    return wxEVT_DESTROY;
  }
  if (!strcmp(name_cstr, "EVT_SHOW"))
  {
    return wxEVT_SHOW;
  }
  if (!strcmp(name_cstr, "EVT_ICONIZE"))
  {
    return wxEVT_ICONIZE;
  }
  if (!strcmp(name_cstr, "EVT_MAXIMIZE"))
  {
    return wxEVT_MAXIMIZE;
  }
  if (!strcmp(name_cstr, "EVT_FULLSCREEN"))
  {
    return wxEVT_FULLSCREEN;
  }
  if (!strcmp(name_cstr, "EVT_MOUSE_CAPTURE_CHANGED"))
  {
    return wxEVT_MOUSE_CAPTURE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_MOUSE_CAPTURE_LOST"))
  {
    return wxEVT_MOUSE_CAPTURE_LOST;
  }
  if (!strcmp(name_cstr, "EVT_PAINT"))
  {
    return wxEVT_PAINT;
  }
  if (!strcmp(name_cstr, "EVT_ERASE_BACKGROUND"))
  {
    return wxEVT_ERASE_BACKGROUND;
  }
  if (!strcmp(name_cstr, "EVT_NC_PAINT"))
  {
    return wxEVT_NC_PAINT;
  }
  if (!strcmp(name_cstr, "EVT_MENU_OPEN"))
  {
    return wxEVT_MENU_OPEN;
  }
  if (!strcmp(name_cstr, "EVT_MENU_CLOSE"))
  {
    return wxEVT_MENU_CLOSE;
  }
  if (!strcmp(name_cstr, "EVT_MENU_HIGHLIGHT"))
  {
    return wxEVT_MENU_HIGHLIGHT;
  }
  if (!strcmp(name_cstr, "EVT_CONTEXT_MENU"))
  {
    return wxEVT_CONTEXT_MENU;
  }
  if (!strcmp(name_cstr, "EVT_SYS_COLOUR_CHANGED"))
  {
    return wxEVT_SYS_COLOUR_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_DISPLAY_CHANGED"))
  {
    return wxEVT_DISPLAY_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_DPI_CHANGED"))
  {
    return wxEVT_DPI_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_QUERY_NEW_PALETTE"))
  {
    return wxEVT_QUERY_NEW_PALETTE;
  }
  if (!strcmp(name_cstr, "EVT_PALETTE_CHANGED"))
  {
    return wxEVT_PALETTE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_JOY_BUTTON_DOWN"))
  {
    return wxEVT_JOY_BUTTON_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_JOY_BUTTON_UP"))
  {
    return wxEVT_JOY_BUTTON_UP;
  }
  if (!strcmp(name_cstr, "EVT_JOY_MOVE"))
  {
    return wxEVT_JOY_MOVE;
  }
  if (!strcmp(name_cstr, "EVT_JOY_ZMOVE"))
  {
    return wxEVT_JOY_ZMOVE;
  }
  if (!strcmp(name_cstr, "EVT_DROP_FILES"))
  {
    return wxEVT_DROP_FILES;
  }
  if (!strcmp(name_cstr, "EVT_INIT_DIALOG"))
  {
    return wxEVT_INIT_DIALOG;
  }
  if (!strcmp(name_cstr, "EVT_IDLE"))
  {
    return wxEVT_IDLE;
  }
  if (!strcmp(name_cstr, "EVT_UPDATE_UI"))
  {
    return wxEVT_UPDATE_UI;
  }
  if (!strcmp(name_cstr, "EVT_SIZING"))
  {
    return wxEVT_SIZING;
  }
  if (!strcmp(name_cstr, "EVT_MOVING"))
  {
    return wxEVT_MOVING;
  }
  if (!strcmp(name_cstr, "EVT_MOVE_START"))
  {
    return wxEVT_MOVE_START;
  }
  if (!strcmp(name_cstr, "EVT_MOVE_END"))
  {
    return wxEVT_MOVE_END;
  }
  if (!strcmp(name_cstr, "EVT_HIBERNATE"))
  {
    return wxEVT_HIBERNATE;
  }
  if (!strcmp(name_cstr, "EVT_TEXT_COPY"))
  {
    return wxEVT_TEXT_COPY;
  }
  if (!strcmp(name_cstr, "EVT_TEXT_CUT"))
  {
    return wxEVT_TEXT_CUT;
  }
  if (!strcmp(name_cstr, "EVT_TEXT_PASTE"))
  {
    return wxEVT_TEXT_PASTE;
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_LEFT_CLICK"))
  {
    return wxEVT_COMMAND_LEFT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_LEFT_DCLICK"))
  {
    return wxEVT_COMMAND_LEFT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_RIGHT_CLICK"))
  {
    return wxEVT_COMMAND_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_RIGHT_DCLICK"))
  {
    return wxEVT_COMMAND_RIGHT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_SET_FOCUS"))
  {
    return wxEVT_COMMAND_SET_FOCUS;
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_KILL_FOCUS"))
  {
    return wxEVT_COMMAND_KILL_FOCUS;
  }
  if (!strcmp(name_cstr, "EVT_COMMAND_ENTER"))
  {
    return wxEVT_COMMAND_ENTER;
  }
  if (!strcmp(name_cstr, "EVT_HELP"))
  {
    return wxEVT_HELP;
  }
  if (!strcmp(name_cstr, "EVT_DETAILED_HELP"))
  {
    return wxEVT_DETAILED_HELP;
  }
  if (!strcmp(name_cstr, "EVT_TOOL"))
  {
    return wxEVT_TOOL;
  }
  if (!strcmp(name_cstr, "EVT_WINDOW_MODAL_DIALOG_CLOSED"))
  {
    return wxEVT_WINDOW_MODAL_DIALOG_CLOSED;
  }
  if (!strcmp(name_cstr, "EVT_FIND"))
  {
    return wxEVT_FIND;
  }
  if (!strcmp(name_cstr, "EVT_FIND_NEXT"))
  {
    return wxEVT_FIND_NEXT;
  }
  if (!strcmp(name_cstr, "EVT_FIND_REPLACE"))
  {
    return wxEVT_FIND_REPLACE;
  }
  if (!strcmp(name_cstr, "EVT_FIND_REPLACE_ALL"))
  {
    return wxEVT_FIND_REPLACE_ALL;
  }
  if (!strcmp(name_cstr, "EVT_FIND_CLOSE"))
  {
    return wxEVT_FIND_CLOSE;
  }
  if (!strcmp(name_cstr, "EVT_FILECTRL_SELECTIONCHANGED"))
  {
    return wxEVT_FILECTRL_SELECTIONCHANGED;
  }
  if (!strcmp(name_cstr, "EVT_FILECTRL_FILEACTIVATED"))
  {
    return wxEVT_FILECTRL_FILEACTIVATED;
  }
  if (!strcmp(name_cstr, "EVT_FILECTRL_FOLDERCHANGED"))
  {
    return wxEVT_FILECTRL_FOLDERCHANGED;
  }
  if (!strcmp(name_cstr, "EVT_FILECTRL_FILTERCHANGED"))
  {
    return wxEVT_FILECTRL_FILTERCHANGED;
  }
  if (!strcmp(name_cstr, "EVT_FILEPICKER_CHANGED"))
  {
    return wxEVT_FILEPICKER_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_DIRPICKER_CHANGED"))
  {
    return wxEVT_DIRPICKER_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_FONTPICKER_CHANGED"))
  {
    return wxEVT_FONTPICKER_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_FSWATCHER"))
  {
    return wxEVT_FSWATCHER;
  }
  if (!strcmp(name_cstr, "EVT_GRID_CELL_LEFT_CLICK"))
  {
    return wxEVT_GRID_CELL_LEFT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_GRID_CELL_RIGHT_CLICK"))
  {
    return wxEVT_GRID_CELL_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_GRID_CELL_LEFT_DCLICK"))
  {
    return wxEVT_GRID_CELL_LEFT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_GRID_CELL_RIGHT_DCLICK"))
  {
    return wxEVT_GRID_CELL_RIGHT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_GRID_LABEL_LEFT_CLICK"))
  {
    return wxEVT_GRID_LABEL_LEFT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_GRID_LABEL_RIGHT_CLICK"))
  {
    return wxEVT_GRID_LABEL_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_GRID_LABEL_LEFT_DCLICK"))
  {
    return wxEVT_GRID_LABEL_LEFT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_GRID_LABEL_RIGHT_DCLICK"))
  {
    return wxEVT_GRID_LABEL_RIGHT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_GRID_ROW_SIZE"))
  {
    return wxEVT_GRID_ROW_SIZE;
  }
  if (!strcmp(name_cstr, "EVT_GRID_ROW_AUTO_SIZE"))
  {
    return wxEVT_GRID_ROW_AUTO_SIZE;
  }
  if (!strcmp(name_cstr, "EVT_GRID_COL_SIZE"))
  {
    return wxEVT_GRID_COL_SIZE;
  }
  if (!strcmp(name_cstr, "EVT_GRID_COL_AUTO_SIZE"))
  {
    return wxEVT_GRID_COL_AUTO_SIZE;
  }
  if (!strcmp(name_cstr, "EVT_GRID_RANGE_SELECTING"))
  {
    return wxEVT_GRID_RANGE_SELECTING;
  }
  if (!strcmp(name_cstr, "EVT_GRID_RANGE_SELECTED"))
  {
    return wxEVT_GRID_RANGE_SELECTED;
  }
  if (!strcmp(name_cstr, "EVT_GRID_CELL_CHANGING"))
  {
    return wxEVT_GRID_CELL_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_GRID_CELL_CHANGED"))
  {
    return wxEVT_GRID_CELL_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_GRID_SELECT_CELL"))
  {
    return wxEVT_GRID_SELECT_CELL;
  }
  if (!strcmp(name_cstr, "EVT_GRID_EDITOR_SHOWN"))
  {
    return wxEVT_GRID_EDITOR_SHOWN;
  }
  if (!strcmp(name_cstr, "EVT_GRID_EDITOR_HIDDEN"))
  {
    return wxEVT_GRID_EDITOR_HIDDEN;
  }
  if (!strcmp(name_cstr, "EVT_GRID_EDITOR_CREATED"))
  {
    return wxEVT_GRID_EDITOR_CREATED;
  }
  if (!strcmp(name_cstr, "EVT_GRID_CELL_BEGIN_DRAG"))
  {
    return wxEVT_GRID_CELL_BEGIN_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_GRID_ROW_MOVE"))
  {
    return wxEVT_GRID_ROW_MOVE;
  }
  if (!strcmp(name_cstr, "EVT_GRID_COL_MOVE"))
  {
    return wxEVT_GRID_COL_MOVE;
  }
  if (!strcmp(name_cstr, "EVT_GRID_COL_SORT"))
  {
    return wxEVT_GRID_COL_SORT;
  }
  if (!strcmp(name_cstr, "EVT_GRID_TABBING"))
  {
    return wxEVT_GRID_TABBING;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_CLICK"))
  {
    return wxEVT_HEADER_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_RIGHT_CLICK"))
  {
    return wxEVT_HEADER_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_MIDDLE_CLICK"))
  {
    return wxEVT_HEADER_MIDDLE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_DCLICK"))
  {
    return wxEVT_HEADER_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_RIGHT_DCLICK"))
  {
    return wxEVT_HEADER_RIGHT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_MIDDLE_DCLICK"))
  {
    return wxEVT_HEADER_MIDDLE_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_SEPARATOR_DCLICK"))
  {
    return wxEVT_HEADER_SEPARATOR_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_BEGIN_RESIZE"))
  {
    return wxEVT_HEADER_BEGIN_RESIZE;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_RESIZING"))
  {
    return wxEVT_HEADER_RESIZING;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_END_RESIZE"))
  {
    return wxEVT_HEADER_END_RESIZE;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_BEGIN_REORDER"))
  {
    return wxEVT_HEADER_BEGIN_REORDER;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_END_REORDER"))
  {
    return wxEVT_HEADER_END_REORDER;
  }
  if (!strcmp(name_cstr, "EVT_HEADER_DRAGGING_CANCELLED"))
  {
    return wxEVT_HEADER_DRAGGING_CANCELLED;
  }
  if (!strcmp(name_cstr, "EVT_HYPERLINK"))
  {
    return wxEVT_HYPERLINK;
  }
  if (!strcmp(name_cstr, "EVT_QUERY_LAYOUT_INFO"))
  {
    return wxEVT_QUERY_LAYOUT_INFO;
  }
  if (!strcmp(name_cstr, "EVT_CALCULATE_LAYOUT"))
  {
    return wxEVT_CALCULATE_LAYOUT;
  }
  if (!strcmp(name_cstr, "EVT_LISTBOOK_PAGE_CHANGED"))
  {
    return wxEVT_LISTBOOK_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_LISTBOOK_PAGE_CHANGING"))
  {
    return wxEVT_LISTBOOK_PAGE_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_LIST_BEGIN_DRAG"))
  {
    return wxEVT_LIST_BEGIN_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_LIST_BEGIN_RDRAG"))
  {
    return wxEVT_LIST_BEGIN_RDRAG;
  }
  if (!strcmp(name_cstr, "EVT_LIST_BEGIN_LABEL_EDIT"))
  {
    return wxEVT_LIST_BEGIN_LABEL_EDIT;
  }
  if (!strcmp(name_cstr, "EVT_LIST_END_LABEL_EDIT"))
  {
    return wxEVT_LIST_END_LABEL_EDIT;
  }
  if (!strcmp(name_cstr, "EVT_LIST_DELETE_ITEM"))
  {
    return wxEVT_LIST_DELETE_ITEM;
  }
  if (!strcmp(name_cstr, "EVT_LIST_DELETE_ALL_ITEMS"))
  {
    return wxEVT_LIST_DELETE_ALL_ITEMS;
  }
  if (!strcmp(name_cstr, "EVT_LIST_ITEM_SELECTED"))
  {
    return wxEVT_LIST_ITEM_SELECTED;
  }
  if (!strcmp(name_cstr, "EVT_LIST_ITEM_DESELECTED"))
  {
    return wxEVT_LIST_ITEM_DESELECTED;
  }
  if (!strcmp(name_cstr, "EVT_LIST_KEY_DOWN"))
  {
    return wxEVT_LIST_KEY_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_LIST_INSERT_ITEM"))
  {
    return wxEVT_LIST_INSERT_ITEM;
  }
  if (!strcmp(name_cstr, "EVT_LIST_COL_CLICK"))
  {
    return wxEVT_LIST_COL_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_LIST_ITEM_RIGHT_CLICK"))
  {
    return wxEVT_LIST_ITEM_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_LIST_ITEM_MIDDLE_CLICK"))
  {
    return wxEVT_LIST_ITEM_MIDDLE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_LIST_ITEM_ACTIVATED"))
  {
    return wxEVT_LIST_ITEM_ACTIVATED;
  }
  if (!strcmp(name_cstr, "EVT_LIST_CACHE_HINT"))
  {
    return wxEVT_LIST_CACHE_HINT;
  }
  if (!strcmp(name_cstr, "EVT_LIST_COL_RIGHT_CLICK"))
  {
    return wxEVT_LIST_COL_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_LIST_COL_BEGIN_DRAG"))
  {
    return wxEVT_LIST_COL_BEGIN_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_LIST_COL_DRAGGING"))
  {
    return wxEVT_LIST_COL_DRAGGING;
  }
  if (!strcmp(name_cstr, "EVT_LIST_COL_END_DRAG"))
  {
    return wxEVT_LIST_COL_END_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_LIST_ITEM_FOCUSED"))
  {
    return wxEVT_LIST_ITEM_FOCUSED;
  }
  if (!strcmp(name_cstr, "EVT_LIST_ITEM_CHECKED"))
  {
    return wxEVT_LIST_ITEM_CHECKED;
  }
  if (!strcmp(name_cstr, "EVT_LIST_ITEM_UNCHECKED"))
  {
    return wxEVT_LIST_ITEM_UNCHECKED;
  }
  /*if (!strcmp(name_cstr, "EVT_MEDIA_LOADED"))
  {
    return wxEVT_MEDIA_LOADED;
  }
  if (!strcmp(name_cstr, "EVT_MEDIA_STOP"))
  {
    return wxEVT_MEDIA_STOP;
  }
  if (!strcmp(name_cstr, "EVT_MEDIA_FINISHED"))
  {
    return wxEVT_MEDIA_FINISHED;
  }
  if (!strcmp(name_cstr, "EVT_MEDIA_STATECHANGED"))
  {
    return wxEVT_MEDIA_STATECHANGED;
  }
  if (!strcmp(name_cstr, "EVT_MEDIA_PLAY"))
  {
    return wxEVT_MEDIA_PLAY;
  }
  if (!strcmp(name_cstr, "EVT_MEDIA_PAUSE"))
  {
    return wxEVT_MEDIA_PAUSE;
  }*/
  if (!strcmp(name_cstr, "EVT_NOTEBOOK_PAGE_CHANGED"))
  {
    return wxEVT_NOTEBOOK_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_NOTEBOOK_PAGE_CHANGING"))
  {
    return wxEVT_NOTEBOOK_PAGE_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_NOTIFICATION_MESSAGE_CLICK"))
  {
    return wxEVT_NOTIFICATION_MESSAGE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_NOTIFICATION_MESSAGE_DISMISSED"))
  {
    return wxEVT_NOTIFICATION_MESSAGE_DISMISSED;
  }
  if (!strcmp(name_cstr, "EVT_NOTIFICATION_MESSAGE_ACTION"))
  {
    return wxEVT_NOTIFICATION_MESSAGE_ACTION;
  }
  if (!strcmp(name_cstr, "EVT_POWER_SUSPENDING"))
  {
    return wxEVT_POWER_SUSPENDING;
  }
  if (!strcmp(name_cstr, "EVT_POWER_SUSPENDED"))
  {
    return wxEVT_POWER_SUSPENDED;
  }
  if (!strcmp(name_cstr, "EVT_POWER_SUSPEND_CANCEL"))
  {
    return wxEVT_POWER_SUSPEND_CANCEL;
  }
  if (!strcmp(name_cstr, "EVT_POWER_RESUME"))
  {
    return wxEVT_POWER_RESUME;
  }
  if (!strcmp(name_cstr, "EVT_END_PROCESS"))
  {
    return wxEVT_END_PROCESS;
  }
  if (!strcmp(name_cstr, "EVT_SASH_DRAGGED"))
  {
    return wxEVT_SASH_DRAGGED;
  }
  if (!strcmp(name_cstr, "EVT_SPINCTRL"))
  {
    return wxEVT_SPINCTRL;
  }
  if (!strcmp(name_cstr, "EVT_SPINCTRLDOUBLE"))
  {
    return wxEVT_SPINCTRLDOUBLE;
  }
  if (!strcmp(name_cstr, "EVT_SPLITTER_SASH_POS_CHANGED"))
  {
    return wxEVT_SPLITTER_SASH_POS_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_SPLITTER_SASH_POS_CHANGING"))
  {
    return wxEVT_SPLITTER_SASH_POS_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_SPLITTER_SASH_POS_RESIZE"))
  {
    return wxEVT_SPLITTER_SASH_POS_RESIZE;
  }
  if (!strcmp(name_cstr, "EVT_SPLITTER_DOUBLECLICKED"))
  {
    return wxEVT_SPLITTER_DOUBLECLICKED;
  }
  if (!strcmp(name_cstr, "EVT_SPLITTER_UNSPLIT"))
  {
    return wxEVT_SPLITTER_UNSPLIT;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_MOVE"))
  {
    return wxEVT_TASKBAR_MOVE;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_LEFT_DOWN"))
  {
    return wxEVT_TASKBAR_LEFT_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_LEFT_UP"))
  {
    return wxEVT_TASKBAR_LEFT_UP;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_RIGHT_DOWN"))
  {
    return wxEVT_TASKBAR_RIGHT_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_RIGHT_UP"))
  {
    return wxEVT_TASKBAR_RIGHT_UP;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_LEFT_DCLICK"))
  {
    return wxEVT_TASKBAR_LEFT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_RIGHT_DCLICK"))
  {
    return wxEVT_TASKBAR_RIGHT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_CLICK"))
  {
    return wxEVT_TASKBAR_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_BALLOON_TIMEOUT"))
  {
    return wxEVT_TASKBAR_BALLOON_TIMEOUT;
  }
  if (!strcmp(name_cstr, "EVT_TASKBAR_BALLOON_CLICK"))
  {
    return wxEVT_TASKBAR_BALLOON_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_TEXT"))
  {
    return wxEVT_TEXT;
  }
  if (!strcmp(name_cstr, "EVT_TEXT_ENTER"))
  {
    return wxEVT_TEXT_ENTER;
  }
  if (!strcmp(name_cstr, "EVT_TEXT_URL"))
  {
    return wxEVT_TEXT_URL;
  }
  if (!strcmp(name_cstr, "EVT_TEXT_MAXLEN"))
  {
    return wxEVT_TEXT_MAXLEN;
  }
  if (!strcmp(name_cstr, "EVT_TOGGLEBUTTON"))
  {
    return wxEVT_TOGGLEBUTTON;
  }
  if (!strcmp(name_cstr, "EVT_TIMER"))
  {
    return wxEVT_TIMER;
  }
  if (!strcmp(name_cstr, "EVT_TOOLBOOK_PAGE_CHANGED"))
  {
    return wxEVT_TOOLBOOK_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_TOOLBOOK_PAGE_CHANGING"))
  {
    return wxEVT_TOOLBOOK_PAGE_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_TREEBOOK_PAGE_CHANGED"))
  {
    return wxEVT_TREEBOOK_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_TREEBOOK_PAGE_CHANGING"))
  {
    return wxEVT_TREEBOOK_PAGE_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_TREEBOOK_NODE_COLLAPSED"))
  {
    return wxEVT_TREEBOOK_NODE_COLLAPSED;
  }
  if (!strcmp(name_cstr, "EVT_TREEBOOK_NODE_EXPANDED"))
  {
    return wxEVT_TREEBOOK_NODE_EXPANDED;
  }
  if (!strcmp(name_cstr, "EVT_TREE_BEGIN_DRAG"))
  {
    return wxEVT_TREE_BEGIN_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_TREE_BEGIN_RDRAG"))
  {
    return wxEVT_TREE_BEGIN_RDRAG;
  }
  if (!strcmp(name_cstr, "EVT_TREE_BEGIN_LABEL_EDIT"))
  {
    return wxEVT_TREE_BEGIN_LABEL_EDIT;
  }
  if (!strcmp(name_cstr, "EVT_TREE_END_LABEL_EDIT"))
  {
    return wxEVT_TREE_END_LABEL_EDIT;
  }
  if (!strcmp(name_cstr, "EVT_TREE_DELETE_ITEM"))
  {
    return wxEVT_TREE_DELETE_ITEM;
  }
  if (!strcmp(name_cstr, "EVT_TREE_GET_INFO"))
  {
    return wxEVT_TREE_GET_INFO;
  }
  if (!strcmp(name_cstr, "EVT_TREE_SET_INFO"))
  {
    return wxEVT_TREE_SET_INFO;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_EXPANDED"))
  {
    return wxEVT_TREE_ITEM_EXPANDED;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_EXPANDING"))
  {
    return wxEVT_TREE_ITEM_EXPANDING;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_COLLAPSED"))
  {
    return wxEVT_TREE_ITEM_COLLAPSED;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_COLLAPSING"))
  {
    return wxEVT_TREE_ITEM_COLLAPSING;
  }
  if (!strcmp(name_cstr, "EVT_TREE_SEL_CHANGED"))
  {
    return wxEVT_TREE_SEL_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_TREE_SEL_CHANGING"))
  {
    return wxEVT_TREE_SEL_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_TREE_KEY_DOWN"))
  {
    return wxEVT_TREE_KEY_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_ACTIVATED"))
  {
    return wxEVT_TREE_ITEM_ACTIVATED;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_RIGHT_CLICK"))
  {
    return wxEVT_TREE_ITEM_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_MIDDLE_CLICK"))
  {
    return wxEVT_TREE_ITEM_MIDDLE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_TREE_END_DRAG"))
  {
    return wxEVT_TREE_END_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_TREE_STATE_IMAGE_CLICK"))
  {
    return wxEVT_TREE_STATE_IMAGE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_GETTOOLTIP"))
  {
    return wxEVT_TREE_ITEM_GETTOOLTIP;
  }
  if (!strcmp(name_cstr, "EVT_TREE_ITEM_MENU"))
  {
    return wxEVT_TREE_ITEM_MENU;
  }
  if (!strcmp(name_cstr, "EVT_TREELIST_SELECTION_CHANGED"))
  {
    return wxEVT_TREELIST_SELECTION_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_TREELIST_ITEM_EXPANDING"))
  {
    return wxEVT_TREELIST_ITEM_EXPANDING;
  }
  if (!strcmp(name_cstr, "EVT_TREELIST_ITEM_EXPANDED"))
  {
    return wxEVT_TREELIST_ITEM_EXPANDED;
  }
  if (!strcmp(name_cstr, "EVT_TREELIST_ITEM_CHECKED"))
  {
    return wxEVT_TREELIST_ITEM_CHECKED;
  }
  if (!strcmp(name_cstr, "EVT_TREELIST_ITEM_ACTIVATED"))
  {
    return wxEVT_TREELIST_ITEM_ACTIVATED;
  }
  if (!strcmp(name_cstr, "EVT_TREELIST_ITEM_CONTEXT_MENU"))
  {
    return wxEVT_TREELIST_ITEM_CONTEXT_MENU;
  }
  if (!strcmp(name_cstr, "EVT_TREELIST_COLUMN_SORTED"))
  {
    return wxEVT_TREELIST_COLUMN_SORTED;
  }
  /*if (!strcmp(name_cstr, "EVT_WEBREQUEST_STATE"))
  {
    return wxEVT_WEBREQUEST_STATE;
  }
  if (!strcmp(name_cstr, "EVT_WEBREQUEST_DATA"))
  {
    return wxEVT_WEBREQUEST_DATA;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_NAVIGATING"))
  {
    return wxEVT_WEBVIEW_NAVIGATING;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_NAVIGATED"))
  {
    return wxEVT_WEBVIEW_NAVIGATED;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_LOADED"))
  {
    return wxEVT_WEBVIEW_LOADED;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_ERROR"))
  {
    return wxEVT_WEBVIEW_ERROR;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_NEWWINDOW"))
  {
    return wxEVT_WEBVIEW_NEWWINDOW;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_TITLE_CHANGED"))
  {
    return wxEVT_WEBVIEW_TITLE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_FULLSCREEN_CHANGED"))
  {
    return wxEVT_WEBVIEW_FULLSCREEN_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED"))
  {
    return wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED;
  }
  if (!strcmp(name_cstr, "EVT_WEBVIEW_SCRIPT_RESULT"))
  {
    return wxEVT_WEBVIEW_SCRIPT_RESULT;
  }*/
  if (!strcmp(name_cstr, "EVT_WIZARD_PAGE_CHANGED"))
  {
    return wxEVT_WIZARD_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_WIZARD_PAGE_CHANGING"))
  {
    return wxEVT_WIZARD_PAGE_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_WIZARD_CANCEL"))
  {
    return wxEVT_WIZARD_CANCEL;
  }
  if (!strcmp(name_cstr, "EVT_WIZARD_HELP"))
  {
    return wxEVT_WIZARD_HELP;
  }
  if (!strcmp(name_cstr, "EVT_WIZARD_FINISHED"))
  {
    return wxEVT_WIZARD_FINISHED;
  }
  if (!strcmp(name_cstr, "EVT_WIZARD_PAGE_SHOWN"))
  {
    return wxEVT_WIZARD_PAGE_SHOWN;
  }
  if (!strcmp(name_cstr, "EVT_WIZARD_BEFORE_PAGE_CHANGED"))
  {
    return wxEVT_WIZARD_BEFORE_PAGE_CHANGED;
  }
  /*if (!strcmp(name_cstr, "EVT_AUITOOLBAR_TOOL_DROPDOWN"))
  {
    return wxEVT_AUITOOLBAR_TOOL_DROPDOWN;
  }
  if (!strcmp(name_cstr, "EVT_AUITOOLBAR_OVERFLOW_CLICK"))
  {
    return wxEVT_AUITOOLBAR_OVERFLOW_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_AUITOOLBAR_RIGHT_CLICK"))
  {
    return wxEVT_AUITOOLBAR_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_AUITOOLBAR_MIDDLE_CLICK"))
  {
    return wxEVT_AUITOOLBAR_MIDDLE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_AUITOOLBAR_BEGIN_DRAG"))
  {
    return wxEVT_AUITOOLBAR_BEGIN_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_PAGE_CLOSE"))
  {
    return wxEVT_AUINOTEBOOK_PAGE_CLOSE;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_PAGE_CHANGED"))
  {
    return wxEVT_AUINOTEBOOK_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_PAGE_CHANGING"))
  {
    return wxEVT_AUINOTEBOOK_PAGE_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_PAGE_CLOSED"))
  {
    return wxEVT_AUINOTEBOOK_PAGE_CLOSED;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_BUTTON"))
  {
    return wxEVT_AUINOTEBOOK_BUTTON;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_BEGIN_DRAG"))
  {
    return wxEVT_AUINOTEBOOK_BEGIN_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_END_DRAG"))
  {
    return wxEVT_AUINOTEBOOK_END_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_DRAG_MOTION"))
  {
    return wxEVT_AUINOTEBOOK_DRAG_MOTION;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_ALLOW_DND"))
  {
    return wxEVT_AUINOTEBOOK_ALLOW_DND;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_TAB_MIDDLE_DOWN"))
  {
    return wxEVT_AUINOTEBOOK_TAB_MIDDLE_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_TAB_MIDDLE_UP"))
  {
    return wxEVT_AUINOTEBOOK_TAB_MIDDLE_UP;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_TAB_RIGHT_DOWN"))
  {
    return wxEVT_AUINOTEBOOK_TAB_RIGHT_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_TAB_RIGHT_UP"))
  {
    return wxEVT_AUINOTEBOOK_TAB_RIGHT_UP;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_DRAG_DONE"))
  {
    return wxEVT_AUINOTEBOOK_DRAG_DONE;
  }
  if (!strcmp(name_cstr, "EVT_AUINOTEBOOK_BG_DCLICK"))
  {
    return wxEVT_AUINOTEBOOK_BG_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_AUI_PANE_BUTTON"))
  {
    return wxEVT_AUI_PANE_BUTTON;
  }
  if (!strcmp(name_cstr, "EVT_AUI_PANE_CLOSE"))
  {
    return wxEVT_AUI_PANE_CLOSE;
  }
  if (!strcmp(name_cstr, "EVT_AUI_PANE_MAXIMIZE"))
  {
    return wxEVT_AUI_PANE_MAXIMIZE;
  }
  if (!strcmp(name_cstr, "EVT_AUI_PANE_RESTORE"))
  {
    return wxEVT_AUI_PANE_RESTORE;
  }
  if (!strcmp(name_cstr, "EVT_AUI_PANE_ACTIVATED"))
  {
    return wxEVT_AUI_PANE_ACTIVATED;
  }
  if (!strcmp(name_cstr, "EVT_AUI_RENDER"))
  {
    return wxEVT_AUI_RENDER;
  }
  if (!strcmp(name_cstr, "EVT_AUI_FIND_MANAGER"))
  {
    return wxEVT_AUI_FIND_MANAGER;
  }*/
  /*if (!strcmp(name_cstr, "EVT_HTML_CELL_CLICKED"))
  {
    return wxEVT_HTML_CELL_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_HTML_CELL_HOVER"))
  {
    return wxEVT_HTML_CELL_HOVER;
  }
  if (!strcmp(name_cstr, "EVT_HTML_LINK_CLICKED"))
  {
    return wxEVT_HTML_LINK_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_PG_SELECTED"))
  {
    return wxEVT_PG_SELECTED;
  }
  if (!strcmp(name_cstr, "EVT_PG_CHANGING"))
  {
    return wxEVT_PG_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_PG_CHANGED"))
  {
    return wxEVT_PG_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_PG_HIGHLIGHTED"))
  {
    return wxEVT_PG_HIGHLIGHTED;
  }
  if (!strcmp(name_cstr, "EVT_PG_RIGHT_CLICK"))
  {
    return wxEVT_PG_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_PG_PAGE_CHANGED"))
  {
    return wxEVT_PG_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_PG_ITEM_COLLAPSED"))
  {
    return wxEVT_PG_ITEM_COLLAPSED;
  }
  if (!strcmp(name_cstr, "EVT_PG_ITEM_EXPANDED"))
  {
    return wxEVT_PG_ITEM_EXPANDED;
  }
  if (!strcmp(name_cstr, "EVT_PG_DOUBLE_CLICK"))
  {
    return wxEVT_PG_DOUBLE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_PG_LABEL_EDIT_BEGIN"))
  {
    return wxEVT_PG_LABEL_EDIT_BEGIN;
  }
  if (!strcmp(name_cstr, "EVT_PG_LABEL_EDIT_ENDING"))
  {
    return wxEVT_PG_LABEL_EDIT_ENDING;
  }
  if (!strcmp(name_cstr, "EVT_PG_COL_BEGIN_DRAG"))
  {
    return wxEVT_PG_COL_BEGIN_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_PG_COL_DRAGGING"))
  {
    return wxEVT_PG_COL_DRAGGING;
  }
  if (!strcmp(name_cstr, "EVT_PG_COL_END_DRAG"))
  {
    return wxEVT_PG_COL_END_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_PAGE_CHANGED"))
  {
    return wxEVT_RIBBONBAR_PAGE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_PAGE_CHANGING"))
  {
    return wxEVT_RIBBONBAR_PAGE_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_TAB_MIDDLE_DOWN"))
  {
    return wxEVT_RIBBONBAR_TAB_MIDDLE_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_TAB_MIDDLE_UP"))
  {
    return wxEVT_RIBBONBAR_TAB_MIDDLE_UP;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_TAB_RIGHT_DOWN"))
  {
    return wxEVT_RIBBONBAR_TAB_RIGHT_DOWN;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_TAB_RIGHT_UP"))
  {
    return wxEVT_RIBBONBAR_TAB_RIGHT_UP;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_TAB_LEFT_DCLICK"))
  {
    return wxEVT_RIBBONBAR_TAB_LEFT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_TOGGLED"))
  {
    return wxEVT_RIBBONBAR_TOGGLED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBAR_HELP_CLICK"))
  {
    return wxEVT_RIBBONBAR_HELP_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBUTTONBAR_CLICKED"))
  {
    return wxEVT_RIBBONBUTTONBAR_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED"))
  {
    return wxEVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONGALLERY_HOVER_CHANGED"))
  {
    return wxEVT_RIBBONGALLERY_HOVER_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONGALLERY_SELECTED"))
  {
    return wxEVT_RIBBONGALLERY_SELECTED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONGALLERY_CLICKED"))
  {
    return wxEVT_RIBBONGALLERY_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONPANEL_EXTBUTTON_ACTIVATED"))
  {
    return wxEVT_RIBBONPANEL_EXTBUTTON_ACTIVATED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONTOOLBAR_CLICKED"))
  {
    return wxEVT_RIBBONTOOLBAR_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED"))
  {
    return wxEVT_RIBBONTOOLBAR_DROPDOWN_CLICKED;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_LEFT_CLICK"))
  {
    return wxEVT_RICHTEXT_LEFT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_RIGHT_CLICK"))
  {
    return wxEVT_RICHTEXT_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_MIDDLE_CLICK"))
  {
    return wxEVT_RICHTEXT_MIDDLE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_LEFT_DCLICK"))
  {
    return wxEVT_RICHTEXT_LEFT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_RETURN"))
  {
    return wxEVT_RICHTEXT_RETURN;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_CHARACTER"))
  {
    return wxEVT_RICHTEXT_CHARACTER;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_CONSUMING_CHARACTER"))
  {
    return wxEVT_RICHTEXT_CONSUMING_CHARACTER;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_DELETE"))
  {
    return wxEVT_RICHTEXT_DELETE;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_STYLESHEET_CHANGING"))
  {
    return wxEVT_RICHTEXT_STYLESHEET_CHANGING;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_STYLESHEET_CHANGED"))
  {
    return wxEVT_RICHTEXT_STYLESHEET_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_STYLESHEET_REPLACING"))
  {
    return wxEVT_RICHTEXT_STYLESHEET_REPLACING;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_STYLESHEET_REPLACED"))
  {
    return wxEVT_RICHTEXT_STYLESHEET_REPLACED;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_CONTENT_INSERTED"))
  {
    return wxEVT_RICHTEXT_CONTENT_INSERTED;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_CONTENT_DELETED"))
  {
    return wxEVT_RICHTEXT_CONTENT_DELETED;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_STYLE_CHANGED"))
  {
    return wxEVT_RICHTEXT_STYLE_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_PROPERTIES_CHANGED"))
  {
    return wxEVT_RICHTEXT_PROPERTIES_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_SELECTION_CHANGED"))
  {
    return wxEVT_RICHTEXT_SELECTION_CHANGED;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_BUFFER_RESET"))
  {
    return wxEVT_RICHTEXT_BUFFER_RESET;
  }
  if (!strcmp(name_cstr, "EVT_RICHTEXT_FOCUS_OBJECT_CHANGED"))
  {
    return wxEVT_RICHTEXT_FOCUS_OBJECT_CHANGED;
  }*/
  /*if (!strcmp(name_cstr, "EVT_STC_CHANGE"))
  {
    return wxEVT_STC_CHANGE;
  }
  if (!strcmp(name_cstr, "EVT_STC_STYLENEEDED"))
  {
    return wxEVT_STC_STYLENEEDED;
  }
  if (!strcmp(name_cstr, "EVT_STC_CHARADDED"))
  {
    return wxEVT_STC_CHARADDED;
  }
  if (!strcmp(name_cstr, "EVT_STC_SAVEPOINTREACHED"))
  {
    return wxEVT_STC_SAVEPOINTREACHED;
  }
  if (!strcmp(name_cstr, "EVT_STC_SAVEPOINTLEFT"))
  {
    return wxEVT_STC_SAVEPOINTLEFT;
  }
  if (!strcmp(name_cstr, "EVT_STC_ROMODIFYATTEMPT"))
  {
    return wxEVT_STC_ROMODIFYATTEMPT;
  }
  if (!strcmp(name_cstr, "EVT_STC_KEY"))
  {
    return wxEVT_STC_KEY;
  }
  if (!strcmp(name_cstr, "EVT_STC_DOUBLECLICK"))
  {
    return wxEVT_STC_DOUBLECLICK;
  }
  if (!strcmp(name_cstr, "EVT_STC_UPDATEUI"))
  {
    return wxEVT_STC_UPDATEUI;
  }
  if (!strcmp(name_cstr, "EVT_STC_MODIFIED"))
  {
    return wxEVT_STC_MODIFIED;
  }
  if (!strcmp(name_cstr, "EVT_STC_MACRORECORD"))
  {
    return wxEVT_STC_MACRORECORD;
  }
  if (!strcmp(name_cstr, "EVT_STC_MARGINCLICK"))
  {
    return wxEVT_STC_MARGINCLICK;
  }
  if (!strcmp(name_cstr, "EVT_STC_NEEDSHOWN"))
  {
    return wxEVT_STC_NEEDSHOWN;
  }
  if (!strcmp(name_cstr, "EVT_STC_PAINTED"))
  {
    return wxEVT_STC_PAINTED;
  }
  if (!strcmp(name_cstr, "EVT_STC_USERLISTSELECTION"))
  {
    return wxEVT_STC_USERLISTSELECTION;
  }
  if (!strcmp(name_cstr, "EVT_STC_URIDROPPED"))
  {
    return wxEVT_STC_URIDROPPED;
  }
  if (!strcmp(name_cstr, "EVT_STC_DWELLSTART"))
  {
    return wxEVT_STC_DWELLSTART;
  }
  if (!strcmp(name_cstr, "EVT_STC_DWELLEND"))
  {
    return wxEVT_STC_DWELLEND;
  }
  if (!strcmp(name_cstr, "EVT_STC_START_DRAG"))
  {
    return wxEVT_STC_START_DRAG;
  }
  if (!strcmp(name_cstr, "EVT_STC_DRAG_OVER"))
  {
    return wxEVT_STC_DRAG_OVER;
  }
  if (!strcmp(name_cstr, "EVT_STC_DO_DROP"))
  {
    return wxEVT_STC_DO_DROP;
  }
  if (!strcmp(name_cstr, "EVT_STC_ZOOM"))
  {
    return wxEVT_STC_ZOOM;
  }
  if (!strcmp(name_cstr, "EVT_STC_HOTSPOT_CLICK"))
  {
    return wxEVT_STC_HOTSPOT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_STC_HOTSPOT_DCLICK"))
  {
    return wxEVT_STC_HOTSPOT_DCLICK;
  }
  if (!strcmp(name_cstr, "EVT_STC_CALLTIP_CLICK"))
  {
    return wxEVT_STC_CALLTIP_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_STC_AUTOCOMP_SELECTION"))
  {
    return wxEVT_STC_AUTOCOMP_SELECTION;
  }
  if (!strcmp(name_cstr, "EVT_STC_INDICATOR_CLICK"))
  {
    return wxEVT_STC_INDICATOR_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_STC_INDICATOR_RELEASE"))
  {
    return wxEVT_STC_INDICATOR_RELEASE;
  }
  if (!strcmp(name_cstr, "EVT_STC_AUTOCOMP_CANCELLED"))
  {
    return wxEVT_STC_AUTOCOMP_CANCELLED;
  }
  if (!strcmp(name_cstr, "EVT_STC_AUTOCOMP_CHAR_DELETED"))
  {
    return wxEVT_STC_AUTOCOMP_CHAR_DELETED;
  }
  if (!strcmp(name_cstr, "EVT_STC_HOTSPOT_RELEASE_CLICK"))
  {
    return wxEVT_STC_HOTSPOT_RELEASE_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_STC_CLIPBOARD_COPY"))
  {
    return wxEVT_STC_CLIPBOARD_COPY;
  }
  if (!strcmp(name_cstr, "EVT_STC_CLIPBOARD_PASTE"))
  {
    return wxEVT_STC_CLIPBOARD_PASTE;
  }
  if (!strcmp(name_cstr, "EVT_STC_AUTOCOMP_COMPLETED"))
  {
    return wxEVT_STC_AUTOCOMP_COMPLETED;
  }
  if (!strcmp(name_cstr, "EVT_STC_MARGIN_RIGHT_CLICK"))
  {
    return wxEVT_STC_MARGIN_RIGHT_CLICK;
  }
  if (!strcmp(name_cstr, "EVT_STC_AUTOCOMP_SELECTION_CHANGE"))
  {
    return wxEVT_STC_AUTOCOMP_SELECTION_CHANGE;
  }*/
  return 0;
}

int constexpr get_style_from_list(const std::vector<std::string>& styles) {
  int out = 0;
  for(int i = 0; i < styles.size(); i++) {
    const char* style_cstr = styles[i].c_str();

    if (!strcmp(style_cstr, "SAVE")) {
      out |= wxFD_SAVE;
    }
    if (!strcmp(style_cstr, "OPEN")) {
      out |= wxFD_OPEN;
    }
    if (!strcmp(style_cstr, "FILE_MUST_EXIST")) {
      out |= wxFD_FILE_MUST_EXIST;
    }
    if (!strcmp(style_cstr, "NO_BORDER")) {
      out |= wxNO_BORDER;
    }
    if (!strcmp(style_cstr, "DOUBLE_BORDER")) {
      out |= wxDOUBLE_BORDER;
    }
    if (!strcmp(style_cstr, "SUNKEN_BORDER")) {
      out |= wxSUNKEN_BORDER;
    }
    if (!strcmp(style_cstr, "RAISED_BORDER")) {
      out |= wxRAISED_BORDER;
    }
    if (!strcmp(style_cstr, "BORDER")) {
      out |= wxBORDER;
    }
    if (!strcmp(style_cstr, "SIMPLE_BORDER")) {
      out |= wxSIMPLE_BORDER;
    }
    if (!strcmp(style_cstr, "STATIC_BORDER")) {
      out |= wxSTATIC_BORDER;
    }
    if (!strcmp(style_cstr, "NO_BORDER")) {
      out |= wxNO_BORDER;
    }


    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_COMPOSITE")) {
      out |= wxRICHTEXT_FIELD_STYLE_COMPOSITE;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_RECTANGLE")) {
      out |= wxRICHTEXT_FIELD_STYLE_RECTANGLE;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_NO_BORDER")) {
      out |= wxRICHTEXT_FIELD_STYLE_NO_BORDER;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_START_TAG")) {
      out |= wxRICHTEXT_FIELD_STYLE_START_TAG;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_END_TAG")) {
      out |= wxRICHTEXT_FIELD_STYLE_END_TAG;
      }
    if(!strcmp(style_cstr, "RE_CENTRE_CARET")) {
      out |= wxRE_CENTRE_CARET;
      }
    if(!strcmp(style_cstr, "RE_MULTILINE")) {
      out |= wxRE_MULTILINE;
      }
    if(!strcmp(style_cstr, "RE_READONLY")) {
      out |= wxRE_READONLY;
      }
    if(!strcmp(style_cstr, "AC_DEFAULT_STYLE")) {
      out |= wxAC_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "AC_NO_AUTORESIZE")) {
      out |= wxAC_NO_AUTORESIZE;
      }
    if(!strcmp(style_cstr, "BU_LEFT")) {
      out |= wxBU_LEFT;
      }
    if(!strcmp(style_cstr, "BU_TOP")) {
      out |= wxBU_TOP;
      }
    if(!strcmp(style_cstr, "BU_RIGHT")) {
      out |= wxBU_RIGHT;
      }
    if(!strcmp(style_cstr, "BU_BOTTOM")) {
      out |= wxBU_BOTTOM;
      }
    if(!strcmp(style_cstr, "CB_READONLY")) {
      out |= wxCB_READONLY;
      }
    if(!strcmp(style_cstr, "CB_SORT")) {
      out |= wxCB_SORT;
      }
    if(!strcmp(style_cstr, "TE_PROCESS_ENTER")) {
      out |= wxTE_PROCESS_ENTER;
      }
    if(!strcmp(style_cstr, "BU_LEFT")) {
      out |= wxBU_LEFT;
      }
    if(!strcmp(style_cstr, "BU_TOP")) {
      out |= wxBU_TOP;
      }
    if(!strcmp(style_cstr, "BU_RIGHT")) {
      out |= wxBU_RIGHT;
      }
    if(!strcmp(style_cstr, "BU_BOTTOM")) {
      out |= wxBU_BOTTOM;
      }
    if(!strcmp(style_cstr, "BU_EXACTFIT")) {
      out |= wxBU_EXACTFIT;
      }
    if(!strcmp(style_cstr, "BU_NOTEXT")) {
      out |= wxBU_NOTEXT;
      }
    if(!strcmp(style_cstr, "BORDER_NONE")) {
      out |= wxBORDER_NONE;
      }
    if(!strcmp(style_cstr, "CAL_SUNDAY_FIRST")) {
      out |= wxCAL_SUNDAY_FIRST;
      }
    if(!strcmp(style_cstr, "CAL_MONDAY_FIRST")) {
      out |= wxCAL_MONDAY_FIRST;
      }
    if(!strcmp(style_cstr, "CAL_SHOW_HOLIDAYS")) {
      out |= wxCAL_SHOW_HOLIDAYS;
      }
    if(!strcmp(style_cstr, "CAL_NO_YEAR_CHANGE")) {
      out |= wxCAL_NO_YEAR_CHANGE;
      }
    if(!strcmp(style_cstr, "CAL_NO_MONTH_CHANGE")) {
      out |= wxCAL_NO_MONTH_CHANGE;
      }
    if(!strcmp(style_cstr, "CAL_SHOW_SURROUNDING_WEEKS")) {
      out |= wxCAL_SHOW_SURROUNDING_WEEKS;
      }
    if(!strcmp(style_cstr, "CAL_SEQUENTIAL_MONTH_SELECTION")) {
      out |= wxCAL_SEQUENTIAL_MONTH_SELECTION;
      }
    if(!strcmp(style_cstr, "CAL_SHOW_WEEK_NUMBERS")) {
      out |= wxCAL_SHOW_WEEK_NUMBERS;
      }
    if(!strcmp(style_cstr, "CHK_2STATE")) {
      out |= wxCHK_2STATE;
      }
    if(!strcmp(style_cstr, "CHK_3STATE")) {
      out |= wxCHK_3STATE;
      }
    if(!strcmp(style_cstr, "CHK_ALLOW_3RD_STATE_FOR_USER")) {
      out |= wxCHK_ALLOW_3RD_STATE_FOR_USER;
      }
    if(!strcmp(style_cstr, "ALIGN_RIGHT")) {
      out |= wxALIGN_RIGHT;
      }
    if(!strcmp(style_cstr, "OK")) {
      out |= wxOK;
      }
    if(!strcmp(style_cstr, "CANCEL")) {
      out |= wxCANCEL;
      }
    if(!strcmp(style_cstr, "CENTRE")) {
      out |= wxCENTRE;
      }
    if(!strcmp(style_cstr, "OK")) {
      out |= wxOK;
      }
    if(!strcmp(style_cstr, "CANCEL")) {
      out |= wxCANCEL;
      }
    if(!strcmp(style_cstr, "CENTRE")) {
      out |= wxCENTRE;
      }
    if(!strcmp(style_cstr, "CB_SORT")) {
      out |= wxCB_SORT;
      }
    if(!strcmp(style_cstr, "CHB_DEFAULT")) {
      out |= wxCHB_DEFAULT;
      }
    if(!strcmp(style_cstr, "CHB_TOP")) {
      out |= wxCHB_TOP;
      }
    if(!strcmp(style_cstr, "CHB_LEFT")) {
      out |= wxCHB_LEFT;
      }
    if(!strcmp(style_cstr, "CHB_RIGHT")) {
      out |= wxCHB_RIGHT;
      }
    if(!strcmp(style_cstr, "CHB_BOTTOM")) {
      out |= wxCHB_BOTTOM;
      }
    if(!strcmp(style_cstr, "CLRP_DEFAULT_STYLE")) {
      out |= wxCLRP_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "CLRP_USE_TEXTCTRL")) {
      out |= wxCLRP_USE_TEXTCTRL;
      }
    if(!strcmp(style_cstr, "CLRP_SHOW_LABEL")) {
      out |= wxCLRP_SHOW_LABEL;
      }
    if(!strcmp(style_cstr, "CLRP_SHOW_ALPHA")) {
      out |= wxCLRP_SHOW_ALPHA;
      }
    if(!strcmp(style_cstr, "CP_DEFAULT_STYLE")) {
      out |= wxCP_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "CP_NO_TLW_RESIZE")) {
      out |= wxCP_NO_TLW_RESIZE;
      }
    if(!strcmp(style_cstr, "CB_READONLY")) {
      out |= wxCB_READONLY;
      }
    if(!strcmp(style_cstr, "CB_SORT")) {
      out |= wxCB_SORT;
      }
    if(!strcmp(style_cstr, "TE_PROCESS_ENTER")) {
      out |= wxTE_PROCESS_ENTER;
      }
    if(!strcmp(style_cstr, "CC_SPECIAL_DCLICK")) {
      out |= wxCC_SPECIAL_DCLICK;
      }
    if(!strcmp(style_cstr, "CC_STD_BUTTON")) {
      out |= wxCC_STD_BUTTON;
      }
    if(!strcmp(style_cstr, "CB_SIMPLE")) {
      out |= wxCB_SIMPLE;
      }
    if(!strcmp(style_cstr, "CB_DROPDOWN")) {
      out |= wxCB_DROPDOWN;
      }
    if(!strcmp(style_cstr, "CB_READONLY")) {
      out |= wxCB_READONLY;
      }
    if(!strcmp(style_cstr, "CB_SORT")) {
      out |= wxCB_SORT;
      }
    if(!strcmp(style_cstr, "TE_PROCESS_ENTER")) {
      out |= wxTE_PROCESS_ENTER;
      }
    if(!strcmp(style_cstr, "DV_SINGLE")) {
      out |= wxDV_SINGLE;
      }
    if(!strcmp(style_cstr, "DV_MULTIPLE")) {
      out |= wxDV_MULTIPLE;
      }
    if(!strcmp(style_cstr, "DV_ROW_LINES")) {
      out |= wxDV_ROW_LINES;
      }
    if(!strcmp(style_cstr, "DV_HORIZ_RULES")) {
      out |= wxDV_HORIZ_RULES;
      }
    if(!strcmp(style_cstr, "DV_VERT_RULES")) {
      out |= wxDV_VERT_RULES;
      }
    if(!strcmp(style_cstr, "DV_VARIABLE_LINE_HEIGHT")) {
      out |= wxDV_VARIABLE_LINE_HEIGHT;
      }
    if(!strcmp(style_cstr, "DV_NO_HEADER")) {
      out |= wxDV_NO_HEADER;
      }
    if(!strcmp(style_cstr, "DP_SPIN")) {
      out |= wxDP_SPIN;
      }
    if(!strcmp(style_cstr, "DP_DROPDOWN")) {
      out |= wxDP_DROPDOWN;
      }
    if(!strcmp(style_cstr, "DP_DEFAULT")) {
      out |= wxDP_DEFAULT;
      }
    if(!strcmp(style_cstr, "DP_ALLOWNONE")) {
      out |= wxDP_ALLOWNONE;
      }
    if(!strcmp(style_cstr, "DP_SHOWCENTURY")) {
      out |= wxDP_SHOWCENTURY;
      }
    if(!strcmp(style_cstr, "CAPTION")) {
      out |= wxCAPTION;
      }
    if(!strcmp(style_cstr, "DEFAULT_DIALOG_STYLE")) {
      out |= wxDEFAULT_DIALOG_STYLE;
      }
    if(!strcmp(style_cstr, "RESIZE_BORDER")) {
      out |= wxRESIZE_BORDER;
      }
    if(!strcmp(style_cstr, "SYSTEM_MENU")) {
      out |= wxSYSTEM_MENU;
      }
    if(!strcmp(style_cstr, "CLOSE_BOX")) {
      out |= wxCLOSE_BOX;
      }
    if(!strcmp(style_cstr, "MAXIMIZE_BOX")) {
      out |= wxMAXIMIZE_BOX;
      }
    if(!strcmp(style_cstr, "MINIMIZE_BOX")) {
      out |= wxMINIMIZE_BOX;
      }
    //if(!strcmp(style_cstr, "THICK_FRAME")) { // got removed
    //    out |= wxTHICK_FRAME;
    //  }
    if(!strcmp(style_cstr, "STAY_ON_TOP")) {
      out |= wxSTAY_ON_TOP;
      }
    //if(!strcmp(style_cstr, "NO_3D")) { // obselete
    //  out |= wxNO_3D;
    //  }
    if(!strcmp(style_cstr, "DIALOG_NO_PARENT")) {
      out |= wxDIALOG_NO_PARENT;
      }
    if(!strcmp(style_cstr, "DIALOG_EX_CONTEXTHELP")) {
      out |= wxDIALOG_EX_CONTEXTHELP;
      }
    if(!strcmp(style_cstr, "DIALOG_EX_METAL")) {
      out |= wxDIALOG_EX_METAL;
      }
    if(!strcmp(style_cstr, "DIRCTRL_DIR_ONLY")) {
      out |= wxDIRCTRL_DIR_ONLY;
      }
    if(!strcmp(style_cstr, "DIRCTRL_3D_INTERNAL")) {
      out |= wxDIRCTRL_3D_INTERNAL;
      }
    if(!strcmp(style_cstr, "DIRCTRL_SELECT_FIRST")) {
      out |= wxDIRCTRL_SELECT_FIRST;
      }
    if(!strcmp(style_cstr, "DIRCTRL_SHOW_FILTERS")) {
      out |= wxDIRCTRL_SHOW_FILTERS;
      }
    if(!strcmp(style_cstr, "DIRCTRL_EDIT_LABELS")) {
      out |= wxDIRCTRL_EDIT_LABELS;
      }
    if(!strcmp(style_cstr, "DIRCTRL_MULTIPLE")) {
      out |= wxDIRCTRL_MULTIPLE;
      }
    if(!strcmp(style_cstr, "DD_DEFAULT_STYLE")) {
      out |= wxDD_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "DD_DIR_MUST_EXIST")) {
      out |= wxDD_DIR_MUST_EXIST;
      }
    if(!strcmp(style_cstr, "DD_CHANGE_DIR")) {
      out |= wxDD_CHANGE_DIR;
      }
    if(!strcmp(style_cstr, "DD_MULTIPLE")) {
      out |= wxDD_MULTIPLE;
      }
    if(!strcmp(style_cstr, "DD_SHOW_HIDDEN")) {
      out |= wxDD_SHOW_HIDDEN;
      }
    if(!strcmp(style_cstr, "DL_LAZY")) {
      out |= wxDL_LAZY;
      }
    if(!strcmp(style_cstr, "DL_NOW")) {
      out |= wxDL_NOW;
      }
    if(!strcmp(style_cstr, "DL_GLOBAL")) {
      out |= wxDL_GLOBAL;
      }
    if(!strcmp(style_cstr, "DL_VERBATIM")) {
      out |= wxDL_VERBATIM;
      }
    if(!strcmp(style_cstr, "DL_DEFAULT")) {
      out |= wxDL_DEFAULT;
      }
    if(!strcmp(style_cstr, "DL_QUIET")) {
      out |= wxDL_QUIET;
      }
    if(!strcmp(style_cstr, "EL_ALLOW_NEW")) {
      out |= wxEL_ALLOW_NEW;
      }
    if(!strcmp(style_cstr, "EL_ALLOW_EDIT")) {
      out |= wxEL_ALLOW_EDIT;
      }
    if(!strcmp(style_cstr, "EL_ALLOW_DELETE")) {
      out |= wxEL_ALLOW_DELETE;
      }
    if(!strcmp(style_cstr, "EL_NO_REORDER")) {
      out |= wxEL_NO_REORDER;
      }
    if(!strcmp(style_cstr, "EL_DEFAULT_STYLE")) {
      out |= wxEL_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "FC_DEFAULT_STYLE")) {
      out |= wxFC_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "FC_OPEN")) {
      out |= wxFC_OPEN;
      }
    if(!strcmp(style_cstr, "FC_SAVE")) {
      out |= wxFC_SAVE;
      }
    if(!strcmp(style_cstr, "FC_MULTIPLE")) {
      out |= wxFC_MULTIPLE;
      }
    if(!strcmp(style_cstr, "FC_NOSHOWHIDDEN")) {
      out |= wxFC_NOSHOWHIDDEN;
      }
    if(!strcmp(style_cstr, "FD_DEFAULT_STYLE")) {
      out |= wxFD_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "FD_OPEN")) {
      out |= wxFD_OPEN;
      }
    if(!strcmp(style_cstr, "FD_SAVE")) {
      out |= wxFD_SAVE;
      }
    if(!strcmp(style_cstr, "FD_OVERWRITE_PROMPT")) {
      out |= wxFD_OVERWRITE_PROMPT;
      }
    if(!strcmp(style_cstr, "FD_NO_FOLLOW")) {
      out |= wxFD_NO_FOLLOW;
      }
    if(!strcmp(style_cstr, "FD_FILE_MUST_EXIST")) {
      out |= wxFD_FILE_MUST_EXIST;
      }
    if(!strcmp(style_cstr, "FD_MULTIPLE")) {
      out |= wxFD_MULTIPLE;
      }
    if(!strcmp(style_cstr, "FD_CHANGE_DIR")) {
      out |= wxFD_CHANGE_DIR;
      }
    if(!strcmp(style_cstr, "FD_PREVIEW")) {
      out |= wxFD_PREVIEW;
      }
    if(!strcmp(style_cstr, "FD_SHOW_HIDDEN")) {
      out |= wxFD_SHOW_HIDDEN;
      }
    if(!strcmp(style_cstr, "FLP_DEFAULT_STYLE")) {
      out |= wxFLP_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "FLP_USE_TEXTCTRL")) {
      out |= wxFLP_USE_TEXTCTRL;
      }
    if(!strcmp(style_cstr, "FLP_OPEN")) {
      out |= wxFLP_OPEN;
      }
    if(!strcmp(style_cstr, "FLP_SAVE")) {
      out |= wxFLP_SAVE;
      }
    if(!strcmp(style_cstr, "FLP_OVERWRITE_PROMPT")) {
      out |= wxFLP_OVERWRITE_PROMPT;
      }
    if(!strcmp(style_cstr, "FLP_FILE_MUST_EXIST")) {
      out |= wxFLP_FILE_MUST_EXIST;
      }
    if(!strcmp(style_cstr, "FLP_CHANGE_DIR")) {
      out |= wxFLP_CHANGE_DIR;
      }
    if(!strcmp(style_cstr, "FLP_SMALL")) {
      out |= wxFLP_SMALL;
      }
    if(!strcmp(style_cstr, "DIRP_DEFAULT_STYLE")) {
      out |= wxDIRP_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "DIRP_USE_TEXTCTRL")) {
      out |= wxDIRP_USE_TEXTCTRL;
      }
    if(!strcmp(style_cstr, "DIRP_DIR_MUST_EXIST")) {
      out |= wxDIRP_DIR_MUST_EXIST;
      }
    if(!strcmp(style_cstr, "DIRP_CHANGE_DIR")) {
      out |= wxDIRP_CHANGE_DIR;
      }
    if(!strcmp(style_cstr, "DIRP_SMALL")) {
      out |= wxDIRP_SMALL;
      }
    if(!strcmp(style_cstr, "FNTP_DEFAULT_STYLE")) {
      out |= wxFNTP_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "FNTP_USE_TEXTCTRL")) {
      out |= wxFNTP_USE_TEXTCTRL;
      }
    if(!strcmp(style_cstr, "FNTP_FONTDESC_AS_LABEL")) {
      out |= wxFNTP_FONTDESC_AS_LABEL;
      }
    if(!strcmp(style_cstr, "FNTP_USEFONT_FOR_LABEL")) {
      out |= wxFNTP_USEFONT_FOR_LABEL;
      }
    if(!strcmp(style_cstr, "DEFAULT_FRAME_STYLE")) {
      out |= wxDEFAULT_FRAME_STYLE;
      }
    if(!strcmp(style_cstr, "ICONIZE")) {
      out |= wxICONIZE;
      }
    if(!strcmp(style_cstr, "CAPTION")) {
      out |= wxCAPTION;
      }
    if(!strcmp(style_cstr, "MINIMIZE")) {
      out |= wxMINIMIZE;
      }
    if(!strcmp(style_cstr, "MINIMIZE_BOX")) {
      out |= wxMINIMIZE_BOX;
      }
    if(!strcmp(style_cstr, "MAXIMIZE")) {
      out |= wxMAXIMIZE;
      }
    if(!strcmp(style_cstr, "MAXIMIZE_BOX")) {
      out |= wxMAXIMIZE_BOX;
      }
    if(!strcmp(style_cstr, "CLOSE_BOX")) {
      out |= wxCLOSE_BOX;
      }
    if(!strcmp(style_cstr, "STAY_ON_TOP")) {
      out |= wxSTAY_ON_TOP;
      }
    if(!strcmp(style_cstr, "SYSTEM_MENU")) {
      out |= wxSYSTEM_MENU;
      }
    if(!strcmp(style_cstr, "RESIZE_BORDER")) {
      out |= wxRESIZE_BORDER;
      }
    if(!strcmp(style_cstr, "FRAME_TOOL_WINDOW")) {
      out |= wxFRAME_TOOL_WINDOW;
      }
    if(!strcmp(style_cstr, "FRAME_NO_TASKBAR")) {
      out |= wxFRAME_NO_TASKBAR;
      }
    if(!strcmp(style_cstr, "FRAME_FLOAT_ON_PARENT")) {
      out |= wxFRAME_FLOAT_ON_PARENT;
      }
    if(!strcmp(style_cstr, "FRAME_SHAPED")) {
      out |= wxFRAME_SHAPED;
      }
    if(!strcmp(style_cstr, "FRAME_EX_CONTEXTHELP")) {
      out |= wxFRAME_EX_CONTEXTHELP;
      }
    if(!strcmp(style_cstr, "FRAME_EX_METAL")) {
      out |= wxFRAME_EX_METAL;
      }
    if(!strcmp(style_cstr, "GA_HORIZONTAL")) {
      out |= wxGA_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "GA_VERTICAL")) {
      out |= wxGA_VERTICAL;
      }
    if(!strcmp(style_cstr, "GA_SMOOTH")) {
      out |= wxGA_SMOOTH;
      }
    if(!strcmp(style_cstr, "GA_TEXT")) {
      out |= wxGA_TEXT;
      }
    if(!strcmp(style_cstr, "GA_PROGRESS")) {
      out |= wxGA_PROGRESS;
      }
    if(!strcmp(style_cstr, "HD_ALLOW_REORDER")) {
      out |= wxHD_ALLOW_REORDER;
      }
    if(!strcmp(style_cstr, "HD_ALLOW_HIDE")) {
      out |= wxHD_ALLOW_HIDE;
      }
    if(!strcmp(style_cstr, "HD_BITMAP_ON_RIGHT")) {
      out |= wxHD_BITMAP_ON_RIGHT;
      }
    if(!strcmp(style_cstr, "HD_DEFAULT_STYLE")) {
      out |= wxHD_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "HLB_DEFAULT_STYLE")) {
      out |= wxHLB_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "HLB_MULTIPLE")) {
      out |= wxHLB_MULTIPLE;
      }
    if(!strcmp(style_cstr, "HL_ALIGN_LEFT")) {
      out |= wxHL_ALIGN_LEFT;
      }
    if(!strcmp(style_cstr, "HL_ALIGN_RIGHT")) {
      out |= wxHL_ALIGN_RIGHT;
      }
    if(!strcmp(style_cstr, "HL_ALIGN_CENTRE")) {
      out |= wxHL_ALIGN_CENTRE;
      }
    if(!strcmp(style_cstr, "HL_CONTEXTMENU")) {
      out |= wxHL_CONTEXTMENU;
      }
    if(!strcmp(style_cstr, "HL_DEFAULT_STYLE")) {
      out |= wxHL_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "LB_DEFAULT")) {
      out |= wxLB_DEFAULT;
      }
    if(!strcmp(style_cstr, "LB_TOP")) {
      out |= wxLB_TOP;
      }
    if(!strcmp(style_cstr, "LB_LEFT")) {
      out |= wxLB_LEFT;
      }
    if(!strcmp(style_cstr, "LB_RIGHT")) {
      out |= wxLB_RIGHT;
      }
    if(!strcmp(style_cstr, "LB_BOTTOM")) {
      out |= wxLB_BOTTOM;
      }
    if(!strcmp(style_cstr, "LB_SINGLE")) {
      out |= wxLB_SINGLE;
      }
    if(!strcmp(style_cstr, "LB_MULTIPLE")) {
      out |= wxLB_MULTIPLE;
      }
    if(!strcmp(style_cstr, "LB_EXTENDED")) {
      out |= wxLB_EXTENDED;
      }
    if(!strcmp(style_cstr, "LB_HSCROLL")) {
      out |= wxLB_HSCROLL;
      }
    if(!strcmp(style_cstr, "LB_ALWAYS_SB")) {
      out |= wxLB_ALWAYS_SB;
      }
    if(!strcmp(style_cstr, "LB_NEEDED_SB")) {
      out |= wxLB_NEEDED_SB;
      }
    if(!strcmp(style_cstr, "LB_NO_SB")) {
      out |= wxLB_NO_SB;
      }
    if(!strcmp(style_cstr, "LB_SORT")) {
      out |= wxLB_SORT;
      }
    if(!strcmp(style_cstr, "LC_LIST")) {
      out |= wxLC_LIST;
      }
    if(!strcmp(style_cstr, "LC_REPORT")) {
      out |= wxLC_REPORT;
      }
    if(!strcmp(style_cstr, "LC_VIRTUAL")) {
      out |= wxLC_VIRTUAL;
      }
    if(!strcmp(style_cstr, "LC_ICON")) {
      out |= wxLC_ICON;
      }
    if(!strcmp(style_cstr, "LC_SMALL_ICON")) {
      out |= wxLC_SMALL_ICON;
      }
    if(!strcmp(style_cstr, "LC_ALIGN_TOP")) {
      out |= wxLC_ALIGN_TOP;
      }
    if(!strcmp(style_cstr, "LC_ALIGN_LEFT")) {
      out |= wxLC_ALIGN_LEFT;
      }
    if(!strcmp(style_cstr, "LC_AUTOARRANGE")) {
      out |= wxLC_AUTOARRANGE;
      }
    if(!strcmp(style_cstr, "LC_EDIT_LABELS")) {
      out |= wxLC_EDIT_LABELS;
      }
    if(!strcmp(style_cstr, "LC_NO_HEADER")) {
      out |= wxLC_NO_HEADER;
      }
    if(!strcmp(style_cstr, "LC_SINGLE_SEL")) {
      out |= wxLC_SINGLE_SEL;
      }
    if(!strcmp(style_cstr, "LC_SORT_ASCENDING")) {
      out |= wxLC_SORT_ASCENDING;
      }
    if(!strcmp(style_cstr, "LC_SORT_DESCENDING")) {
      out |= wxLC_SORT_DESCENDING;
      }
    if(!strcmp(style_cstr, "LC_HRULES")) {
      out |= wxLC_HRULES;
      }
    if(!strcmp(style_cstr, "LC_VRULES")) {
      out |= wxLC_VRULES;
      }
    if(!strcmp(style_cstr, "MC_NO_AUTORESIZE")) {
      out |= wxMC_NO_AUTORESIZE;
      }
    if(!strcmp(style_cstr, "ICONIZE")) {
      out |= wxICONIZE;
      }
    if(!strcmp(style_cstr, "CAPTION")) {
      out |= wxCAPTION;
      }
    if(!strcmp(style_cstr, "MINIMIZE")) {
      out |= wxMINIMIZE;
      }
    if(!strcmp(style_cstr, "MINIMIZE_BOX")) {
      out |= wxMINIMIZE_BOX;
      }
    if(!strcmp(style_cstr, "MAXIMIZE")) {
      out |= wxMAXIMIZE;
      }
    if(!strcmp(style_cstr, "MAXIMIZE_BOX")) {
      out |= wxMAXIMIZE_BOX;
      }
    if(!strcmp(style_cstr, "CLOSE_BOX")) {
      out |= wxCLOSE_BOX;
      }
    if(!strcmp(style_cstr, "STAY_ON_TOP")) {
      out |= wxSTAY_ON_TOP;
      }
    if(!strcmp(style_cstr, "SYSTEM_MENU")) {
      out |= wxSYSTEM_MENU;
      }
    if(!strcmp(style_cstr, "RESIZE_BORDER")) {
      out |= wxRESIZE_BORDER;
      }
    if(!strcmp(style_cstr, "OK")) {
      out |= wxOK;
      }
    if(!strcmp(style_cstr, "CANCEL")) {
      out |= wxCANCEL;
      }
    if(!strcmp(style_cstr, "YES_NO")) {
      out |= wxYES_NO;
      }
    if(!strcmp(style_cstr, "HELP")) {
      out |= wxHELP;
      }
    if(!strcmp(style_cstr, "NO_DEFAULT")) {
      out |= wxNO_DEFAULT;
      }
    if(!strcmp(style_cstr, "CANCEL_DEFAULT")) {
      out |= wxCANCEL_DEFAULT;
      }
    if(!strcmp(style_cstr, "YES_DEFAULT")) {
      out |= wxYES_DEFAULT;
      }
    if(!strcmp(style_cstr, "OK_DEFAULT")) {
      out |= wxOK_DEFAULT;
      }
    if(!strcmp(style_cstr, "ICON_NONE")) {
      out |= wxICON_NONE;
      }
    if(!strcmp(style_cstr, "ICON_ERROR")) {
      out |= wxICON_ERROR;
      }
    if(!strcmp(style_cstr, "ICON_WARNING")) {
      out |= wxICON_WARNING;
      }
    if(!strcmp(style_cstr, "ICON_QUESTION")) {
      out |= wxICON_QUESTION;
      }
    if(!strcmp(style_cstr, "ICON_INFORMATION")) {
      out |= wxICON_INFORMATION;
      }
    if(!strcmp(style_cstr, "ICON_EXCLAMATION")) {
      out |= wxICON_EXCLAMATION;
      }
    if(!strcmp(style_cstr, "ICON_HAND")) {
      out |= wxICON_HAND;
      }
    if(!strcmp(style_cstr, "ICON_AUTH_NEEDED")) {
      out |= wxICON_AUTH_NEEDED;
      }
    if(!strcmp(style_cstr, "STAY_ON_TOP")) {
      out |= wxSTAY_ON_TOP;
      }
    if(!strcmp(style_cstr, "CENTRE")) {
      out |= wxCENTRE;
      }
    if(!strcmp(style_cstr, "NB_TOP")) {
      out |= wxNB_TOP;
      }
    if(!strcmp(style_cstr, "NB_LEFT")) {
      out |= wxNB_LEFT;
      }
    if(!strcmp(style_cstr, "NB_RIGHT")) {
      out |= wxNB_RIGHT;
      }
    if(!strcmp(style_cstr, "NB_BOTTOM")) {
      out |= wxNB_BOTTOM;
      }
    if(!strcmp(style_cstr, "NB_FIXEDWIDTH")) {
      out |= wxNB_FIXEDWIDTH;
      }
    if(!strcmp(style_cstr, "NB_MULTILINE")) {
      out |= wxNB_MULTILINE;
      }
    if(!strcmp(style_cstr, "NB_NOPAGETHEME")) {
      out |= wxNB_NOPAGETHEME;
      }
    if(!strcmp(style_cstr, "ODCB_DCLICK_CYCLES")) {
      out |= wxODCB_DCLICK_CYCLES;
      }
    if(!strcmp(style_cstr, "ODCB_STD_CONTROL_PAINT")) {
      out |= wxODCB_STD_CONTROL_PAINT;
      }
    if(!strcmp(style_cstr, "PB_USE_TEXTCTRL")) {
      out |= wxPB_USE_TEXTCTRL;
      }
    if(!strcmp(style_cstr, "PU_CONTAINS_CONTROLS")) {
      out |= wxPU_CONTAINS_CONTROLS;
      }
    if(!strcmp(style_cstr, "PD_APP_MODAL")) {
      out |= wxPD_APP_MODAL;
      }
    if(!strcmp(style_cstr, "PD_AUTO_HIDE")) {
      out |= wxPD_AUTO_HIDE;
      }
    if(!strcmp(style_cstr, "PD_SMOOTH")) {
      out |= wxPD_SMOOTH;
      }
    if(!strcmp(style_cstr, "PD_CAN_ABORT")) {
      out |= wxPD_CAN_ABORT;
      }
    if(!strcmp(style_cstr, "PD_CAN_SKIP")) {
      out |= wxPD_CAN_SKIP;
      }
    if(!strcmp(style_cstr, "PD_ELAPSED_TIME")) {
      out |= wxPD_ELAPSED_TIME;
      }
    if(!strcmp(style_cstr, "PD_ESTIMATED_TIME")) {
      out |= wxPD_ESTIMATED_TIME;
      }
    if(!strcmp(style_cstr, "PD_REMAINING_TIME")) {
      out |= wxPD_REMAINING_TIME;
      }
    if(!strcmp(style_cstr, "RA_SPECIFY_ROWS")) {
      out |= wxRA_SPECIFY_ROWS;
      }
    if(!strcmp(style_cstr, "RA_SPECIFY_COLS")) {
      out |= wxRA_SPECIFY_COLS;
      }
    if(!strcmp(style_cstr, "RB_GROUP")) {
      out |= wxRB_GROUP;
      }
    if(!strcmp(style_cstr, "RB_SINGLE")) {
      out |= wxRB_SINGLE;
      }
    if(!strcmp(style_cstr, "SW_3D")) {
      out |= wxSW_3D;
      }
    if(!strcmp(style_cstr, "SW_3DSASH")) {
      out |= wxSW_3DSASH;
      }
    if(!strcmp(style_cstr, "SW_3DBORDER")) {
      out |= wxSW_3DBORDER;
      }
    if(!strcmp(style_cstr, "SW_BORDER")) {
      out |= wxSW_BORDER;
      }
    if(!strcmp(style_cstr, "SB_HORIZONTAL")) {
      out |= wxSB_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "SB_VERTICAL")) {
      out |= wxSB_VERTICAL;
      }
    if(!strcmp(style_cstr, "HSCROLL")) {
      out |= wxHSCROLL;
      }
    if(!strcmp(style_cstr, "VSCROLL")) {
      out |= wxVSCROLL;
      }
    if(!strcmp(style_cstr, "ALWAYS_SHOW_SB")) {
      out |= wxALWAYS_SHOW_SB;
      }
    if(!strcmp(style_cstr, "RETAINED")) {
      out |= wxRETAINED;
      }
    if(!strcmp(style_cstr, "SL_HORIZONTAL")) {
      out |= wxSL_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "SL_VERTICAL")) {
      out |= wxSL_VERTICAL;
      }
    if(!strcmp(style_cstr, "SL_AUTOTICKS")) {
      out |= wxSL_AUTOTICKS;
      }
    if(!strcmp(style_cstr, "SL_MIN_MAX_LABELS")) {
      out |= wxSL_MIN_MAX_LABELS;
      }
    if(!strcmp(style_cstr, "SL_VALUE_LABEL")) {
      out |= wxSL_VALUE_LABEL;
      }
    if(!strcmp(style_cstr, "SL_LABELS")) {
      out |= wxSL_LABELS;
      }
    if(!strcmp(style_cstr, "SL_LEFT")) {
      out |= wxSL_LEFT;
      }
    if(!strcmp(style_cstr, "SL_RIGHT")) {
      out |= wxSL_RIGHT;
      }
    if(!strcmp(style_cstr, "SL_TOP")) {
      out |= wxSL_TOP;
      }
    if(!strcmp(style_cstr, "SL_BOTTOM")) {
      out |= wxSL_BOTTOM;
      }
    if(!strcmp(style_cstr, "SL_BOTH")) {
      out |= wxSL_BOTH;
      }
    if(!strcmp(style_cstr, "SL_SELRANGE")) {
      out |= wxSL_SELRANGE;
      }
    if(!strcmp(style_cstr, "SL_INVERSE")) {
      out |= wxSL_INVERSE;
      }
    if(!strcmp(style_cstr, "SP_HORIZONTAL")) {
      out |= wxSP_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "SP_VERTICAL")) {
      out |= wxSP_VERTICAL;
      }
    if(!strcmp(style_cstr, "SP_ARROW_KEYS")) {
      out |= wxSP_ARROW_KEYS;
      }
    if(!strcmp(style_cstr, "SP_WRAP")) {
      out |= wxSP_WRAP;
      }
    if(!strcmp(style_cstr, "SP_ARROW_KEYS")) {
      out |= wxSP_ARROW_KEYS;
      }
    if(!strcmp(style_cstr, "SP_WRAP")) {
      out |= wxSP_WRAP;
      }
    if(!strcmp(style_cstr, "TE_PROCESS_ENTER")) {
      out |= wxTE_PROCESS_ENTER;
      }
    if(!strcmp(style_cstr, "ALIGN_LEFT")) {
      out |= wxALIGN_LEFT;
      }
    if(!strcmp(style_cstr, "ALIGN_CENTRE_HORIZONTAL")) {
      out |= wxALIGN_CENTRE_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "ALIGN_RIGHT")) {
      out |= wxALIGN_RIGHT;
      }
    if(!strcmp(style_cstr, "SP_ARROW_KEYS")) {
      out |= wxSP_ARROW_KEYS;
      }
    if(!strcmp(style_cstr, "SP_WRAP")) {
      out |= wxSP_WRAP;
      }
    if(!strcmp(style_cstr, "SP_3D")) {
      out |= wxSP_3D;
      }
    if(!strcmp(style_cstr, "SP_THIN_SASH")) {
      out |= wxSP_THIN_SASH;
      }
    if(!strcmp(style_cstr, "SP_3DSASH")) {
      out |= wxSP_3DSASH;
      }
    if(!strcmp(style_cstr, "SP_3DBORDER")) {
      out |= wxSP_3DBORDER;
      }
    if(!strcmp(style_cstr, "SP_BORDER")) {
      out |= wxSP_BORDER;
      }
    if(!strcmp(style_cstr, "SP_NOBORDER")) {
      out |= wxSP_NOBORDER;
      }
    if(!strcmp(style_cstr, "SP_NO_XP_THEME")) {
      out |= wxSP_NO_XP_THEME;
      }
    if(!strcmp(style_cstr, "SP_PERMIT_UNSPLIT")) {
      out |= wxSP_PERMIT_UNSPLIT;
      }
    if(!strcmp(style_cstr, "SP_LIVE_UPDATE")) {
      out |= wxSP_LIVE_UPDATE;
      }
    if(!strcmp(style_cstr, "TE_PROCESS_TAB")) {
      out |= wxTE_PROCESS_TAB;
      }
    if(!strcmp(style_cstr, "TE_NOHIDESEL")) {
      out |= wxTE_NOHIDESEL;
      }
    if(!strcmp(style_cstr, "TE_LEFT")) {
      out |= wxTE_LEFT;
      }
    if(!strcmp(style_cstr, "TE_CENTRE")) {
      out |= wxTE_CENTRE;
      }
    if(!strcmp(style_cstr, "TE_RIGHT")) {
      out |= wxTE_RIGHT;
      }
    if(!strcmp(style_cstr, "TE_CAPITALIZE")) {
      out |= wxTE_CAPITALIZE;
      }
    if(!strcmp(style_cstr, "LI_HORIZONTAL")) {
      out |= wxLI_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "LI_VERTICAL")) {
      out |= wxLI_VERTICAL;
      }
    if(!strcmp(style_cstr, "ALIGN_LEFT")) {
      out |= wxALIGN_LEFT;
      }
    if(!strcmp(style_cstr, "ALIGN_RIGHT")) {
      out |= wxALIGN_RIGHT;
      }
    if(!strcmp(style_cstr, "ALIGN_CENTRE_HORIZONTAL")) {
      out |= wxALIGN_CENTRE_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "ST_NO_AUTORESIZE")) {
      out |= wxST_NO_AUTORESIZE;
      }
    if(!strcmp(style_cstr, "ST_ELLIPSIZE_START")) {
      out |= wxST_ELLIPSIZE_START;
      }
    if(!strcmp(style_cstr, "ST_ELLIPSIZE_MIDDLE")) {
      out |= wxST_ELLIPSIZE_MIDDLE;
      }
    if(!strcmp(style_cstr, "ST_ELLIPSIZE_END")) {
      out |= wxST_ELLIPSIZE_END;
      }
    if(!strcmp(style_cstr, "STB_SIZEGRIP")) {
      out |= wxSTB_SIZEGRIP;
      }
    if(!strcmp(style_cstr, "STB_SHOW_TIPS")) {
      out |= wxSTB_SHOW_TIPS;
      }
    if(!strcmp(style_cstr, "STB_ELLIPSIZE_START")) {
      out |= wxSTB_ELLIPSIZE_START;
      }
    if(!strcmp(style_cstr, "STB_ELLIPSIZE_MIDDLE")) {
      out |= wxSTB_ELLIPSIZE_MIDDLE;
      }
    if(!strcmp(style_cstr, "STB_ELLIPSIZE_END")) {
      out |= wxSTB_ELLIPSIZE_END;
      }
    if(!strcmp(style_cstr, "STB_DEFAULT_STYLE")) {
      out |= wxSTB_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "TE_PROCESS_ENTER")) {
      out |= wxTE_PROCESS_ENTER;
      }
    if(!strcmp(style_cstr, "TE_PROCESS_TAB")) {
      out |= wxTE_PROCESS_TAB;
      }
    if(!strcmp(style_cstr, "TE_MULTILINE")) {
      out |= wxTE_MULTILINE;
      }
    if(!strcmp(style_cstr, "TE_PASSWORD")) {
      out |= wxTE_PASSWORD;
      }
    if(!strcmp(style_cstr, "TE_READONLY")) {
      out |= wxTE_READONLY;
      }
    if(!strcmp(style_cstr, "TE_RICH")) {
      out |= wxTE_RICH;
      }
    if(!strcmp(style_cstr, "TE_RICH2")) {
      out |= wxTE_RICH2;
      }
    if(!strcmp(style_cstr, "TE_AUTO_URL")) {
      out |= wxTE_AUTO_URL;
      }
    if(!strcmp(style_cstr, "TE_NOHIDESEL")) {
      out |= wxTE_NOHIDESEL;
      }
    if(!strcmp(style_cstr, "HSCROLL")) {
      out |= wxHSCROLL;
      }
    if(!strcmp(style_cstr, "TE_NO_VSCROLL")) {
      out |= wxTE_NO_VSCROLL;
      }
    if(!strcmp(style_cstr, "TE_LEFT")) {
      out |= wxTE_LEFT;
      }
    if(!strcmp(style_cstr, "TE_CENTRE")) {
      out |= wxTE_CENTRE;
      }
    if(!strcmp(style_cstr, "TE_RIGHT")) {
      out |= wxTE_RIGHT;
      }
    if(!strcmp(style_cstr, "TE_DONTWRAP")) {
      out |= wxTE_DONTWRAP;
      }
    if(!strcmp(style_cstr, "TE_CHARWRAP")) {
      out |= wxTE_CHARWRAP;
      }
    if(!strcmp(style_cstr, "TE_WORDWRAP")) {
      out |= wxTE_WORDWRAP;
      }
    if(!strcmp(style_cstr, "TE_BESTWRAP")) {
      out |= wxTE_BESTWRAP;
      }
    if(!strcmp(style_cstr, "TE_CAPITALIZE")) {
      out |= wxTE_CAPITALIZE;
      }
    if(!strcmp(style_cstr, "TB_FLAT")) {
      out |= wxTB_FLAT;
      }
    if(!strcmp(style_cstr, "TB_DOCKABLE")) {
      out |= wxTB_DOCKABLE;
      }
    if(!strcmp(style_cstr, "TB_HORIZONTAL")) {
      out |= wxTB_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "TB_VERTICAL")) {
      out |= wxTB_VERTICAL;
      }
    if(!strcmp(style_cstr, "TB_TEXT")) {
      out |= wxTB_TEXT;
      }
    if(!strcmp(style_cstr, "TB_NOICONS")) {
      out |= wxTB_NOICONS;
      }
    if(!strcmp(style_cstr, "TB_NODIVIDER")) {
      out |= wxTB_NODIVIDER;
      }
    if(!strcmp(style_cstr, "TB_NOALIGN")) {
      out |= wxTB_NOALIGN;
      }
    if(!strcmp(style_cstr, "TB_HORZ_LAYOUT")) {
      out |= wxTB_HORZ_LAYOUT;
      }
    if(!strcmp(style_cstr, "TB_HORZ_TEXT")) {
      out |= wxTB_HORZ_TEXT;
      }
    if(!strcmp(style_cstr, "TB_NO_TOOLTIPS")) {
      out |= wxTB_NO_TOOLTIPS;
      }
    if(!strcmp(style_cstr, "TB_BOTTOM")) {
      out |= wxTB_BOTTOM;
      }
    if(!strcmp(style_cstr, "TB_RIGHT")) {
      out |= wxTB_RIGHT;
      }
    if(!strcmp(style_cstr, "TB_DEFAULT_STYLE")) {
      out |= wxTB_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "TBK_BUTTONBAR")) {
      out |= wxTBK_BUTTONBAR;
      }
    if(!strcmp(style_cstr, "TBK_HORZ_LAYOUT")) {
      out |= wxTBK_HORZ_LAYOUT;
      }
    if(!strcmp(style_cstr, "TR_EDIT_LABELS")) {
      out |= wxTR_EDIT_LABELS;
      }
    if(!strcmp(style_cstr, "TR_NO_BUTTONS")) {
      out |= wxTR_NO_BUTTONS;
      }
    if(!strcmp(style_cstr, "TR_HAS_BUTTONS")) {
      out |= wxTR_HAS_BUTTONS;
      }
    if(!strcmp(style_cstr, "TR_TWIST_BUTTONS")) {
      out |= wxTR_TWIST_BUTTONS;
      }
    if(!strcmp(style_cstr, "TR_NO_LINES")) {
      out |= wxTR_NO_LINES;
      }
    if(!strcmp(style_cstr, "TR_FULL_ROW_HIGHLIGHT")) {
      out |= wxTR_FULL_ROW_HIGHLIGHT;
      }
    if(!strcmp(style_cstr, "TR_LINES_AT_ROOT")) {
      out |= wxTR_LINES_AT_ROOT;
      }
    if(!strcmp(style_cstr, "TR_HIDE_ROOT")) {
      out |= wxTR_HIDE_ROOT;
      }
    if(!strcmp(style_cstr, "TR_ROW_LINES")) {
      out |= wxTR_ROW_LINES;
      }
    if(!strcmp(style_cstr, "TR_HAS_VARIABLE_ROW_HEIGHT")) {
      out |= wxTR_HAS_VARIABLE_ROW_HEIGHT;
      }
    if(!strcmp(style_cstr, "TR_SINGLE")) {
      out |= wxTR_SINGLE;
      }
    if(!strcmp(style_cstr, "TR_MULTIPLE")) {
      out |= wxTR_MULTIPLE;
      }
    if(!strcmp(style_cstr, "TR_DEFAULT_STYLE")) {
      out |= wxTR_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "TL_SINGLE")) {
      out |= wxTL_SINGLE;
      }
    if(!strcmp(style_cstr, "TL_MULTIPLE")) {
      out |= wxTL_MULTIPLE;
      }
    if(!strcmp(style_cstr, "TL_CHECKBOX")) {
      out |= wxTL_CHECKBOX;
      }
    if(!strcmp(style_cstr, "TL_3STATE")) {
      out |= wxTL_3STATE;
      }
    if(!strcmp(style_cstr, "TL_USER_3STATE")) {
      out |= wxTL_USER_3STATE;
      }
    if(!strcmp(style_cstr, "TL_NO_HEADER")) {
      out |= wxTL_NO_HEADER;
      }
    if(!strcmp(style_cstr, "TL_DEFAULT_STYLE")) {
      out |= wxTL_DEFAULT_STYLE;
      }
#pragma region Window Styles
    if(!strcmp(style_cstr, "BORDER_DEFAULT")) {
      out |= wxBORDER_DEFAULT;
      }
    if(!strcmp(style_cstr, "BORDER_SIMPLE")) {
      out |= wxBORDER_SIMPLE;
      }
    if(!strcmp(style_cstr, "BORDER_SUNKEN")) {
      out |= wxBORDER_SUNKEN;
      }
    if(!strcmp(style_cstr, "BORDER_RAISED")) {
      out |= wxBORDER_RAISED;
      }
    if(!strcmp(style_cstr, "BORDER_STATIC")) {
      out |= wxBORDER_STATIC;
      }
    if(!strcmp(style_cstr, "BORDER_THEME")) {
      out |= wxBORDER_THEME;
      }
    if(!strcmp(style_cstr, "BORDER_NONE")) {
      out |= wxBORDER_NONE;
      }
    if(!strcmp(style_cstr, "BORDER_DOUBLE")) {
      out |= wxBORDER_DOUBLE;
      }
    if(!strcmp(style_cstr, "TRANSPARENT_WINDOW")) {
      out |= wxTRANSPARENT_WINDOW;
      }
#pragma endregion
    if(!strcmp(style_cstr, "TAB_TRAVERSAL")) {
      out |= wxTAB_TRAVERSAL;
      }
    if(!strcmp(style_cstr, "WANTS_CHARS")) {
      out |= wxWANTS_CHARS;
      }
    if(!strcmp(style_cstr, "NO_FULL_REPAINT_ON_RESIZE")) {
      out |= wxNO_FULL_REPAINT_ON_RESIZE;
      }
    if(!strcmp(style_cstr, "VSCROLL")) {
      out |= wxVSCROLL;
      }
    if(!strcmp(style_cstr, "HSCROLL")) {
      out |= wxHSCROLL;
      }
    if(!strcmp(style_cstr, "ALWAYS_SHOW_SB")) {
      out |= wxALWAYS_SHOW_SB;
      }
    if(!strcmp(style_cstr, "CLIP_CHILDREN")) {
      out |= wxCLIP_CHILDREN;
      }
    if(!strcmp(style_cstr, "FULL_REPAINT_ON_RESIZE")) {
      out |= wxFULL_REPAINT_ON_RESIZE;
      }
    if(!strcmp(style_cstr, "WS_EX_BLOCK_EVENTS")) {
      out |= wxWS_EX_BLOCK_EVENTS;
      }
    if(!strcmp(style_cstr, "WS_EX_TRANSIENT")) {
      out |= wxWS_EX_TRANSIENT;
      }
    if(!strcmp(style_cstr, "WS_EX_CONTEXTHELP")) {
      out |= wxWS_EX_CONTEXTHELP;
      }
    if(!strcmp(style_cstr, "WS_EX_PROCESS_IDLE")) {
      out |= wxWS_EX_PROCESS_IDLE;
      }
    if(!strcmp(style_cstr, "WS_EX_PROCESS_UI_UPDATES")) {
      out |= wxWS_EX_PROCESS_UI_UPDATES;
      }
    if(!strcmp(style_cstr, "WIZARD_EX_HELPBUTTON")) {
      out |= wxWIZARD_EX_HELPBUTTON;
      }
    if(!strcmp(style_cstr, "AUI_TB_TEXT")) {
      out |= wxAUI_TB_TEXT;
      }
    if(!strcmp(style_cstr, "AUI_TB_NO_TOOLTIPS")) {
      out |= wxAUI_TB_NO_TOOLTIPS;
      }
    if(!strcmp(style_cstr, "AUI_TB_NO_AUTORESIZE")) {
      out |= wxAUI_TB_NO_AUTORESIZE;
      }
    if(!strcmp(style_cstr, "AUI_TB_GRIPPER")) {
      out |= wxAUI_TB_GRIPPER;
      }
    if(!strcmp(style_cstr, "AUI_TB_OVERFLOW")) {
      out |= wxAUI_TB_OVERFLOW;
      }
    if(!strcmp(style_cstr, "AUI_TB_VERTICAL")) {
      out |= wxAUI_TB_VERTICAL;
      }
    if(!strcmp(style_cstr, "AUI_TB_HORZ_LAYOUT")) {
      out |= wxAUI_TB_HORZ_LAYOUT;
      }
    if(!strcmp(style_cstr, "AUI_TB_HORIZONTAL")) {
      out |= wxAUI_TB_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "AUI_TB_PLAIN_BACKGROUND")) {
      out |= wxAUI_TB_PLAIN_BACKGROUND;
      }
    if(!strcmp(style_cstr, "AUI_TB_HORZ_TEXT")) {
      out |= wxAUI_TB_HORZ_TEXT;
      }
    if(!strcmp(style_cstr, "AUI_TB_DEFAULT_STYLE")) {
      out |= wxAUI_TB_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "AUI_NB_DEFAULT_STYLE")) {
      out |= wxAUI_NB_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "AUI_NB_TAB_SPLIT")) {
      out |= wxAUI_NB_TAB_SPLIT;
      }
    if(!strcmp(style_cstr, "AUI_NB_TAB_MOVE")) {
      out |= wxAUI_NB_TAB_MOVE;
      }
    if(!strcmp(style_cstr, "AUI_NB_TAB_EXTERNAL_MOVE")) {
      out |= wxAUI_NB_TAB_EXTERNAL_MOVE;
      }
    if(!strcmp(style_cstr, "AUI_NB_TAB_FIXED_WIDTH")) {
      out |= wxAUI_NB_TAB_FIXED_WIDTH;
      }
    if(!strcmp(style_cstr, "AUI_NB_SCROLL_BUTTONS")) {
      out |= wxAUI_NB_SCROLL_BUTTONS;
      }
    if(!strcmp(style_cstr, "AUI_NB_WINDOWLIST_BUTTON")) {
      out |= wxAUI_NB_WINDOWLIST_BUTTON;
      }
    if(!strcmp(style_cstr, "AUI_NB_CLOSE_BUTTON")) {
      out |= wxAUI_NB_CLOSE_BUTTON;
      }
    if(!strcmp(style_cstr, "AUI_NB_CLOSE_ON_ACTIVE_TAB")) {
      out |= wxAUI_NB_CLOSE_ON_ACTIVE_TAB;
      }
    if(!strcmp(style_cstr, "AUI_NB_CLOSE_ON_ALL_TABS")) {
      out |= wxAUI_NB_CLOSE_ON_ALL_TABS;
      }
    if(!strcmp(style_cstr, "AUI_NB_MIDDLE_CLICK_CLOSE")) {
      out |= wxAUI_NB_MIDDLE_CLICK_CLOSE;
      }
    if(!strcmp(style_cstr, "AUI_NB_TOP")) {
      out |= wxAUI_NB_TOP;
      }
    if(!strcmp(style_cstr, "AUI_NB_BOTTOM")) {
      out |= wxAUI_NB_BOTTOM;
      }
    if(!strcmp(style_cstr, "AUI_MGR_ALLOW_FLOATING")) {
      out |= wxAUI_MGR_ALLOW_FLOATING;
      }
    if(!strcmp(style_cstr, "AUI_MGR_ALLOW_ACTIVE_PANE")) {
      out |= wxAUI_MGR_ALLOW_ACTIVE_PANE;
      }
    if(!strcmp(style_cstr, "AUI_MGR_TRANSPARENT_DRAG")) {
      out |= wxAUI_MGR_TRANSPARENT_DRAG;
      }
    if(!strcmp(style_cstr, "AUI_MGR_TRANSPARENT_HINT")) {
      out |= wxAUI_MGR_TRANSPARENT_HINT;
      }
    if(!strcmp(style_cstr, "AUI_MGR_VENETIAN_BLINDS_HINT")) {
      out |= wxAUI_MGR_VENETIAN_BLINDS_HINT;
      }
    if(!strcmp(style_cstr, "AUI_MGR_RECTANGLE_HINT")) {
      out |= wxAUI_MGR_RECTANGLE_HINT;
      }
    if(!strcmp(style_cstr, "AUI_MGR_HINT_FADE")) {
      out |= wxAUI_MGR_HINT_FADE;
      }
    if(!strcmp(style_cstr, "AUI_MGR_NO_VENETIAN_BLINDS_FADE")) {
      out |= wxAUI_MGR_NO_VENETIAN_BLINDS_FADE;
      }
    if(!strcmp(style_cstr, "AUI_MGR_LIVE_RESIZE")) {
      out |= wxAUI_MGR_LIVE_RESIZE;
      }
    if(!strcmp(style_cstr, "AUI_MGR_DEFAULT")) {
      out |= wxAUI_MGR_DEFAULT;
      }
    if(!strcmp(style_cstr, "HW_SCROLLBAR_NEVER")) {
      out |= wxHW_SCROLLBAR_NEVER;
      }
    if(!strcmp(style_cstr, "HW_SCROLLBAR_AUTO")) {
      out |= wxHW_SCROLLBAR_AUTO;
      }
    if(!strcmp(style_cstr, "HW_NO_SELECTION")) {
      out |= wxHW_NO_SELECTION;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_DEFAULT_STYLE")) {
      out |= wxRIBBON_BAR_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_FOLDBAR_STYLE")) {
      out |= wxRIBBON_BAR_FOLDBAR_STYLE;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_SHOW_PAGE_LABELS")) {
      out |= wxRIBBON_BAR_SHOW_PAGE_LABELS;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_SHOW_PAGE_ICONS")) {
      out |= wxRIBBON_BAR_SHOW_PAGE_ICONS;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_FLOW_HORIZONTAL")) {
      out |= wxRIBBON_BAR_FLOW_HORIZONTAL;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_FLOW_VERTICAL")) {
      out |= wxRIBBON_BAR_FLOW_VERTICAL;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_SHOW_PANEL_EXT_BUTTONS")) {
      out |= wxRIBBON_BAR_SHOW_PANEL_EXT_BUTTONS;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_SHOW_PANEL_MINIMISE_BUTTONS")) {
      out |= wxRIBBON_BAR_SHOW_PANEL_MINIMISE_BUTTONS;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_SHOW_TOGGLE_BUTTON")) {
      out |= wxRIBBON_BAR_SHOW_TOGGLE_BUTTON;
      }
    if(!strcmp(style_cstr, "RIBBON_BAR_SHOW_HELP_BUTTON")) {
      out |= wxRIBBON_BAR_SHOW_HELP_BUTTON;
      }
    if(!strcmp(style_cstr, "RIBBON_PANEL_DEFAULT_STYLE")) {
      out |= wxRIBBON_PANEL_DEFAULT_STYLE;
      }
    if(!strcmp(style_cstr, "RIBBON_PANEL_NO_AUTO_MINIMISE")) {
      out |= wxRIBBON_PANEL_NO_AUTO_MINIMISE;
      }
    if(!strcmp(style_cstr, "RIBBON_PANEL_EXT_BUTTON")) {
      out |= wxRIBBON_PANEL_EXT_BUTTON;
      }
    if(!strcmp(style_cstr, "RIBBON_PANEL_MINIMISE_BUTTON")) {
      out |= wxRIBBON_PANEL_MINIMISE_BUTTON;
      }
    if(!strcmp(style_cstr, "RIBBON_PANEL_STRETCH")) {
      out |= wxRIBBON_PANEL_STRETCH;
      }
    if(!strcmp(style_cstr, "RIBBON_PANEL_FLEXIBLE")) {
      out |= wxRIBBON_PANEL_FLEXIBLE;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_COMPOSITE")) {
      out |= wxRICHTEXT_FIELD_STYLE_COMPOSITE;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_RECTANGLE")) {
      out |= wxRICHTEXT_FIELD_STYLE_RECTANGLE;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_NO_BORDER")) {
      out |= wxRICHTEXT_FIELD_STYLE_NO_BORDER;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_START_TAG")) {
      out |= wxRICHTEXT_FIELD_STYLE_START_TAG;
      }
    if(!strcmp(style_cstr, "RICHTEXT_FIELD_STYLE_END_TAG")) {
      out |= wxRICHTEXT_FIELD_STYLE_END_TAG;
      }
    if(!strcmp(style_cstr, "RE_CENTRE_CARET")) {
      out |= wxRE_CENTRE_CARET;
      }
    if(!strcmp(style_cstr, "RE_MULTILINE")) {
      out |= wxRE_MULTILINE;
      }
    if(!strcmp(style_cstr, "RE_READONLY")) {
      out |= wxRE_READONLY;
      }
    if(!strcmp(style_cstr, "RICHTEXTSTYLELIST_HIDE_TYPE_SELECTOR")) {
      out |= wxRICHTEXTSTYLELIST_HIDE_TYPE_SELECTOR;
      }
  }

  return out;
}
