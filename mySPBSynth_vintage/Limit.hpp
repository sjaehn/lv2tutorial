#ifndef LIMIT_HPP_
#define LIMIT_HPP_

template <class T>
T limit (const T x, const T min, const T max)
{
    return (x < min ? min : (x > max ? max : x));
}

#endif /* LIMIT_HPP_ */