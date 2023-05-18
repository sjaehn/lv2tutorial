#ifndef LINEARFADER_HPP_
#define LINEARFADER_HPP_

#include <cstdint>

template <class T>
class LinearFader
{
private:
    T destination_;
    uint32_t distance_;
    T value_;

public:
    LinearFader (const T destination) :
        destination_ (destination),
        distance_ (0),
        value_ (destination)
    {

    }

    void set (const T destination, const uint32_t distance)
    {
        destination_ = destination;
        distance_ = distance;
        if (distance == 0) value_ = destination;
    }

    T get () const
    {
        return value_;
    }

    void proceed ()
    {
        if (distance_ == 0) value_ = destination_;
        else
        {
            value_ += (destination_ - value_) * (1.0 / static_cast<double> (distance_));
            distance_ -= 1;
        }
    }
};

#endif /* LINEARFADER_HPP_ */