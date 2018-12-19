# Source-Map Combiner

Project in its current state can generate a valid Valve Map Format file from a handful of genuine VMF files created with the Hammer World Editor program.

At the moment none of the imported file's contents are translated to other locations, this shall be implemented soon.

Simple testing has revealed that at this point in time:
- The time complexity of the script is linear with each additional file imported.
- The time complexity of the script is linear with each additional byte per file.
- With my particular machine, a large 3MB project file takes 13 seconds to process.

This program is available to you as free software licensed under the GNU General Public License (GPL-3.0-or-later)