/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-07 14:48:00
 * @FilePath: /yolox_sort/src/lib/accs_processer.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include "accs_processer.h"


bool AccsProcesser::Init(const char* alg_yml, const char* channel_yml)
{
    if(access(alg_yml, 0) == -1 || access(channel_yml, 0) == -1)
	{
		return false;
	}
    cv::FileStorage fs_alg(alg_yml, cv::FileStorage::READ);
    cv::FileStorage fs_cha(channel_yml, cv::FileStorage::READ);

    m_img_width_ = (int)fs_alg["width"];
    m_img_height_ = (int)fs_alg["height"];

    DetectorInitOptions det_options;
    det_options.input_width = m_img_width_;
    det_options.input_height = m_img_height_;
    det_options.num_classes = (int)fs_alg["num_classes"];
    det_options.score_threshold = (float)fs_alg["score_threshold"];
    det_options.nms_threshold = (float)fs_alg["nms_threshold"];
    det_options.ues_fp16 = (int)fs_alg["ues_fp16"];
    fs_alg["ox_path"] >> det_options.onnx_path;
    fs_alg["eg_path"] >> det_options.engine_path;
    
    TrackerInitOptions trk_options;

    RegionIntrusionOptions reg_options;
    reg_options.img_width = m_img_width_;
    reg_options.img_height = m_img_height_;
    fs_cha["A"] >> reg_options.matA;
    fs_cha["B"] >> reg_options.matB;
    fs_cha["C"] >> reg_options.matC;
    fs_cha["D"] >> reg_options.matD;
    fs_cha["E"] >> reg_options.matE;
    fs_cha["X"] >> reg_options.matX;
    fs_cha["Y"] >> reg_options.matY;
    reg_options.CheckValid();
    // reg_options.plot_region = false;

    bool status = m_detector_.Init(det_options);
    if(!status)
    {
        FETAL << "Detector init failed !!!";
        return false;
    }

    status = m_tracker_.Init(trk_options);
    if(!status)
    {
        FETAL << "Tracker init failed !!!";
        return false;
    }

    status = m_region_intrusion_.Init(reg_options);
    if(!status)
    {
        FETAL << "RegionIntrusion init failed !!!";
        return false;
    }
    
    fs_alg.release();
    fs_cha.release();
    return true;
}


bool AccsProcesser::Process(CameraFrame* camera_frame)
{
#ifdef DEBUG_TIME_ACCS_PROCESSER
    clock_t time_start, time_end;
    time_start = clock();
#endif
    
    camera_frame->ResizeImg(m_img_width_, m_img_height_);

    bool status;
    status = m_detector_.Detect(camera_frame);
    if(!status)
    {
        ERROR << "Detect failed !!!";
        return false;
    }

    status = m_tracker_.Track(camera_frame);
    if(!status)
    {
        ERROR << "Track failed !!!";
        return false;
    }

    status = m_region_intrusion_.Process(camera_frame);
    if(!status)
    {
        ERROR << "Region Process failed !!!";
        return false;
    }
    camera_frame->PlotTrks();
    
#ifdef DEBUG_TIME_ACCS_PROCESSER
    time_end = clock();
    double cur_time = (double)(time_end - time_start) / CLOCKS_PER_SEC * 1000;
    m_time_avg_ = m_time_avg_ * 0.9 + 0.1 * cur_time;
    DEBUG << "cur frame using : " << cur_time << "ms, avg time : " << m_time_avg_ << "ms.";
#endif

    return true;
}