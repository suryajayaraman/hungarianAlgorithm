#include<hungarianAlgorithm/hungarian.h>

hungarian::hungarian() {}


std::vector<uint16_t> hungarian::solve(const matXd costMtx)
{
    std::vector<uint16_t> optimalAssignmentVec(costMtx.size(), 0U);
    matXd inputMtx {costMtx};


    step1(inputMtx);
    std::cout << "------------\nStep 1 output : \n";
    std::cout << inputMtx;

    step1(inputMtx);
    std::cout << "------------\nStep 2 output : \n";
    std::cout << inputMtx;



    return optimalAssignmentVec;
}



void hungarian::step1(matXd &costMtx)
{
    if(costMtx.size() > 0U)
    {
        double rowMinValue{1.0E+10};
        vecXd rowMin(costMtx.size(), 0.0F);
        
        for(std::size_t rowIndex = 0U; rowIndex < costMtx.size(); rowIndex++)
        {
            rowMinValue = 1.1E10;
            for(auto &element : costMtx[rowIndex])
            {
                if(element < rowMinValue)
                    rowMinValue = element;
            }
            rowMin[rowIndex] = rowMinValue;
        }


        for(std::size_t rowIndex = 0U; rowIndex < costMtx.size(); rowIndex++)
        {
            for(auto &element : costMtx[rowIndex])
            {
                element -= rowMin[rowIndex];
            }
        }
    }

    else
    {}
}



void hungarian::step2(matXd &costMtx)
{
    if(costMtx.size() > 0U)
    {
        if(costMtx[0].size() > 0U)
        {
            double colMinValue{1.0E+10};
            std::size_t numCols{costMtx[0].size()};
            vecXd colMin(numCols, 0.0F);
            
            for(std::size_t colIndex = 0U; colIndex < numCols; colIndex++)
            {
                colMinValue = 1.0E10;
                for(auto &element : costMtx[rowIndex])
                {
                    if(element < rowMinValue)
                        rowMinValue = element;
                }
                rowMin[rowIndex] = rowMinValue;
            }


            for(std::size_t rowIndex = 0U; rowIndex < costMtx.size(); rowIndex++)
            {
                for(auto &element : costMtx[rowIndex])
                {
                    element -= rowMin[rowIndex];
                }
            }
        }
    }

    else{}
}