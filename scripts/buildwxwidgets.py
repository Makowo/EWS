import subprocess
import sys
import os

def build_wxwidgets(arch):

  platform = "x64"
  if arch == "x86":
    platform = "Win32"

  os.chdir("./Dependencies/wxWidgets/build/msw/") # change the working directory for msbuild

  project_file = "./wx_vc17.sln"

  command_line_to_run = f"msbuild.exe {project_file} /p:configuration=Release /p:platform={platform} /p:PreferredToolArchitecture={arch}"

  msbuild_process = subprocess.Popen(command_line_to_run, stdout=subprocess.PIPE)
  while True:
    output = msbuild_process.stdout.readline()
    if msbuild_process.poll() is not None:
      break
    if output:
      print(output.strip().decode())
  msbuild_process.wait()
  sys.exit(msbuild_process.returncode)

  pass

if __name__ == "__main__":
  build_wxwidgets(sys.argv[1])
