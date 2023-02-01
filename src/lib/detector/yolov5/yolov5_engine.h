#ifndef YOLOV5_H_
#define YOLOV5_H_

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

#include "NvInfer.h"
#include "NvInferPlugin.h"
#include "cuda_runtime_api.h"


// #define NMS_THRESH 0.4
// #define CONF_THRESH 0.5
#define BATCH_SIZE 1
#define KEEP_TOPK 20
#define V5_INPUT_H 640
#define V5_INPUT_W 640

using namespace nvinfer1;

typedef struct _RecBox 
{
    // int x1;
    // int y1;
    // int x2;
    // int y2;

    cv::Rect rect;
    float conf;
    int class_id;
} RecBox;


class YoloV5Detector 
{

public:
    YoloV5Detector();
    ~YoloV5Detector();

    bool InitModel(std::string engine_name, int gpu_id=0);    // 
    // void Forward(cv::Mat& img, YoloV5DetectionResult *det_result);
    int Process(cv::Mat &img, std::vector<RecBox> &result);

private:
    // void doInference();
    void doInference(float *input, int *counts, float *boxes, float *scores, float *classes, int batchSize);


private:
    IRuntime *m_runtime_;
    ICudaEngine *m_engine_;
    IExecutionContext *m_context_;

    const char *INPUT_NAME = "data";
    const char *OUTPUT_COUNTS = "count";
    const char *OUTPUT_BOXES = "box";
    const char *OUTPUT_SCORES = "score";
    const char *OUTPUT_CLASSES = "class";

};
#endif

