/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-17 11:34:40
 * @FilePath: /yolox_deploy/test/test_tracker.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include <chrono>
#include "lib/tracker/sortx.h"
#include "lib/detector/detector_yolox.h"
#include "lib/detector/yolov5/detector_yolov5_wrapper.h"
#include "lib/tracker/tracker_utils/tracker_utils.h"

// #define SAVE_NOT_SHOW
// #define SAVE_DIR "../test/trk_5/"
// #define USE_YOLOX_DETECTOR

using namespace cv;
void track_test_video(std::string video_path);

int main()
{
    // iou_test();
    // track_test_folder();
    track_test_video("../test/tieyun2.mp4");
}


void track_test_video(std::string video_path)
{
    std::cout << "Video path = " << video_path << std::endl;
    VideoCapture cap(video_path);

#ifdef USE_YOLOX_DETECTOR
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init Detector (YOLOX)
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    DetectorInitOptions options;
    options.input_width = 640;
    options.input_height = 480;
    options.num_classes = 3;
    options.batch_size = 1;
    options.engineInputTensorNames = {"images"};
    options.engineOutputTensorNames = {"output"};
    options.onnx_path   = "../models/zc_nano_rep_480_640_nopad.onnx";
    options.engine_path = "../models/zc_nano_rep_480_640_nopad.engine";
    // options.onnx_path   = "../models/zc_nano_480_640_pad.onnx";
    // options.engine_path = "../models/zc_nano_480_640_pad.engine";
    options.score_threshold = 0.60;
    options.nms_threshold = 0.25;
    options.gpu_id = 0;
    options.ues_fp16 = true;

    DetectorYoloX detector;
    std::cout << "Detecor name = " << detector.Name() << std::endl;
    bool status = detector.Init(options);
    std::cout << "Detecor Init status = " << status << std::endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init Detector (YOLOv5)
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    DetectorInitOptions options;
    options.engine_path = "../models/zc_y5_20230109_3cls.engine";
    options.gpu_id = 0;

    YoloV5DetectorWrapper detector;
    std::cout << "Detecor name = " << detector.Name() << std::endl;
    bool status = detector.Init(options);
    std::cout << "Detecor Init status = " << status << std::endl;
    //////////
#endif

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init Tracker
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    TrackerInitOptions trk_options;
    trk_options.use_giou = false;
    trk_options.use_gpu = false;
    trk_options.max_age = 2;
    trk_options.min_hits = 2;
    trk_options.max_predict = 5;
    trk_options.iou_threshold = 0.1;

    SortX sortx;
    std::cout << "Tracker name = " << sortx.Name() << std::endl;
    status = sortx.Init(trk_options);
    std::cout << "Tracker Init status = " << status << std::endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float avg_det_time = 0.0;
    float avg_trk_time = 0.0;
    float avg_dur_time = 0.0;

    int i=0;
    Mat frame;
    while(cap.isOpened())
    {
        cap >> frame;
        if(frame.empty()){
            break;
        }
        CameraFrame camera_frame;
        camera_frame.timestamp = i * 0.1;
        i++;

        auto t0 = std::chrono::system_clock::now();

        camera_frame.image_ptr = &frame;
        detector.Detect(&camera_frame);
        // std::cout << "Det size = " << camera_frame.det_objects.size() << std::endl;

        auto t1 = std::chrono::system_clock::now();
        if(i > 0)
            avg_det_time += std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        
        sortx.Track(&camera_frame);
        camera_frame.PlotTrks();

        auto t2 = std::chrono::system_clock::now();
        if(i > 0)
            avg_trk_time += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        
        // auto end = std::chrono::system_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t0).count();
        std::cout << "Frame " << i << " using: " << dur  << "ms, track_num = " << camera_frame.trk_objects.size() << std::endl;
        if(i > 0)
            avg_dur_time += dur;

// #ifdef SAVE_NOT_SHOW
//         camera_frame.SaveImg(SAVE_DIR + std::to_string(i) + ".jpg");
// #else
//         imshow("track", *camera_frame.image_ptr);
//         if (waitKey(30) == 'q') {
//             break;
//         }
// #endif
    }  
    destroyAllWindows();
    cap.release();

    std::cout << "Detect avg using avg: " << avg_det_time / (i-1) << "ms " << std::endl;
    std::cout << "Track avg using avg: " << avg_trk_time / (i-1) << "ms " << std::endl;
    std::cout << "Duration avg using avg: " << avg_dur_time / (i-1) << "ms " << std::endl;
    
}


