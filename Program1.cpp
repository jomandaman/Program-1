// Program1.cpp
// This code exercises some OpenCV methods to demonstrate basic functionality

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;

struct SecondWindow {
    Mat grayscale;
    Mat edges;
    int sigma_slider;
    int threshold1_slider;
    int threshold2_slider;
	int aperture_size_slider;
    int L2gradient_slider;
};

struct ThirdWindow {
    Mat original;
    Mat embossed;
    int enhancement_intensity_slider;
    int emboss_intensity_slider;
    int kernel_size_slider;
};

void on_trackbar(int, void *);
void on_trackbar2(int, void *);

const int sigma_max = 10;
const int threshold_max = 100;
const int aperture_max = 7;
const int kernel_size_max = 2;
const int enhance_max = 100;
const int emboss_max = 7;

// on_trackbar - this method updates the window when the track bar is moved
// preconditions: the named window has been created
// postconditions: the named window has been updated according to the new settings
void on_trackbar(int, void *src) {
	SecondWindow* data = (SecondWindow*)src;
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

    data->edges = edges;
    imshow("Edges with Trackbar", edges);
}

void on_trackbar2(int, void *src) {
    ThirdWindow* data = (ThirdWindow*)src;

    // Fetch trackbar values
    data->enhancement_intensity_slider = getTrackbarPos("Enhance Edges", "Filter Effects with Trackbars");
    data->emboss_intensity_slider = getTrackbarPos("Emboss Edges", "Filter Effects with Trackbars");
    data->kernel_size_slider = getTrackbarPos("Kernel Size", "Filter Effects with Trackbars");

    // Determine the kernel size
    int kernel_size = (data->kernel_size_slider * 2)  + 3;
    std::cout << "Kernel size: " << kernel_size << std::endl;

    // Calculate intensity factor based on the intensity_slider value
    float enhancement_intensity_factor = data->enhancement_intensity_slider / 50.0f;
    int emboss_intensity_factor = data->emboss_intensity_slider;

    // Create the enhancement and emboss kernels based on the selected size and intensity
    Mat enhancement_kernel, emboss_kernel;
    
    if (kernel_size == 3) { // Create 3x3 kernels
        enhancement_kernel = enhancement_intensity_factor > 0 ? (Mat_<float>(3, 3) <<
            -1, -1, -1,
            -1, 1 + (8 * enhancement_intensity_factor), -1,
            -1, -1, -1) : Mat();

         emboss_kernel = emboss_intensity_factor > 0 ? (Mat_<int>(3, 3) <<
            -emboss_intensity_factor, 0, emboss_intensity_factor,
            -emboss_intensity_factor, 1, emboss_intensity_factor,
            -emboss_intensity_factor, 0, emboss_intensity_factor) : Mat();

    } else if (kernel_size == 5) { // Create 5x5 kernels
        enhancement_kernel = enhancement_intensity_factor > 0 ? (Mat_<float>(5, 5) <<
            -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1,
            -1, -1, 1 + (24 * enhancement_intensity_factor), -1, -1,
            -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1) : Mat();

        emboss_kernel = emboss_intensity_factor > 0 ? (Mat_<int>(5, 5) <<
            -emboss_intensity_factor, -emboss_intensity_factor, 0, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, -emboss_intensity_factor, 0, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, -emboss_intensity_factor, 1, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, -emboss_intensity_factor, 0, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, -emboss_intensity_factor, 0, emboss_intensity_factor, emboss_intensity_factor) : Mat();

    } else if (kernel_size == 7) { // Create 7x7 kernels
         enhancement_kernel = enhancement_intensity_factor > 0 ? (Mat_<float>(7, 7) <<
            -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, 1 + (48 * enhancement_intensity_factor), -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1) : Mat();

        emboss_kernel = emboss_intensity_factor > 0 ? (Mat_<int>(7, 7) <<
            -emboss_intensity_factor, -emboss_intensity_factor, -emboss_intensity_factor, 0, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, -emboss_intensity_factor, -emboss_intensity_factor, 0, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, -emboss_intensity_factor, -emboss_intensity_factor, 1, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, -emboss_intensity_factor, 0, 0, 0, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, -emboss_intensity_factor, emboss_intensity_factor, 1, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor,
            -emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor,
            emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor, emboss_intensity_factor) : Mat();
    }

    // Apply the enhancement and emboss kernels
    Mat enhanced, embossed;
    if (enhancement_intensity_factor > 0.5) {
        filter2D(data->original, enhanced, -1, enhancement_kernel);
    } else {
        enhanced = data->original.clone();
    }

    if (emboss_intensity_factor > 0) {
        filter2D(enhanced, embossed, -1, emboss_kernel);
    } else {
        embossed = enhanced.clone();
    } 

    data->embossed = embossed;
    imshow("Filter Effects with Trackbars", embossed);
}

