#include "CircularBuffer.h"

#include <algorithm>
#include <cmath>

CircularBuffer::CircularBuffer(int maxSize)
    : buffer_(static_cast<size_t>(maxSize), 0.0f),
      maxSize_(maxSize),
      size_(maxSize)
{
}

void CircularBuffer::push(float sample)
{
    auto size = size_.load(std::memory_order_relaxed);
    auto squared = sample * sample;

    int removeIndex = (writeIndex_ - size + maxSize_) % maxSize_;
    runningSum_ -= buffer_[static_cast<size_t>(removeIndex)];
    runningSum_ += squared;
    buffer_[static_cast<size_t>(writeIndex_)] = squared;

    writeIndex_ = (writeIndex_ + 1) % maxSize_;
}

float CircularBuffer::getRms() const
{
    auto size = size_.load(std::memory_order_relaxed);
    if (size == 0)
        return 0.0f;

    auto sum = runningSum_ > 0.0f ? runningSum_ : 0.0f;
    return std::sqrt(sum / static_cast<float>(size));
}

void CircularBuffer::setSize(int size)
{
    if (size > 0 && size <= maxSize_)
        size_.store(size, std::memory_order_relaxed);
}

void CircularBuffer::clear()
{
    std::fill(buffer_.begin(), buffer_.end(), 0.0f);
    writeIndex_ = 0;
    runningSum_ = 0.0f;
}
