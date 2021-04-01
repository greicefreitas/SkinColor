#include"Header.h"

#include <vector>
#include <iostream>
#include <string> 
#include <filesystem>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using std::cout;
using namespace cv;
using namespace filesystem;
namespace fs = std::filesystem;



/// <summary>
/// Return a vector with the RGB values of the skin areas.
/// </summary>
/// <param name="path"></param>
/// <returns></returns>

vector<vector<int>> read_files(string  path)
{
    
    string foldername;
    string imagename;
    string maskname;
    Mat image;
    Mat mask;
    Mat out_image;
    vector<vector<int>> values;
    vector<Point> locations;   // output, locations of non-zero pixels
    vector<int> BGR = {0,0,0};

    for (const auto& entry1 : directory_iterator(path)) {
        foldername = entry1.path().filename().string();

            if (foldername.find("GroundT_") == std::string::npos) { // Get the subfolders without 'GroundT_'
                cout << "Foldername: " << foldername << endl; //Print the name of the file (or folder)
                cout << " . . . " << endl;
                for (const auto& entry2 : directory_iterator(entry1.path())) { //Looking into foldername: loading images! 

                    imagename = entry2.path().filename().string();

                    cout << "File: " << imagename << endl;

                    image = imread(entry2.path().string()); //ok!
                
                    if (image.empty())
                    {
                        break;
                    }

                    maskname = entry1.path().parent_path().string() + "/GroundT_" + 
                        foldername + "/" + imagename.replace(imagename.find("jpg"), 3, "png");


                    cout << maskname << endl;

                    mask = imread(maskname, CV_8UC1);
                    

                    if (mask.empty())
                    {
                        cout << "Mask not found " << endl;
                        break;
                    }

                                
                    //bitwise_and(image, image, out_image, mask); // Applying mask to the image!
                    
                    findNonZero(mask, locations);
                                   
                    
                    for (Point p : locations) {


                        BGR[0] = (int)image.at<Vec3b>(p)[0]; //getting only one channel
                        BGR[1] = (int)image.at<Vec3b>(p)[1]; //getting only one channel
                        BGR[2] = (int)image.at<Vec3b>(p)[2]; //getting only one channel

                        values.push_back(BGR); //including the values in the vector

                    }


                    //imshow("Image", bitwise_and(image, out_image, mask));
                    //imshow("Mask", mask);
                    //waitKey();
                                       

                }

                cout << "-------------------------" << endl;
            }
    }

    return values;
}