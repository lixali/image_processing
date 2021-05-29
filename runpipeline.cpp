#include <fstream>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class ImagePPM { // only class that declare image manipulation functions
    public:
        ImagePPM(int height, int width);
        ImagePPM() {}

        struct RGB { // struct for reb, green, blue colors
            unsigned char r;
            unsigned char g;
            unsigned char b;

        };

        void setWidth(int width) {this->width = width;}
        void setHeight(int height) {this->height = height;}
        void setVersion(string version) {this->version = version;}

        void saveImage(std::string name_file);
        int * readImage(std::string name_file);

        void horizonFlip(); // flip from left to right
        void rotateEdage(int tR, int tC, int dR, int dC);
        void rotate_90(); // rotate 90 degree clock wise
        void greyScale(); // convert to grey scale
        void scaleSize(int height, int width); // scale image 

        void deleteImage();


        RGB **image = nullptr;


    private:
        int height = 0;
        int  width = 0;
        string version = "P6"; // by default, it is processing P6 ppm file; 
                               // it can also process P3 ppm file by using the following setVersion function

        void createImage();
    
};

ImagePPM::ImagePPM(int height, int width) {
    setHeight(height);
    setWidth(width);
    createImage();
}

// save user input to a new ppm file
void ImagePPM::saveImage(std::string name_file) {
    ofstream output(name_file, ios::binary); // use ofstream to write to image

    if(output.is_open()) {
        output << version << endl;
        output << width << endl;
        output << height << endl;
        output << 255 << endl;

        if(version == "P3") { 
            for(int i = 0; i < height; i++) {
                for (int j = 0; j < width ; j++) {
                    output << (int) image[i][j].r << ' ';
                    output << (int) image[i][j].g << ' ';
                    output << (int) image[i][j].b << '\n';
                }
            }
        } else 
            for(int i = 0; i < height ; i++) {
                for (int j = 0; j < width; j ++) {
                    output.write((char*) &image[i][j], sizeof(RGB));
                }
            }
        output.close();

    }
}

// read user input ppm file; return depth, width for future scaling usage
int * ImagePPM::readImage(string name_file) {
    ifstream input(name_file, ios::binary); // use ifstream to read an image
    int count = 0;
    static int dimensions[2];

    if(input.is_open()) {
        int color;
        char ver[3];

        input.read(ver, 2);
        version = ver;
        input >> width;
        input >> height;
        input >> color;
        input.read(ver, 1);

        printf("image height is %u \n", height); // as required in 1, image dimension are printed out 
        printf("image width is %u \n", width);

        if (color == 255) {                     // // as required in 1, bit depth is printed out 
            printf("image bit depth is %u \n", 8); 
        } else if (color == 15) {
            printf("image bit depth is %u \n", 4);
        } else if (color == 6635) {
            printf("image bit depth is %u \n", 16);
        }

        createImage();

        int box;

        if(version == "P3"){ // when it is P3 header
            for(int i= 0; i < height; i++) 
                for (int j = 0; i< width; j++) {
                    input >> box;
                    image[i][j].r = box;

                    input >> box;
                    image[i][j].g = box;

                    input >> box;
                    image[i][j].b = box;

                    count += 1;
                }
        } else { // when it is P6 header

            for(int i=0; i < height; i++) 
                for(int j = 0; j < width; j++) {
                    input.read((char*) &image[i][j], sizeof(RGB));
                    count += 1;
                }
        }

        if (input.eof()) { // as required by 1 , it validate the pixel number matches by looking at if it reaches the end of file
            printf("it is  not reaching the end of if file; there is error in image dimension \n");
        } else {
            printf("it is reaching the end of if file; there is no error in image dimension \n");
        }

        input.close();
    }

    dimensions[0] = height;
    dimensions[1] = width;

    return dimensions;
}

// create a new ppm image
void ImagePPM::createImage() {
    if(image != nullptr) {
        deleteImage();
    }

    image = new RGB*[height];

    for(int i = 0; i < height; i++) {

        image[i] = new RGB[width];

        for(int j=0; j < width; j++) {
            image[i][j].r = 255;
            image[i][j].g = 255;
            image[i][j].b = 255;
        }
    }
}

// this is a function to flip a picture from left to
void ImagePPM::horizonFlip() {

    for(int i=0; i<height; i++) {
        for(int j = 0; j < width/2; j++) {
            swap(image[i][j], image[i][width-1-j]);
        }
    }
}

