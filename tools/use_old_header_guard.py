from pathlib import Path
import os
import sys

def filename_to_define(filename: str) -> str:
    return filename.upper().replace("/", "_").replace(".", "_")

def change_include_guard(filename: str):
    lines = []
    with open(filename, "r") as file:
        lines = file.readlines()
        if lines[0] != "#pragma once on\n":
            return

    lines[0] = "#ifndef {0}\n#define {0}\n".format(filename_to_define(filename))
    lines.append("\n#endif\n")

    with open(filename, "w+") as file:
        file.writelines(lines)

def main():
    if len(sys.argv) != 2:
        print(f"usage: {sys.argv[0]} <directory>")
        os._exit(1)

    path = Path(sys.argv[1])
    piglet_headers = path.glob("Piglet/**/*.h")
    engine_headers = path.glob("engine/**/*.h")
    for file in piglet_headers:
        change_include_guard(str(file))
    for file in engine_headers:
        change_include_guard(str(file))

if __name__ == "__main__":
    main()
