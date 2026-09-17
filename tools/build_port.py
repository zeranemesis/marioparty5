#!/usr/bin/env python3

###
# Configures and builds the native port with a CMake preset.
#
# On Windows the MSVC environment is loaded from Visual Studio (vswhere +
# vcvars64.bat) so the build works from any shell, including Git Bash.
#
# Usage:
#   python tools/build_port.py                         (configure if needed, build all)
#   python tools/build_port.py --target dol            (build one target)
#   python tools/build_port.py --reconfigure           (force a CMake configure)
#   python tools/build_port.py --preset linux-default-relwithdebinfo
#
# files.cmake is regenerated from configure.py first, so a unit that became
# Matching or Equivalent is picked up without further steps.
###

import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

root = Path(__file__).resolve().parent.parent


def msvc_environment() -> dict:
    vswhere = Path(os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
    if not vswhere.exists():
        raise SystemExit("vswhere.exe not found: install Visual Studio with the C++ workload")
    install = subprocess.run(
        [str(vswhere), "-latest", "-products", "*", "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64", "-property", "installationPath"],
        capture_output=True, text=True, check=True,
    ).stdout.strip().splitlines()
    if not install:
        raise SystemExit("no Visual Studio installation with MSVC x64 tools found")
    vcvars = Path(install[0]) / "VC" / "Auxiliary" / "Build" / "vcvars64.bat"
    out = subprocess.run(f'cmd /s /c ""{vcvars}" >nul && set"', capture_output=True, text=True, shell=True, check=True).stdout
    env = {}
    for line in out.splitlines():
        if "=" in line:
            key, _, value = line.partition("=")
            env[key] = value
    # CMake and Ninja bundled with Visual Studio
    cmake_dir = Path(install[0]) / "Common7" / "IDE" / "CommonExtensions" / "Microsoft" / "CMake"
    extra = [str(cmake_dir / "CMake" / "bin"), str(cmake_dir / "Ninja")]
    path_key = next((k for k in env if k.upper() == "PATH"), "PATH")
    env[path_key] = os.pathsep.join(extra + [env.get(path_key, "")])
    # MSBuild's launcher rejects environments holding both Path and PATH
    seen, unique = set(), {}
    for key, value in env.items():
        if key.upper() in seen:
            continue
        seen.add(key.upper())
        unique[key] = value
    return unique


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    default_preset = "windows-msvc-relwithdebinfo" if sys.platform == "win32" else (
        "macos-default-relwithdebinfo" if sys.platform == "darwin" else "linux-default-relwithdebinfo")
    parser.add_argument("--preset", default=default_preset)
    parser.add_argument("--target", action="append", default=[])
    parser.add_argument("--reconfigure", action="store_true")
    parser.add_argument("--jobs", type=int, default=0)
    parser.add_argument("--keep-going", action="store_true", help="continue after errors (ninja -k 0)")
    args = parser.parse_args()

    subprocess.run([sys.executable, str(root / "tools" / "gen_files_cmake.py")], cwd=root, check=True)

    use_msvc = sys.platform == "win32" and args.preset.startswith(("windows-", "x-windows-"))
    env = msvc_environment() if use_msvc else dict(os.environ)
    presets = json.loads((root / "CMakePresets.json").read_text(encoding="utf-8"))
    preset = next((p for p in presets["configurePresets"] if p["name"] == args.preset), None)
    if preset is None:
        raise SystemExit(f"unknown preset {args.preset}")
    build_dir = root / "build" / args.preset
    # CreateProcess searches the parent's PATH, not the one handed to the child
    path_value = next((v for k, v in env.items() if k.upper() == "PATH"), "")
    cmake = shutil.which("cmake", path=path_value) or "cmake"
    if args.reconfigure or not (build_dir / "CMakeCache.txt").exists():
        result = subprocess.run([cmake, "--preset", args.preset], cwd=root, env=env)
        if result.returncode != 0:
            return result.returncode

    cmd = [cmake, "--build", str(build_dir)]
    for target in args.target:
        cmd += ["--target", target]
    if args.jobs:
        cmd += ["--parallel", str(args.jobs)]
    if args.keep_going:
        cmd += ["--", "-k", "0"]
    return subprocess.run(cmd, cwd=root, env=env).returncode


if __name__ == "__main__":
    sys.exit(main())
