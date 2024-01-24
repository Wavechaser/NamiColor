# NamiColor

## Who is NamiColor for

- People who care about both fidelity and beauty.
- People who want to take the guesswork out of the workflow.
- People who have an obscene amount of film scans to deal with.
- People who have experiences with scene-referred color pipelines.
- People who have access to DaVinci Resolve Studio.

## What NamiColor Does

## How NamiColor Works

Your pain of finding black, white, and gray points in traditional ways of color transforming a film scan comes 




## Using NamiColor

### Recommeneded Hardware

NamiColor is not a compute-intensive script. If you are dealing with stills, then realtime rendering requirements should be low enough to run on most mainstream-ish hardwares. If you are dealing with motion pictures, then it's rather likely that their resolutions would be low enough to not impose a problem.

However, by Resolve's nature of loading entire chunks of the timeline into the VRAM, using Resolve to color transform film scans will be very memory intensive regardless of using NamiColor or not.

- For 135 scans (10-20MP), 8-12GB of VRAM should be enough.
- For 120 scans (40-60MP), 16-24GB is recommended.
- If you are shooting large formats (100-200MP) at this day and age, I am assuming you can afford a GPU with at least 24GB of VRAM.


### Installation

Just simply put the DCTL in the Resolve's LUT filder, like any other LUT or DCTL.

