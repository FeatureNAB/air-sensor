#pragma once
#include <string>
#include <math.h>
#include <cstdio>
#include "esphome.h"

// outputs
struct AQIAdvice {
  std::string status;
  std::string action;
  std::string reason;
};

class AQIAlgo {
public:
  // Directly takes and returns the ESPHome image references
  static esphome::image::Image* get_icon_from_action(
      const std::string& action, 
      esphome::image::Image* img_window,
      esphome::image::Image* img_purifier,
      esphome::image::Image* img_ok) {
        
    std::string lower_action = action;
    std::transform(lower_action.begin(), lower_action.end(), lower_action.begin(),
        [](unsigned char c){ return std::tolower(c); });
        
    if (lower_action.find("vent") != std::string::npos) {
        return img_window;
    } else if (lower_action.find("purif") != std::string::npos) {
        return img_purifier;
    }
    return img_ok;
  }

  static AQIAdvice get_advice(
      float co2, 
      float pm25, 
      float voc_index, 
      float humidity, 
      float outdoor_co2 = NAN, 
      float outdoor_pm25 = NAN) {
        
    AQIAdvice advice;

    // If any of the sensors are NaN, the sensor is probably booting. 
    if (std::isnan(co2) || std::isnan(pm25) || std::isnan(voc_index) || std::isnan(humidity)) {
      advice.status = "Starting";
      advice.action = "No Data";
      advice.reason = "Please Wait...";
      return advice; 
    }
    
    // =========================================================
    // 1. OVERALL STATUS (Directly matches WHO 2021 JS logic)
    // =========================================================
    if (co2 > 1500 || pm25 > 35) {
      advice.status = "Poor";
    } else if (co2 > 1000 || pm25 > 25) {
      advice.status = "Fair";
    } else if (co2 > 800 || pm25 > 15) {
      advice.status = "Moderate";
    } else if (co2 > 600 || pm25 > 5) {
      advice.status = "Good";
    } else {
      advice.status = "Excellent";
    }

    // =========================================================
    // HELPER: Reason Formatter (e.g. "CO2 at 860ppm")
    // =========================================================
    auto make_reason = [](const char* prefix, float val, const char* suffix) {
      char buf[64];
      // Handles uninitialized sensors gracefully
      if (std::isnan(val)) {
        snprintf(buf, sizeof(buf), "%s Calibrating", prefix);
      } else {
        snprintf(buf, sizeof(buf), "%s at %.0f%s", prefix, val, suffix);
      }
      return std::string(buf);
    };

    // =========================================================
    // 2 & 3. ACTION AND REASON (Waterfall logic)
    // =========================================================
    std::string raw_action = "";

    if (co2 > 1500) {
      raw_action = "Urgent Vent";
      advice.reason = make_reason("CO2", co2, "ppm");
    } else if (pm25 > 35) {
      raw_action = "Run Purifier";
      advice.reason = make_reason("PM", pm25, "ug/m3");
    } else if (voc_index >= 150) { 
      // Sensirion VOC Index is normal at 100. >=150 is Elevated/Actionable.
      raw_action = "Ventilate";
      advice.reason = make_reason("VOC Index", voc_index, "");
    } else if (pm25 > 25 && co2 > 1000) {
      raw_action = "Ventilate";
      advice.reason = "PM & CO2 High";
    } else if (pm25 > 25) {
      raw_action = "Run Purifier";
      advice.reason = make_reason("PM2.5", pm25, "ug/m3");
    } else if (co2 > 1000) {
      raw_action = "Ventilate";
      advice.reason = make_reason("CO2", co2, "ppm");
    } else if (humidity < 30) {
      raw_action = "Too Dry";
      advice.reason = make_reason("Humidity", humidity, "%");
    } else if (humidity > 60) {
      raw_action = "Too Humid";
      advice.reason = make_reason("Humidity", humidity, "%");
    } else if (co2 > 800 || pm25 > 15) {
      raw_action = "Cons. Vent";
      if (co2 > 800) {
         advice.reason = make_reason("CO2", co2, "ppm");
      } else {
         advice.reason = make_reason("PM2.5", pm25, "ug/m3");
      }
    } else {
      raw_action = "No Actions";
      advice.reason = "Optimal Air"; // Or can be left blank: ""
    }

    // =========================================================
    // SMART OUTDOOR OVERRIDE
    // =========================================================
    bool outdoorIsWorse = false;
    if (!std::isnan(outdoor_pm25) && outdoor_pm25 > pm25) outdoorIsWorse = true;
    if (!std::isnan(outdoor_co2) && outdoor_co2 > co2) outdoorIsWorse = true;

    if (outdoorIsWorse && (
        raw_action == "Urgent Vent" || 
        raw_action == "Cons. Vent" ||
        raw_action == "Ventilate"
    )) {
        if (pm25 > 15) {
            raw_action = "Run Purifier";
            advice.reason = "Worse Outside";
        } else {
            raw_action = "Keep Closed";
            advice.reason = "Worse Outside";
        }
    }

    advice.action = raw_action;
    return advice;
  }
};