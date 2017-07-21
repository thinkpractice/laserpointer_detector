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
    namedWindow("Original Image",WINDOW_AUTOSIZE);
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
    medianBlur(redHueImage, redHueImage, 3);

    vector<Vec3f> circles;
    HoughCircles(redHueImage, circles, HOUGH_GRADIENT,2, redHueImage.rows / 16, 100, 20, 0, 0);
    //printf("Number of detected circles: %d\n",(int)circles.size());
    for(Vec3i c : circles)
    {
        circle(image, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        circle(image, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
    }
    imshow("Original Image",image);
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

