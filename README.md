# cs184
Authors: Flora Dai (-bm), Tabitha King (-cs), Claire Thompson (-dw)
Platform: OSX
Source Code: cs184-dw (Claire Thompson) - Platform: OSX
 	Runs on s275-12.cs.berkeley.edu or any s275-XX.cs.berkeley.edu inst machines

Location of image-XX.png: assign2/
Location of input-XX:	  assign2/
Location of notes-nn:	  ./	

Implemented Features:
	Renders arbitrarily oriented ellipsoids
	Renders polygons (ie read a simple .obj file and render the polygons in it)
	Use simple Phong Shading (in color)
	Compute shadows
	Compute reflections
	Apply linear transformations to objects
	Use point and directional lights
	Writes output to PNG format as "test.png"

/* Ignore Below */
Run '''make check''' to run the tests.

FreeImage Library:
	Make sure to recompile FreeImage library file (.a) on instruction machine being used to turn in!!

	Instructions (LINUX/OSX)
		1. Unzip FreeImage3160.zip file
		2. '''cd FreeImage/'''
		3. '''make'''
		4. Copy over .a file and FreeImage.h in FreeImage/Source/ to project directory
		5. Compile project
