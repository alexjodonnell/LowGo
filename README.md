# LowGo
Working on a tiny renderer in c++ that uses no external libraries. The goal is to see how low I can go.
The real purpose for a project like this is to get an intuition for how graphics libraries like OpenGL
work under the cover. 

# Uses
Currently the software can draw lines and basic shapes to a TGA formatted image. Can parse .obj files 
using the Model class to make wacky lofi looking images. I used it to make my current profile picture. 
It can now read tga diffuse images and set the texture of a poly based
 on the obj file. Drawing in perspective has been added. With the use of a simple matrix transformation, a camera is modeled that can travel along the z axis.