void track_test_folder()
{
    std::vector<cv::String> filenames;
    cv::glob("../test/tieyun/", filenames);
    std::cout << "Total file nums = " << filenames.size() << std::endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init Detector
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    DetectorInitOptions options;
    options.input_width = 640;
    options.input_height = 480;
    options.num_classes = 3;
    options.batch_size = 1;
    options.engineInputTensorNames = {"images"};
    options.engineOutputTensorNames = {"output"};
    options.onnx_path   = "/home/zhanghao/code/others/zc/yolox_deploy/models/zc_nano_480_640_pad.onnx";
    options.engine_path = "/home/zhanghao/code/others/zc/yolox_deploy/models/zc_nano_480_640_pad.engine";
    options.score_threshold = 0.45;
    options.nms_threshold = 0.25;
    options.gpu_id = 0;
    options.ues_fp16 = true;

    DetectorYoloX detector;
    std::cout << "Detecor name = " << detector.Name() << std::endl;
    bool status = detector.Init(options);
    std::cout << "Detecor Init status = " << status << std::endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init Tracker
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    TrackerInitOptions trk_options;
    trk_options.use_giou = true;
    trk_options.use_gpu = false;
    trk_options.max_age = 2;
    trk_options.min_hits = 5;
    trk_options.max_predict = 3;
    trk_options.iou_threshold = -0.5;

    SortX sortx;
    std::cout << "Tracker name = " << sortx.Name() << std::endl;
    status = sortx.Init(trk_options);
    std::cout << "Tracker Init status = " << status << std::endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for(int i=0; i<filenames.size(); i++)
    {
        CameraFrame camera_frame;
        camera_frame.timestamp = i * 0.1;
        cv::Mat image = cv::imread(filenames[i]);

        auto start = std::chrono::system_clock::now();

        camera_frame.image_ptr = &image;
        detector.Detect(&camera_frame);
        std::cout << "Det size = " << camera_frame.det_objects.size() << std::endl;

        sortx.Track(&camera_frame);
        camera_frame.PlotTrks();

        auto end = std::chrono::system_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << filenames[i] << " using: " << dur  << "ms, track_num = " << camera_frame.trk_objects.size() << std::endl;

        camera_frame.SaveImg("../test/trk/" + filenames[i].substr(15, 24));
    }  

}


void iou_test()
{
    float iou, giou;
    int loop = 1000000000;
    std::cout << "Loop times = " << loop << std::endl;

    auto start = std::chrono::system_clock::now();
    for(int jj = 0; jj<loop; jj++)
    {
        cv_rect r1(0, 0, 10+loop%33, 10);
        cv_rect r2(5, 5, 10+loop%33, 10);

        iou = iou_rect(r1, r2);
    }
    auto end = std::chrono::system_clock::now();
    std::cout << "Iou Total time using: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;

    start = std::chrono::system_clock::now();
    for(int jj = 0; jj<loop; jj++)
    {
        cv_rect r1(0, 0, 10+loop%33, 10);
        cv_rect r2(5, 5, 10+loop%33, 10);

        giou = giou_rect(r1, r2);
    }
    end = std::chrono::system_clock::now();
    std::cout << "GIou Total time using: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;

    std::cout << "iou = " << iou << ", giou=" << giou << std::endl;

}

