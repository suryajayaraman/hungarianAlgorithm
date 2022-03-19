#ifndef _HUNGARIAN_H_
#define _HUNGARIAN_H_

#include<vector>
#include<stdint.h>
#include<iostream>
#include "hungarianAlgorithm/utils.h"


typedef std::vector<double> vecXd;
typedef std::vector<std::vector<double>> matXd;


class hungarian
{
    public:
        hungarian(void);
        ~hungarian(void);

        static std::vector<uint16_t> solve(const matXd costMtx);

    private:
        /**
         * @brief function does row min subtraction - step1 of hungarian algorithm
         * @param costMtx input Matrix
         * @return NA
         */
        static void step1(matXd &costMtx);


        /**
         * @brief function does column min subtraction - step2 of hungarian algorithm
         * @param costMtx input Matrix
         * @return NA
         */
        static void step2(matXd &costMtx);

};


#endif // _HUNGARIAN_H_