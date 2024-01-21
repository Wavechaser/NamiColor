# NamiColor

## Installation:

Just simply put the DCTL in the DaVinci Resolve's LUT filder:

Windows - C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\
macOS - ~/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/

Relaunch Resolve, and load the script in a DCTL node (requires Studio version).

## How NamiColor Works

WIP.


## Using NamiColor

WIP.

## Changelog:
### customLog
- Initial concept by HikariDragon.

### NamiColor 1.0
- Rewrote customLog.
- Cleaned up variables.

### NamiColor 2.0
- Added "reversals" processing mode.
- Added shift control to each individual channel.
- Added "fit to Cineon base" option to allow switching between 0/0 blackpoint and Cineon blackpoint on the fly.

### NamiColor 2.1
- Added "master shift"
- Adjusted "master gain" maths to be more coherent with individual channel gains.
- Removed "input offset" control.
- Code cleanups.

### NamiColor 2.2
- Added "Log-to-Log" processing mode.
- Code cleanups.

### NamiColor 3.0
- Integrated Adobe RGB and ProPhoto RGB to Rec. 2020 transform options at import.
