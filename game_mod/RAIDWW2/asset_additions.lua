local function get_full_path(assetname)
  return "./mods/ews/assets/" .. assetname
end

EWS_Log("Registering assets")

DB:create_entry("xml", "core/settings/editor_types", get_full_path("editor_types.xml"))
DB:create_entry("xml", "lib/utils/dev/editor/xml/predefined_instances", get_full_path("predefined_instances.xml"))
DB:create_entry("xml", "lib/utils/dev/editor/xml/predefined_worlds", get_full_path("predefined_worlds.xml"))

DB:create_entry("controller", "core/lib/utils/dev/editor/xml/default_controller", get_full_path("default_controller.xml")) -- from Lead and Gold