#include "esphome.h"

extern esphome::font::Font *montserrat_14;
extern esphome::font::Font *montserrat_20;
extern esphome::font::Font *montserrat_24;
extern esphome::font::Font *droplet;
extern esphome::font::Font *temperature;

// Handles NaN while the sensor boots
inline std::string format_sensor(const char* format, float val) {
    if (std::isnan(val)) {
        return "--"; // Simply return dashes for booting/NaN sensors
    }
    char buf[32];
    snprintf(buf, sizeof(buf), format, val);
    return std::string(buf);
}

// 2. Define your layout function
// Note: We pass the display buffer 'it' by reference (&)
void draw_main_screen(esphome::display::Display &it, esphome::image::Image *icon_image, esphome::ESPTime current_time, const std::string &aqi_status, const std::string &aqi_action, const std::string &aqi_reason, float temp, float hum, float pm100, float co2, float voc) {
    // Defining colors (ensures colors are not inverted on monochrome)
    Color background_color = Color(0, 0, 0);
    Color primary_color = Color(255, 255, 255);

    // Background colour
    it.fill(background_color);
    
    // Sanitising the time
    if (current_time.is_valid()) {
        // strftime takes: X, Y, Font, Color, Format string, Time object
        it.strftime(3, 3, montserrat_14, primary_color, esphome::display::TextAlign::TOP_LEFT, "%b %d", current_time);
        it.strftime(119, 3, montserrat_14, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%H:%M", current_time);
    } else {
        // Fallback for before time sync is complete
        it.print(3, 3, montserrat_14, primary_color, esphome::display::TextAlign::TOP_LEFT, "----");
        it.print(119, 3, montserrat_14, primary_color, esphome::display::TextAlign::TOP_RIGHT, "--:--");
    }
    
    // Top Section
    it.image(61, 61, icon_image, esphome::display::ImageAlign::CENTER_HORIZONTAL, primary_color);
    it.printf(61, 30, montserrat_24, primary_color, esphome::display::TextAlign::CENTER_HORIZONTAL, aqi_status.c_str());

    // Middle Section
    it.print(61, 158, montserrat_20, primary_color, esphome::display::TextAlign::CENTER_HORIZONTAL, aqi_action.c_str());
    it.printf(61, 188, montserrat_14, primary_color, esphome::display::TextAlign::CENTER_HORIZONTAL, aqi_reason.c_str());


    it.line(8, 218, 114, 218, primary_color);

    // Bottom Section
    // it.print(-6, 222, thermometer, primary_color, "\U0000e1ff");
    it.print(4, 222, montserrat_20, primary_color, esphome::display::TextAlign::TOP_LEFT, format_sensor("%.0f°C", temp).c_str());
    // it.print(62, 222, droplet, primary_color, "\U0000f87e");
    it.print(118, 222, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, format_sensor("%.0f%%%", hum).c_str());
}

void draw_detail_screen(esphome::display::Display &it, esphome::ESPTime current_time, float temp, float hum, float pm10, float pm25, float pm40, float pm100, float co2, float voc, float nox) {
    // Defining colors (ensures colors are not inverted on monochrome)
    Color background_color = Color(0, 0, 0);
    Color primary_color = Color(255, 255, 255);

    // Background colour
    it.fill(background_color);
    
    if (current_time.is_valid()) {
        // strftime takes: X, Y, Font, Color, Format string, Time object
        it.strftime(3, 3, montserrat_14, primary_color, esphome::display::TextAlign::TOP_LEFT, "%b %d", current_time);
        it.strftime(119, 3, montserrat_14, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%H:%M", current_time);
    } else {
        // Fallback for before time sync is complete
        it.print(3, 3, montserrat_14, primary_color, esphome::display::TextAlign::TOP_LEFT, "----");
        it.print(119, 3, montserrat_14, primary_color, esphome::display::TextAlign::TOP_RIGHT, "--:--");
    }
     
    // Top Section
    it.print(4, 18, montserrat_20, primary_color, "PM1");
    it.print(4, 40, montserrat_20, primary_color, "PM2.5");
    it.print(4, 60, montserrat_20, primary_color, "PM4");
    it.print(4, 80, montserrat_20, primary_color, "PM10");
    it.printf(119, 20, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.1f", pm10);
    it.printf(119, 40, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.1f", pm25);
    it.printf(119, 60, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.1f", pm40);
    it.printf(119, 80, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.1f", pm100);

    it.line(8, 110, 114, 110, primary_color);

    // Middle Section
    it.print(46, 116, thermometer, primary_color, "\U0000e1ff");
    it.printf(119, 116, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.0f °C", temp);
    it.print(46, 143, droplet, primary_color, "\U0000f87e");
    it.printf(119, 143, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.0f  %%", hum);

    it.line(8, 168, 114, 168, primary_color);

    // Bottom Section
    it.print(4, 173, montserrat_20, primary_color, "CO2");
    it.print(4, 199, montserrat_20, primary_color, "VOC");
    it.print(4, 222, montserrat_20, primary_color, "NOx");
    it.printf(119, 173, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.0f", co2);
    it.printf(119, 199, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.0f", voc);
    it.printf(119, 222, montserrat_20, primary_color, esphome::display::TextAlign::TOP_RIGHT, "%.0f", nox);
}

