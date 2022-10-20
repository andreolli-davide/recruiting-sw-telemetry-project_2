#include "statistics.hpp"

#include <ctime>
#include <iostream>

using namespace std;

// If env variable DEBUG is setted (any value), logs are printed on the console.
#ifdef DEBUG
    void log(int count, char* id) {  
        cout << "Statistics -> count: " << count << ", id: " << id << endl;
    }
#else
    void log(int count, char* id) {}
#endif


Statistics::Statistics() {
    uint count = 0;
}

void Statistics::report(uint16_t id, char *_id, time_t timestamp) {

    if (stats.contains(id)) {
        stats[id].lastReport = timestamp;
        stats[id].count++;
        return;
    }

    Stats idStats;
    idStats.firstReport = timestamp;
    idStats.lastReport = timestamp;
    idStats.count = 1;
    stats.insert({id, idStats});
}

Statistics::~Statistics() {
    if (file.is_open()) file.close();
}

void Statistics::build() {
    file.open(filename);
    
    // Write csv headers
    file << "id,number_of_messages,mean_time" << endl;

    char idString[3];

    for (auto id = stats.begin(); id != stats.end(); ++id) {
        sprintf(idString, "%02X", id->first);
        int a = id->first;
        char *b = id->second.id;
        file << idString << "," << id->second.count << "," << ((float)(id->second.lastReport - id->second.firstReport)) / id->second.count << endl;
    }

    file.close();
}

void Statistics::init(time_t timestamp) {
    // Create filename
    sprintf(filename, "./%ld_STATISTICS.csv", timestamp);
    count = 0;
}