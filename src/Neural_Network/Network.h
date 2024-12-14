#ifndef NETWORK_DEFENCE
#define NETWORK_DEFENCE 1

#include <Eigen/Eigen>
#include <exception>
#include "csv.h"
#include <vector>

/*
    Neural network class v 1.0

    made Ruslan Sharafutdinov 09.11.2024

    this class implements simple network.
*/

namespace NeuralNetwork
{
    using Number            = size_t;
    using Scalar            = long double;
    using Matrix            = Eigen::MatrixXf;
    using RowVector         = Eigen::RowVectorXf;
    using ColumnVector      = Eigen::VectorXf;
    using VectorNumber      = std::vector<Number>;
    using VectorScalar      = std::vector<Scalar>;
    using VectorMatrixPtr   = std::vector<Matrix*>;
    using VectorRowPtr      = std::vector<RowVector*>;

    static const std::string exception_message_invalid_data_sizes{
        "amount of input elements must be "
        "equal to amount of output elements"
   };
    static const std::string exception_message_invalid_learning_rate{
        "learning rate mustn't be less than zero"
    };
    static const std::string exception_message_invalid_structure_length{
        "amount of layers mustn't be less than 1"
    };
    static const std::string exception_message_invalid_neuron_amount{
        "amount of neurons mustn't be less than 1"
    };
    static const std::string exception_message_invalid_input_size{
        "amount of input values must be equal to input neurons"
    };
    static const std::string exception_message_invalid_output_size{
        "amount of output values must be equal to output neurons"
    };
    static const std::string exception_message_invalid_layers_amount{
        "invalid layers amount given"
    };
    static const std::string exception_message_invalid_layer_matrix{
        "invalid layer matrix given"
    };

    constexpr Scalar default_learning_rate_value { 1e-10 };


    /**
     * @brief   Will be thrown when validation of given object failed.
     * 
     */
    class NetworkInvalidValue : public std::exception {
    public:
        /**
         * @brief   Construct a new Network Invalid Value object
         * 
         * @param msg exception message.
         */
        NetworkInvalidValue(const std::string msg)
        : message(msg)  { }

        /**
         * @brief   Returns exception message.
         * 
         * @return const char* 
         */
        const char* what() const throw()
        {
            return (message.c_str());
        }
    
    private:
        /**
         * @brief   exception message.
         * 
         */
        std::string message;
    };

    /**
     * @brief   Simple implementation of feedforward neural network
     *          with given activation function, trained by stohastic gradient decent.
     * 
     */
    template <class T>
    class FFNN {
    public:
        /**
         * @brief   Construct a new Neural Network object,
         *          ready to train.
         * 
         * @param s structure of network.
         * @param l learning rate. Has default value.
         */
	    FFNN(
            VectorNumber s,
            Scalar l = default_learning_rate_value,
            T f =
            ([] (Scalar x) 
                {
                    return 1 / (1 + std::pow(std::exp(1.0), -x));
                }
            ),
            T f_der =
            ([] (Scalar x)
                {
                    return std::pow(std::exp(1.0), -x) / std::pow(1 + std::pow(std::exp(1.0), -x), 2);
                }
            )
        )
        {
            validate_structure(s);
            structure = s;
            validate_learning_rate(l);
            learning_rate = l;
            initialize_storage_objects();
            activation_function = f;
            activation_function_derivative = f_der;
        }

        /**
         * @brief Destroy the Neural Network object
         * 
         */
        ~FFNN()
        {
            for (auto layer : neuron_layers)
                delete layer;
            for (auto layer : deltas)
                delete layer;
            for (auto layer : weights)
                delete layer;
        }

        /**
         * @brief   Calculates prediction by input.
         * 
         * @param input 
         * @return RowVector
         */
        RowVector predict(
            RowVector& input
        )
        {
            propagate_forward(input);
            return *neuron_layers.back();
        }

        /**
         * @brief Train model on dataset.
         * 
         * @param input_data
         * @param output_data
         * 
         */
        void train(
            VectorRowPtr input_data,
            VectorRowPtr output_data
        )
        {
            validate_data(input_data, output_data);

            for (Number element{0}; element < input_data.size(); ++element)
            {
                propagate_forward(*input_data[element]);
                propagate_backward(*output_data[element]);
            }
        }

        /**
         * @brief   Calculates main square error on dataset.
         * 
         * @param input_data 
         * @param output_data 
         * @return Scalar
         */
        VectorScalar test(
            VectorRowPtr input_data,
            VectorRowPtr output_data
        )
        {
            validate_data(input_data, output_data);

            Number dataset_size{input_data.size()};

            VectorScalar main_error(structure.back());

            for (Number element{0}; element < dataset_size; ++element)
            {
                VectorScalar error{get_abs_error(*input_data[element], *output_data[element])};
                for (Number i{0}; i < error.size(); ++i)
                    main_error[i] += sqrtl(error[i] / dataset_size);
            }

            return main_error;
        }

