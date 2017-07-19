#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat image = imread("/home/tjadejong/Pictures/image008.jpg", 1);
    if (!image.data)
    {
        printf("No image data\n");
        return -1;
    }

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 5);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
                 gray.rows/16, // change this value to detect circles with different distances to each other
                 100, 30, 1, 30 // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                 );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( image, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        circle( image, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
    }
    imshow("detected circles", image);
    namedWindow("Image",WINDOW_AUTOSIZE);
    imshow("Image", image);
    waitKey(0);

    return 0;
}
