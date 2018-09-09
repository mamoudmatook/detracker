#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>


using namespace cv;
using namespace std;
Ptr<Tracker> tracker;
struct userdata
{
	Mat img;
	Rect2d bbox;
};
void mousehandler(int event,int x,int y,int flag,void *data_ptr)
{
	if  ( event == EVENT_LBUTTONDOWN )
    {
        userdata *data = ((userdata *) data_ptr);
    data->bbox = selectROI(data->img, false);

    tracker->init(data->img, data->bbox);
}
}
// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()



int main(int argc, char **argv)
{
    // List of tracker types in OpenCV 3.2
    // NOTE : GOTURN implementation is buggy and does not work.
    string trackerTypes[7] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "CSRT"};
    // vector <string> trackerTypes(types, std::end(types));

    // Create a tracker
    string trackerType = trackerTypes[2];


    #if 0
    {
        tracker = Tracker::create(trackerType);
    }
    #else
    {
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
        if (trackerType == "CSRT")
            tracker = TrackerCSRT::create();
    }
    #endif
    // Read video
    VideoCapture video(0);
    
    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
        
    }
    userdata data;
    // Read first frame
    Mat frame;
    bool ok = video.read(frame);
    data.img=frame;
    
    // Define initial boundibg box
   // Rect2d bbox(287, 23, 86, 320);
    
    // Uncomment the line below to select a different bounding box

    // Display bounding box.
   // rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
   // imshow("Tracking", frame);
      namedWindow("Tracking", 1);

     setMouseCallback("Tracking", mousehandler, &data);

    while(video.read(frame))
    {
     //data.img=frame;
        // Start timer
        double timer = (double)getTickCount();
        
        // Update the tracking result
        bool ok = tracker->update(frame, data.bbox);
        
        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
        
        
        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, data.bbox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
        
        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
        
        // Display FPS on frame
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);

        // Display frame.
        imshow("Tracking", frame);
        
        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }

    }
    

    
}
