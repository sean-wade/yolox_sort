/* 
 * @Author: zhanghao
 * @LastEditTime: 2023-02-06 14:22:40
 * @FilePath: /yolox_sort/src/lib/tracker/kalman_box_trakcker.cpp
 * @LastEditors: zhanghao
 * @Description: 
 */
#include "kalman_box_trakcker.h"


int KalmanBoxTrackerCV::K_count_ = 0;
KalmanBoxTrackerCV::KalmanBoxTrackerCV(DetectObject& det_obj, double time, int max_predict, int min_hits, int max_age) 
: m_kalman_filter_(7, 4) 
{
    m_id_ = K_count_;
    K_count_++;
    
    m_age_ = 0;
    m_hits_count_ = 0;
    m_hit_streak_ = 0;
    m_time_since_update_ = 0;
    m_delta_time_ = 0.1;
    m_curr_time_ = time;
    m_start_time_ = time;

    m_max_age_ = max_age;
    m_min_hits_ = min_hits;
    m_max_predict_ = max_predict;

    if(K_count_ >= MAX_COUNT_TRACK_ID){
        Reset();
    }

    // m_history_.clear();

    m_tracked_obj_.rect = det_obj.rect;
    m_tracked_obj_.det_score = det_obj.det_score;
    m_tracked_obj_.cls = det_obj.cls;
    m_tracked_obj_.track_id = m_id_;
    
    _InitCoef();

    m_kalman_filter_.x_.head(4) << ConvertDetToObservation(det_obj);
}


KalmanBoxTrackerCV::~KalmanBoxTrackerCV()
{
    // std::cout << "deconstruct : " << m_id_ << std::endl;
}


void KalmanBoxTrackerCV::_InitCoef()
{
    m_kalman_filter_.F_ = Eigen::MatrixXd::Identity(7, 7);
    _UpdateF(m_delta_time_);
    
    m_kalman_filter_.H_ = Eigen::MatrixXd::Identity(4, 7);
    m_kalman_filter_.P_ = Eigen::MatrixXd::Identity(7, 7);
    m_kalman_filter_.Q_ = Eigen::MatrixXd::Identity(7, 7);
    m_kalman_filter_.R_ = Eigen::MatrixXd::Identity(4, 4);

    m_kalman_filter_.R_ << 
        1,    0,    0,    0, 
        0,    1,    0,    0, 
        0,    0,    1,    0, 
        0,    0,    0,    1;

    m_kalman_filter_.P_(4,4) = m_kalman_filter_.P_(5,5) = m_kalman_filter_.P_(6,6) = 100;
    // m_kalman_filter_.Q_(4,4) = m_kalman_filter_.Q_(5,5) = 0.1;
    // m_kalman_filter_.Q_(6,6) = 0.01;
}


void KalmanBoxTrackerCV::_UpdateF(float time_lag)
{
    m_kalman_filter_.F_(0,4) = m_kalman_filter_.F_(1, 5) = m_kalman_filter_.F_(2, 6) = time_lag;
}


void KalmanBoxTrackerCV::Update(DetectObject& new_det_obj, double time)
{
    m_delta_time_ = time - m_curr_time_;
    m_curr_time_ = time;
    _UpdateF(m_delta_time_);
    
    m_time_since_update_ = 0;
    // m_history_.clear();
    m_hits_count_++;
    m_hit_streak_++;
    
    m_tracked_obj_.det_score = new_det_obj.det_score;
    // m_tracked_obj_.cls = new_det_obj.cls;
    
    m_kalman_filter_.Update(ConvertDetToObservation(new_det_obj));
    
    return;
}


void KalmanBoxTrackerCV::Predict()
{
    m_kalman_filter_.Predict();

    m_age_++;

    if(m_time_since_update_ > m_max_predict_)
        m_hit_streak_ = 0;

    m_time_since_update_++;
    m_predict_rect_ = ConvertObservationToRect(m_kalman_filter_.x_);
}


TrackedObject KalmanBoxTrackerCV::GetState()
{
    m_tracked_obj_.rect = ConvertObservationToRect(m_kalman_filter_.x_);
    m_tracked_obj_.track_age = m_age_;
    
    return m_tracked_obj_;
}