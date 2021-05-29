Currently 1, 2 ,3a, 3b, 3c, 3d are basically finished. 

The command to run the "runpipeline" is as followed:

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
