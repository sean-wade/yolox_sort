/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 14:08:15
 * @FilePath: /yolox_deploy/src/types/base_macros.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once
#include <fstream>
#include <opencv2/opencv.hpp>

// function switch.
#define DRAW_TRACK_TEXT_INFOS
// #define PLOT_FINAL_RESULT_AS_TEXT
// #define PLOT_ATTRIBUTES


// for all
#define DEFAULT_IMAGE_WIDTH 1920
#define DEFAULT_IMAGE_HEIGHT 1080


// for detect
// Rect.x: left-top x, Rect.y: left-top y
typedef cv::Rect_<float> cv_rect;
#define MIN_SIDE_PIXEL_THRESH 20
#define MIN_LIGHT_PIXEL_WIDTH 10


// for track
#define BOX_MIN_PIXEL 5
#define BOX_RATIO_THRESHOLD 5.0
#define MAX_COUNT_TRACK_ID 65535
#define SEMANTIC_CHANGE_MIN_HITS 3

