#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

class ImagePPM {
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

        void saveImage(string name_file);
        void readImage(string name_file);

        void horizontal_flip();
        void rotate_90();
        void greyScale();
        void resize(int height, int width);

        void deleteImage();

        RGB **image = nullptr;


    private:
        int height = 0;
        int  width = 0;
        string version = "P6";

        void create_image();
    
};

ImagePPM::ImagePPM(int height, int width) {
    setHeight(height);
    setWidth(width);
    create_image();
}


void ImagePPM::saveImage(string name_file) {
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

        create_image();

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

void ImagePPM::create_image() {
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



int main(int argc, char *argv[], char **envp) {

    ImagePPM ins1(500, 1000);

    for(int i=0; i < 500; i ++)
        for(int j =0; j< 1000; j++) {
            ins1.image[i][j].r = 50;
            ins1.image[i][j].g = 168;
            ins1.image[i][j].b = 82;
        }
    ins1.setVersion("P3");  // tetting the setVersion and saveImage methods with above radom rgb values 
    ins1.saveImage("Result.ppm");   

    // main program begins here 
   if(argc>=2) {

        std::string sinput1 (argv[1]);
        
        if(sinput1.find("i")) {  // -i or --i input will work; 

            printf("nothing2");

            ImagePPM ins2;
            ins2.readImage(argv[2]);

            ins2.saveImage("Result.ppm");


        } else {
            printf("need to specify userinput; please refer to github readme file");
        }


    }


}





