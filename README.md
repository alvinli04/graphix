# Graphics Final Project
---
## Names: Alvin Li and Jeremy Ku-Benjet
## Period 4

---
## New Features:
- [ ] Multiple lights
- [ ] Gouraud and Phong Shading
- [x] Meshes
- [ ] Cel Shading

---
## The Details:
The implementation of most things listed will be in accordance with how each command is outlined in MDL.spec.
- Multiple lights: when a light is added using `light r g b x y z`, it adds a new light to the list. When lighting is calculated
while drawing a frame, we loop through the list of lights.
- Gouraud and Phong shading: the user can choose what lighting model to use that looks best. We will be implementing both types.
- Meshes: The user will be able to upload a mesh onto the screen from an external `.obj` file and draw it to the screen.
- Cel Shading: This is an illumination model similar, similar to the Phong reflection model, except instead of color varying almost continously with the intensity the light on the surface, the colors are limited to only a few gradiants. This is to be reminicent of cartoon drawings.
