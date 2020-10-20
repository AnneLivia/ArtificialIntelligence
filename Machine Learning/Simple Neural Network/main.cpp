#include <iostream>
#include <vector>

using namespace std;

/*
    Neurons: a neuron is connected to many or thousands of neurons
    In a human brain: neurons are in the cortex. Each neuron has a job to communicate with other neurons
    neurons talk to each other with electric and chemical changes.
    The communication between neurons is called synapses


    In computer:
    neural networks are the basis for deep learning (in deep learning we have more intermediate layers)
    Neural networks gets some data (those date are features inserted into only one layer with enough neurons) then,
    it does computations with those values and then predict the output or a new set of similar data.
        Input layer -> receives the input
        Output Layer -> predicts the final answer
        in between those two layers there are many intermediate layers that performs all the computations. they are the hidden layers

    Suppose we want to detect a circle: a image of a circle of dimensions of 20x20. each of these pixels is going to be in the input layer
    the neurons of one layer is connected to others through channels, in each of these channels is assigned a numerical value
    the output is corresponding  to the weight of the neuron times the numerical value of this channel plus the bias. = ((w1 * value_channel1) + (wn * value_channeln)) + bias
    this value goes to the activation functions, the result of the activation determines if the particular neuron is going to activated or not.
    the activated neuron transmits data to the next layer of neurons (this is called forward propagation) (the values are basically a probability)

    along the process of training the network must have the values of the actual input (we are talking about a supervised learning)
    and the values of the error (the magnitude of the error indicates how wrong the neural is and suggest with the predicted vales or higher or lower than expected.
    this information is transfers to the input layer (this is called back propagation).
    based on this information the weights are adjusted  (this cycle of forward and backward propagation
    is repeated until the neural can predict the corrected value in almost test correctly
    the training can take hours to weeks

    a layer can have one or more neurons
*/

/*
    A neuron has a weight and a bias.
    that is: a neuron receives a input and output a value

    More about bias and weights:
        Accordingly to Malik from https://medium.com/fintechexplained/neural-networks-bias-and-weights-10b53e6285da:
            weights and bias are possibly the most important concept of a neural network.
            When the inputs are transmitted between neurons,
            the weights are applied to the inputs and passed into an activation function along with the bias.

    Weights are the co-efficients of the equation which you are trying to resolve. Negative weights reduce the value of an output.
    When a neural network is trained on the training set, it is initialized with a set of weights.
    These weights are then optimized during the training period and the optimum weights are produced.

    Y = sum (weight * input) + bias.

    if we have 3 neurons the calculation is:

    ((input1 * weight1) + (input2 * weight2) + (input3 * weight3)) + bias.

    after all of this computation, the computed value is fed into the activation function, which then prepares an output.

    Think of the activation function as a mathematical function that can normalize the inputs.

    The weights are essentially reflecting how important an input is.


    Bias is simply a constant value (or a constant vector) that is added to the product of inputs and weights. Bias is utilized to offset the result.
    The bias is used to shift the result of activation function towards the positive or negative side.


    From https://hackernoon.com/everything-you-need-to-know-about-neural-networks-8988c3ee4491:

    Neuron(Node) - It is the basic unit of a neural network.
    It gets certain number of inputs and a bias value. When a signal(value) arrives,
    it gets multiplied by a weight value. If a neuron has 4 inputs, it has 4 weight values which can be adjusted during training time.


    Connections - It connects one neuron in one layer to another neuron in other layer or the same layer.
    A connection always has a weight value associated with it. Goal of the training is to update this weight value to decrease the loss(error).

    Bias(Offset) - It is an extra input to neurons and it is always 1, and has it's own connection weight.
    This makes sure that even when all the inputs are none (all 0's) there's gonna be an activation in the neuron.


    Activation Function(Transfer Function) - Activation functions are used to introduce non-linearity to neural networks.
    It squashes the values in a smaller range viz. a Sigmoid activation function squashes values between a range 0 to 1.
    There are many activation functions used in deep learning industry and ReLU, SeLU and TanH are preferred over sigmoid activation function.

    activation functions:
    in a RELU -> rectified linear unit = f(x) = { 0 if x < 0; x if x >= 0} === max (x, 0)
    in a PReLU -> parametric rectified linear unit = f(x) = {alfa * x if x < 0; x if x >= 0
    binary step -> 0 if x < 0 and 1 if x >= 0
    sigmoid = f(x) = 1 / (1 + e ^ -x);
    identify is just x


    Input Layer - This is the first layer in the neural network. It takes input signals(values) and passes them on to the next layer.
    It doesn't apply any operations on the input signals(values) & has no weights and biases values associated.



    affine sum -> if there are many inputs and weights

    // one layer (a set of neurons)

*/

class Neuron {
private:
    double weight, bias, x_temp;
    double alpha = 1e-3; // this is the learning rate
public:
    Neuron(double w, double b) : weight(w), bias(b) {}

    void setWeight(double w) {this->weight = w;}
    double getWeight() const {return this->weight;}

    void setBias(double b) {this->bias = b;}
    double getBias() const {return this->bias;}

    // identity activation function
    double getIdentity(const double& x) {
        return x;
    }

    // RELU activation function
    double getRELU(const double& x) {
        return x < 0.0 ? 0.0 : x;
    }

    double getGradientIdentity() {
        return 1.0;
    }

    // x is going to be the input
    double propagation_Foward(const double& x) {
        x_temp = x;
        // affine sum
        const double sum = this->weight * x + this->bias;
        // activate the neuron
        return getIdentity(sum);
    }

    // gradient descent to find numbers
    void propagation_backward(const double& dedy) {
        const double dedw = dedy * getGradientIdentity() * x_temp;
        const double dedb = dedy * getGradientIdentity() * 1.0;

        weight -= alpha * dedw;
        bias -= alpha * dedb;
    }
};

int main()
{
    // initializing the weight and bias of the neurons
    Neuron n(2.0, 1.0);

    const double x_input = 1.0;
    const double y = n.propagation_Foward(x_input);

    cout << "Before training " << y << endl;

    // cout << "First neuron: " << n.propagation_Foward(-1.0) << endl;
    // cout << "First neuron: " << n.propagation_Foward(0.0) << endl;
    // cout << "First neuron: " << n.propagation_Foward(1.0) << endl;


    // now let's implement the back propagation
    // want it to be number 9 currently with input of 1.0 is 3

    const double y_target = 1000.0;

    int cont = 0;
    while(1) { // training loop while squared_error is not small enough

        const double y = n.propagation_Foward(x_input);

        // defining an error function
        const double error = y_target - y;

        const double squared_error = 0.5 * error * error;

         // break this loop when squared_error is less than 1e-3
        if (squared_error < 1e-8 || cont > 100000)
            break;

        cout << "Squared error: " << squared_error << endl;

        // need to calculate the derivative
        const double DeDy = y - y_target; // - error

        n.propagation_backward(DeDy); // update w and b in this function
        cont++;
    }

     const double y_after_training = n.propagation_Foward(x_input);

     cout << "After training " << y_after_training << endl;

    return 0;
}
