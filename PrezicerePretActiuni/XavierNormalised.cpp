#include "XavierNormalised.h"

double XavierNormalised::generateNumber()
{
    return distributie(generator);
}

std::vector<double> XavierNormalised::generateVector(int size)
{
    std::vector<double> temp_vector(size);
    for (int i = 0; i < size; i++)
    {
        temp_vector[i] = distributie(generator);
    }

    return temp_vector;
}

std::vector<std::vector<double>> XavierNormalised::generateMatrix(int size_r, int size_c)
{
    std::vector<std::vector<double>> temp_matrix(size_r, std::vector<double>(size_c,0));
    
    for (int i = 0; i < size_r; i++)
    {
        for (int j = 0; j < size_c; j++)
        {
            temp_matrix[i][j] = distributie(generator);
        }
    }
    return temp_matrix;
}
