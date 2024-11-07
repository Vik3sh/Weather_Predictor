// Function to parse CSV data
function parseCSV(data) {
    const lines = data.split("\n");
    const result = [];
  
    for (let i = 1; i < lines.length; i++) {
      const line = lines[i].trim();
      if (!line) continue;
      
      const [date, temperature, precipitation] = line.split(",");
      result.push({
        date: date,
        temperature: parseFloat(temperature),
        precipitation: parseFloat(precipitation)
      });
    }
  
    return result;
  }
  
  // Function to load CSV file
  function loadCSVFile(event) {
    return new Promise((resolve, reject) => {
      const file = event.target.files[0];
      if (!file) {
        return reject("No file selected");
      }
  
      const reader = new FileReader();
      reader.onload = () => resolve(parseCSV(reader.result));
      reader.onerror = () => reject("Failed to read file");
      reader.readAsText(file);
    });
  }
  
  // Function to predict temperature for each day up to the entered days
  function predictWeatherForDays(data, days) {
    const a = 0.7, b = 0.2, c = 0.1;
    const temperatures = data.map(entry => entry.temperature);
    const precipitation = data.map(entry => entry.precipitation);
  
    let predictions = [];
  
    // Loop through each day and calculate predicted values
    for (let day = 1; day <= days; day++) {
      let dpTemp = [];
      dpTemp[1] = temperatures[0];
      if (day > 1) dpTemp[2] = temperatures[1];
  
      for (let i = 3; i <= day; i++) {
        dpTemp[i] = a * temperatures[i - 2] + b * temperatures[i - 3] + c * precipitation[i - 2];
      }
      predictions.push({ day, temperature: dpTemp[day].toFixed(2) });
    }
  
    return predictions;
  }
  
  // Main function to load data, calculate, and display predictions
  async function predictWeather() {
    const daysInput = document.getElementById("daysInput").value;
    const fileInput = document.getElementById("fileInput");
    const days = parseInt(daysInput);
  
    if (!days || days <= 0) {
      document.getElementById("result").textContent = "Please enter a valid number of days.";
      return;
    }
  
    if (!fileInput.files.length) {
      document.getElementById("result").textContent = "Please upload a CSV file.";
      return;
    }
  
    try {
      const weatherData = await loadCSVFile({ target: fileInput });
      const predictions = predictWeatherForDays(weatherData, days);
  
      let resultText = "Predicted Temperatures:\n";
      predictions.forEach(prediction => {
        resultText += `Day ${prediction.day}: ${prediction.temperature} °C\n`;
      });
  
      document.getElementById("result").textContent = resultText;
    } catch (error) {
      document.getElementById("result").textContent = `Error: ${error}`;
    }
  }
  function predictWeather() {
    const fileInput = document.getElementById('fileInput');
    const daysInput = document.getElementById('daysInput');
    const resultElement = document.getElementById('result');

    if (!fileInput.files.length || !daysInput.value) {
        alert("Please upload a CSV file and enter the number of days.");
        return;
    }

    const days = parseInt(daysInput.value);

    // Simulated prediction result for demonstration
    let predictionResult = `Predicted temperature for ${days} days:\n\n`;
    for (let i = 1; i <= days; i++) {
        predictionResult += `Day ${i}: ${Math.round(Math.random() * 10 + 20)} °C\n`;
    }

    resultElement.textContent = predictionResult;

    // Show the popup
    document.getElementById('popupOverlay').classList.add('show');
}

function closePopup() {
    document.getElementById('popupOverlay').classList.remove('show');
}
