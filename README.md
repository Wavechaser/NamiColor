# NamiColor

## Who is NamiColor for

- People who care about what the film actually saw.
- People who want to take the guesswork out of transforming a film scan.
- People who hate manually finding a gray point in Ps or FlexColor or whatnot.
- People who have an obscene amount of film scans to deal with.
- People who know what a fully color managed pipeline looks like.
- People who have access to DaVinci Resolve Studio.

## How NamiColor Works

Your pain of finding black, white, and gray points in traditional ways of color transforming a film scan comes 


## Using NamiColor

### Recommeneded Hardware

NamiColor is not a compute-intensive script. If you are dealing with stills, then realtime rendering requirements should be low enough to run on most mainstream-ish hardwares. If you are dealing with motion pictures, then it's rather likely that their resolutions would be low enough to not impose a problem.

However, by Resolve's nature of loading entire chunks of the timeline into the VRAM, using Resolve to color transform film scans will be very memory intensive regardless of using NamiColor or not.

- For 135 scans (10-20MP), at least 8GB of VRAM should be enough.
- For 120 scans (40-60MP), at least 16GB is recommended.
- If you are shooting large formats at this day and age, I am assuming you can afford a GPU with at least 32GB of VRAM.

### Installation

Just simply put the DCTL in the Resolve's LUT filder, like any other LUT or DCTL.

Windows - `C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\`
macOS - `~/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/`

Relaunch Resolve, and load the script in a DCTL node.

Should be compatible with Resolve 17 Studio and later.

### Color Transforming Film Scans

The power of NamiColor lies not only in its exceptional efficiency at linearizing film scans, but also in the fact that it allows further adjustments like any other raw digital images.

#### Preparing Input Files

Before anything starts, make sure your file is scene linear, and is readable to Resolve.

This is extremely important. All will fail unless the input is strictly scene linear. There should be absolutely no rolloff and no clipping of any kind on your input.

If you are using a dedicated film scanner, you are pretty much good to go. The DNG or TIFF files coming out of Epsons, Nikons, and Flextights should automatically satisfy these two conditions. FFFs would need a bit of a conversion, but if you are using a Flextight, you should know how to make that happen.

If you are using a camera to scan your film, things can get trickier. TIFFs exported from Lightroom CC/Classic and Adobe Camera Raw **are NOT scene linear**. DNGs from Adobe DNG Converter are scene linear and readable to Resolve, but Resolve may or may not know how to correctly deal with the embedded CCMs depending on your camera model. If you opt for the DNG workflow, select decode using clip settings in Resolve's Camera Raw panel, decode to `P3 D60` and `linear` EOTF, and I wish you good luck from there. If you opt not for the DNG workflow, which would be more manageable and a bit less blackboxy, you may want to preprocess your raw using a LibRaw product. These include LibRaw itself, RawDigger, and Affinity Photo. Either TIFF and EXR would work, as long as you keep them scene linear and mapped to a reasonable color space.

#### Setting NamiColor

NamiColor should act like your IDT node. 

Select a corresponding processing mode. `Negatives` and `Reversals` should be rather self explanatory. `Log-to-Log` bypasses both the inversion and the log<sub>10</sub> scaling. Only use `Log-to-Log` if you are very sure what you are doing.

If your input image is in a color space NamiColor knows how to manage (Adobe RGB and ProPhoto RGB), then NamiColor should be your first node. Select the corresponding color space in the input color space dropdown. In this case, NamiColor will output Rec. 2020, make sure any subsequent nodes and the ODT are configured for that.

If your input image is in a color space native to Resolve (ACES, Rec. 2020, P3, sRGB, etc.), NamiColor should also be your first node. Select `bypass` in the input color space dropdown and you are good to go. In this case, NamiColor will not perform any color space transform. Therefore, any subsquent nodes and the ODT should be configured for the same color space as the input. 

If your input image is neither, please do manage it somehow. Add any necessary CST, LUT, DCTL, dark magic, sorcery, or mango smoothie **before** Namicolor, and select `bypass` in the input color space dropdown. Note that linear transformations are not commutative, managing your input after NamiColor may not yield correct results.

Ideally, what comes out of NamiColor would be Rec. 2020 on Cineon Film Log.

WIP.


## An Unexhaustive List of Frequently Asked Questions

- My films now look like digital images!
- They are supposed to. Congratulations.

- My HDR wheels (or whatever adjustment nodes) isn't working!
- Check if your timeline color space is set to Cineon and Rec. 2020. If not, fix that. Individually setting that in HDR wheels would also work, should your timeline demand some other color spaces. 

- My source color space isn't included in your input color space list!
- Select bypass and deal with it.


## Changelog
### customLog
- Initial concept by HikariDragon.

### NamiColor 1.0
- Rewrote customLog.
- Cleaned up variables.

### NamiColor 2.0
- Added `Reversals` processing mode.
- Added shift control to each individual channel.
- Added `fit to Cineon base` option to allow switching between 0/0 blackpoint and Cineon blackpoint on the fly.

### NamiColor 2.1
- Added `master shift`
- Adjusted `master gain` maths to be more coherent with individual channel gains.
- Removed `input offset` control.
- Code cleanups.

### NamiColor 2.2
- Added `Log-to-Log` processing mode.
- Code cleanups.

### NamiColor 3.0
- Integrated Adobe RGB and ProPhoto RGB to Rec. 2020 transform options at import.
