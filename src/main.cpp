#include <iostream>
#include <fstream>  // opens and reads files
#include <sstream>  // parses each line into pieces
#include <string>
#include <vector>

/*
STRUCT 1: SensorPing
Represents one raw GPS report from a tracked asset such as from a drone.
Each line in sensors.txt will become one SensorPing struct.
*/

struct SensorPing
{
    std::string timestamp;
    double latitude;
    double longitude;
    std::string trackId;
};

/*
STRUCT 2: TrackState
Maintains the running state of one tracked asset.
Updates every time a new ping arrives for that asset.
the drone's running story over time. 
It remembers where the drone was last seen, 
how many times it's reported in, and how far it's traveled total.
*/

struct TrackState
{
    std::string trackId;
    SensorPing lastPing;
    double totalDistanceKm = 0.0;
    int pingCount = 0;
    bool initialized = false;
};

/*
PARSER FUNCTION
Opens sensor.txt and reads it line by line.
Each line becomes one SensorPing.
Returns a vector (list) of all pings found.
*/

std::vector<SensorPing> parseSensorFile(const std::string& filename) {
    std::vector<SensorPing> pings; // our list of results. std::vector is like a Python list or Java ArrayList
    std::ifstream file(filename); // open the file. Like Python's open()

    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << filename << std::endl;
        return pings; // return an empty list
    }

    std::string line;   // a line of text from the file

    // Read the file one line at a time
    while (std::getline(file, line)) {
        // Skip blank lines and comment lines (lines starting with #)
        if (line.empty() || line[0] == '#') continue;

        // Parse the line into pieces using a string stream
        std::istringstream ss(line);                // treat a string like a file and read pieces out of it 
                                                    // + split each line by commas into individual values
        std::string timestamp, trackId, latStr, lonStr;

        // Each piece is separated by a comma
        std::getline(ss, timestamp, ',');
        std::getline(ss, latStr, ',');
        std::getline(ss, lonStr, ',');
        std::getline(ss, trackId, ',');

        // Build a SensorPing from the pieces
        SensorPing ping;
        ping.timestamp = timestamp;
        ping.latitude = std::stod(latStr);      // convert string to double
        ping.longitude = std::stod(lonStr);     // convert string to double
        ping.trackId = trackId;

        pings.push_back(ping);  // add to the list. Same as Python's list.append()
    }

    file.close();
    return pings;
}

/****************************
MAIN 
****************************/
int main(int argc, char *argv[])
{
    std::cout << "=== Sentinel-Sim: Sensor Fusion Simulator ===" << std::endl;
    std::cout << "Ingesting sensor feed..." << std::endl;

    // Parse the sensor feed from a file
    std::vector<SensorPing> pings = parseSensorFile("sensors.txt");

    // Print each ping to confirm parsing workload
    std::cout << "\n[SENSOR FEED - RAW PINGS RECEIVED]" << std::endl;
    std::cout << "----------------------------------" << std::endl;

    // C++ range-based for loop
//Equivalent to Python's for loop (for ping in pings)
    // '&' = reading each ping by reference instead of making a copy which is more efficient
    for (const SensorPing& ping : pings) {
        std::cout << " [" << ping.timestamp << "] "
        << ping.trackId
        << " | LAT: " << ping.latitude
        << " | LON: " << ping.longitude
        << std::endl;
    }

    std::cout << "----------------------------------" << std::endl;
    std::cout << "Total pings ingested: " << pings.size() << std::endl;

    return 0;
}