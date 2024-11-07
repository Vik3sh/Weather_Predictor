//#include "crow_all.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class WeatherData {
private:
    std::string date;
    double temperature;
    double precipitation;

public:
    WeatherData(const std::string &date, double temperature, double precipitation)
        : date(date), temperature(temperature), precipitation(precipitation) {}

    std::string getDate() const { return date; }
    double getTemperature() const { return temperature; }
    double getPrecipitation() const { return precipitation; }
};

std::vector<WeatherData> loadWeatherData(const std::string &filePath) {
    std::vector<WeatherData> data;
    std::ifstream file(filePath);
    std::string line, word;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string date;
        double temperature, precipitation;

        getline(ss, date, ',');
        getline(ss, word, ',');
        temperature = stod(word);
        getline(ss, word, ',');
        precipitation = stod(word);

        data.emplace_back(date, temperature, precipitation);
    }

    file.close();
    return data;
}

double predictTemperature(const std::vector<double> &temperatures, const std::vector<double> &precipitation, int days) {
    double a = 0.7, b = 0.2, c = 0.1; // Coefficients can be tuned
    std::vector<double> dp(days + 1, 0.0);

    dp[1] = temperatures[0];
    if (days > 1) {
        dp[2] = temperatures[1];
    }

    for (int i = 3; i <= days; ++i) {
        dp[i] = a * temperatures[i - 2] + b * temperatures[i - 3] + c * precipitation[i - 2];
    }

    return dp[days];
}

int main() {
    // crow::SimpleApp app;

    // CROW_ROUTE(app, "/predict/<int>")
    ([](int days) {
        std::string filePath = "C:\\path\\to\\your\\weather_data.csv"; // Update path
        auto weatherData = loadWeatherData(filePath);

        std::vector<double> temperatures, precipitation;
        for (const auto &entry : weatherData) {
            temperatures.push_back(entry.getTemperature());
            precipitation.push_back(entry.getPrecipitation());
        }

        double predictedTemp = predictTemperature(temperatures, precipitation, days);
       // return crow::response{std::to_string(predictedTemp)};
    });

    //app.port(8080).multithreaded().run();
    return 0;
}
