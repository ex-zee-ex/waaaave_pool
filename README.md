# waaaave_pool

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool_splashscreen_final.png)

a port of video_waaaves to raspberry pi
the larger desktop versions can be found here:  
full sized
https://github.com/ex-zee-ex/VIDEO_WAAAVES_1_75,  
lil sized
https://github.com/ex-zee-ex/lil_waaaves

there are multiple versions up for playing with right now but the one titled WAAAVE_POOL_MAIN is the most recent and bestest build, also the only one that works with the picapture sd so try that one first.  unfortunately the ofxOMXcamera addon does not seem to currently work for buster and openFrameworks 0.11 so for best results i advise raspbian Stretch (http://downloads.raspberrypi.org/raspbian/images/raspbian-2019-04-09/) and openFrameworks 0.10 (https://openframeworks.cc/download/older/ choose linux armv7)).  Eventually I will have a custom image for download that you can just flash to an sd card but until then it involves a bit of linuxing!  If you only have buster to play with then use the folder that ends with "noOmx" for a compatible build but be forewarned that there are kind of a bunch of gl issues with buster still and full screen doesnt seem to work plus some obnoxious banding as well.  

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool4.png)

hello and welcome to the waaaves pool!  to get started with this program you will need at least raspberry pi(i built and tested this in raspbian on a raspberry pi 3 b v1.2), a usb camera (all images and demo videos were made with a logitech c310 which can be found very affordably on ebay.  i highly reccomend only logitech usb cameras as the only brand im aware of with consistent driver support cross platforms) or a picapturesd for input, a usb keyboard for controls.  There is option to support a usb midi keyboard as well(the default midi controls are mapped to the default cc values for the knobs on a korg nanokontrol2 in the current version, there are instructions on midi mapping below (eventually)).  A build of openFrameworks is also necessary, https://openframeworks.cc/setup/raspberrypi/ for more info.  Once you have openFrameworks installed and working you will also need to install the ofxMidi addon https://github.com/danomatika/ofxMidi and the ofxOMXCamera addon https://github.com/langolierz/ofxOMXCamera.  For midi controllers this code is built around a korg nanokontrol2, for best operations there is a firmware thing you can do with the korg kontrol editor that you load and then send over to the midi controller and then its pretty much plug and play from there!

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool3.png)

once you have openFrameworks downloaded, built, and tested, and all of the addons installed as well, clone or download this repository and then move the folder WAAAAVE_POOL_MAIN into the openFrameworks->apps->myApps folder.  If you are running on a gui style raspbian you can then right click on the folder and select "open in terminal" then type 'make', wait for a minute or two until that step is finished, and then 'make run' once you have a prompt again.


keyboard controls
for each pair x/y means that the corresponding alphanumerical keys on a keyboard increment and decrement the values

a/z - zoom in/out  
s/x - framebuffer0 x displacement  
d/c - framebuffer0 y displacement  
f/v - framebuffer0 hue attenuation  
g/b - framebuffer0 saturation atte!uation  
h/n - framebuffer0 brightness attentuation  
j/m - frambuffer0 mix  
k/, - framebuffer0 lumakey  
[/] - framebuffer0 change delay amount  
q/w - framebuffer0 rotate  
o/p - temporal filter  
;/' - temporal 'filter resonance'
l/. - input attenuate
e/r - chaotic hue mod
t/y - chaotic hue lfo
u/i - chaotic hue offset
-/= - y skew

9/0 - x skew

1   - clears the framebuffers  

!   - resets all controls

2   - invert framebuffer0 brightness

3   - invert framebuffer0 hue

4   - invert framebuffer0 saturation

5   - vertical mirror

6   - horizontal mirror

7   - toriodal universe

yes i am aware that much of this labelling is mysterious nonsense.  I'm in the process of writing a better manual right now but honestly words are not going to help much more than just saying "ok if you touch these things something will happen if you look at the screen you will def notice a thing or two that will then map into some dendritic connections between axons or something!".  but yeah also i have to like draw some diagrams too and that kind of takes me some time too so yeah.


