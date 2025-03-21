import re
import sys

pro_file = "new_speedtrade.pro"  # Ensure correct filename

try:
    with open(pro_file, "r", encoding="utf-8") as file:
        lines = file.readlines()

    # Strictly match only the VERSION line
    version_pattern = re.compile(r"^\s*VERSION\s*=\s*(\d+)\.(\d+)\.(\d+)\s*$")

    new_lines = []
    new_version = None

    for line in lines:
        match = version_pattern.match(line)
        if match:
            major, minor, patch = map(int, match.groups())
            patch += 1  # Increment the patch version
            new_version = f"{major}.{minor}.{patch}"
            new_lines.append(f"VERSION = {new_version}\n")
        else:
            new_lines.append(line)

    if new_version:
        with open(pro_file, "w", encoding="utf-8") as file:
            file.writelines(new_lines)
        print(f"✅ Version updated to {new_version}")
    else:
        print("❌ [ERROR] VERSION line not found in new_speedtrade.pro! Check the file format.")
        sys.exit(1)

except Exception as e:
    print(f"❌ [ERROR] Failed to update version: {e}")
    sys.exit(1)
