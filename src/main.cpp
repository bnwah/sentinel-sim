#include <iostream>
#include <fstream>  // opens and reads files
#include <sstream>  // parses each line into pieces
#include <string>
#include <vector>
#include <cmath>    // sin, cos, sqrt, asin - needed for the Haversine formula
#include <iomanip>  // std::fixed + std::setprecision needed for clean decimal formatting

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

/****************************
CONSTANTS
****************************/

const double EARTH_RADIUS_KM = 6371.0;
const double PI = 3.14159265358979323846;

/****************************
STRUCT 1: SensorPing
Represents one raw GPS report from a tracked asset such as from a drone.
Each line in sensors.txt will become one SensorPing struct.
****************************/

struct SensorPing
{
    std::string timestamp;
    double latitude;
    double longitude;
    std::string trackId;
};

/****************************
STRUCT 2: TrackState
Maintains the running state of one tracked asset.
Updates every time a new ping arrives for that asset.
the drone's running story over time. 
It remembers where the drone was last seen, 
how many times it's reported in, and how far it's traveled total.
****************************/

struct TrackState
{
    std::string trackId;
    SensorPing lastPing;
    double totalDistanceKm = 0.0;
    int pingCount = 0;
    bool initialized = false;
};

/****************************
 * UTILITY: Convert degrees to radians
 * The Harvesine formula works in radians, not degrees
 *  GPS coordinates are in degrees so we need to convert them to radians
****************************/

double toRadians(double degrees) {
    return degrees * (PI / 180.0);
}

/****************************
 * HARVESINE FORMULA:
 * Calculates the distance between two GPS coordinates on Earth's surface (in km).
 * 
 * Formula:
 * a = sin(dlat/2)^2 + cos(lat1) * cos(lat2) * sin(dlon/2)^2
 * d = 2 * R * asin(sqrt(a))
****************************/
double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    // Convert all coordinates from degrees to radians
    double rlat1 = toRadians(lat1);
    double rlat2 = toRadians(lat2);
    double dlat = toRadians(lat2 - lat1);       // Distance b/w latitudes
    double dlon = toRadians(lon2 - lon1);       // Distance b/w longitudes

    // Haversine Formula
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) + std::cos(rlat1) * std::cos(rlat2) * std::sin(dlon / 2) * std::sin(dlon / 2);
    double c = 2 * std::asin(std::sqrt(a));

    // Multiply by the Earth's radius to get km
    return EARTH_RADIUS_KM * c;
}


/****************************
PARSER FUNCTION
Opens sensor.txt and reads it line by line.
Each line becomes one SensorPing.
Returns a vector (list) of all pings found.
****************************/

std::vector<SensorPing> parseSensorFile(const std::string& filename) {
    std::vector<SensorPing> pings; // our list of SensorPings. std::vector is like a Python list or Java ArrayList
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
                                                    // loads the whole line into ss
        std::string timestamp, trackId, latStr, lonStr;

        // Each piece is separated by a comma
        std::getline(ss, timestamp, ',');
        std::getline(ss, latStr, ',');
        std::getline(ss, lonStr, ',');
        std::getline(ss, trackId, ',');

        // Build a SensorPing from the pieces
        SensorPing ping;
        ping.timestamp = trim(timestamp);
        ping.latitude = std::stod(trim(latStr));      // convert string to double
        ping.longitude = std::stod(trim(lonStr));     // convert string to double
        ping.trackId = trim(trackId);

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

    // TrackState to maintain running distance
    TrackState track;

    // C++ range-based for loop
    //Equivalent to Python's for loop (for ping in pings)
    // '&' = reading each ping by reference instead of making a copy which is more efficient
    for (const SensorPing& ping : pings) {

        // Calculate distance between this ping and the last one
        double distanceKm = 0.0;

        if (track.initialized) {
            distanceKm = haversineDistance(
                track.lastPing.latitude,
                track.lastPing.longitude,
                ping.latitude,
                ping.longitude
            );
            track.totalDistanceKm += distanceKm;
        }

        // Update track state
        track.lastPing = ping;
        track.pingCount += 1;
        track.trackId = ping.trackId;
        track.initialized = true;

        // Print this ping with distance info
        std::cout << std::fixed << std::setprecision(4);
        std::cout << " [" << ping.timestamp << "] "
        << ping.trackId
        << " | LAT: " << ping.latitude
        << " | LON: " << ping.longitude;

        if (distanceKm > 0.0) {
            std::cout << " | Distance: " << distanceKm << " km";
        } else {
            std::cout << " | Distance: -- (first ping)";
        }

        std::cout << std::endl;
    }

    // Summary
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Total pings ingested: " << track.pingCount << std::endl;
    std::cout << "Total distance: " << track.totalDistanceKm << " km" << std::endl;
    std::cout << "==================================" << std::endl;

    return 0;
}