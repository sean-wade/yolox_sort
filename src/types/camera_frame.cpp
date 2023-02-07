/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-06 14:12:22
 * @FilePath: /yolox_sort/src/types/camera_frame.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include "camera_frame.h"
#include "tracker_utils/tracker_utils.h"
#include "detector_utils/detector_utils.h"


void CameraFrame::PlotDets()
{
    // draw_det_objects(*image_ptr, det_objects, cv::Scalar(52,179,255));
    draw_det_objects(*image_ptr, det_objects, cv::Scalar(165,79,255));
}


void CameraFrame::PlotTrks()
{
    draw_trk_objects(*image_ptr, trk_objects);
}


void CameraFrame::SaveImg(std::string save_path)
{
    cv::imwrite(save_path, *image_ptr);
}


void CameraFrame::ResizeImg(int w, int h)
{
    cv::resize(*image_ptr, *image_ptr, cv::Size(w, h));
}


void CameraFrame::RemoveInvalidDets()
{
    for (auto it = det_objects.begin(); it != det_objects.end();)
    {        
        if(!it->CheckWidthValid())
        {
            it = det_objects.erase(it);
        }
        else
        {
            it++;
        }
    }
}