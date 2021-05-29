Currently 1, 2 ,3a, 3b, 3c, 3d are basically finished. The runpipeline executable file is generated in Windows and it is .exe file. To run it in Macbook or Linux, the runpipeline.cpp file can be re-compiled (there should not be any error when re-compiling in Macbook or Linux; it should only take seconds). house_1.ppm is a "P6" ppm image file for testing purpose. The runpipeline.cpp supports both P6 and P3 headers. 

The command to run the "runpipeline" executable is as followed:

./runpipeline -i house_1.ppm -o house_1_out.ppm -grey 

or 

./runpipeline -i house_1.ppm -o house_1_out.ppm -flip

or 

./runpipeline -i house_1.ppm -o house_1_out.ppm -rot90 

or 

./runpipeline -i house_1.ppm -o house_1_out.ppm -scale


currently -grey, -flip, rot90 and -scale needs to be run seperately. 

All the input -i -o -grey/-flip/-rot90/-scale needs to be specifed. 

The output file is a ".ppm" file (file name is specified by user in -o option).


######################################################################

What still needs to be done?


(1) 3e - 3i still needs to be finished 

(2) runpipeline.cpp file needs to be refactored into smaller cpp and header files

(3) more error handling cases needs to be taken into consideration with edge cases users input option

(4) memory optimization still needs to be done
