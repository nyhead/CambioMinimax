import os
import platform
import subprocess
import sys

def find(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            return os.path.join(root, name)
    return None  # Return None if not found

arguments = sys.argv[1:]

# Create the "build" directory if it does not exist
build_dir = os.path.join(os.getcwd(), 'build')
if not os.path.exists(build_dir):
    os.makedirs(build_dir)
os.chdir('build')

# Initialize CMake and build the project
subprocess.run(['cmake', '..'])
subprocess.run(['cmake', '--build', '.', '--config', 'Release'])

# Find the executable
executable_name = 'CambioMinimax.exe' if platform.system() == "Windows" else 'CambioMinimax'
executable_path = find(executable_name, build_dir)

# Check if the executable was found and execute it
if executable_path and os.path.isfile(executable_path) and 'build' not in arguments:
    print(f"Executing {executable_path}")
    subprocess.run([executable_path] + arguments)
elif 'build' in arguments:
    print(f"Compilation finished")
else:
    print(f"{executable_name} not found in {build_dir}")
