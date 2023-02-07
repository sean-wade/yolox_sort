/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-07 10:21:09
 * @FilePath: /yolox_sort/src/base/base_tracker.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once

#include <memory>
#include <string>
#include "camera_frame.h"


//TODO: 去掉默认参数，改为实现中继承并赋值
struct TrackerInitOptions
{
    bool use_giou = false;
    bool use_gpu = false;
    int max_age = 5;
    int min_hits = 2;
    int max_predict = 5;
    
    float iou_threshold = 0.1;
};


class BaseTracker
{
    public:
        BaseTracker() = default;
        virtual ~BaseTracker() = default;
    

        /* 
         * @description: Init the tracker by options.
         * @param {TrackerInitOptions} &options
         * @return {status}
         * @author: zhanghao
         */
        virtual bool Init(const TrackerInitOptions &options) = 0;


        /* 
         * @description: Do track.
         * @param {CameraFrame*} camera_frame
         * @return {status}
         * @author: zhanghao
         */
        virtual bool Track(CameraFrame* camera_frame) = 0;
        

        /* 
         * @description: Name of this tracker
         * @return {Tracker name}
         * @author: zhanghao
         */
        virtual std::string Name() const = 0;


        BaseTracker(const BaseTracker &) = delete;
        BaseTracker &operator=(const BaseTracker &) = delete;

};
