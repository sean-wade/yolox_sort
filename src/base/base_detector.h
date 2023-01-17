/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 14:58:35
 * @FilePath: /yolox_deploy/src/base/base_detector.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once

#include <memory>
#include <string>
#include <NvInfer.h>
#include <NvOnnxConfig.h>
#include <NvOnnxParser.h>
#include <cuda_runtime_api.h>
#include "trt_logging.h"
#include "types/camera_frame.h"


struct DetectorInitOptions
{
    std::string onnx_path;
    std::string engine_path;

    std::vector<std::string> engineInputTensorNames = {"images"};
    std::vector<std::string> engineOutputTensorNames = {"output"};

    int gpu_id = 0;
    bool ues_fp16 = true;
    int batch_size = 1;
    
    float nms_threshold = 0.25;
    float score_threshold = 0.6;

    int input_width = 640;
    int input_height = 480;
    int num_classes = 3;
};


/* 
 * @description: Base Tensorrt Detector.
 * @author: zhanghao
 */
class BaseDetector
{
    public:
        BaseDetector() = default;
        virtual ~BaseDetector() = default;
    

        /* 
         * @description: Init the detector by options.
         * @param {DetectorInitOptions} &options
         * @return {status}
         * @author: zhanghao
         */
        virtual bool Init(const DetectorInitOptions &options) = 0;


        /* 
         * @description: Do infrence.
         * @param {CameraFrame*} camera_frame
         * @return {status}
         * @author: zhanghao
         */
        virtual bool Detect(CameraFrame* camera_frame) = 0;
        

        /* 
         * @description: Name of this detector
         * @return {Detector name}
         * @author: zhanghao
         */
        virtual std::string Name() const = 0;


        BaseDetector(const BaseDetector &) = delete;
        BaseDetector &operator=(const BaseDetector &) = delete;


    protected:
        bool _ParseOnnxToEngine();
        bool _SerializeEngineToFile();
        bool _DeserializeEngineFromFile();


    protected:
        DetectorInitOptions m_options_;
        Logger m_logger_;

        nvinfer1::ICudaEngine* m_engine_;
        nvinfer1::IExecutionContext* m_context_;
};


