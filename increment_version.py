import re

pro_file = "New_Speedtrade.pro"  # Change this if your .pro file has a different name

with open(pro_file, "r") as file:
    lines = file.readlines()

version_pattern = re.compile(r"VERSION\s*=\s*(\d+)\.(\d+)\.(\d+)")

new_lines = []
for line in lines:
    match = version_pattern.search(line)
    if match:
        major, minor, patch = map(int, match.groups())
        patch += 1  # Increment the patch version
        new_version = f"{major}.{minor}.{patch}"
        new_lines.append(f"VERSION = {new_version}\n")
    else:
        new_lines.append(line)

with open(pro_file, "w") as file:
    file.writelines(new_lines)

print(f"Version updated to {new_version}")
