#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
using namespace cv;
using namespace std;

Mat background;
Ptr<BackgroundSubtractor> pMog2;
  
void detectAndDisplay(Mat image, int i)
{
    namedWindow("Image",WINDOW_AUTOSIZE);
    Mat filteredImage;
    medianBlur(image, filteredImage, 9);

    Mat bgr[3];
    split(filteredImage, bgr);

    Mat foregroundImage;
    pMog2->apply(bgr[2],foregroundImage);
    
    Mat threshold1;
    Mat threshold2;
    Mat threshold3;
    threshold(bgr[0], threshold1, 250, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    imshow("Image",threshold1 & threshold2 & threshold3);
}

int main(int argc, char** argv)
{
    VideoCapture capture;
    Mat frame;

    pMog2 = createBackgroundSubtractorMOG2();
    capture.open(0);
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
    capture.release();
}

