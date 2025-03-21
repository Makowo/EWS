
function WorldHolder:_worldfile_generation(file_type, file_path)
	if file_type == "level" then
		return "level"
	end

	if not Application:editor() then
		return "new"
	end

  if DB:has(file_type:id(), file_path:id()) then
    -- loading a level from packaged game files
    return "new"
  end

	local path = managers.database:entry_expanded_path(file_type, file_path)
	local node = SystemFS:parse_xml(path)

	if not node then
		return "missing"
	end

	if node:name() == "worlds" then
		return "old"
	end

	if node:name() == "generic_scriptdata" then
		return "new"
	end

	return "unknown"
end