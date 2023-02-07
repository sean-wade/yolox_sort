/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-07 14:46:31
 * @FilePath: /yolox_sort/src/lib/accs_processer.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once
#include "lib/tracker/sortx.h"
#include "lib/detector/detector_yolox.h"
#include "lib/intrusion/region_intrusion.h"


class AccsProcesser
{
    public:
        AccsProcesser() = default;
        ~AccsProcesser() = default;


    public:
        /* 
         * @description: 
         * @param {RegionIntrusionOptions} &options
         * @return {*}
         * @author: zhanghao
         */
        bool Init(const char* alg_yml, const char* channel_yml);


        /* 
         * @description: 
         * @param {CameraFrame*} camera_frame
         * @return {*}
         * @author: zhanghao
         */
        bool Process(CameraFrame* camera_frame);


    private:
        DetectorYoloX m_detector_;
        SortX m_tracker_;
        RegionIntrusion m_region_intrusion_;

        int m_img_width_;
        int m_img_height_;
        
#ifdef DEBUG_TIME_ACCS_PROCESSER
        double m_time_avg_;
#endif
};


