# Naive Raytracer

## Description

A naive, mostly commented, basic raytracing system in C++.

This is a pretty textbook implementation of a raytracer currently.  If you're looking for any advanced techniques, you won't find them here.  You're welcome to look if you want though.

## Purpose

The main purposes of this project are to:  
* Refresh and increase my knowledge of raytracing through the implementation of a raytracer.  
* To become familiar with using git.

## Existing Functionality

### Scene Elements

* Planes  
* Infinite Planes  
* Spheres  
* Cameras  
* Point Lights  
* Disks  
* Cuboids  
* Triangles  

### Anti-Aliasing Options

* none - no anti-aliasing  
* naive-average - a naive weighted average of a pixel with its neighbors  
* fsaa4 - 4xFSAA (scene is sampled at 4x the normal resolution and averaged down to normal)  
* fsaa16 - 16xFSAA (scene is sampled at 16x the normal resolution and averaged down to normal)  
* edaa4 - 4xEDAA (scene is sampled at 4x the normal resolution where edges are detected)  
* edaa16 - 16xEDAA (scene is sampled at 16x the normal resolution where edges are detected)  
* fxaa - FXAA (scene is blurred using a gaussian blur where edges are detected)  

## Planned Functionality

### Scene Elements

* Tubes  
* Cylinders  

### Anti-Aliasing Options

* None

## Usage

This currently outputs a .bmp file for each camera in a scene: \<scene_name\>-\<camera_number\>.bmp or \<scene_name\>-\<name\>.bmp if the camera name parameter is defined.

Usage: ./\<program\> \<path-to-scene-file\>

This code is being run in Windows. It may run in Mac/Linux, but no testing has been done to that effect.

Example scene files are in /scenes.

## License

This project is licensed under the BSD license.
