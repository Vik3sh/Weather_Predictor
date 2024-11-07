#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class WeatherData {
private:
    string date;
    double temprature;
    double precipitation;

public:
    WeatherData(const string &date, double temprature, double precipitation)
        : date(date), temprature(temprature), precipitation(precipitation) {}

    string getDate() const {
        return date;
    }

    double getTemprature() const {
        return temprature;
    }

    double getPrecipitation() const {
        return precipitation;
    }

    void display() const {
        cout << "Date: " << date
             << ", Temperature: " << temprature << " C"
             << ", Precipitation: " << precipitation << " mm" << endl;
    }
};

vector<WeatherData> loadWeatherData(const string &filePath) {
    vector<WeatherData> data;
    ifstream file(filePath);
    string line, word;
    getline(file, line); // Skip header line

    while (getline(file, line)) {
        stringstream ss(line);
        string date;
        double temprature, precipitation;

        getline(ss, date, ',');
        getline(ss, word, ',');
        temprature = stod(word);
        getline(ss, word, ',');
        precipitation = stod(word);

        data.emplace_back(date, temprature, precipitation);
    }
    file.close();
    return data;
}

double predictTemperature(const vector<double>& temperatures, const vector<double>& precipitation, int days) {
    double a = 0.7; // Coefficients can be tuned
    double b = 0.2;
    double c = 0.1;

    vector<double> dp(days + 1, 0.0);

    // Initialize the first days with actual temperature data
    dp[1] = temperatures[0]; // First day's actual temperature
    if (days > 1) {
        dp[2] = temperatures[1]; // Second day's actual temperature
    }

    // Fill the DP table for prediction
    for (int i = 3; i <= days; ++i) {
        dp[i] = a * temperatures[i - 2] + b * temperatures[i - 3] + c * precipitation[i - 2];
    }

    return dp[days]; // Return predicted temperature for the given day
}

int main() {
    string filePath = "C:\\Users\\negiv\\Desktop\\DSA weather predictor\\export (1).csv";
    vector<WeatherData> weatherData = loadWeatherData(filePath);

    // Display the loaded weather data
    for (const auto &entry : weatherData) {
        entry.display();
    }

    // Prepare data for prediction
    vector<double> temperatures;
    vector<double> precipitation;

    for (const auto &entry : weatherData) {
        temperatures.push_back(entry.getTemprature());
        precipitation.push_back(entry.getPrecipitation());
    }

    int predictForDays = 5; // Example: Predict temperature for the 5th day
    if (predictForDays <= temperatures.size()) {
        double predictedTemperature = predictTemperature(temperatures, precipitation, predictForDays);
        cout << "Predicted Temperature for Day " << predictForDays << ": " << predictedTemperature << " C" << endl;
    } else {
        cout << "Not enough data to predict for day " << predictForDays << endl;
    }

    return 0;
}
