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
    std::atomic<float> ratio_{2.0f};
    std::atomic<float> downwardSlewRate_{10000.0f};
    std::atomic<float> upwardSlewRate_{10000.0f};
    std::atomic<float> minGain_{-40.0f};
    std::atomic<float> maxGain_{0.0f};
    float currentGainDb_ = 0.0f;
    double sampleRate_ = 48000.0;
};
