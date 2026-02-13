#pragma once

#include <atomic>

class GainComputer
{
public:
    void prepare(double sampleRate);
    float process(float rms);

    void setCurve(float curve);
    void setRmsMin(float dB);
    void setRmsMax(float dB);
    void setAttackMs(float ms);
    void setReleaseMs(float ms);
    void setMinGain(float dB);
    void setMaxGain(float dB);

private:
    std::atomic<float> curve_{};
    std::atomic<float> rmsMin_{};
    std::atomic<float> rmsMax_{};
    std::atomic<float> attackMs_{};
    std::atomic<float> releaseMs_{};
    std::atomic<float> minGain_{};
    std::atomic<float> maxGain_{};
    float currentGainDb_ = 0.0f;
    double sampleRate_ = 48000.0;
};
