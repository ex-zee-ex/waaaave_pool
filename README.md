# waaaave_pool

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool4.png)

a port of video_waaaves to raspberry pi
the larger desktop versions can be found here
full sized
https://github.com/ex-zee-ex/VIDEO_WAAAVES_1_75
lil sized
https://github.com/ex-zee-ex/lil_waaaves

hello and welcome to the waaaves pool!  to get started with this program you will need a raspberry pi(i built and tested this in raspbian on a raspberry pi 3 b v1.2), a usb camera (all images and demo videos were made with a logitech c310 which can be found very affordably on ebay.  i highly reccomend only logitech usb cameras as the only brand im aware of with consistent driver support cross platforms), a usb keyboard for controls.  There is option to support a usb midi keyboard as well(the default midi controls are mapped to the default cc values for the knobs on a korg nanostudio in the current version, there are instructions on midi mapping below).  A build of openFrameworks is also necessary, https://openframeworks.cc/setup/raspberrypi/ for more info.  The midi version requires the ofxMidi addon https://github.com/danomatika/ofxMidi.

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool3.png)

keyboard controls
for each pair x/y means that the corresponding alphanumerical keys on a keyboard increment and decrement the values

a/z - zoom in/out
s/x - framebuffer0 x displacement
d/c - framebuffer0 y displacement
f/v - framebuffer0 hue attenuation
g/b - framebuffer0 saturation attenuation
h/n - framebuffer0 brightness attentuation
j/m - frambuffer0 mix
k/, - framebuffer0 lumakey
[/] - framebuffer0 change delay amount
q/w - framebuffer0 rotate
o/p - framebuffer1 mix
1   - clears the framebuffers

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool2.png)


if you are using the midi version then these keyboard controls will work as well but if you change them then they change the ranges of the midi controls heads up

for midi mapping your usb midi controller check ofApp.cpp and ctrl-f for "MIDIMAPZONES".  check the instructions there and have fun!

![Image description](https://github.com/ex-zee-ex/pictures/blob/master/waaavepool0.png)

