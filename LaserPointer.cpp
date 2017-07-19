#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

Mat background;

void detectAndDisplay(Mat image, int i)
{
    namedWindow("Image",WINDOW_AUTOSIZE);
    
    if (i==0)
    {
        background = image;
    }
    Mat imageHsv;
    cvtColor(image,imageHsv,CV_RGB2HSV);

    Mat hsvComponents[3];
    split(imageHsv, hsvComponents);

    Mat thresholdImage1;
    Mat thresholdImage2;
    Mat thresholdImage3;
    threshold(hsvComponents[0], thresholdImage1, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    threshold(hsvComponents[1], thresholdImage2, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    threshold(hsvComponents[2], thresholdImage3, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    

    Mat thresholdedImage;
    thresholdedImage = thresholdImage1 & thresholdImage2 & thresholdImage3;
    imshow("Image",thresholdedImage);
}

int main(int argc, char** argv)
{
    VideoCapture capture;
    Mat frame;

    capture.open(1);
    if (!capture.isOpened())
    {
        printf("Error opening video capture\n");
        return -1;
    }

    int i = 0;
    while (capture.read(frame))
    {
        if (frame.empty())
            break;
        detectAndDisplay(frame,i);
        char c = (char)waitKey(10);
        if (c == 27)
            break;
        i++;
    }
}