if you are using the midi version then these keyboard controls will work as well but if you change them then they can potentially change or offset the ranges of the midi controls heads up

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/78.jpg)

(notes: parenthesis (U) and (B) after the controls means Unipolar and Bipolar respectively.  unipolar means that values go from 0-1, bipolar means that values go from -1 to 1.  this means that if you want to set values to 0 on a unipolar knob then you turn it all of the way to the left and if you want to set values to 0 on a bipolar knob you set it at 12 o clock.  Unipolar 0 on a slider means all of the way down, bipolar 0 means exactly in the middle.  Most of these parameters we will assume are normalized, which means we will think of 0 as being all of the way off and 1 as being 100 percent on.  This can get a little strange because some values are set so that they actually go from like -200 percent off to +200 percent on but bear with me, it is all happening for reasons!  if the default parameter set is anything other than (0,1) or (-1,1) it will be shown in parenthesis as well.  the framebuffer delay channel will be henceforth referred to as fb0)

1. Luma Key Level. (U)  This selects a brightness value in the video input channel to key fb0 into.  For example, if I turn my Luma Key Level knob to 12 o clock, that means everything in the input channel with brightness less than 50 percent will be replaced by fb0.  0 means that none of fb0 is keyed in, 1 means that none of the input channel will be visible.

2. Mix level. (B) (-1.1,1.1)  For values between 0 and 1 this functions as a fade between the input channel and fb0.  For values outside of this range interesting distortions can happen.  Strobing can occur when there are very short (less than 5 frames) delay times and negative mix values.  Mix is downstream from Key so you can key and mix at the same time, the mixing will only affect the unkeyed part of the input channel though!  The actual mathematics being performed is a linear interpolation (aka lerp in many programming languages) between each pixel. Let Input pixel be I, Mix level be M and fb0 pixel be F, then the value of each pixel will be set by I*(1-M)+F*M.

3. Fb0 Hue Attenuation (B) (.25,1.75)  This is a multiplier for fb0 hue.  Because of the feedback loop involved, multiplicative operations have exponential effects so it is sometimes best to play around with very small changes with a very small delay time to get a feel for what kind of patterns and behaviors can emerge.  Attenuating hue will often want to cycle through all the values in the spectrum so if you do not want your screen to be just rainbow zones 24 hours a day try playing around with the hue chaos controls as well.

4. Fb0 Saturation Attenuation (B) (0,2)  This is a multiplier for fb0 saturation.  Turn all the off for greyscale feedback, crank it up for supersaturated feedback

5. Fb0 Brightness Attenuation (B) (0,2)  This is a multiplier for fb0 brightness.  Values around .8 or .9 will result in trails that fade away softly, values above 1 will clamp out and fill the screen.

6.  Temporal filter mix (B) (-1.1,1.1) For values between 0 and .99 the temporal filter can help smooth out some of the potential harshness, business, and/or strobing that can very often occur when using this system.  For values over 1 the temporal filter goes into a fairly pure digital feedback mode and can slow things down considerably while still oozing along like a wet digital oil painting. For values less than 0 many interesting patterns can occur as well as some amount of strobing.  The temporal filter is another linear interpolation function performed downstream from the keying and fb0 mixing.

7. Temporal filter strength (U)  This attenuates the brightness and saturation of the temporal filter to enhance complex pattern formation if desired.

8. Input enhance (U) (0,2) This can be used to increase the brightness of the input channel which can be very helpful when working with lumakeying.  There can also be some potential issues when keying or mixing that the input will not be as eye popping as the feedback so some enhancement can help keep everything more cohesive as well.

