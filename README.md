### Description for Emission Prediction Program

#### Overview

The Emission Prediction Program is designed to predict vehicle emissions (CO2, NOX, PM, VOC, SO2) based on various input parameters using a neural network. The program features a graphical user interface (GUI) that allows users to input vehicle data, visualize predictions, and manage historical data.

\
![image](https://github.com/user-attachments/assets/9ab911af-68c6-4a25-aace-100d93878430)


#### Program Structure

- **Main Functionality**: The neural network preparation is divided into two main modes:
    
    - **Training Mode**: Trains the neural networks using a dataset and saves the trained weights.
    - **Testing Mode**: Loads pre-trained weights and allows users to input vehicle parameters for predictions.

- **GUI Components**: The GUI provides an interactive interface for users to input data and view results. It includes:
    
    - Input fields for vehicle parameters.
    - Buttons for actions such as predicting emissions, saving inputs, loading historical data, and clearing inputs.
    - A graph area to visualize emissions data.

#### Getting Started

1. **Compile the Program**: Ensure that all dependencies are installed and compile the program using CMake.
2. **Run the Program**: Execute the compiled binary to launch the GUI.

#### Using the GUI

1. **Input Parameters**:
    
    - Enter the following vehicle parameters in the provided input fields:
        - Vehicle Type (Car, Truck, Motorcycle, Bus)
        - Fuel Type (Petrol, Diesel, Electric, Hybrid)
        - Engine Size (liters)
        - Age of Vehicle (years)
        - Mileage (km)
        - Acceleration (m/s²)
        - Road Type (City, Rural, Highway)
        - Traffic Conditions (Heavy, Moderate, Free flow)
        - Temperature (°C)
        - Humidity (relative %)
        - Wind Speed (m/s)
        - Air Pressure (hPa)

    Then you need to input the max speed value. It is the limit up to which the graph will be drawn.
2. **Buttons**:

    - **Predict**: Click this button to calculate emissions based on the input parameters. The results will be displayed in the graph area.
    - **Save**: Saves the current input parameters to a file named `saved_inputs.txt`.
    - **Load**: Loads previously saved input parameters from `saved_inputs.txt` into the input fields.
    - **Clear**: Clears all input fields and resets the graph.
    - **Help**: Displays a help window with instructions on how to use the program.
    - **Next/Previous Gas**: Navigate through different gas emissions (CO₂, NOx, PM2.5, VOCs, SO₂) to view their respective graphs.
3. **Graph Visualization**: The program will display a graph representing the predicted emissions based on the input parameters. The graph will update when predictions are made.


#### Error Handling

- If invalid data is entered, the program will display an error message and prompt the user to correct the input.
- Ensure that the input values are within reasonable ranges to avoid errors during prediction.

#### Conclusion

This program provides a user-friendly interface for predicting vehicle emissions using advanced neural network techniques. By following the instructions above, users can effectively utilize the program to analyze and visualize emissions data.
