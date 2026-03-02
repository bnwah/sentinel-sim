#include <iostream>
#include <string>

/*
STRUCT 1: SensorPing
Represents one raw GPS report from a tracked asset.
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
*/

struct TrackState
{
    std::string trackId;
    SensorPing lastPing;
    double totalDistanceKm = 0.0;
    int pingCount = 0;
    bool initialized = false;
};

int main(int argc, char *argv[])
{
    std::cout << "=== Sentinel-Sim: Sensor Fusion Simulator ===" << std::endl;

    // Test: create a SensorPing manually and print it
    SensorPing testPing;
    testPing.timestamp = "12:00:01";
    testPing.latitude = 38.8977;
    testPing.longitude = -77.0365;
    testPing.trackId = "DRONE-01";

    std::cout << "Test ping loaded: " << testPing.trackId
              << " at " << testPing.timestamp
              << " [" << testPing.latitude
              << ", " << testPing.longitude << "]"
              << std::endl;

    // Test: create a TrackState and print it
    TrackState testTrack;
    testTrack.trackId = "DRONE-01";
    testTrack.pingCount = 1;
    testTrack.initialized = true;

    std::cout << "Track initialized: " << testTrack.trackId
              << " | Pings received: " << testTrack.pingCount
              << std::endl;

    return 0;
}