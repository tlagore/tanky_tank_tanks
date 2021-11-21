# tanky_tank_tanks
A 2d game developed for the Atari ST using the STEem Engine Atari Emulator.

Here's a gross little hand recorded video of an [early version](https://www.youtube.com/watch?v=MACQNG1RRLw) in action if you can get it running: 

## Introduction ##
Tanky tank tanks was a project written for the 1984 Atari using the [STEem Engine Atari Emulator](http://tamw.atari-users.net/steem.htm).
It was written as a school project for a course, Computing Machinery 2. 

This project interacts directly with (virtual) hardware to create sound effects, music, animations, and more. A great learning project if you want to understand how the low level stuff (originally) worked. 

## Overview ##
Unfortunately, at the time of writing the project - I was not concerned with/aware of version control. So what you see is what you get.

I took a great interest in this project and felt it deserved to be maintained on some level of version control and made it open source  in case anyone wanted to examine the code and/or use the code to continue the project.

## Installation ##
Unfortunately it has been a loooong time since I have used/run this code, but essentially head on over to the STEem web page (linked above).

Download the engine and place the code somewhere in the virtual file system of the emulator. Thankfully, I at least was competent enough at the time to create a makefile for every version (I believe every version.. the very earliest ones might not, but they do not really matter in terms of the project). Should be able to just compile and run... that is up until stage 10C, explained below.

## Stages ##
Each stage is a single iteration of the project, implementing new features. Unfortunately, I have somehow lost Stage10A and Stage10B. Stage10C thankfully encompasses both 10A and 10B. 

Stage 10 was to remove the operating system control completely by implementing our own interrupts (overwriting the vector table).

Essentially, you boot the tanky tank tanks image and the STEem emulator will simply act as a tanky tank tanks emulator (it will boot directly into the game). The bad news is that I don't remember the exact process behind mounting the tanky tank tanks image.

Note that 10C attempted to implement networking over a serial line. I failed to complete the networking in time, nor was I able to test it. I did implement what I *believed* was the correct process to network - but it wasn't at all tested so I'm certain that it doesn't work. Feel free to try to make it work!

##Conclusion##
Might add more to this readme if anyone stumbles across this and is interested. But I guess if you plan to work on this project, you probably already know more about STEem than I could possibly remember.
