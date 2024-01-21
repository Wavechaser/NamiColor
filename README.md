# NamiColor

## Installation:

Just simply put the DCTL in the DaVinci Resolve's LUT filder:
Windows - C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\
macOS - ~/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/

Relaunch Resolve, and load the script in a DCTL node (requires Studio version).


## Using NamiColor

WIP.


## Changelog:
### customLog
- Initial concept by HikariDragon.

### NamiColor 1.0
- Rewrote customLog and cleaned up variables.

### NamiColor 2.0
- Added "Reversals" mode.
- Added shift control to each individual channel.
- Added 95/1023 lift control to allow switching between 0/0 blackpoint and Cineon blackpoint on the fly.

### NamiColor 2.1
- Added "master shift"
- Adjusted "master gain" maths to be more coherent with individual channel gains.
- Removed "input offset" control.
- Code cleanups.

### NamiColor 2.2
- Added "Log-to-Log" mode, which bypasses _log10f at import.
- Code cleanups.

### NamiColor 3.0
- Integrated Adobe RGB and ProPhoto RGB to Rec. 2020 transform options at import.


## MIT License

### Copyright (c) 2024 Wavechaser

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
