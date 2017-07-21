#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <cstdlib>
using namespace cv;
using namespace std;

Mat background;
Ptr<BackgroundSubtractor> pMog2;

vector<Vec6i> overlayCircles;

void initOverlayCircles(Mat image, int numberOfCircles=150)
{
    for (int i = 0; i < numberOfCircles; i++)
    {
        int x = rand() % image.rows;
        int y = rand() % image.cols;
        int size = rand() % 10;
        int dx = rand() % 10;
        int dy = rand() % 10;
        Vec6i overlayCircle(x,y,size,dx,dy,0);

        overlayCircles.push_back(overlayCircle);
    }
}

void drawOverlayCircles(Mat image, vector<Vec6i>& circles)
{
    for (Vec6i& overlayCircle : circles)
    {
        overlayCircle[0] += overlayCircle[3];
        overlayCircle[1] += overlayCircle[4];
        if (overlayCircle[0] <= 0 || overlayCircle[0] >= image.cols)
            overlayCircle[3] *= -1;

        if (overlayCircle[1] <= 0 || overlayCircle[1] >= image.cols)
            overlayCircle[4] *= -1;

        circle(image, Point(overlayCircle[0], overlayCircle[1]), overlayCircle[2], Scalar(0,0,255), CV_FILLED, LINE_AA);
    }
}

void detectAndDisplay(Mat image, int i)
{
    namedWindow("Original Image",WINDOW_AUTOSIZE);
    namedWindow("Image",WINDOW_AUTOSIZE);
    Mat filteredImage;
    medianBlur(image, filteredImage, 9);

    Mat hsvImage;
    cvtColor(filteredImage, hsvImage, cv::COLOR_BGR2HSV);

    Mat lowerRedHueRange;
    Mat upperRedHueRange;
    
    inRange(hsvImage, Scalar(0,100,100), Scalar(10, 255, 255), lowerRedHueRange);
    inRange(hsvImage, Scalar(160,100,100), Scalar(179, 255, 255),upperRedHueRange);

    Mat redHueImage;
    addWeighted(lowerRedHueRange,1.0, upperRedHueRange,1.0, 0.0, redHueImage);

    vector<KeyPoint> keypoints;
    SimpleBlobDetector detector;
    detector.detect(redHueImage, keypoints);

    //drawKeypoints(image, keypoints, image, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    //vector<Vec3f> circles;
    //HoughCircles(redHueImage, circles, HOUGH_GRADIENT,2, 10, 100, 50, 0, 0);
    //////printf("Number of detected circles: %d\n",(int)circles.size());
    //for(Vec3i c : circles)
    //{
    //    circle(image, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
    //    circle(image, Point(c[0], c[1]), 2, Scalar(0,0,255), 3, LINE_AA);
    //}
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
        if (i == 0)
            initOverlayCircles(frame);

        drawOverlayCircles(frame, overlayCircles);
        detectAndDisplay(frame,i);
        
        char c = (char)waitKey(10);
        if (c == 27)
            break;
        i++;
    }
    capture.release();
}

