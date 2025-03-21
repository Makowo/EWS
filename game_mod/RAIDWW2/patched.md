patched
```lua
--collect_members(CoreParticleEditorInitializers, stack_members.initializer)
--collect_members(CoreParticleEditorSimulators, stack_members.simulator)
--collect_members(CoreParticleEditorVisualizers, stack_members.visualizer)
--collect_member_names(stack_members.initializer, stack_member_names.initializer)
--collect_member_names(stack_members.simulator, stack_member_names.simulator)
--collect_member_names(stack_members.visualizer, stack_member_names.visualizer)
 ```
 in coreparticleeditor


commented out `core:_copy_module_to_global("CoreEws")` in core/lib/system/coresystem

call init, preinit and dieselapp init in CoreSetup:init

commented out error calls in `callback(...)` (coreevent.lua)


`coreeditorutils.lua`'s `parse_layer_types` function modified to do nothing, Lead and Gold contains a editor_types file.

wrapped
```lua
self._gui:child("camera"):child("select_units_amount"):set_text("" .. self._current_layer:selected_amount_string())
```
with `if self._current_layer`

commented out `MassUnitManager:save(path)` in `BrushLayer:_save_brushfile` (corebrushlayer.lua)


**for modern ui styling + transparency**
change the subsystem of raid_win64_release from GUI to Console, and changed the Windows version to Windows 8 instead of Windows Vista

DatabaseManager:root_path changed to ```lua

  local path = Application:base_path()
	--local path = Application:base_path() .. (CoreApp.arg_value("-assetslocation") or "..\\..\\")
  ```

