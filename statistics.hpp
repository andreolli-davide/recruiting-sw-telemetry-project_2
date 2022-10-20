#pragma once

#include <ctime>
#include <fstream>
#include <unordered_map>

#define MAX_STATISTICS_ARRAY_SIZE 1000

using namespace std;

typedef struct Stats {
    char id[3];
    int count;
    time_t firstReport;
    time_t lastReport;
} IdStats;

class Statistics {

    private:
        ofstream file;

        /**
         *  filename will be the timestamp of the first message + "_STATISTICS_csv".
         */
        char filename[28];
        uint count;
        unordered_map<uint16_t, Stats> stats;
    public:
        Statistics();
        ~Statistics();

        /**
         *  Init prepare the statistics object for store data, when 0A0 id appear.
         */
        void init(time_t timestamp);

        /**
         *  report add to statistics the message id.
         */
        void report(uint16_t id, char *_id, time_t timestamp);

        /**
         *  build creates the csv file with stored statistics.
         */ 
        void build();
};