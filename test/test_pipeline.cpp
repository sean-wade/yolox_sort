/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-07 14:49:25
 * @FilePath: /yolox_sort/test/test_pipeline.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include <chrono>
#include "accs_processer.h"

using namespace cv;
void test_video(std::string video_path);

int main()
{
    // test_video("../test/videos/2022GC1-28.mp4");
    test_video("../test/videos/tieyun.mp4");
}


void test_video(std::string video_path)
{
    FNLog::FastStartDefaultLogger();
    INFO << "Video path = " << video_path;
    VideoCapture cap(video_path);
    
    AccsProcesser accs_processer;
    accs_processer.Init("../config/alg.yml", "../config/C3.yml");

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
        camera_frame.image_ptr = &frame;
        camera_frame.ResizeImg(640, 480);

        accs_processer.Process(&camera_frame);
        
        std::string str_people, str_axis;
        camera_frame.accs_result.GetAlarmString(str_people, str_axis);
        INFO << "Frame " << i << " nums : " << str_people << ", axis : " << str_axis;

        // imshow("res", *camera_frame.image_ptr);
        // if (waitKey(30) == 'q') {
        //     break;
        // }
    }
    // destroyAllWindows();
    cap.release();
}
