import subprocess
import matplotlib.pyplot as plt

input_data = [  # Truck Petrol 23.765 13 42687 0 1.888 City Moderate 17.45 39.743 1.9347 1001.86831
    "Truck",
    "Petrol",
    "23.765",
    "13",
    "42687",
    "speed",
    "1.888",
    "City",
    "Moderate",
    "17.45",
    "39.743",
    "1.9347",
    "1001.86831",
]
output_data = []

speed = range(0, 100, 1)
for speed_value in speed:
    print(speed_value)
    process = subprocess.Popen(
        ["build/emission_predict.exe"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        shell=True,
    )

    str_to_send = ""

    for input_cell in input_data:
        if input_cell == "speed":
            str_to_send += str(speed_value)
        else:
            str_to_send += input_cell
        str_to_send += " "
    str_to_send = str_to_send[:-1] + "\n"

    output, stderr = process.communicate(input=str_to_send.encode())
    output = float(output.decode())

    output_data.append(output)

print(output_data)

plt.scatter(speed, output_data)
plt.show()
