#include <iostream>
#include <gpiod.hpp>
#include <chrono>
#include <thread>

int main() {
    std::cout << "System initialized. Preparing GPIO 17..." << std::endl;
    
    // On Raspberry Pi 4, the primary GPIO controller is "gpiochip4"
    std::string chip_name = "gpiochip4"; 
    unsigned int pin_num = 17;

    try {
        // Open the GPIO hardware chip
        gpiod::chip chip(chip_name);
        gpiod::line line = chip.get_line(pin_num);
        
        // Claim the pin as an output immediately (no race conditions!)
        line.request({
            "telemetry_daemon",
            gpiod::line_request::DIRECTION_OUTPUT,
            0
        });

        std::cout << "Toggling GPIO 17 (Blinking LED)..." << std::endl;

        while (true) {
            std::cout << "LED ON" << std::endl;
            line.set_value(1);
            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::cout << "LED OFF" << std::endl;
            line.set_value(0);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cerr << "GPIO Hardware Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
