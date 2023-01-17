/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 15:54:00
 * @FilePath: /yolox_deploy/test/test_detector.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */


#include <glob.h>
#include "lib/detector/detector_yolox.h"


int main()
{
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

    DetectorYoloX yolox_det;
    std::cout << "Detecor name = " << yolox_det.Name() << std::endl;
    
    bool status = yolox_det.Init(options);
    std::cout << "Init status = " << status << std::endl;
    
    std::vector<cv::String> filenames;
    cv::glob("../test/2020H3/", filenames);
    // cv::glob("/mnt/data/OTHERS/ZC/accs/20221231_all/testset/", filenames);
    std::cout << "Total file nums = " << filenames.size() << std::endl;

    float avg_time = 0.0;
    CameraFrame camera_frame;
    for(auto fff: filenames)
    {
        cv::Mat image = cv::imread(fff);
        camera_frame.image_ptr = &image;

        auto start = std::chrono::system_clock::now();

        yolox_det.Detect(&camera_frame);
        camera_frame.PlotDets();

        auto end = std::chrono::system_clock::now();
        float cur_time = float(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        std::cout << fff << " using: " << cur_time << "ms" << std::endl;
        avg_time += cur_time;

        // camera_frame.SaveImg("../test/det/" + fff.substr(46, 24));
        camera_frame.SaveImg("../test/det/" + fff.substr(15, 24));

        // cv::imshow("r", *camera_frame.image_ptr);
        // cv::waitKey(0);
    }
    
    std::cout << "Average time using: " << avg_time / filenames.size() << "ms" << std::endl;

    return 0;
}



int main1()
{
    DetectorInitOptions options;
    options.input_width = 1024;
    options.input_height = 576;
    options.num_classes = 1;
    options.batch_size = 1;
    options.engineInputTensorNames = {"images"};
    options.engineOutputTensorNames = {"output"};
    options.onnx_path   = "/home/zhanghao/code/GitLab/camera_traffic_light_pipeline/sample/models/sgtls_s_725.onnx";
    options.engine_path = "/home/zhanghao/code/GitLab/camera_traffic_light_pipeline/sample/models/sgtls_s_725.engine";
    options.score_threshold = 0.5;
    options.nms_threshold = 0.25;
    options.gpu_id = 0;
    options.ues_fp16 = true;

    DetectorYoloX yolox_det;
    std::cout << "Detecor name = " << yolox_det.Name() << std::endl;
    
    bool status = yolox_det.Init(options);
    std::cout << "Init status = " << status << std::endl;

    cv::Mat image = cv::imread("/home/zhanghao/code/GitLab/traffic_light_pipeline_py/data/test_seq/demo/gs1/1661418670.100000143.jpg");
    
    CameraFrame camera_frame;
    camera_frame.image_ptr = &image;

    std::cout << "Start detect." << std::endl;
    auto start = std::chrono::system_clock::now();

    for(int jj = 0; jj<100; jj++)
    {
        yolox_det.Detect(&camera_frame);
    }
    std::cout << "Detect obj nums = " << camera_frame.det_objects.size() << std::endl;

    auto end = std::chrono::system_clock::now();
    std::cout << "Avg time using: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()/100 << "ms" << std::endl;

    // draw_objects_save(image, camera_frame.det_objects, "det_res_main3.jpg");

    camera_frame.PlotDets();
    camera_frame.SaveImg("camera_res1.jpg");


    return 0;
}

