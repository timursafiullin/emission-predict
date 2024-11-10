#ifndef NETWORK_DEFENCE
#define NETWORK_DEFENCE 1

#include "eigen-3.4.0/Eigen/Eigen"
#include <exception>

/*
    Neural network class v 1.0

    made Ruslan Sharafutdinov 09.11.2024

    this class implements simple network.
*/

namespace NeuralNetwork
{
    typedef size_t              Number;
    typedef double              Scalar;
    typedef Eigen::MatrixXf     Matrix;
    typedef Eigen::RowVectorXf  RowVector;
    typedef Eigen::VectorXf     ColumnVector;

    const std::string exception_message_invalid_data_sizes{
        "amount of input elements must be "
        "equal to amount of output elements"
    };
    const std::string exception_message_invalid_learning_rate{
        "learning rate mustn't be less than zero"
    };
    const std::string exception_message_invalid_structure_length{
        "amount of layers mustn't be less than 1"
    };
    const std::string exception_message_invalid_neuron_amount{
        "amount of neurons mustn't be less than 1"
    };
    const std::string exception_message_invalid_input_size{
        "amount of input values must be equal to input neurons"
    };
    const std::string exception_message_invalid_output_size{
        "amount of output values must be equal to output neurons"
    };
    const std::string exception_message_invalid_layers_amount{
        "invalid layers amount given"
    };
    const std::string exception_message_invalid_layer_matrix{
        "invalid layer matrix given"
    };

    constexpr Scalar default_learning_rate_value                { 0.005 };
    constexpr Scalar default_critical_difference_between_errors { -0.05 };


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
     *          with linear activation function, trained by stohastic gradient decent.
     * 
     */
    class NeuralNetwork {
    public:
        /**
         * @brief   Construct a new Neural Network object,
         *          ready to train.
         * 
         * @param s structure of network.
         * @param l learning rate. Has default value.
         */
	    NeuralNetwork(
            std::vector<Number> s,
            Scalar l = default_learning_rate_value
        );

        /**
         * @brief   Construct a new Neural Network object,
         *          with already given weights.
         * 
         * @param s structure of network.
         * @param w already trained weights of model.
         */
        NeuralNetwork(
            std::vector<Number> s,
            std::vector<Matrix*> w
        );

        /**
         * @brief   Calculates prediction by input.
         * 
         * @param input 
         * @return RowVector
         */
        RowVector predict(
            RowVector& input
        );

        /**
         * @brief Train model on dataset.
         * 
         * @param input_data 
         * @param output_data 
         * @param critical_difference_between_errors difference between previous
         *              and current errors when training stops to prevent
         *              divergence of gradient decent.
         * 
         */
        void train(
            std::vector<RowVector*> input_data,
            std::vector<RowVector*> output_data,
            Scalar critical_difference_between_errors = default_critical_difference_between_errors
        );

        /**
         * @brief   Calculates main square error on dataset.
         * 
         * @param input_data 
         * @param output_data 
         * @return Scalar
         */
        Scalar test(
            std::vector<RowVector*> input_data,
            std::vector<RowVector*> output_data
        );

        /**
         * @brief   Get the weights of connections.
         * 
         * @return std::vector<Matrix*> 
         */
        std::vector<Matrix*> get_weights()
        {
            return weights;
        }

        NeuralNetwork(
            const NeuralNetwork&
        ) = delete;

        NeuralNetwork& operator= (
            const NeuralNetwork&
        ) = delete;

    private:
        /**
         * @brief   Calculates neuron values.
         * 
         * @param input 
         */
	    void propagate_forward(
            RowVector& input
        );

        /**
         * @brief   Calculates delta and updates weights.
         * 
         * @param output correct output.
         */
	    void propagate_backward(
            RowVector& output
        );

        /**
         * @brief   Calculates delta using neuron values.
         * 
         * @param output correct output.
         */
	    void calculate_deltas(
            RowVector& output
        );

        /**
         * @brief   Get the main square error of prediction by input.
         * 
         * @param input 
         * @param output 
         * @return Scalar
         */
        Scalar get_main_square_error(
            RowVector& input,
            RowVector& output
        );

        /**
         * @brief   Updates values of weights after calculating delta.
         * 
         */
	    void update_weights();

        /**
         * @brief   Calculating value of changing single weight.
         * 
         * @param layer layer of weight.
         * @param c column of weight.
         * @param r row of weight.
         * @return Scalar
         */
        Scalar NeuralNetwork::calculate_weights_change(
            Number layer,
            Number c,
            Number r
        );

        /**
         * @brief   Creates storage obgects layers according to the
         *          structure and initializes them.
         * 
         */
        void initialize_storage_objects();

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
        );

        /**
         * @brief   Applies activation function to all neyrons in given layer.
         * 
         * @param layer number of layer.
         */
        void apply_activation_function_to_neuron_layer(
            Number layer
        );

        /**
         * @brief   Linear activation function f(x) = x.
         * 
         * @param x some Scalar.
         * @return Scalar
         */
        static Scalar activation_function(
            Scalar x
        )
        {
            return x;
        }

        /**
         * @brief   Derivative of linear activation function f(x) = x.
         * 
         * @return Scalar
         */
        static Scalar activation_function_derivative(
            Scalar
        )
        {
            return 1;
        }

        /**
         * @brief   Validates structure according to its element values and size.
         * 
         * @param s vector pretending to be structure.
         */
        void validate_structure(
            const std::vector<Number>& s
        ) const;

        /**
         * @brief   Validates given learning rate according to its value.
         * 
         * @param l scalar pretending to be learning rate.
         */
        void validate_learning_rate(
            Scalar l
        ) const;

        /**
         * @brief   Validates given weights according to the structure.
         * 
         * @param w vector pretending to be weights.
         */
        void validate_weights(
            std::vector<Matrix*> w
        ) const;

        /**
         * @brief   Validates given vectors of inputs and outputs
         *          according to their sizes.
         * 
         * @param i_d vector pretending to be inputs.
         * @param o_d vector pretending to be outputs.
         */
        void validate_data(
            std::vector<RowVector*> i_d,
            std::vector<RowVector*> o_d
        ) const;

        /**
         * @brief   Validates given input according to network structure.
         * 
         * @param i vector pretending to be input.
         */
        void validate_input(
            RowVector& i
        ) const;

        /**
         * @brief   Validates given output according to network structure.
         * 
         * @param o vector pretending to be output.
         */
        void validate_output(
            RowVector& o
        ) const;

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
        std::vector<Number> structure;

        /**
         * @brief   Layers of network during calculations.
         * //
         *          Contains neurons of network including bias neuron.
         * 
         */
        std::vector<RowVector*> neuron_layers;

        /**
         * @brief   Error contribution of each neuron.
         * //
         *          Contains calculated errors, using in
         *          updating of weigts.
         * 
         */
        std::vector<RowVector*> deltas;

        /**
         * @brief   Connection weights between neurons.
         * //
         *          It must be conformed with structure of network.
         * 
         */
        std::vector<Matrix*> weights;

        /**
         * @brief   Coeffitient of gradient decent rate.
         * //
         *          Has default value. It must be > 0.
         * 
         */
        Scalar learning_rate;
    };
}

#endif