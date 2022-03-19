#ifndef _UTILS_H
#define _UTILS_H

#include<vector>
#include<ostream>

template <typename TType>
std::ostream& operator<<(std::ostream& os, const std::vector<TType> &vec)
{
    typename std::vector<TType>::const_iterator it;
    for(it = vec.begin(); it != vec.end(); it++)
    {
        if(it!= vec.begin()) std::cout << ",";
        os << (*it);
    }
    return os;
}


template <typename T2>
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T2>> &vec)
{
    for(auto it= vec.begin(); it!= vec.end(); it++)
    {
        os << (*it);
        os << "\n";
    }
    return os;
}

#endif // _UTILS_H