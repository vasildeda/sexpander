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
    auto timeMs = (diff > 0.0f) ? attackMs_.load() : releaseMs_.load();
    auto coeff = 1.0f - std::exp(-1000.0f / (timeMs * static_cast<float>(sampleRate_)));

    currentGainDb_ += coeff * diff;

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

void GainComputer::setAttackMs(float ms)
{
    attackMs_.store(ms);
}

void GainComputer::setReleaseMs(float ms)
{
    releaseMs_.store(ms);
}

void GainComputer::setMinGain(float dB)
{
    minGain_.store(dB);
}

void GainComputer::setMaxGain(float dB)
{
    maxGain_.store(dB);
}
