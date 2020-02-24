# SourceEngine Map Combiner

Project in its current state can generate a valid Valve Map Format file from a handful of genuine VMF files created with the Hammer World Editor program.

Imported maps can now be placed wherever desirable in 3D space within the exported file.

Simple testing has revealed that at this point in time:
- The time complexity of the script is linear with each additional file imported.
- The time complexity of the script is linear with each additional byte per file.
- With my particular machine, a large 3MB project file takes 13 seconds to process.

This program is available to you as free software licensed under the GNU General Public License (GPL-3.0-or-later)