// rotate edge is to be called by rotate_90 to rotate 90 degree
void ImagePPM::rotateEdage(int tR, int tC, int dR, int dC) {

    RGB tmp;

    int times = dR - tR;
    for (int i = 0; i != times; ++i) {
        tmp = image[tR][tC + i];
        image[tR][tC + i] = image[dR - i][tC];
        image[dR - i][tC] = image[dR][dC - i];
        image[dR][dC - i] = image[tR + i][dC];
        image[tR + i][dC] = tmp;
    }
}

//  rotate an image 90 degree clock wise
void ImagePPM::rotate_90() {

    int tR = 0;
    int tC = 0;
    int dR = height -1;
    int dC = width - 1;

    while (tR < dR) {
        ImagePPM::rotateEdage(tR++, tC++, dR--, dC--);
    }
    
}

//  covert a picture to grey scale
void ImagePPM::greyScale() {
    const float r = 0.299f; // as required in the google doc, these 3 are the grey scale factor 
    const float g = 0.587f;
    const float b = 0.114f;

    float greyscaleValue;

    for(int i = 0; i < height; i++) {
        for(int j=0; j<width; j++) {

            greyscaleValue = image[i][j].r*r + image[i][j].g*g + image[i][j].b*b;
            image[i][j].r = greyscaleValue;
            image[i][j].g = greyscaleValue;
            image[i][j].b = greyscaleValue;
        }
    }
}

// scale an image ; height and width defined by user
void ImagePPM::scaleSize(int height, int width) {

    RGB **image_resized = new RGB*[height];

    for(int i =0; i<height; i++) {
        image_resized[i] = new RGB[width];

        for(int j = 0; j < width ; j++) {
            image_resized[i][j].r = 255;
            image_resized[i][j].g = 255;
            image_resized[i][j].b = 255;
        }
    }

    for(int i = 0; i < height; i++) {
        for(int j =0; j < width; j++) {
            image_resized[i][j] = image[i*this->height/height][j*this->width/width];
        }
    }

    deleteImage();
    image = image_resized;

    this->height = height;
    this->width = width;
}

// delete image 
void ImagePPM::deleteImage() {
    if(image != nullptr){
        for(int i =0; i<height; i++) {
            delete image[i];
        }
        delete image;
    }
}


// main program begins here 
int main(int argc, char *argv[], char **envp) {

    ImagePPM ins1(400, 950); // insstance1(ins1) is only for testing and debuggig purpose to make sure that all the functions are working
    ImagePPM ins2;           // instance2(ins2) is used to generate user output
    std::string outFile (argv[4]);;
    int * dimen;

    if (argc>=5) {

        std::string sinput2 (argv[3]);

        if (sinput2 == "-o") {  //as required in 2 in google doc , look for -o output file name
             std::string outFile (argv[4]);
        } else {
             std::string outFile (argv[4]);
        }
    } else {
        std::string outFile ("default_output.ppm");
    }

    for(int i=0; i < 400; i ++)
        for(int j =0; j< 950; j++) {
            ins1.image[i][j].r = 50;
            ins1.image[i][j].g = 168;
            ins1.image[i][j].b = 82;
        }
    ins1.setVersion("P3");  // setting the setVersion and saveImage methods with above radom rgb values 
    ins1.saveImage(outFile);   


    dimen = ins2.readImage(argv[2]);

   if(argc>=2) {

        std::string sinput1 (argv[1]);
        
        if(sinput1 == "-i") {  // as required in 1 in google doc, looking for -i input ppm file

            if(argc>=6) {
                std::string sinput3 (argv[5]);

                if(sinput3 == "-grey") { // as required in 3a in google doc, convert to greyscale; 
                    printf("grey \n");
                    ins2.greyScale();
                } else if (sinput3 == "-flip") { // as required in 3b in google doc, flip image; 
                    printf("flip \n");
                    ins2.horizonFlip();
                } else if (sinput3 == "-rot90") { // as required in 3c in google doc, rotate image by 90 degree; 
                    printf("rotate \n");
                    ins2.rotate_90();
                } else if (sinput3 == "-scale") { // as required in 3d in google doc, scale image by 0.25; 
                    int newDepth = *(dimen);
                    int newWidth = *(dimen+1);
                    printf("the new dimension is %u x %u \n", newDepth/4, newWidth/4); //
                    ins2.scaleSize(round(newDepth/4), round(newWidth/4));
                }
            }
        } else {
            printf("need to specify userinput; please refer to github readme file");
        }
    }
    ins2.saveImage(outFile);

}





