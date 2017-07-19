#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

void detectAndDisplay(Mat image)
{
    Mat planes[3];
    split(image,planes); 
    namedWindow("Image",WINDOW_AUTOSIZE);
    imshow("Image", planes[2]);
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

    while (capture.read(frame))
    {
        if (frame.empty())
            break;
        detectAndDisplay(frame);

        char c = (char)waitKey(10);
        if (c == 27)
            break;
    }
}

