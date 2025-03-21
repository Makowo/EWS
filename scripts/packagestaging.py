import os
import sys
import shutil

def stage_build(game, gamearch, stage_pdbs_separately = False):
  if not os.path.exists("./staging"):
    os.mkdir("./staging")
  base_staging_path = f"./staging/{game}/"
  if os.path.exists(base_staging_path):
    shutil.rmtree(base_staging_path)
  os.mkdir(base_staging_path)

  base_binaries_path = f"./build/windows/{gamearch}/release/"

  dll_name = "EWS-" + game.upper() + ".dll"
  pdb_name = "EWS-" + game.upper() + ".pdb"


  build_dll_path = base_binaries_path + dll_name
  build_pdb_path = base_binaries_path + pdb_name

  staging_dll_path = base_staging_path + dll_name
  

  shutil.copyfile(build_dll_path, staging_dll_path)
  if not stage_pdbs_separately:
    staging_pdb_path = base_staging_path + pdb_name
    shutil.copyfile(build_pdb_path, staging_pdb_path)

  shutil.copyfile(base_binaries_path + "VERSION.dll", base_staging_path + "VERSION.dll")
  shutil.copyfile(base_binaries_path + "VERSION.pdb", base_staging_path + "VERSION.pdb")

  os.mkdir(base_staging_path + "mods")

  shutil.copytree("./game_mod/" + game.upper() + "/", base_staging_path + "mods/ews/")

  pass

if __name__ == "__main__":
  stage_build(sys.argv[1], sys.argv[2])