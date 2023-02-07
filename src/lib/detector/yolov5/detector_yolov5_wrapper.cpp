// /* 
//  * @Author: zhanghao
//  * @LastEditTime: 2023-01-30 17:37:24
//  * @FilePath: /yolox_deploy/src/lib/detector/detector_yolox.cpp
//  * @LastEditors: zhanghao
//  * @Description: 
//  */
// #include <unistd.h>
// #include "yolov5/detector_yolov5_wrapper.h"


// bool YoloV5DetectorWrapper::Init(const DetectorInitOptions &options)
// {
//     m_options_ = options;

//     return yolodetector_.InitModel(m_options_.engine_path, m_options_.gpu_id);
// }


// bool YoloV5DetectorWrapper::Detect(cv::Mat* image_ptr, DetObjectList& results)
// {
//     std::vector<RecBox> res;
//     yolodetector_.Process(*image_ptr, res);
    
//     for (int n_i = 0; n_i < res.size(); n_i++)
//     {
//         DetectObject tmp_obj;
//         tmp_obj.cls = ACCS_CLASS(res[n_i].class_id);
//         tmp_obj.rect = res[n_i].rect;
//         tmp_obj.det_score = res[n_i].conf;

//         results.push_back(tmp_obj);
//     }
//     res.clear();

//     return true;
// }


// bool YoloV5DetectorWrapper::Detect(CameraFrame* camera_frame)
// {
//     std::vector<RecBox> res;
//     yolodetector_.Process(*camera_frame->image_ptr, res);
    
//     for (int n_i = 0; n_i < res.size(); n_i++)
//     {
//         DetectObject tmp_obj;
//         tmp_obj.cls = ACCS_CLASS(res[n_i].class_id);
//         tmp_obj.rect = res[n_i].rect;
//         tmp_obj.det_score = res[n_i].conf;

//         camera_frame->det_objects.push_back(tmp_obj);
//     }
//     res.clear();

//     return true;
// }