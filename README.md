# waaaave_pool

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool4.png)

a port of video_waaaves to raspberry pi
the larger desktop versions can be found here:  
full sized
https://github.com/ex-zee-ex/VIDEO_WAAAVES_1_75,  
lil sized
https://github.com/ex-zee-ex/lil_waaaves
there are multiple versions up for playing with right now but the one titled WAAAVE_POOL_MAIN is the most recent and bestest build, also the only one that works with the picapture sd so try that one first.

hello and welcome to the waaaves pool!  to get started with this program you will need at least raspberry pi(i built and tested this in raspbian on a raspberry pi 3 b v1.2), a usb camera (all images and demo videos were made with a logitech c310 which can be found very affordably on ebay.  i highly reccomend only logitech usb cameras as the only brand im aware of with consistent driver support cross platforms) or a picapturesd for input, a usb keyboard for controls.  There is option to support a usb midi keyboard as well(the default midi controls are mapped to the default cc values for the knobs on a korg nanostudio in the current version, there are instructions on midi mapping below).  A build of openFrameworks is also necessary, https://openframeworks.cc/setup/raspberrypi/ for more info.  Once you have openFrameworks installed and working you will also need to install the ofxMidi addon https://github.com/danomatika/ofxMidi and the ofxOMXCamera addon https://github.com/langolierz/ofxOMXCamera.  For midi controllers this code is built around a korg nanokontrol2 and its

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool3.png)

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