// main 
// preconditions: walt.jpg must exist in the code directory and be a valid jpeg
// postconditions: intermediate results are displayed on the screen, no changes are made to the file system
int main(int argc, char* argv[]) {
	Mat image = imread("walt.jpg");
	namedWindow("Main Image");
	imshow("Main Image", image);
	// waitKey(0);

	// Flip the image
	Mat flipped;
	flip(image, flipped, -1);
	imshow("Main Image", flipped);
	// waitKey(0);

	// Convert to grayscale
	Mat grayscale;
	cvtColor(flipped, grayscale, COLOR_BGR2GRAY);
	imshow("Main Image", grayscale);
	// waitKey(0);

	// Gaussian blur
    // Mat blurred;
    // GaussianBlur(grayscale, blurred, Size(0, 0), 2.0, 2.0);
    // imshow("Main Image", blurred);
    // waitKey(0);

	// // Edge detection
    // Mat edges;
    // Canny(blurred, edges, 20, 60);
    // imshow("Main Image", edges);
    // waitKey(0);

    // // Write to output
    // imwrite("output.jpg", edges);

	// Create second window
	// SecondWindow secondWindow;
	// secondWindow.grayscale = grayscale;
	// secondWindow.sigma_slider = 2;
	// secondWindow.threshold1_slider = 20; 
	// secondWindow.threshold2_slider = 60; 
    // secondWindow.aperture_size_slider = 0; // default
    // secondWindow.L2gradient_slider = 0; // 0 = off (default)

    // // Second window with sigma trackbars
    // namedWindow("Edges with Trackbar");
    // moveWindow("Edges with Trackbar", 20, 0); // Offset the position of the trackbar window
    // createTrackbar("Sigma", "Edges with Trackbar", NULL, sigma_max, on_trackbar, &secondWindow);
    // createTrackbar("Threshold1", "Edges with Trackbar", NULL, threshold_max, on_trackbar, &secondWindow);
    // createTrackbar("Threshold2", "Edges with Trackbar", NULL, threshold_max, on_trackbar, &secondWindow);
    // createTrackbar("Aperture", "Edges with Trackbar", NULL, 2, on_trackbar, &secondWindow);
    // createTrackbar("L2gradient", "Edges with Trackbar", NULL, 1, on_trackbar, &secondWindow);

    // // Set initial positions for trackbars in the second window
    // setTrackbarPos("Sigma", "Edges with Trackbar", secondWindow.sigma_slider);
    // setTrackbarPos("Threshold1", "Edges with Trackbar", secondWindow.threshold1_slider);
    // setTrackbarPos("Threshold2", "Edges with Trackbar", secondWindow.threshold2_slider);
    // setTrackbarPos("Aperture", "Edges with Trackbar", secondWindow.aperture_size_slider);
    // setTrackbarPos("L2gradient", "Edges with Trackbar", secondWindow.L2gradient_slider);

    // on_trackbar(0, &secondWindow); // Initialize the window with the initial values
    // imwrite("output_edges_with_trackbar.jpg", secondWindow.edges);
    // waitKey(0);

    // Third window with enhancement and emboss kernels
    ThirdWindow thirdWindow;
    thirdWindow.original = image;
    thirdWindow.enhancement_intensity_slider = 0;
    thirdWindow.emboss_intensity_slider = 0;
    thirdWindow.kernel_size_slider = 0;

    namedWindow("Filter Effects with Trackbars");
    moveWindow("Filter Effects with Trackbars", 40, 0); // Offset the position of the new window
    createTrackbar("Enhance Edges", "Filter Effects with Trackbars", NULL, enhance_max, on_trackbar2, &thirdWindow);
    createTrackbar("Emboss Edges", "Filter Effects with Trackbars", NULL, emboss_max, on_trackbar2, &thirdWindow);
    createTrackbar("Kernel Size", "Filter Effects with Trackbars", NULL, kernel_size_max, on_trackbar2, &thirdWindow);

    // Set initial positions for trackbars in the third window
    setTrackbarPos("Enhance Edges", "Filter Effects with Trackbars", thirdWindow.enhancement_intensity_slider);
    setTrackbarPos("Emboss Edges", "Filter Effects with Trackbars", thirdWindow.emboss_intensity_slider);
    setTrackbarPos("Kernel Size", "Filter Effects with Trackbars", thirdWindow.kernel_size_slider);

    on_trackbar2(0, &thirdWindow); // Initialize the new window with the initial values
    imwrite("output_filter_effects_with_trackbars.jpg", thirdWindow.embossed);
    waitKey(0);


    return 0;
}