*****interlude to explain a couple things***
if you are using the nanokontrol2 controller then you will notice for the sliders 9 through 16 there are 3 buttons next to each slider.  for sliders 9 through 15 what these buttons do is control the total range for each corresponding slider.  so for example no button pressed is a pretty small range, S toggled on means a Slightly larger range, M toggled is a Medium larger range, R toggled is a completely Ridiculous range.  Larger range buttons bypass smaller range buttons so if M is toggled on and then R is toggled on then only R is active.  Since each of these sliders will have 4 total ranges available each will be listed in order from default small to largest.  The toroidal universe toggle will have drastic effects on xyz and rotational displacements as well, a toroidal universe means that that left/right and up/down edges of the screen wrap around much like in the video game Pac Man.  Enabling toroidal universe means that feedback can wrap around the screen, when used in conjunction with extreme displacements this can result in extreme fractal patterns.

9.  X displace (B) (-6.4,6.4), (-12.8,12.8), (-32,32), (-64,64)  This displaces the x position of fb0 measured in pixels.  For a sense of scale internal processing is done at a resolution of 640x480 so a displacement of 64 pixels means 1/10 of the screen.  Fractional displacement values correspond to averaging between pixels, so a displacement of .5 means an average between two pixels is being calculated implicitly.  Loosely speaking displacement amount for a certain scale corresponds to the speed of feedback trails and coarseness or fineness of patterns emerging.  Small displacement values result in smooth oozing trails and complex reaction diffusion structures.  Large displacement values result in distinct copies of the source material repeating themselves. Ridiculous displacement values can be used for manipulating large scale abstract shapes in a strange feedback synthesis version loosely reflective of more traditional analog style video synthesis.

10.  Y displace (B) (-4.8,4.8), (-9.6,9.6), (-24,24), (-48,48)  This works exactly the same as X displace but in the Y axis.  I'm not going to just cut and paste everything from the x displace description and crtl-f replace all x with y.  Do you ever read manuals where they do that for like 5 or 6 parameters?  What on earth are they thinking?  Why are manuals so often written as though they are instructing a robot in a task.  Like its pretty likely that a human or human like intelligence is reading these words, is it actually that difficult to try to communicate with them using anything other than "the Z PLANE FILTER SYSTEMtm is altered by the secret proprietary QUATERNION ROTATIONtm thus multimodal IIR filters ensue when the k plane of local resonance is hyperbolically mapped into the POINT AT INFINITYtm" being copy and pasted 18 times throughout the text.

11. Z displace (B) (.95,1.05), (.9,1.1), (.75,1.25), (.5,1.5)  The ranges for z displacement are calculated in terms of blowing up or shrinking as a percentage, so 1.5 means the image is increased in size by 150 percent and .5 means that the image is shrunk by 50 percent.  The exact operation being performed is a simultaneous multiplication in x and y positions.  Perhaps a more useful way to get a feel for how z displacement works is to turn on brightness invert, turn off toroidal universe, and just zoom in and out at each range.  When you are comfortable with that range of things then zoom out as far as possible and then turn on toroidal universe.  Ok start wiggling around x and y tiny bits now.  Then try some rotations next.  Make sure to get up and stretch your legs and arms and back out at least once every 45 minutes!  Physical comfort is an important priority.  I don't necessarily recommend smoking cigarettes but you could always try just pretending to just to have an excuse to step outside and stop staring at a screen at least 1nce per hour!

12 Rotate (B) (-PI/10), (-PI/5), (-2*PI/5,2*PI/5), (-8*PI/5,8*PI/5).  Note on terminology: these rotations are measured in radians not degrees, because degrees kind of only makes sense as a default measurement if one is a babylonian surveyor working in a sexagesimal number system.  As this is not one of the more common job descriptions in modern life lets all agree to let degrees drift away.  Honestly Tau seems like a better choice than PI as well but incremental updates are best to maintain backwards compatibility.  Radians measure rotations as how far one has traveled around the perimeter of a circle with radius 1.  2xPI -> 1 full rotation, PI/2 is a 90 degrees, PI is 180 degrees, etc etc.  Default rotations are all set to generate variations on 5 fold symmetrical shapes as opposed to 4 fold symmetries.  This is on purpose, try out 4 fold feedback symmetries and see if you can figure out why I would steer away from that as a default state.

