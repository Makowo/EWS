-- this entire file exists to fix one bug in CoreEditor:update where _current_layer is not checked for validity before calling self._current_layer:selected_amount_string()
-- MUG pls fix

function CoreEditor:update(time, rel_time)
	if self._enabled then
		self:update_title_bar(time, rel_time)

		if self._in_window then
			entering_window()
		end

		if #managers.editor._editor_data.virtual_controller:down_list() == 0 and self._wants_to_leave_window then
			self:leave_window()
		end

		if #managers.editor._editor_data.virtual_controller:pressed_list() > 0 then
			self._confirm_on_new = true
		end

		if self._clear_and_reset_layer_timer then
			if self._clear_and_reset_layer_timer == 0 then
				self._clear_and_reset_layer_timer = nil

				self:reset_layers()
			else
				self._clear_and_reset_layer_timer = self._clear_and_reset_layer_timer - 1
			end
		end

		if self._resizing_appwin then
			self._resizing_appwin = false

			self:resize_appwin_done()
		end

		self:_help_draw_all_units(time, rel_time)

		if self._draw_occluders then
			self:draw_occluders(time, rel_time)
		end

		if self._draw_bodies_on then
			self:_draw_bodies(time, rel_time)
		end

		if self._camera_controller then
			local camera = self._vp:camera()
			local cam_pos = camera:position()
			local cam_rot = camera:rotation()
			local far_range = camera:far_range()

			self._gui:child("camera"):child("cam_pos"):set_text(string.format("Cam pos:   \"%.2f %.2f %.2f\" [cm]", cam_pos.x, cam_pos.y, cam_pos.z))
			self._gui:child("camera"):child("cam_rot"):set_text(string.format("Cam rot:   \"%.2f %.2f %.2f\"", cam_rot:yaw(), cam_rot:pitch(), cam_rot:roll()))
			self._gui:child("camera"):child("far_range"):set_text(string.format("Far range: %.2f [m]", far_range / 100))
      if self._current_layer then -- bug fix
			  self._gui:child("camera"):child("select_units_amount"):set_text("" .. self._current_layer:selected_amount_string())
      end
			self._light:set_local_position(cam_pos)

			if not self._camera_locked or self._camera_controller:creating_cube_map() then
				if not self._orthographic then
					if self._skipped_freeflight_frames > 0 then
						self._camera_controller:update(time, rel_time)
					else
						self._skipped_freeflight_frames = self._skipped_freeflight_frames + 1
					end
				else
					self._camera_controller:update_orthographic(time, rel_time)
				end
			end

			if self._camera_locked then
				self._camera_controller:update_locked(time, rel_time)
			end

			if self._draw_hidden_units then
				for _, unit in ipairs(self._hidden_units) do
					Application:draw(unit, 0, 0, 0.75)
				end
			end

			self._groups:update(time, rel_time)

			if not self._camera_controller:creating_cube_map() then
				if self._current_layer then
					self._current_layer:update(time, rel_time)
				end

				for _, layer in pairs(self._layers) do
					layer:update_always(time, rel_time)
				end
			end

			if self._autosave_time > 0 then
				self._autosave_timer = self._autosave_timer + rel_time

				if self._autosave_timer > self._autosave_time * 60 then
					self._autosave_timer = 0
				end
			end

			if not ctrl() and not alt() and not shift() then
				if self._ctrl:down(Idstring("decrease_view_distance")) then
					camera:set_far_range(camera:far_range() - 5000 * rel_time)
				end

				if self._ctrl:down(Idstring("increase_view_distance")) then
					camera:set_far_range(camera:far_range() + 5000 * rel_time)
				end
			end

			if shift() then
				if self._ctrl:pressed(Idstring("increase_grid_altitude")) then
					self:set_grid_altitude(self:grid_altitude() + self:grid_size())
				end

				if self._ctrl:pressed(Idstring("decrease_grid_altitude")) then
					self:set_grid_altitude(self:grid_altitude() - self:grid_size())
				end
			end

			if self._show_center then
				local pos = Vector3(0, 0, 0)
				local rot = Rotation:yaw_pitch_roll(0, 0, 0)

				Application:draw_sphere(pos, 50, 1, 1, 1)
				Application:draw_rotation(pos, rot)

				local length = (cam_pos - pos):length()
				local from = Vector3(pos.x, pos.y, pos.z - length / 2)
				local to = Vector3(pos.x, pos.y, pos.z + length / 2)

				Application:draw_cylinder(from, to, 50, 1, 1, 1)
			end

			self._unit_transform:update(time, rel_time)
			self._camera_transform_type_in:update(time, rel_time)

			if self._mission_graph then
				self._mission_graph:update(time, rel_time)
			end
		end

		for _, marker in pairs(self._markers) do
			marker:draw()
		end

		self:update_ruler(time, rel_time)

		if self._dialogs.edit_unit then
			self._dialogs.edit_unit:update(time, rel_time)
		end

		self:_tick_generate_dome_occlusion(time, rel_time)
	end

	self:_update_helper_queues()
	self:_update_mute_state(time, rel_time)
end