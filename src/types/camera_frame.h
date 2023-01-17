/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 14:13:44
 * @FilePath: /yolox_deploy/src/types/camera_frame.h
 * @LastEditors: zhanghao
 * @Description: 
 */

#pragma once
#include <memory>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "base_macros.h"
#include "object_tracked.h"
#include "object_detected.h"


struct CameraFrame 
{
    int frame_id = 0;
    double timestamp = 0.0;

    cv::Mat* image_ptr;

    DetObjectList det_objects;
    TrkObjectList trk_objects;

    void PlotDets();
    void PlotTrks();
    void SaveImg(std::string save_path);

    CameraFrame()
    {
        image_ptr = nullptr;
        det_objects.clear();
        trk_objects.clear();
    }

    void Reset()
    {
        det_objects.clear();
        trk_objects.clear();
    }

    void RemoveInvalidDets();

};

