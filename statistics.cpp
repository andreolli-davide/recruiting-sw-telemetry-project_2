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

void Statistics::report(char *id, time_t timestamp) {

    for (int i = 0; i < count; i++) {
        // Check if id already exists
        if(strncmp(id, stats[i].id, 3) == 0) {
            stats[i].lastReport = timestamp;
            stats[i].count++;
            return;
        }
    }
    // if id has never been inserted before
    stpncpy((char*)(stats[count].id), id, 3);
    stats[count].firstReport = timestamp;
    stats[count].lastReport = timestamp;
    stats[count].count = 1;
    count++;
}

Statistics::~Statistics() {
    if (file.is_open()) file.close();
}

void Statistics::build() {
    file.open(filename);
    
    // Write csv headers
    file << "id,number_of_messages,mean_time" << endl;

    for (int i = 0; i < count; i++) {
        file << stats[i].id << "," << stats[i].count << "," << ((float)(stats[i].lastReport - stats[i].firstReport)) / count << endl;
    }

    file.close();
}

void Statistics::init(time_t timestamp) {
    // Create filename
    sprintf(filename, "./%ld_STATISTICS.csv", timestamp);
    count = 0;
}