#include "Network.h"
#include <vector>
#include <iostream>

namespace NeuralNetwork
{
    NeuralNetwork::NeuralNetwork(
        std::vector<Number> s,
        Scalar l)
    {
        validate_structure(s);
        structure = s;
        validate_learning_rate(l);
        learning_rate = l;
        initialize_storage_objects();
    }

    NeuralNetwork::~NeuralNetwork()
    {
        for (auto layer : neuron_layers)
            delete layer;
        for (auto layer : deltas)
            delete layer;
        for (auto layer : weights)
            delete layer;
    }

    RowVector NeuralNetwork::predict(
        RowVector &input)
    {
        propagate_forward(input);
        return *neuron_layers.back();
    }

    std::vector<Scalar> NeuralNetwork::test(
        std::vector<RowVector *> input_data,
        std::vector<RowVector *> output_data)
    {
        validate_data(input_data, output_data);

        Number dataset_size{input_data.size()};

        std::vector<Scalar> main_error(structure.back());

        for (Number element{0}; element < dataset_size; ++element)
        {
            std::vector<Scalar> error{get_abs_error(*input_data[element], *output_data[element])};
            for (Number i{0}; i < error.size(); ++i)
                main_error[i] += sqrtl(error[i] / dataset_size);
        }

        return main_error;
    }

    void NeuralNetwork::train(
        std::vector<RowVector *> input_data,
        std::vector<RowVector *> output_data)
    {
        validate_data(input_data, output_data);

        for (Number element{0}; element < input_data.size(); ++element)
        {
            propagate_forward(*input_data[element]);
            propagate_backward(*output_data[element]);
        }
    }

    void NeuralNetwork::propagate_forward(
        RowVector &input)
    {
        validate_input(input);
        // set the input to input layer
        // block returns a part of the given vector or matrix
        neuron_layers.front()->block(0, 0, 1, neuron_layers.front()->size() - 1) = input;

        // propagate the data forward and then
        // apply the activation function to network
        for (Number layer{1}; layer < structure.size(); ++layer)
        {
            (*neuron_layers[layer]) = (*neuron_layers[layer - 1]) * (*weights[layer - 1]);
            apply_activation_function_to_neuron_layer(layer);
        }
    }

    void NeuralNetwork::propagate_backward(
        RowVector &output)
    {
        validate_output(output);
        calculate_deltas(output);
        update_weights();
    }

    void NeuralNetwork::calculate_deltas(
        RowVector &output)
    {
        (*deltas.back()) = output - (*neuron_layers.back());

        for (Number layer{structure.size() - 2}; layer > 0; --layer)
            (*deltas[layer]) = (*deltas[layer + 1]) * (weights[layer]->transpose());
    }

    std::vector<Scalar> NeuralNetwork::get_abs_error(
        RowVector &input,
        RowVector &output)
    {
        validate_input(input);
        validate_output(output);

        std::vector<Scalar> error{};

        RowVector prediction{predict(input)};

        for (Number i{0}; i < output.size(); ++i)
            error.push_back(abs(prediction[i] - output[i]));

        return error;
    }

    void NeuralNetwork::update_weights()
    {
        // structure.size()-1 = weights.size()
        for (Number layer{0}; layer < structure.size() - 1; ++layer)
        {
            // iterating over the different layers (from first hidden to output layer)
            // if this layer is the output layer, there is no bias neuron there, number of neurons specified = number of cols
            // if this layer not the output layer, there is a bias neuron and number of neurons specified = number of cols -1
            if (layer != structure.size() - 2)
                for (Number c = 0; c < weights[layer]->cols() - 1; ++c)
                    for (Number r = 0; r < weights[layer]->rows(); ++r)
                        weights[layer]->coeffRef(r, c) += calculate_weights_change(layer, c, r);

            else
                for (Number c = 0; c < weights[layer]->cols(); c++)
                    for (Number r = 0; r < weights[layer]->rows(); r++)
                        weights[layer]->coeffRef(r, c) += calculate_weights_change(layer, c, r);
        }
    }

    Scalar NeuralNetwork::calculate_weights_change(
        Number layer,
        Number c,
        Number r)
    {
        return learning_rate * deltas[layer + 1]->coeffRef(c) *
               activation_function_derivative(neuron_layers[layer + 1]->coeffRef(c)) *
               neuron_layers[layer]->coeffRef(r);
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

    void NeuralNetwork::initialize_storage_objects_layer(
        Number layer)
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
        Number layer)
    {
        Number neuron_amount{neuron_layers[layer]->size()};
        if (layer != neuron_layers.size() - 1)
            neuron_amount -= 1;
        // activation function shoudn't be applied to bias neuron
        for (Number neuron{0}; neuron < neuron_amount; ++neuron)
            (*neuron_layers[layer])[neuron] = activation_function((*neuron_layers[layer])[neuron]);
    }

