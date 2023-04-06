// Program1.cpp
// This code exercises some OpenCV methods to demonstrate basic functionality

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

struct TrackbarData {
    Mat grayscale;
    int sigma_slider;
    int threshold1_slider;
    int threshold2_slider;
	int aperture_size_slider;
    int L2gradient_slider;
};

void on_trackbar(int, void *);

const int sigma_max = 10;
const int threshold_max = 100;
const int aperture_max = 7;

// on_trackbar - this method updates the window when the track bar is moved
// preconditions: the named window has been created
// postconditions: the named window has been updated according to the new settings
void on_trackbar(int sigma_slider, void *src) {
	TrackbarData* data = (TrackbarData*)src;
    Mat blurred, edges;

    int sigma = data->sigma_slider;
    int threshold1 = data->threshold1_slider;
    int threshold2 = data->threshold2_slider;
    int aperture_size = (data->aperture_size_slider * 2) + 3;
    bool L2gradient = data->L2gradient_slider != 0;

    // Set a minimum value for sigma
    if (sigma < 1) sigma = 1;

    GaussianBlur(data->grayscale, blurred, Size(0,0), sigma, sigma);

    // Check if both threshold values are 0
    if (threshold1 == 0 && threshold2 == 0) {
        edges = blurred.clone();
    } else {
        Canny(blurred, edges, threshold1, threshold2, aperture_size, L2gradient);
    }
    String L2gradient_text = (data->L2gradient_slider == 0) ? "off" : "on";
    putText(edges, "L2gradient: " + L2gradient_text, Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, LINE_AA);

    imshow("Edges with Trackbar", edges);
}

// main 
// preconditions: walt.jpg must exist in the code directory and be a valid jpeg
// postconditions: intermediate results are displayed on the screen, no changes are made to the file system
int main(int argc, char* argv[]) {
	Mat image = imread("walt.jpg");
	namedWindow("Main Image");
	imshow("Main Image", image);
	waitKey(0);

	// Flip the image
	Mat flipped;
	flip(image, flipped, -1);
	imshow("Main Image", flipped);
	waitKey(0);

	// Convert to grayscale
	Mat grayscale;
	cvtColor(flipped, grayscale, COLOR_BGR2GRAY);
	imshow("Main Image", grayscale);
	waitKey(0);

	// Gaussian blur
    Mat blurred;
    GaussianBlur(grayscale, blurred, Size(0, 0), 2.0, 2.0);
    imshow("Main Image", blurred);
    waitKey(0);

	// Edge detection
    Mat edges;
    Canny(blurred, edges, 20, 60);
    imshow("Main Image", edges);
    waitKey(0);

    // Write to output
    imwrite("output.jpg", edges);

	// Create trackbar
	TrackbarData trackbarData;
	trackbarData.grayscale = grayscale;
	trackbarData.sigma_slider = 2;
	trackbarData.threshold1_slider = 20; 
	trackbarData.threshold2_slider = 60; 
    trackbarData.aperture_size_slider = 0; // default
    trackbarData.L2gradient_slider = 0; // 0 = off (default)


	namedWindow("Edges with Trackbar");
    moveWindow("Edges with Trackbar", 20, 0); // Offset the position of the trackbar window
    createTrackbar("Sigma", "Edges with Trackbar", &trackbarData.sigma_slider, sigma_max, on_trackbar, &trackbarData);
    createTrackbar("Threshold1", "Edges with Trackbar", &trackbarData.threshold1_slider, threshold_max, on_trackbar, &trackbarData);
    createTrackbar("Threshold2", "Edges with Trackbar", &trackbarData.threshold2_slider, threshold_max, on_trackbar, &trackbarData);
	createTrackbar("Aperture", "Edges with Trackbar", &trackbarData.aperture_size_slider, 2, on_trackbar, &trackbarData);
    createTrackbar("L2gradient", "Edges with Trackbar", &trackbarData.L2gradient_slider, 1, on_trackbar, &trackbarData);

    on_trackbar(0, &trackbarData); // Initialize the window with the initial values
    waitKey(0);

    return 0;
}
