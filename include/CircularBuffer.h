#pragma once

#include <atomic>
#include <vector>

class CircularBuffer
{
public:
    explicit CircularBuffer(int maxSize);

    void push(float sample);
    float getRms() const;
    void setSize(int newSize);
    void clear();

private:
    std::vector<float> buffer_;
    int maxSize_;
    std::atomic<int> size_;
    int writeIndex_ = 0;
    float runningSum_ = 0.0f;
};
