#ifndef NORMALIZATION_FUNCS_H
#define NORMALIZATION_FUNCS_H 1

#include "Network.h"

using namespace NeuralNetwork;

RowVector *get_input_ptr(std::vector<long double> vector)
{
    RowVector *input = new RowVector(13);
    for (Number i{0}; i < 13; ++i)
    {
        (*input)[i] = vector[i];
    }
    return input;
}

RowVector *get_output_CO2_ptr(std::vector<long double> vector)
{
    RowVector *output = new RowVector(1);
    (*output)[0] = vector[13];
    return output;
}

RowVector *get_output_NOX_ptr(std::vector<long double> vector)
{
    RowVector *output = new RowVector(1);
    (*output)[0] = vector[14];
    return output;
}

RowVector *get_output_PM_ptr(std::vector<long double> vector)
{
    RowVector *output = new RowVector(1);
    (*output)[0] = vector[15];
    return output;
}

RowVector *get_output_VOC_ptr(std::vector<long double> vector)
{
    RowVector *output = new RowVector(1);
    (*output)[0] = vector[16];
    return output;
}

RowVector *get_output_SO2_ptr(std::vector<long double> vector)
{
    RowVector *output = new RowVector(1);
    (*output)[0] = vector[17];
    return output;
}

std::vector<Scalar> turn_CO2_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] / 6 * 500;
    return ans;
}

std::vector<Scalar> turn_NOX_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] / 6 * 2;
    return ans;
}

std::vector<Scalar> turn_PM_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] / 6 * 0.2;
    return ans;
}

std::vector<Scalar> turn_VOC_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] / 6 * 0.1;
    return ans;
}

std::vector<Scalar> turn_SO2_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] / 6 * 0.1;
    return ans;
}

#endif