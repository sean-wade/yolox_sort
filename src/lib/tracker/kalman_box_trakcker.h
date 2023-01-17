/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-01-16 15:08:06
 * @FilePath: /yolox_deploy/src/lib/tracker/kalman_box_trakcker.h
 * @LastEditors: zhanghao
 * @Description: 
 */
#pragma once

#include <memory>
#include <iostream>
#include <eigen3/Eigen/Eigen>

#include "object_tracked.h"
#include "object_detected.h"
#include "tracker_utils/kalman_filter.h"
#include "tracker_utils/tracker_utils.h"
#include "types/base_macros.h"


class KalmanBoxTrackerCV
{
    static int K_count_;

    public:
        KalmanBoxTrackerCV() = default;
        KalmanBoxTrackerCV(DetectObject& det_obj, double time, int max_predict=3, int min_hits=5, int max_age=3);
        ~KalmanBoxTrackerCV();

        void Predict();
        void Update(DetectObject& new_det_obj, double time);
        TrackedObject GetState();
        cv_rect GetPredict() const{
            return m_predict_rect_;
        };


        int GetAge(){
            return m_age_;
        }

        double GetTrackDurSec(){
            return m_curr_time_ - m_start_time_;
        }

        static void Reset(){
            K_count_ = 0;
        }

        bool CheckValid()
        {
            bool is_life_valid = m_time_since_update_ <= m_max_predict_ && m_hit_streak_ >= m_min_hits_;
            bool is_box_valid = m_tracked_obj_.IsValid();
                                 
            return is_life_valid && is_box_valid;
        }

        bool CheckDelete()
        {
            return m_time_since_update_ > m_max_age_;
        }

    private:
        void _UpdateF(float time_lag);
        void _InitCoef();

    private:
        KalmanFilter m_kalman_filter_;

        int m_id_;
        double m_delta_time_;
        double m_curr_time_;
        double m_start_time_;

        TrackedObject m_tracked_obj_;
        // std::vector<TrackedObject> m_history_;

        int m_age_;
        int m_hits_count_;
        int m_hit_streak_;
        int m_time_since_update_;

        int m_max_age_;
        int m_min_hits_;
        int m_max_predict_;
        
        cv_rect m_predict_rect_;
};


