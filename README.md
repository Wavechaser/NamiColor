# NamiColor

## Who is NamiColor for

- People who cares about what your film actually saw.
- People who want to take the guesswork out of transforming a film scan.
- People who hate manually finding a gray point in Ps or FlexColor or whatnot.
- People who have some basic understanding of color management.
- People who have access to DaVinci Resolve Studio.

## How NamiColor Works

WIP.


## Using NamiColor

### Recommeneded Hardware

NamiColor is not a compute-intensive script. If you are dealing with stills, then realtime rendering requirements should be low enough to run on most recent mainstream hardwares. If you are dealing with motion pictures, then it's rather likely that their resolutions would be low enough to not impose a problem.

However, by Resolve's nature of loading much of the timeline into VRAM, using Resolve to color transform film scans will be very memory intensive regardless of using NamiColor or not.

- For 135 scans (10-20MP), at least 8GB of VRAM should be enough.
- For 120 scans (40-60MP), at least 16GB is recommended.
- If you are shooting large formats at this day and age, I am assuming you can afford a GPU with at least 24GB of VRAM.

### Installation

Just simply put the DCTL in the Resolve's LUT filder, like any other LUT or DCTL.

Windows - C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\
macOS - ~/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/

Relaunch Resolve, and load the script in a DCTL node.

Should be compatible with Resolve 17 Studio and later.

### Color Transforming Film Scans

NamiColor should act like your IDT.

Ideally, what comes out of NamiColor would be Rec. 2020 on Cineon Film Log.

WIP.


## Changelog
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
