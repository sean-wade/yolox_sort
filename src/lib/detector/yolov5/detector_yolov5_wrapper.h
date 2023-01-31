#ifndef __YOLOV5_WRAPPER_H__
#define __YOLOV5_WRAPPER_H__

#include "yolov5/yolov5_engine.h"
#include "base_detector.h"
#include "types/camera_frame.h"
#include "types/object_detected.h"
#include "detector_utils/detector_utils.h"


class YoloV5DetectorWrapper : public BaseDetector
{
    public:
        YoloV5DetectorWrapper(){};
        ~YoloV5DetectorWrapper(){};
    
        /* 
         * @description: 
         * @param {DetectorInitOptions} &options
         * @return {Init status}
         * @author: zhanghao
         */
        virtual bool Init(const DetectorInitOptions &options);

        /* 
         * @description: 
         * @param {CameraFrame*} camera_frame
         * @return {Process status}
         * @author: zhanghao
         */
        virtual bool Detect(CameraFrame* camera_frame);

        /* 
         * @description: 
         * @param {Mat*} image_ptr
         * @param {DetObjectList&} results
         * @return {*}
         * @author: zhanghao
         */
        virtual bool Detect(cv::Mat* image_ptr, DetObjectList& results);

        /* 
         * @description: Name of this detector
         * @return {Detector name}
         * @author: zhanghao
         */
        virtual std::string Name() const {
            return "zc_v5";
        };


    private:
        YoloV5Detector yolodetector_;
};


#endif