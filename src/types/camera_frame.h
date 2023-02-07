/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-06 18:31:34
 * @FilePath: /yolox_sort/src/types/camera_frame.h
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
#include "object_region.h"


struct CameraFrame 
{
    int frame_id = 0;
    double timestamp = 0.0;

    cv::Mat* image_ptr;

    DetObjectList det_objects;
    TrkObjectList trk_objects;
    AccsResult accs_result;

    void PlotDets();
    void PlotTrks();
    void SaveImg(std::string save_path);
    void ResizeImg(int w, int h);

    CameraFrame()
    {
        image_ptr = nullptr;
        Reset();
    }

    void Reset()
    {
        det_objects.clear();
        trk_objects.clear();
        accs_result.Reset();
    }

    void RemoveInvalidDets();

};