    void NeuralNetwork::validate_structure(
        const std::vector<Number> &s) const
    {
        if (s.size() < 2)
            throw NetworkInvalidValue(exception_message_invalid_structure_length);

        for (Number i{0}; i < s.size(); ++i)
            if (s[i] < 1)
                throw NetworkInvalidValue(exception_message_invalid_neuron_amount);
    }

    void NeuralNetwork::validate_learning_rate(
        Scalar l) const
    {
        if (l <= 0)
            throw NetworkInvalidValue(exception_message_invalid_learning_rate);
    }

    void NeuralNetwork::validate_weights(
        std::vector<Matrix *> w) const
    {
        if (w.size() != structure.size() - 1)
            throw NetworkInvalidValue(exception_message_invalid_layers_amount);
        for (Number layer{0}; layer < w.size() - 1; ++layer)
            if (w[layer]->rows() != structure[layer] + 1 || w[layer]->cols() != structure[layer + 1] + 1)
                throw NetworkInvalidValue(exception_message_invalid_layer_matrix);
        if (w.back()->rows() != structure.back() || w.back()->cols() != structure.back())
            throw NetworkInvalidValue(exception_message_invalid_layer_matrix);
    }

    void NeuralNetwork::validate_data(
        std::vector<RowVector *> input_data,
        std::vector<RowVector *> output_data) const
    {
        if (input_data.size() != output_data.size())
            throw NetworkInvalidValue(exception_message_invalid_data_sizes);
    }

    void NeuralNetwork::validate_input(
        RowVector &input) const
    {
        if (input.size() != structure.front())
            throw NetworkInvalidValue(exception_message_invalid_input_size);
    }

    void NeuralNetwork::validate_output(
        RowVector &output) const
    {
        if (output.size() != structure.back())
            throw NetworkInvalidValue(exception_message_invalid_output_size);
    }

    Scalar sum(std::vector<Scalar> a)
    {
        Scalar summary{0};
        for (Scalar s : a)
            summary += s;
        return summary;
    }

    std::vector<Matrix *> NeuralNetwork::matrix_vector_from_vectors(std::vector<std::vector<std::vector<Scalar>>> &weights)
    {
        std::vector<Matrix *> matrices;
        for (std::vector<std::vector<Scalar>> &w : weights)
        {
            if (!w.empty())
            {
                long long unsigned int rows{w.size()};
                long long unsigned int cols{w[0].size()};
                Matrix *current_matrix = new Matrix(rows, cols);

                for (size_t i = 0; i < rows; i++)
                    for (size_t j = 0; j < cols; j++)
                        (*current_matrix)(i, j) = w[i][j];

                matrices.push_back(current_matrix);
            }
        }
        return matrices;
    }

    std::vector<std::vector<std::string>> NeuralNetwork::matrix_to_vector(Matrix &matrix)
    {
        std::vector<std::vector<std::string>> m;
        for (size_t i = 0; i < matrix.rows(); i++)
        {
            std::vector<std::string> row;
            for (size_t j = 0; j < matrix.cols(); j++)
            {
                row.push_back(std::to_string(matrix(i, j)));
            }
            m.push_back(row);
        }
        return m;
    }

    void NeuralNetwork::save_weights_to_file(std::string filename)
    {
        CsvWriter csv_weights{filename};
        csv_weights.open_file();

        std::vector<Matrix *> weights{get_weights()};
        for (Matrix *l : weights)
        {
            std::vector<std::vector<std::string>> matrix_vector{matrix_to_vector(*l)};
            for (std::vector<std::string> v : matrix_vector)
            {
                csv_weights.write_row(v);
            }
            if (l != weights.back())
                csv_weights.write_empty_row();
        }

        csv_weights.close_file();
    }

    void NeuralNetwork::load_weights_from_file(std::string filename)
    {
        CsvReader csv_reader(filename);
        std::vector<std::vector<std::vector<Scalar>>> vector_weights;
        csv_reader.open_file();
        while (true)
        {
            try
            {

                std::vector<std::vector<std::string>> layer_weights_string = csv_reader.read_until_blank_line();

                // if (layer_weights_string[0][0] == "")
                //     throw FileIsClosedError();
                std::vector<std::vector<Scalar>> matrix_weights;
                for (const auto &row : layer_weights_string)
                {
                    std::vector<Scalar> row_scalar;
                    for (const auto &element : row)
                    {
                        row_scalar.push_back(std::stold(element));
                    }
                    matrix_weights.push_back(row_scalar);
                }
                vector_weights.push_back(matrix_weights);
            }
            catch (const FileIsClosedError &e)
            {
                break;
            }
        }
        csv_reader.close_file();
        std::vector<Matrix *> weights_converted = matrix_vector_from_vectors(vector_weights);
        for (Number layer{0}; layer < weights_converted.size(); ++layer)
            *weights[layer] = *weights_converted[layer];
    }
};
