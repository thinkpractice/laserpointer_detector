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

    Mat hsv_image;
    cvtColor(filteredImage, hsv_image, cv::COLOR_BGR2HSV);

    Mat lowerRedHueRange;
    Mat upperRedHueRange;
    
    inRange(hsv_image, Scalar(0,100,100), Scalar(10, 255, 255), lowerRedHueRange);
    inRange(hsv_image, Scalar(160,100,100), Scalar(179, 255, 255),upperRedHueRange);

    Mat redHueImage;
    addWeighted(lowerRedHueRange,1.0, upperRedHueRange,1.0, 0.0, redHueImage);
    GaussianBlur(redHueImage, redHueImage, Size(9,9), 2,2);

    imshow("Image",redHueImage);
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

