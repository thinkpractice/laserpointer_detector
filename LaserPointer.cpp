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
    namedWindow("Inverted Image",WINDOW_AUTOSIZE);
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

    SimpleBlobDetector::Params params;
    //TODO tweak parameters to detect laser dots of different size
    // Change thresholds
    params.minThreshold = 10;
    params.maxThreshold = 200;
     
    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.1;
     
    // Filter by Convexity
    params.filterByConvexity = true;
    params.minConvexity = 0.87;
     
    // Filter by Inertia
    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;

    Mat invertedImage;
    threshold(redHueImage, invertedImage, 200, 255, cv::THRESH_BINARY_INV);

    vector<KeyPoint> keypoints;
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    detector->detect(invertedImage, keypoints);

    drawKeypoints(image, keypoints, image, Scalar(0,255,0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(redHueImage, keypoints, redHueImage, Scalar(0,255,0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(invertedImage, keypoints,invertedImage, Scalar(0,255,0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    imshow("Original Image",image);
    imshow("Image",redHueImage);
    imshow("Inverted Image",invertedImage);
}

int main(int argc, char** argv)
{
    VideoCapture capture;
    Mat frame;

    pMog2 = createBackgroundSubtractorMOG2();
    capture.open(1);
    if (!capture.isOpened())
    {
        printf("Error opening video capture\n");
        return -1;
    }

    //capture.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);

    int i = 0;
    while (capture.read(frame))
    {
        if (frame.empty())
            break;
        if (i == 0)
            initOverlayCircles(frame);

        //drawOverlayCircles(frame, overlayCircles);
        detectAndDisplay(frame,i);
        
        char c = (char)waitKey(10);
        if (c == 27)
            break;
        i++;
    }
    capture.release();
}

