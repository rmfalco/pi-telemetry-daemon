#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

// Helper function to write values to sysfs files
void write_sysfs(const std::string& path, const std::string& value) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return;
    }
    file << value;
    file.close();
}

int main() {
    std::cout << "System initialized. Preparing GPIO 17..." << std::endl;

    // 1. Export the pin (creates the gpio17 directory)
    write_sysfs("/sys/class/gpio/export", "17");

    // 2. Wait a moment for the OS to dynamically create the filesystem nodes
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 3. Set the direction to output
    write_sysfs("/sys/class/gpio/gpio17/direction", "out");

    std::cout << "Toggling GPIO 17 (Blinking LED)..." << std::endl;

    // 4. Main loop to blink the LED
    try {
        while (true) {
            std::cout << "LED ON" << std::endl;
            write_sysfs("/sys/class/gpio/gpio17/value", "1");
            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::cout << "LED OFF" << std::endl;
            write_sysfs("/sys/class/gpio/gpio17/value", "0");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (...) {
        // Cleanup on exit
        write_sysfs("/sys/class/gpio/unexport", "17");
    }

    return 0;
}
