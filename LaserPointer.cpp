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
    Mat planes[3];
    split(image,planes); 
    namedWindow("Image",WINDOW_AUTOSIZE);
    
    Mat blurredImage;
    GaussianBlur(planes[2], blurredImage, Size( 31, 31 ), 0, 0);

    Mat normalizedRed;
    //normalize(blurredImage, normalizedRed, 0, 255, NORM_MINMAX);
    normalizedRed = blurredImage;
    if (i==0)
    {
        background = normalizedRed;
    }
    Mat thresholdedImage;
    threshold(normalizedRed-background, thresholdedImage, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
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