13.  Chaotic Hue mod (U) Ok so to fully understand whats happening in a signal flow with this and the following two parameters check the appendix entry on "the logistic map and hue."  Otherwise I will try to just stick with qualitative explanations for these parameters in this section.  Chaotic Hue Mod sets a limit on the range that hue can cycle through.  Very high (low) values for this will result in monochrome or very minimal color spaces.  Wildly different behaviors can happen depending on the settings of HSB attenuation and Chaotic Hue LFO and Chaotic Hue Offset

14.  Chaotic Hue LFO (U) This is not exactly an LFO in the traditional sense of audio signal processing but it tends to feel like one in operation.  This can be used to change the rate, texture, and patterns of hue cycling.  For very small values smooth shifts occur.  For larger values intense hue strobing can happen.  Some certain settings can cause both saturation to drop out as well as brightness as well.  If you are curious as to why messing around with hue values can affect saturation and brightness it is because HSB is not the native color space, RBG is.  When RGB gets mapped into HSB and then we do very chaotic things to hue before mapping back into RGB, the effects can be unpredictable and can actually make the maps from HSB to RGB and back 'glitch out' a bit.

15.  Chaotic Hue Offset (U)  This adds an offset to the hue.  When Chaotic Hue mod is set to very high values resulting in a very restricted palette one can use the offset to choose the color.  When Chaotic Hue LFO is running and you would like to mix up the cycling you can use this as well.  Really the only way to get a handle on these 3 parameters is to use them heavily and changing them only in the default small ranges at first.  Each parameter will have different effects depending on where the other two parameters are at.  One very fun technique to use with these 3 parameters is to turn off keying, turn out mix to about .5, turn temporal filter to about .25 and temporal filter strength to about .5 and then experiment with different combinations of the Chaotic Hue parameters to use them as a kind of chaotic colorizer on your input!

16.  Delay time (U) (1-60)  Delay time is measured in Frames.  Waaave_pool operates at 30 frames per second so there is a total of 2 seconds of delay to draw from.  Delays will behave quite differently depending on if the framebuffer is fed from the input or from the final output.





![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool1.png)



list of default midi ccs
***(&(^%continuous things((**&&^^^

Luma key               -16

Mix                    -17

Hue_x                  -18

Sat_x                  -19

Bright_x               -20

Temporal filter mix    -21

Temp filter Q          -22

Input boost            -23

X displace             -120

Y displace             -121

Z displace             -122

Rotate                 -123

Chaotic hue mod        -124

Chaotic hue lfo        -125

Chaotic hue offset     -126

Delay time (in frames) -127


**77***&toggle switches&**^****(((

bright invert          -43

sat invert             -44

hue invert             -42

H mirror               -41

V mirror               -45

Toroidal Universe      -46

x skew                 -39

y skew                 -55

key swap b/w           -60

alt h mirror           -61

alt v mirror           -62 


on the nanokontrol for each of the sliders (midi cc values 0 through 6) there are three buttons alongside which I have programmed to change the total range of what each slider does.  I will list those cc values here  in the order (medium, large, ridiculous)  (small is default setting.  the larger buttons override smaller buttons so if medium, large, and ridiculous are all toggled then only ridiculous is actually enabled.  ridiculous settings for the xyz and rotate plus toroidal universe is where very ornate fractal painting mode gets started

x displace          (32,48,64)

y displace          (33,49,65)

z displace          (34,50,66)

rotate              (35,51,67)

chaotic hue mod     (36,52,68)

chaotic hue lfo     (37,53,69)

chaotic hue offset  (38,54,70)


there are 7 more toggles/momentary unassigned on the default control schemes at this moment. I have not explored different ranges on the total delay times yet so let me know if that seems desireable.  Suggestions for more switches are welcome as well 
