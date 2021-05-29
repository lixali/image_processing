#include <fstream>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class ImagePPM { // only class that declare image manipulation functions
    public:
        ImagePPM(int height, int width);
        ImagePPM() {}

        struct RGB {
            unsigned char r;
            unsigned char g;
            unsigned char b;

        };

        void setWidth(int width) {this->width = width;}
        void setHeight(int height) {this->height = height;}
        void setVersion(string version) {this->version = version;}

        void saveImage(std::string name_file);
        void readImage(std::string name_file);

        void horizonFlip(); // flip from left to right
        void rotate_90(); // rotate 90 degree clock wise
        void greyScale(); // convert to grey scale
        void scaleSize(int height, int width); // scale image 

        void deleteImage();

        RGB **image = nullptr;


    private:
        int height = 0;
        int  width = 0;
        string version = "P6";

        void createImage();
    
};

ImagePPM::ImagePPM(int height, int width) {
    setHeight(height);
    setWidth(width);
    createImage();
}


void ImagePPM::saveImage(std::string name_file) {
    ofstream output(name_file, ios::binary);

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


void ImagePPM::readImage(string name_file) {
    ifstream input(name_file, ios::binary);

    if(input.is_open()) {
        int color;
        char ver[3];

        input.read(ver, 2);
        version = ver;
        input >> width;
        input >> height;
        input >> color;
        input.read(ver, 1);
        //printf("inside read version is %s \n", version);

        createImage();

        int box;

        if(version == "P3"){
            for(int i= 0; i < height; i++) 
                for (int j = 0; i< width; j++) {
                    input >> box;
                    image[i][j].r = box;

                    input >> box;
                    image[i][j].g = box;

                    input >> box;
                    image[i][j].b = box;
                }
        } else {

            for(int i=0; i < height; i++) 
                for(int j = 0; j < width; j++) {
                    input.read((char*) &image[i][j], sizeof(RGB));
                }
        }

        input.close();
    }
}

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

// this is a function to rotate an image 90 degree clock wise
void ImagePPM::rotate_90() {

    for(int i =0; i < height/2; i++){
        for(int j=0; j < width; j++) {
            swap(image[i][j], image[height-1-i][j]);
        }
    }
}

// this is a function to covert a picture to grey scale
void ImagePPM::greyScale() {
    const float r = 0.299f; // these 3 are the grey scale factor as specified in the google doc
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


void ImagePPM::deleteImage() {

    if(image != nullptr){
        for(int i =0; i<height; i++) {
            delete image[i];
        }

        delete image;
    }
}


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

// main program begins here 
int main(int argc, char *argv[], char **envp) {

    ImagePPM ins1(400, 950);
    ImagePPM ins2;
    std::string outFile (argv[4]);;

    if (argc>=5) {

        std::string sinput2 (argv[3]);

        if (sinput2 == "-o") {
             std::string outFile (argv[4]);
        } else {
             std::string outFile (argv[4]);
        }
    } else {
        std::string outFile ("Result.ppm");
    }

    for(int i=0; i < 400; i ++)
        for(int j =0; j< 950; j++) {
            ins1.image[i][j].r = 50;
            ins1.image[i][j].g = 168;
            ins1.image[i][j].b = 82;
        }
    ins1.setVersion("P3");  // tetting the setVersion and saveImage methods with above radom rgb values 
    ins1.saveImage(outFile);   


    ins2.readImage(argv[2]);

   if(argc>=2) {

        std::string sinput1 (argv[1]);
        
        if(sinput1 == "-i") {  // looking for -i

            //printf("nothing2");

            if(argc>=6) {
                std::string sinput3 (argv[5]);

                if(sinput3 == "-grey") {
                    printf("enter");
                    ins2.greyScale();
                } else if (sinput3 == "-flip") {
                    printf("flip");
                    ins2.horizonFlip();
                } else if (sinput3 == "-rot90") {
                    printf("rotate");
                    ins2.rotate_90();
                } else if (sinput3 == "-scale") {
                    printf("scale");
                    ins2.scaleSize(242, 384);
                }
            }



        } else {
            printf("need to specify userinput; please refer to github readme file");
        }


    }

    ins2.saveImage(outFile);



}





