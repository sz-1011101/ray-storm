# ray-storm - Yet Another CPU Path Tracer

## Introduction

This is the path tracer I'm currently developing for educational purposes.
It attempts to solve the famous *Rendering Equation* by Kajiya[^Kajiya86].

Note that the code is not optimized for speed. I'm doing this mostly for practise. And because getting a physically plausible image at the end of the day is kinda cool.

Physical correctness may vary.

## Features

The follwing features are implemented:

* Different Integrators (Naive path tracing, path tracing with direct lighting, Simple bidirectional path tracing)
* Materials: Lambertian BRDF, Phong BRDF, Perfect mirror, glass, Fresnel handling, ...
* Camera Models: Pinhole camera, thin lens camera
* Basic geometric primitives: Spheres, boxes, rectangles
* Interfaces to render ray marched primitives
* Simple procedural textures

```c
// TODO implement at some time in the future
```
* Accelerator structures: Grids, octrees, that sort of stuff
* Support for Participating media
* Primitives: Triangles, triangle models, more ray marched stuff
* Weird/advanced camera models and effects, e.g. fisheye lenses, lens flare
* ...

## Libraries

This project uses the following libraries:

* **GLM** for linear algebra math
* **OpenMP** for parallelization
* **OpenCV** for image operations, generation and display
* **boost** for cmd line parsing

[^Kajiya86]: James T. Kajiya. 1986. The rendering equation. In Proceedings of the 13th annual conference on Computer graphics and interactive techniques (SIGGRAPH '86), David C. Evans and Russell J. Athay (Eds.). ACM, New York, NY, USA, 143-150.