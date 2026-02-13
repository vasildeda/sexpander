#include "GainComputer.h"

#include <algorithm>
#include <cmath>

void GainComputer::prepare(double sampleRate)
{
    sampleRate_ = sampleRate;
    currentGainDb_ = 0.0f;
}

float GainComputer::process(float rms)
{
    constexpr float epsilon = 1e-10f;
    auto rmsDb = 20.0f * std::log10(std::max(rms, epsilon));

    auto rmsMinDb = rmsMin_.load();
    auto rmsMaxDb = rmsMax_.load();
    auto minDb = minGain_.load();
    auto maxDb = maxGain_.load();
    auto cur = curve_.load();

    auto range = rmsMaxDb - rmsMinDb;
    auto t = (range > 0.0f)
        ? std::clamp((rmsDb - rmsMinDb) / range, 0.0f, 1.0f)
        : (rmsDb >= rmsMaxDb ? 1.0f : 0.0f);

    t = std::pow(t, cur);

    auto targetGainDb = minDb + t * (maxDb - minDb);

    auto diff = targetGainDb - currentGainDb_;
    auto slew = (diff < 0.0f)
        ? downwardSlewRate_.load() / static_cast<float>(sampleRate_)
        : upwardSlewRate_.load() / static_cast<float>(sampleRate_);

    if (diff > slew)
        currentGainDb_ += slew;
    else if (diff < -slew)
        currentGainDb_ -= slew;
    else
        currentGainDb_ = targetGainDb;

    return std::pow(10.0f, currentGainDb_ / 20.0f);
}

void GainComputer::setCurve(float curve)
{
    curve_.store(curve);
}

void GainComputer::setRmsMin(float dB)
{
    rmsMin_.store(dB);
}

void GainComputer::setRmsMax(float dB)
{
    rmsMax_.store(dB);
}

void GainComputer::setDownwardSlewRate(float dBPerSecond)
{
    downwardSlewRate_.store(dBPerSecond);
}

void GainComputer::setUpwardSlewRate(float dBPerSecond)
{
    upwardSlewRate_.store(dBPerSecond);
}

void GainComputer::setMinGain(float dB)
{
    minGain_.store(dB);
}

void GainComputer::setMaxGain(float dB)
{
    maxGain_.store(dB);
}