        /**
         * @brief   Get the weights of connections.
         * 
         * @return std::vector<Matrix*> 
         */
        VectorMatrixPtr get_weights()
        {
            return weights;
        }

        /**
         * @brief   Save the weights of connections to file.
         *
         * @param filename Path to weights file.
         */
        void save_weights_to_file(std::string filename)
        {
            CsvWriter csv_weights{filename};
            csv_weights.open_file();

            VectorMatrixPtr weights{get_weights()};
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

        /**
         * @brief   Set the weights of connections. Get them from file.
         *
         * @param filename Path to weights file.
         */
        void load_weights_from_file(std::string filename)
        {
            CsvReader csv_reader(filename);
            std::vector<std::vector<VectorScalar>> vector_weights;
            csv_reader.open_file();
            while (true)
            {
                try
                {

                    std::vector<std::vector<std::string>> layer_weights_string = csv_reader.read_until_blank_line();

                    // if (layer_weights_string[0][0] == "")
                    //     throw FileIsClosedError();
                    std::vector<VectorScalar> matrix_weights;
                    for (const auto &row : layer_weights_string)
                    {
                        VectorScalar row_scalar;
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
            VectorMatrixPtr weights_converted = matrix_vector_from_vectors(vector_weights);
            for (Number layer{0}; layer < weights_converted.size(); ++layer)
                *weights[layer] = *weights_converted[layer];
        }

        FFNN(
            const FFNN &) = delete;

        FFNN& operator= (
            const FFNN &
        ) = delete;

    private:
        /**
         * @brief   Convert vector of vectors of vectors of scalars to vector of matrices.
         *
         * @return std::vector<Matrix*>
         */
        VectorMatrixPtr matrix_vector_from_vectors(std::vector<std::vector<std::vector<Scalar>>> &weights)
        {
            VectorMatrixPtr matrices;
            for (std::vector<VectorScalar> &w : weights)
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

        /**
         * @brief   Convert matrix to vector of vectors of (scalar) strings.
         *
         * @return std::vector<std::vector<std::string>>
         */
        std::vector<std::vector<std::string>> matrix_to_vector(Matrix &matrix)
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

        /**
         * @brief   Calculates neuron values.
         * 
         * @param input 
         */
	    void propagate_forward(
            RowVector& input
        )
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

        /**
         * @brief   Calculates delta and updates weights.
         * 
         * @param output correct output.
         */
	    void propagate_backward(
            RowVector& output
        )
        {
            validate_output(output);
            calculate_deltas(output);
            update_weights();
        }

        /**
         * @brief   Calculates delta using neuron values.
         * 
         * @param output correct output.
         */
	    void calculate_deltas(
            RowVector& output
        )
        {
            (*deltas.back()) = output - (*neuron_layers.back());

            for (Number layer{structure.size() - 2}; layer > 0; --layer)
                (*deltas[layer]) = (*deltas[layer + 1]) * (weights[layer]->transpose());
        }

        /**
         * @brief   Get the main square error of prediction by input.
         * 
         * @param input 
         * @param output 
         * @return Scalar
         */
        VectorScalar get_abs_error(
            RowVector& input,
            RowVector& output
        )
        {
            validate_input(input);
            validate_output(output);

            VectorScalar error{};

            RowVector prediction{predict(input)};

            for (Number i{0}; i < output.size(); ++i)
                error.push_back(abs(prediction[i] - output[i]));

            return error;
        }

        /**
         * @brief   Updates values of weights after calculating delta.
         * 
         */
	    void update_weights()
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

        /**
         * @brief   Calculating value of changing single weight.
         * 
         * @param layer layer of weight.
         * @param c column of weight.
         * @param r row of weight.
         * @return Scalar
         */
        Scalar calculate_weights_change(
            Number layer,
            Number c,
            Number r
        )
        {
            return learning_rate * deltas[layer + 1]->coeffRef(c) *
                activation_function_derivative(neuron_layers[layer + 1]->coeffRef(c)) *
                neuron_layers[layer]->coeffRef(r);
        }

        /**
         * @brief   Creates storage obgects layers according to the
         *          structure and initializes them.
         * 
         */
        void initialize_storage_objects()
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

        /**
         * @brief   Initializes one layer of storage objects.
         * //
         *          After initializing weights (except connections
         *          with bias neuron) contain random values,
         *          neurons (except bias neuron) and deltas contains
         *          0 values.
         * 
         * @param layer layer to initialize.
         */
        void initialize_storage_objects_layer(
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

        /**
         * @brief   Applies activation function to all neyrons in given layer.
         * 
         * @param layer number of layer.
         */
        void apply_activation_function_to_neuron_layer(
            Number layer
        )
        {
            Number neuron_amount{neuron_layers[layer]->size()};
            if (layer != neuron_layers.size() - 1)
                neuron_amount -= 1;
            // activation function shoudn't be applied to bias neuron
            for (Number neuron{0}; neuron < neuron_amount; ++neuron)
                (*neuron_layers[layer])[neuron] = activation_function((*neuron_layers[layer])[neuron]);
        }

        /**
         * @brief   Validates structure according to its element values and size.
         * 
         * @param s vector pretending to be structure.
         */
        void validate_structure(
            const VectorNumber& s
        ) const
        {
            if (s.size() < 2)
                throw NetworkInvalidValue(exception_message_invalid_structure_length);

            for (Number i{0}; i < s.size(); ++i)
                if (s[i] < 1)
                    throw NetworkInvalidValue(exception_message_invalid_neuron_amount);
        }

        /**
         * @brief   Validates given learning rate according to its value.
         * 
         * @param l scalar pretending to be learning rate.
         */
        void validate_learning_rate(
            Scalar l
        ) const
        {
            if (l <= 0)
                throw NetworkInvalidValue(exception_message_invalid_learning_rate);
        }

        /**
         * @brief   Validates given weights according to the structure.
         * 
         * @param w vector pretending to be weights.
         */
        void validate_weights(
            VectorMatrixPtr w
        ) const
        {
            if (w.size() != structure.size() - 1)
                throw NetworkInvalidValue(exception_message_invalid_layers_amount);
            for (Number layer{0}; layer < w.size() - 1; ++layer)
                if (w[layer]->rows() != structure[layer] + 1 || w[layer]->cols() != structure[layer + 1] + 1)
                    throw NetworkInvalidValue(exception_message_invalid_layer_matrix);
            if (w.back()->rows() != structure.back() || w.back()->cols() != structure.back())
                throw NetworkInvalidValue(exception_message_invalid_layer_matrix);
        }

        /**
         * @brief   Validates given vectors of inputs and outputs
         *          according to their sizes.
         * 
         * @param i_d vector pretending to be inputs.
         * @param o_d vector pretending to be outputs.
         */
        void validate_data(
            VectorRowPtr i_d,
            VectorRowPtr o_d
        ) const
        {
            if (i_d.size() != o_d.size())
                throw NetworkInvalidValue(exception_message_invalid_data_sizes);
        }

        /**
         * @brief   Validates given input according to network structure.
         * 
         * @param i vector pretending to be input.
         */
        void validate_input(
            RowVector& i
        ) const
        {
            if (i.size() != structure.front())
                throw NetworkInvalidValue(exception_message_invalid_input_size);
        }

        /**
         * @brief   Validates given output according to network structure.
         * 
         * @param o vector pretending to be output.
         */
        void validate_output(
            RowVector& o
        ) const
        {
            if (o.size() != structure.back())
                throw NetworkInvalidValue(exception_message_invalid_output_size);
        }

        /**
         * @brief Activation function of nerual network
         * 
         */
        T activation_function;

        /**
         * @brief Activation function derivative
         * 
         */
        T activation_function_derivative;

        // Storage objects for working of neural network:

        /**
         * @brief   Topology of neural network.
         * //
         *          Contains amount of neurons in each layer.
         * //
         *          It must contain at least 2 elements,
         *          each of them must be > 0.
         * 
         */
        VectorNumber structure;

        /**
         * @brief   Layers of network during calculations.
         * //
         *          Contains neurons of network including bias neuron.
         * 
         */
        VectorRowPtr neuron_layers;

        /**
         * @brief   Error contribution of each neuron.
         * //
         *          Contains calculated errors, using in
         *          updating of weigts.
         * 
         */
        VectorRowPtr deltas;

        /**
         * @brief   Connection weights between neurons.
         * //
         *          It must be conformed with structure of network.
         * 
         */
        VectorMatrixPtr weights;

        /**
         * @brief   Coeffitient of gradient decent rate.
         * //
         *          Has default value. It must be > 0.
         * 
         */
        Scalar learning_rate;
    };

    // Scalar sum_ffnn(VectorScalar a)
    // {
    //     Scalar summary{0};
    //     for (Scalar s : a)
    //         summary += s;
    //     return summary;
    // }
}

#endif