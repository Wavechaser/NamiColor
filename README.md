# NamiColor

## What NamiColor Does

NamiColor is designed to allow you to take full ownership of your film images.

Like digital sensors, film is nothing but a device of acquisition. In either format, the acquisition device captures and transports a slice of the scene in an intermediate form. Given that this intermediate image retains enough resolution, dynamic range, and color, it grants the possibility of being transformed into any desired look. 

With that being said, film in and of itself, is not an end, but merely means to it. Obviously, different emulsions and filmstocks can *influence* the final image, but should by no way, shape, or form *determine* your looks. 

To make film sufficiently workable in post, the scan needs to be transformed from transmittance to density, and to have its channels aligned, all while maintaining scene-linearity. NamiColor is designed to do just that by acting as an ingest node for scanned film images. 

By linearizing film in the density domain, NamiColor also removes the guesswork in traditional workflows, especially finding gray points. Having Resolve as its host application also allows NamiColor to be extremely scalable, suitable for dealing with an obscene amounts of film scans. 


## How NamiColor Works

Film scanners (flatbed or camera) measure transmittance, not directly density. To work in the density domain, NamiColor performs a log<sub>10</sub> on the input. 

Working in the density domain also significantly simplifies the channel alignment process. On most films' datasheets, you would usually find a characteristic curve like the one below. Note that the linear portion of each channel are in fact straight, and is only offset from each other by some amount in log<sub>10</sub> space. 

This is not the case in linear/transmittance space. The EOTF of each channel remain highly nonlinear, and is therefore very tricky to align. Conventional methods of channel alignment is essentially using gamma correction (power functions) to approximate logarithm maths. This could often be mathematically valid, but is undoubtly very finicky in practice. 

After the log<sub>10</sub> scaling, NamiColor aligns the RGB with shift and gain controls. This part is rather straightforward. After alignment, the output would approximately be Cineon Film Log, and is ready for any further color grading.

There are no secret sauce to NamiColor's maths. All of what NamiColor does can potentially be approximated with Resolve's stock tools, but at the expense of using many more nodes, being more expensive to compute, and being a lot less flexible. One possible way of approximating NamiColor with stock tools could be performing input management with CSTs and LUTs, and then using LGG wheels to align each channel, should you lose your access to Resolve Studio or something.


## Using NamiColor

### Recommeneded Hardware

NamiColor is not a compute-intensive script. It does not ask for more compute power than applying a LUT or two. Given the resolution of most film scans, NamiColor should run on most mainstream hardwares from the past couple of years.

Even for very high resolution inputs, realtime rendering requirements should be low enough to be usable. If you are dealing with motion pictures, then it's rather likely that their resolutions would be low enough to not impose a problem.

However, by Resolve's nature of loading entire chunks of the timeline into the VRAM, using Resolve to color transform film scans will be very memory intensive regardless of using NamiColor or not.

- For 135 scans (10-20MP), 8GB of VRAM should be enough.
- For 120 scans (40-60MP), 12-16GB is recommended.
- If you are shooting large formats (100-200MP) at this day and age, I am assuming you can afford a GPU with at least 24GB of VRAM.

Being an DCTL script, NamiColor will require DaVinci Resolve Studio to run. It has been tested to be compatible with Resolve 17 and later.


### Installation

Just simply put the DCTL in the Resolve's LUT filder, like any other LUT or DCTL.

