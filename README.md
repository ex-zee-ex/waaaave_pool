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

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool2.png)


if you are using the midi version then these keyboard controls will work as well but if you change them then they can potentially change or offset the ranges of the midi controls heads up

for midi mapping your usb midi controller check ofApp.cpp and ctrl-f for "MIDIMAPZONES".  check the instructions there and have fun!

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

X displace             -0

Y displace             -1

Z displace             -2

Rotate                 -3

Chaotic hue mod        -4

Chaotic hue lfo        -5

Chaotic hue offset     -6

Delay time (in frames) -7


**77***&toggle switches&**^****(((

bright invert          -43

sat invert             -44

hue invert             -42

H mirror               -41

V mirror               -45

Toroidal Universe      -46

x skew                 -39

y skew                 -55


on the nanokontrol for each of the sliders (midi cc values 0 through 6) there are three buttons alongside which I have programmed to change the total range of what each slider does.  I will list those cc values here  in the order (medium, large, ridiculous)  (small is default setting.  the larger buttons override smaller buttons so if medium, large, and ridiculous are all toggled then only ridiculous is actually enabled.  ridiculous settings for the xyz and rotate plus toroidal universe is where very ornate fractal painting mode gets started

x displace          (32,48,64)

y displace          (33,39,65)

z displace          (34,40,66)

rotate              (35,41,67)

chaotic hue mod     (36,42,68)

chaotic hue lfo     (37,43,69)

chaotic hue offset  (38,44,70)

there are 7 more toggles/momentary unassigned on the default control schemes at this moment. I have not explored different ranges on the total delay times yet so let me know if that seems desireable.  Suggestions for more switches are welcome as well 