Windows - `C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\`

macOS - `~/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/`

Relaunch Resolve, apply `DCTL OFX` to a node, and load `NamiColor` in the `DCTL list` dropdown.

Should be compatible with Resolve 17 Studio and later.


### Preparing Input Files

> [!IMPORTANT]
> Make sure your input is scene linear.

Before anything starts, make sure your file is scene linear, and is readable to Resolve.

There should be absolutely no rolloff of any kind on your input. Ideally there should be no clippings either.

If you are using a dedicated film scanner, you are pretty much good to go. The DNG or TIFF files coming out of Epsons, Nikons, and Flextights should automatically satisfy these two conditions. 

If you are using a camera to scan your film, things are about to get trickier. This is a result of Resolve's inherent inability to natively decode most stills cameras' raw files, and there is not much NamiColor can do to help. I would recommend converting your raw into linear TIFFs or EXRs using LibRaw (commandline LibRaw, RawDigger, Affinity Photo, etc.). 

You may also want to set your timeline to be an appropriate resolution and aspect ratio.

> [!CAUTION]
> TIFFs exported by Lightroom CC/Classic and Adobe Camera Raw **are NOT scene linear**. Not even the HDR ones. NEVER use Lr and ACR to prepare your digital intermediates. 


### Setting up NamiColor

NamiColor should act like your IDT node. 

Select a corresponding processing mode. `Negatives` and `Reversals` should be rather self explanatory. `Log-to-Log` bypasses both the inversion and the log<sub>10</sub> scaling. Only use `Log-to-Log` if you are very sure what you are doing.

If your input image is in a color space NamiColor knows how to manage (Adobe RGB and ProPhoto RGB), then NamiColor should be your first node. Select the corresponding color space in the input color space dropdown. In this case, NamiColor will output Rec. 2020, make sure any subsequent nodes and the ODT are configured for that.

If your input image is in a color space native to Resolve (ACES, Rec. 2020, P3, sRGB, etc.), NamiColor should also be your first node. Select `bypass` in the input color space dropdown and you are good to go. In this case, NamiColor will not perform any color space transform. Any subsquent nodes and the ODT should be configured for the same color space as the input. 

If your input image is neither, please still try to manage it somehow. You are using Resolve, not Photoshop. Manage your colors. Add any necessary CST, LUT, DCTL, dark magic, sorcery, or mango smoothie **before** Namicolor, and select `bypass` in the input color space dropdown. Note that linear transformations are not commutative, managing your input after NamiColor may not yield correct results.


### Color Transforming Film Images

First, use master controls to fit the range of input roughly into the 0-1 floatpoint space. Refer to your waveform and other scopes to determine where your code values are. Don't worry about not being exact, there are second chances. 

> [!NOTE]
> Always drag the number box in the DCTL UI. The graphical slider themselves tend to be overly sensitive.

#### Input Scaling

`Input Gain` scales the entire input before inversion and log<sub>10</sub> scaling. From a WYSIWYG perspective, it scales the entire output with respect to 1 in floatpoint space, which could be helpful if your input somehow has a bunch of code values extending below 0 after the inversion maths. 

`Master Gain` is like exposure. It works after the inversion and logarithm maths, and scales all three channels with respect to 0 (or the Cineon blackpoint if `fit to Cineon base` is checked).

`Master shift` also works the inversion and logarithm maths. It linearly shifts all three channels up and down (in log space).

If you are dealing with negatives, chances are you don't have to do very much. Some may benefit from increasing master gain a bit, but most will do fine without. A reasonably scanned negative image should be reasonably close to be in (0, 1) range after inversion and log<sub>10</sub> scaling.

If you are dealing with reversals, you may have to decrease `input gain` somewhat.

> [!NOTE]
> The key is, again, to fit everything inside 0-1 space. 

Being essentially an IDT, you would want NamiColor to capture and prepare a digital intermediate with the full scene information. Therefore, the goal of NamiColor is not necessarily to get the "correct exposure" at this stage, but to retain as much dynamic range as possible. You can always readjust exposure in later nodes.

#### Channel Alignment

There are no single correct way of aligning your RGB channels. I personally recommend using `offset` to align the black levels of each channel, and using `gain` to finish aligning the highlights. But I am not going to stop you from using `blackpoint` and `gain` to stretch each channel in place. Whatever works.

You may continue to fine tune each channel's `blackpoint` and `gain` to get a more reasonable result. Especially for negatives where the B channel/Y dye layer has the steepest curve and less SNR, pulling `B Blackpoint` slighly below that of R and G channels' tend to yield more balanced shadows.

You are done after the blackpoints and whitepoints of each channel are aligned. There is no finding gray point.

Blacks, whites, done. Just like that.


### After NamiColor

The power of NamiColor lies not only in its exceptional efficiency at linearizing film scans, but also in the fact that it allows further adjustments like any other raw digital images. 

You are free to put any kind of adjustments after NamiColor, as you would for other log footages.

If you let NamiColor manage your input color spaces, then the output would be Cineon Film Log with Rec. 2020.

If you bypass NamiColor's color space conversion, NamiColor's output color space will be the same as its input, with an EOTF of Cineon Film Log.

Either case will remain scene linear as long as the input is so.

Having access to a highly linearlized scene-referred image also enables you to prepare an HDR output. If you have the correct working environment of dealing with HDR in Resolve, I trust that you already know what you are doing. Good luck.

In most cases though, a single HDR wheels node and an ODT should suffice. If you don't already know, HDR wheels do not necessitate an HDR output, it just works well with scene-referred workflows such as what NamiColor provides.

Setup your ODT like you normally would. Just don't forget to adapt it for Cineon/Rec. 2020. 

Most cameras' LUT would provide a reasonably faithful rendering of your film image, be it S-Log 3 to s709 or Log C to Arri 709, you know the deal. You may also opt for any looks of your preference. 


### Scaling NamiColor

Owning to Resolve's nature of being designed with large productions in mind, NamiColor is by extension highly scalable. 

Copy your grades from clip to clip as you normally would, or you can also setup NamiColor on an adjustment layer, and let it span multiple clips. Like other methods of color transforming film scans, the same NamiColor settings should be applicable to multiple clips of the same emulsion, similar lighting and exposure conditions, and similar processing environments. The difference is that you have access to multiple frames, even multiple strips of your film scans on the same timeline.

By setting each clip (assuming you are processing stills) to be 1 frame long, and by rendering the timeline as TIFF sequences, a single NamiColor setup can potentially service hundreds of frames across multiple rolls. 


## Changelog
### NamiColor 3.0
- Integrated Adobe RGB and ProPhoto RGB to Rec. 2020 transform options at import.

### NamiColor 2.2
- Added `Log-to-Log` processing mode.
- Code cleanups.

### NamiColor 2.1
- Added `master shift`
- Adjusted `master gain` maths to be more coherent with individual channel gains.
- Removed `input offset` control.
- Code cleanups.

### NamiColor 2.0
- Added `Reversals` processing mode.
- Added shift control to each individual channel.
- Added `fit to Cineon base` option to allow switching between 0/0 blackpoint and Cineon blackpoint on the fly.

### NamiColor 1.0
- Rewrote customLog.
- Cleaned up variables.

### customLog
- Initial concept by HikariDragon.