Windows - `C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\`

macOS - `~/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/`

Relaunch Resolve, apply `DCTL OFX` to a node, and load `NamiColor` in the `DCTL list` dropdown.


### Preparing Input Files

> [!IMPORTANT]
> Make sure your input is scene linear.

Before anything starts, make sure your file is scene linear, and is readable to Resolve.

There should be absolutely no rolloff of any kind on your input. Ideally there should be no clippings either.

If you are using a dedicated film scanner, you are pretty much good to go. The DNG or TIFF files coming out of Epsons, Nikons, and Flextights should automatically satisfy these two conditions. 

If you are using a camera to scan your film, things are about to get trickier. This is a result of Resolve's inherent inability to natively decode most stills cameras' raw files, and there is not much NamiColor can do to help. I would recommend converting your raw into linear TIFFs or EXRs using LibRaw (commandline LibRaw, RawDigger, Affinity Photo, etc.). 

You may also want to set your timeline to be an appropriate resolution and aspect ratio at this point.

> [!CAUTION]
> TIFFs exported by Lightroom CC/Classic and Adobe Camera Raw **are NOT scene linear**. Not even the HDR ones. NEVER use Lr and ACR to prepare your digital intermediates. 

> [!CAUTION]
> If you convert FFFs into TIFFs by simply changing its extensions, it would remain in your Flextight's raw gamut and not any formal color space. Although managing these TIFFs as Adobe RGB would often give visually reasonable results, such practice still remain technically incorrect. 


### Setting up NamiColor

NamiColor should act like your IDT node. 

Select a corresponding processing mode. `Negatives` and `Reversals` should be rather self explanatory. `Log-to-Log` bypasses both the inversion and the log<sub>10</sub> scaling. Only use `Log-to-Log` if you are very sure what you are doing.

NamiColor can manage the input in 3 ways.

- NamiColor managed: Adobe RGB/ProPhoto RGB & linear input, Rec. 2020 & Cineon output.

  If your input image is linear and in a color space NamiColor knows how to manage, then NamiColor should be your first node. Simply select the corresponding color space in the input color space dropdown. 

- Resolve managed: Resolve-native color space (ACES, Rec. 2020, P3, sRGB, etc.) & linear input, outputs same color space as input & Cineon Film Log.

  In this case, NamiColor should also be your first node. Select `bypass` in the input color space dropdown and you are good to go. In this case, NamiColor will not perform any color space transform, but will still do the linear to Cineon scaling.

- User managed: (I hope you know what you are doing)

  If your input image is neither, please still try to manage it somehow. You are using Resolve, not Photoshop. Manage your colors. Add any necessary CST, LUT, DCTL, dark magic, sorcery, or mango smoothie **before** NamiColor, and select `bypass` in the input color space dropdown. NamiColor will output the same color space as its input and hopefully Cineon Film Log.


### Color Transforming Film Images

First, use master controls to fit the range of input roughly into the 0-1 floatpoint space. Refer to your waveform and other scopes to determine where your code values are. Don't worry about not being exact, there are second chances. 

> [!NOTE]
> Always drag the number box in the DCTL UI. The graphical sliders tend to be overly sensitive.

#### Input Scaling

`Input Gain` scales the entire input before inversion and log<sub>10</sub> scaling. From a WYSIWYG perspective, it scales the entire output with respect to 1 in floatpoint space, which could be helpful if your input somehow has a bunch of code values extending below 0 after the inversion maths. 

`Master Gain` is like exposure. It works after the inversion and logarithm maths, and scales all three channels with respect to 0 (or the Cineon blackpoint if `fit to Cineon base` is checked).

`Master shift` also works the inversion and logarithm maths. It linearly shifts all three channels up and down (in log space).

If you are dealing with negatives, chances are you don't have to do very much. Some may benefit from increasing master gain a bit, but most will do fine without. A reasonably scanned negative image should be reasonably close to be in (0, 1) range after inversion and log<sub>10</sub> scaling.

If you are dealing with reversals, you may have to decrease `input gain` somewhat.

> [!NOTE]
> The key is, again, to fit everything inside the 0-1 range. 

Being essentially an IDT, you would want NamiColor to capture and prepare a digital intermediate with the full scene information. Therefore, the goal of NamiColor is not necessarily to get the "correct exposure" at this stage, but to retain as much dynamic range as possible. You can always readjust exposure in later nodes.

But if you don't have an excessive amount of highlights to cram between 0-1, you don't necessarily have to stretch your white all the way to the top. This is by all means optional, but it could make exposure adjustments a bit easier. Cineon defines gray at 470/1023 and diffuse white at 685/1023, so use your best judgements determining how you scale the input, given you can fit all the data in range.


#### Channel Alignment

There are no single correct way of aligning your RGB channels. I personally recommend using `offset` to align the black levels of each channel, and using `gain` to finish aligning the highlights. But I am not going to stop you from using `blackpoint` and `gain` to stretch each channel in place. Whatever works.

Use common sense when aligning channels. Finding a relatively neutral black and highlight point is highly recommended, even if it may not be imperative. For example, if your highlight is the sky, you would not want that to be your alignment reference, or else everything would look very yellow. You would want to find another reference for that. The good thing is, even if you can't find a reasonable highlight/black reference in a frame, setups from other frames on the same roll with similar lighting conditions would likely be usable. Just copy your grades.

You may continue to fine tune each channel's `blackpoint` and `gain` to get a more reasonable result. Especially for negatives where the B channel/Y dye layer tends to have the steepest curve and least SNR, pulling `B Blackpoint` slighly below that of R and G channels' tend to yield more balanced shadows.

You are done after the blackpoints and whitepoints of each channel are aligned. There is no finding gray point.

> [!NOTE]
> Make sure you check `fit to Cineon base` after you believe your channels have been reasonably aligned.


### After NamiColor

The power of NamiColor lies not only in its exceptional efficiency at linearizing film scans, but also in the fact that it allows further adjustments like any other raw digital images. 

You are free to put any kind of adjustments after NamiColor, as you would for other log footages.

If you let NamiColor manage your input color spaces, then the output would be Cineon Film Log with Rec. 2020.

If you bypass NamiColor's color space conversion, NamiColor's output color space will be the same as its input, with an EOTF of Cineon Film Log.

Either case will remain scene linear as long as the input is so.

Having access to a highly linearlized scene-referred image also enables you to prepare an HDR output. If you have the correct working environment of dealing with HDR in Resolve, I trust that you already know what you are doing. Good luck.

If you don't have a specific look in mind, using a camera's LUT (S-Log to s709, Log C to Arri 709, etc.) as the ODT will do your image justice by rendering a faithful representation of the scene. Just don't forget to adapt the LUT of your choice for Cineon/Rec. 2020. 

Otherwise, setup your ODT like you normally would. 


### Scaling NamiColor

Owning to Resolve's nature of being designed with large productions in mind, NamiColor is by extension highly scalable. 

Copy your grades from clip to clip as you normally would, or you can also setup NamiColor on an adjustment layer, and let it span multiple clips. Like other methods of color transforming film scans, the same NamiColor settings should be applicable to multiple clips of the same emulsion, similar lighting and exposure conditions, and similar processing environments. The difference is that you have access to multiple frames, even multiple strips of your film scans on the same timeline.

By setting each clip (assuming you are processing stills) to be 1 frame long, and by rendering the timeline as TIFF sequences, a single NamiColor setup can potentially service hundreds of frames across multiple rolls. 


## Changelog
### Upcoming
- Integrate `gamma 2.2` input transform.
- More flexible color space transform options.
- Code refactor, split main function into modularized ones.

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
- Added `shift` control to each individual channel.
- Added `fit to Cineon base` option.

### NamiColor 1.0
- Rewrote RemoveColorMask.
- Cleaned up variables.

### RemoveColorMask
- Initial concept by @OwenYou.
