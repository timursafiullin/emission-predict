#include "Network.h"
#include <vector>

namespace NeuralNetwork
{
    NeuralNetwork::NeuralNetwork(
        std::vector<Number> s,
        Scalar l
    )
    {
        validate_structure(s);
        structure = s;
        validate_learning_rate(l);
        learning_rate = l;
        initialize_storage_objects();
    }

    NeuralNetwork::NeuralNetwork(
        std::vector<Number> s,
        std::vector<Matrix*> w
    )
    {
        validate_structure(s);
        validate_weights(w);
        structure = s;
        weights = w;
    }

    RowVector NeuralNetwork::predict(
        RowVector& input
    )
    {
        propagate_forward(input);
        return *neuron_layers.back();
    }

    Scalar NeuralNetwork::test(
        std::vector<RowVector*> input_data,
        std::vector<RowVector*> output_data
    )
    {
        validate_data(input_data, output_data);

        Number dataset_size{input_data.size()};

        Scalar main_error{0};

        for (Number element{0}; element < dataset_size; ++element)
            main_error += get_main_square_error(*input_data[element], *output_data[element]) / dataset_size;
        
        return main_error;
    }

    void NeuralNetwork::train(
        std::vector<RowVector*> input_data,
        std::vector<RowVector*> output_data
    )
    {
        validate_data(input_data, output_data);
        Scalar previous_error {test(input_data, output_data)};
        Scalar current_error {};
        for (Number element{0}; element < input_data.size(); ++element)
        {
            propagate_forward(*input_data[element]);
            propagate_backward(*output_data[element]);
            current_error = test(input_data, output_data);
            if (previous_error-current_error < critical_difference_between_errors)
                return;
            previous_error = current_error;
        }
    }

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
        for (Number layer{1}; layer < structure.size(); ++layer)
        {
            (*neuron_layers[layer]) = (*neuron_layers[layer - 1]) * (*weights[layer - 1]);
            apply_activation_function_to_neuron_layer(layer);
        }
    }

    void NeuralNetwork::propagate_backward(
        RowVector& output
    )
    {
        validate_output(output);
        calculate_deltas(output);
        update_weights();
    }

    void NeuralNetwork::calculate_deltas(
        RowVector& output
    )
    {
        (*deltas.back()) = output - (*neuron_layers.back());

        for (Number layer{structure.size() - 2}; layer > 0; --layer)
            (*deltas[layer]) = (*deltas[layer + 1]) * (weights[layer]->transpose());
    }

    Scalar NeuralNetwork::get_main_square_error(
        RowVector& input,
        RowVector& output
    )
    {
        validate_input(input);
        validate_output(output);

        Scalar error{};

        RowVector prediction{predict(input)};

        for (Number i{0}; i < output.size(); ++i)
            error += pow(prediction[i] - output[i], 2) / 2 / output.size();

        return error;
    }

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
                            activation_function_derivative(neuron_layers[layer + 1]->coeffRef(c)) *
                            neuron_layers[layer]->coeffRef(r);

            else
                for (Number c = 0; c < weights[layer]->cols(); c++)
                    for (Number r = 0; r < weights[layer]->rows(); r++)
                        weights[layer]->coeffRef(r, c) +=
                            learning_rate *
                            deltas[layer + 1]->coeffRef(c) *
                            activation_function_derivative(neuron_layers[layer + 1]->coeffRef(c)) *
                            neuron_layers[layer]->coeffRef(r);
        }
    }

    void NeuralNetwork::initialize_storage_objects()
    {
        for (Number layer{0}; layer < structure.size(); ++layer)
        {
            if (layer == structure.size() - 1)
                neuron_layers.push_back(new RowVector(structure[layer]));
            else
                neuron_layers.push_back(new RowVector(structure[layer] + 1));

            deltas.push_back(new RowVector(neuron_layers[layer]->size()));

            initialize_storage_objects_layer(layer);
        }
    }

    // initializes layers of neurons and weights
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

        // input layer of neurons needn't be initialized
        if (layer == 0)
            return;

        // inner layers processing includes adding bias neuron
        weights.push_back(new Matrix(structure[layer - 1] + 1, structure[layer] + 1));
        weights.back()->setRandom();
        weights.back()->col(structure[layer]).setZero();
        weights.back()->coeffRef(structure[layer - 1], structure[layer]) = 1.0;
        return;
    }

    void NeuralNetwork::apply_activation_function_to_neuron_layer(
        Number layer
    )
    {
        for (Number neuron{0}; neuron < neuron_layers[layer]->size(); ++neuron)
            (*neuron_layers[layer])[neuron] = activation_function((*neuron_layers[layer])[neuron]);
    }

    void NeuralNetwork::validate_structure(
        const std::vector<Number>& s
    ) const
    {
        if (s.size() < 2)
            throw NetworkInvalidValue(exception_message_invalid_structure_length);
        
        for (Number i{0}; i < s.size(); ++i)
            if (s[i] < 1)
                throw NetworkInvalidValue(exception_message_invalid_neuron_amount);
    }

    void NeuralNetwork::validate_learning_rate(
        Scalar l
    ) const
    {
        if (l <= 0)
            throw NetworkInvalidValue(exception_message_invalid_learning_rate);
    }

    void NeuralNetwork::validate_weights(
        std::vector<Matrix*> w
    ) const
    {
        if (w.size() != structure.size() - 1)
            throw NetworkInvalidValue(exception_message_invalid_layers_amount);
        for (Number layer{0}; layer < w.size(); ++layer)
            if (w[layer]->rows() != structure[layer] || w[layer]->cols() != structure[layer + 1])
                throw NetworkInvalidValue(exception_message_invalid_layer_matrix);
    }

    void NeuralNetwork::validate_data(
            std::vector<RowVector*> input_data,
            std::vector<RowVector*> output_data
    ) const
    {
        if (input_data.size() != output_data.size())
            throw NetworkInvalidValue(exception_message_invalid_data_sizes);
    }

    void NeuralNetwork::validate_input(
        RowVector& input
    ) const
    {
        if (input.size() != structure.front())
            throw NetworkInvalidValue(exception_message_invalid_input_size);
    }

    void NeuralNetwork::validate_output(
        RowVector& output
    ) const
    {
        if (output.size() != structure.back())
            throw NetworkInvalidValue(exception_message_invalid_output_size);
    }
}