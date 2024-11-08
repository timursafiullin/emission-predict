#include "Network.h"
#include <vector>

namespace NeuralNetwork
{
    // ready
    NeuralNetwork::NeuralNetwork(
        std::vector<Number> s,
        Scalar l = default_learning_rate_value
    ) 
    : structure {validate_structure(s)}, learning_rate{validate_learning_rate(l)}
    {
        initialize_storage_objects();
    }

    // ready
    NeuralNetwork::NeuralNetwork(
        std::vector<Number> s,
        std::vector<Matrix*> w
    )
    : structure {validate_structure(s)}, weights{validate_weights(w)}
    {
    }

    // ready
    RowVector NeuralNetwork::predict(
        RowVector& input
    )
    {
        propagate_forward(input);
        return *cache_layers.back();
    }

    Scalar NeuralNetwork::get_error(
        std::vector<RowVector*> input_data,
        std::vector<RowVector*> output_data
    )
    {
        validate_data(input_data, output_data);
        //pass
    }

    // ready
    void NeuralNetwork::train(
        std::vector<RowVector*> input_data,
        std::vector<RowVector*> output_data
    )
    {
        validate_data(input_data, output_data);
        for (Number element{0}; element < input_data.size(); ++element)
        {
            propagate_forward(*input_data[element]);
            propagate_backward(*output_data[element]);
        }
    }

    // ready
    void NeuralNetwork::propagate_forward(
        RowVector& input
    )
    {
        validate_input(input);
        // set the input to input layer
        // block returns a part of the given vector or matrix
        neuron_layers.front()->block(0, 0, 1, neuron_layers.front()->size() - 1) = input;
    
        // propagate the data forward and then 
        // apply the activation function to your network
        // unaryExpr applies the given function to all elements
        for (Number layer{1}; layer < structure.size(); ++layer)
        {
            (*cache_layers[layer]) = (*neuron_layers[layer - 1]) * (*weights[layer - 1]);
            cache_layers[layer]->block(0, 0, 1, neuron_layers[layer]).unaryExpr(std::ptr_fun(activation_function));
        }
    }

    // ready
    void NeuralNetwork::propagate_backward(
        RowVector& output
    )
    {
        validate_output(output);
        calculate_errors(output);
        update_weights();
    }

    // ready
    void NeuralNetwork::calculate_errors(
        RowVector& output
    )
    {
        (*deltas.back()) = output - (*cache_layers.back());

        for (Number layer{structure.size() - 2}; layer > 0; --layer) {
            (*deltas[layer]) = (*deltas[layer + 1]) * (weights[layer]->transpose());
        }
    }

    // ready
    void NeuralNetwork::update_weights()
    {
        // structure.size()-1 = weights.size()
        for (Number layer{0}; layer < structure.size() - 1; ++layer) {
            // in this loop we are iterating over the different layers (from first hidden to output layer)
            // if this layer is the output layer, there is no bias neuron there, number of neurons specified = number of cols
            // if this layer not the output layer, there is a bias neuron and number of neurons specified = number of cols -1
            if (layer != structure.size() - 2)
                for (Number c = 0; c < weights[layer]->cols() - 1; ++c)
                    for (Number r = 0; r < weights[layer]->rows(); ++r)
                        weights[layer]->coeffRef(r, c) +=
                            learning_rate *
                            deltas[layer + 1]->coeffRef(c) *
                            activation_function_derivative(cache_layers[layer + 1]->coeffRef(c)) *
                            neuron_layers[layer]->coeffRef(r);

            else
                for (Number c = 0; c < weights[layer]->cols(); c++)
                    for (Number r = 0; r < weights[layer]->rows(); r++)
                        weights[layer]->coeffRef(r, c) +=
                            learning_rate * deltas[layer + 1]->coeffRef(c) *
                            activation_function_derivative(cache_layers[layer + 1]->coeffRef(c)) *
                            neuron_layers[layer]->coeffRef(r);
        }
    }

    // ready
    void NeuralNetwork::initialize_storage_objects()
    {
        for (Number layer{0}; layer < structure.size(); ++layer)
        {
            if (layer == structure.size() - 1)
                neuron_layers.push_back(new RowVector(structure[layer]));
            else
                neuron_layers.push_back(new RowVector(structure[layer] + 1));

            cache_layers.push_back(new RowVector(neuron_layers[layer]->size()));
            deltas.push_back(new RowVector(neuron_layers[layer]->size()));

            initialize_storage_objects_layer(layer);
        }
    }

    // ready
    void NeuralNetwork::initialize_storage_objects_layer(
        Number layer
    )
    {
        // output layer mustn't have bias neuron
        if (layer == structure.size() - 1)
        {
            weights.push_back(new Matrix(structure[layer - 1] + 1, structure[layer]));
            weights.back()->setRandom();
            return;
        }

        // coeffRef gives the reference of value at that place 
        neuron_layers.back()->coeffRef(structure[layer]) = 1.0;
        cache_layers.back()->coeffRef(structure[layer]) = 1.0;

        // input layer of neurons needn't be initialized
        if (layer == 0)
            return;

        // inner layers processing includes adding bias neuron
        weights.push_back(new Matrix(structure[layer - 1] + 1, structure[layer] + 1));
        weights.back()->setRandom();
        weights.back()->col(structure[layer]).setZero();
        weights.back()->coeffRef(structure[layer - 1], structure[layer]) = 1.0; // bias neuron
        return;
    }

    // ready
    std::vector<Number> NeuralNetwork::validate_structure(
        const std::vector<Number>& s
    ) const
    {
        if (s.size() < 2)
            throw NetworkInvalidValue(exception_message_invalid_structure_length.c_str());
        
        for (Number i{0}; i < s.size(); ++i)
            if (s[i] < 1)
                throw NetworkInvalidValue(exception_message_invalid_neuron_amount.c_str());
        
        return s;
    }

    // ready
    Scalar NeuralNetwork::validate_learning_rate(
        Scalar l
    ) const
    {
        if (l <= 0)
            throw NetworkInvalidValue(exception_message_invalid_learning_rate.c_str());
        
        return l;
    }

    // ready
    std::vector<Matrix*> NeuralNetwork::validate_weights(
        std::vector<Matrix*> w
    ) const
    {
        if (w.size() != structure.size() - 1)
            throw NetworkInvalidValue(exception_message_invalid_layers_amount.c_str());
        for (Number layer{0}; layer < w.size(); ++layer)
        {
            if (w[layer]->size() != structure[layer] || w[layer][0].size() != structure[layer + 1])
                throw NetworkInvalidValue(exception_message_invalid_layer_matrix.c_str());
        }
    }

    // ready
    void NeuralNetwork::validate_data(
            std::vector<RowVector*> input_data,
            std::vector<RowVector*> output_data
    ) const
    {
        if (input_data.size() != output_data.size())
            throw NetworkInvalidValue(exception_message_invalid_data_sizes.c_str());
    }

    // ready
    void NeuralNetwork::validate_input(
        RowVector& input
    ) const
    {
        if (input.size() != structure[0])
            throw NetworkInvalidValue(exception_message_invalid_input_size.c_str());
    }

    // ready
    void NeuralNetwork::validate_output(
        RowVector& output
    ) const
    {
        if (output.size() != structure[0])
            throw NetworkInvalidValue(exception_message_invalid_output_size.c_str());
    }
}