#ifndef NETWORK_DEFENCE
#define NETWORK_DEFENCE 1

#include "eigen-3.4.0/Eigen/Eigen"
#include <exception>

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

    constexpr Scalar default_learning_rate_value        { 0.005 };
    constexpr Scalar critical_difference_between_errors { -0.5 };


    // will be thrown when invalid values given
    class NetworkInvalidValue : public std::exception {
    public:
        NetworkInvalidValue(const std::string msg)
        : message(msg.c_str())  { }

        const char* what() const throw()
        {
            return (message).c_str();
        }
    
    private:
        std::string message;
    };


    class NeuralNetwork {
    public:
	    NeuralNetwork(
            std::vector<Number> s,
            Scalar l = default_learning_rate_value
        );

        NeuralNetwork(
            std::vector<Number> s,
            std::vector<Matrix*> w
        );

        RowVector predict(
            RowVector& input
        );

        void train(
            std::vector<RowVector*> input_data,
            std::vector<RowVector*> output_data
        );

        Scalar test(
            std::vector<RowVector*> input_data,
            std::vector<RowVector*> output_data
        );

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
	    void propagate_forward(
            RowVector& input
        );

	    void propagate_backward(
            RowVector& output
        );

	    void calculate_errors(
            RowVector& output
        );

        Scalar get_main_square_error(
            RowVector& input,
            RowVector& output
        );

	    void update_weights();

        void initialize_storage_objects();

        // initializes layer of weigths with random values
        // initializes bias neurons
        void initialize_storage_objects_layer(
            Number layer
        );

        void apply_activation_function_to_neuron_layer(
            Number layer
        );

        // linear activation function
        static Scalar activation_function(
            Scalar x
        )
        {
            return x;
        }

        static Scalar activation_function_derivative(
            Scalar
        )
        {
            return 1;
        }

        void validate_structure(
            const std::vector<Number>& s
        ) const;

        void validate_learning_rate(
            Scalar l
        ) const;

        void validate_weights(
            std::vector<Matrix*> w
        ) const;

        void validate_data(
            std::vector<RowVector*> input_data,
            std::vector<RowVector*> output_data
        ) const;

        void validate_input(
            RowVector& input
        ) const;

        void validate_output(
            RowVector& output
        ) const;

	    // storage objects for working of neural network
	    /*
		use pointers when using std::vector<Class> as std::vector<Class> calls destructor of 
		Class as soon as it is pushed back! when we use pointers it can't do that, besides
		it also makes our neural network class less heavy!! It would be nice if you can use
		smart pointers instead of usual ones like this
		*/

        std::vector<Number>     structure;
        std::vector<RowVector*> neuron_layers;  // stores the different layers of out network
        std::vector<RowVector*> deltas;         // stores the error contribution of each neurons
	    std::vector<Matrix*>    weights;        // the connection weights itself
        Scalar                  learning_rate;
    };
}

#endif