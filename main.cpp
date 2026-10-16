#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

// We will use GPIO Pin 17 (Physical Board Pin 11) from your Freenove kit layout
const std::string GPIO_PIN = "17";
const std::string SYSFS_GPIO_DIR = "/sys/class/gpio";

void write_file(const std::string& path, const std::string& value) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return;
    }
    file << value;
    file.close();
}

int main() {
    std::cout << "Initializing Embedded Linux Telemetry Daemon..." << std::endl;

    // Step 1: Export the GPIO pin to the Linux sysfs pool
    write_file(SYSFS_GPIO_DIR + "/export", GPIO_PIN);
    
    // Give the Linux kernel a brief moment to instantiate the virtual files
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Step 2: Set the pin direction to "out"
    std::string path_direction = SYSFS_GPIO_DIR + "/gpio" + GPIO_PIN + "/direction";
    write_file(path_direction, "out");

    std::string path_value = SYSFS_GPIO_DIR + "/gpio" + GPIO_PIN + "/value";
    std::cout << "System initialized. Toggling GPIO " << GPIO_PIN << " (Blinking LED)..." << std::endl;

    // Step 3: Loop to blink the physical LED 10 times
    for (int i = 0; i < 10; ++i) {
        std::cout << "LED ON" << std::endl;
        write_file(path_value, "1"); // Set pin HIGH (3.3V)
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "LED OFF" << std::endl;
        write_file(path_value, "0"); // Set pin LOW (0V)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Step 4: Clean up by unexporting the pin when done
    write_file(SYSFS_GPIO_DIR + "/unexport", GPIO_PIN);
    std::cout << "Daemon execution complete. Cleaning up resources." << std::endl;
    
    return 0;
}
