/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-07 10:28:33
 * @FilePath: /yolox_sort/src/lib/intrusion/region_intrusion.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once
#include "object_region.h"
#include "camera_frame.h"


struct RegionIntrusionOptions
{
    int img_width;
    int img_height;

    // load from cv-yml
    cv::Mat matA;
    cv::Mat matB;
    cv::Mat matC;
    cv::Mat matD;
    cv::Mat matE;
    cv::Mat matX;
    cv::Mat matY;

    bool valid_A = false;
    bool valid_B = false;
    bool valid_C = false;
    bool valid_D = false;
    bool valid_E = false;
    bool valid_X = false;
    bool valid_Y = false;

    bool plot_region = true;

    void CheckValid()
    {
        valid_A = (matA.rows > 2) && (matA.rows % 2 == 0);
        valid_B = (matB.rows > 2) && (matB.rows % 2 == 0);
        valid_C = (matC.rows > 2) && (matC.rows % 2 == 0);
        valid_D = (matD.rows > 2) && (matD.rows % 2 == 0);
        valid_E = (matE.rows > 2) && (matE.rows % 2 == 0);
        valid_X = (matX.rows > 2) && (matX.rows % 2 == 0);
        valid_Y = (matY.rows > 2) && (matY.rows % 2 == 0);
    }
};


class RegionIntrusion
{
    public:
        RegionIntrusion() = default;
        ~RegionIntrusion() = default;


    public:
    
        /* 
         * @description: 
         * @param {RegionIntrusionOptions} &options
         * @return {*}
         * @author: zhanghao
         */
        bool Init(const RegionIntrusionOptions &options);


        /* 
         * @description: 
         * @param {CameraFrame*} camera_frame
         * @return {*}
         * @author: zhanghao
         */
        bool Process(CameraFrame* camera_frame);


    private:
        void get_mask_img();


    private:
        RegionIntrusionOptions m_options_;
        AccsRegions m_accs_regions_;
        cv::Mat m_region_mask_;

};
