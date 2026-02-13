#pragma once

#include <atomic>

class GainComputer
{
public:
    void prepare(double sampleRate);
    float process(float rms);

    void setRatio(float ratio);
    void setDownwardSlewRate(float dBPerSecond);
    void setUpwardSlewRate(float dBPerSecond);
    void setMinGain(float dB);
    void setMaxGain(float dB);

private:
    std::atomic<float> ratio_{};
    std::atomic<float> downwardSlewRate_{};
    std::atomic<float> upwardSlewRate_{};
    std::atomic<float> minGain_{};
    std::atomic<float> maxGain_{};
    float currentGainDb_ = 0.0f;
    double sampleRate_ = 48000.0;
};
