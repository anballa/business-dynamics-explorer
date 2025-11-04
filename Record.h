//
// Created by anany on 11/3/2025.
//

#ifndef RECORD_H
#define RECORD_H

#include <string>
using namespace std;

struct Record {
    string state;
    int year;
    int dhsDenominator;
    int numberOfFirms;
    int netJobCreation;
    float netJobCreationRate;
    float reallocationRate;
    int establishmentsEntered;
    float enteredRate;
    int establishmentsExited;
    float exitedRate;
    int physicalLocations;
    int firmExits;
    int jobCreation;
    float jobCreationRate;
    int jobDestruction;
    float jobDestructionRate;

    Record()
        : state(""), year(0), dhsDenominator(0), numberOfFirms(0),
          netJobCreation(0), netJobCreationRate(0.0f), reallocationRate(0.0f),
          establishmentsEntered(0), enteredRate(0.0f),
          establishmentsExited(0), exitedRate(0.0f),
          physicalLocations(0), firmExits(0), jobCreation(0),
          jobCreationRate(0.0f), jobDestruction(0),
          jobDestructionRate(0.0f) {}

    Record(string s, int y, int dhs, int firms, int netJob, float jobRate)
        : state(s), year(y), dhsDenominator(dhs), numberOfFirms(firms),
          netJobCreation(netJob), netJobCreationRate(jobRate),
          reallocationRate(0.0f), establishmentsEntered(0), enteredRate(0.0f),
          establishmentsExited(0), exitedRate(0.0f), physicalLocations(0),
          firmExits(0), jobCreation(0), jobCreationRate(0.0f),
          jobDestruction(0), jobDestructionRate(0.0f) {}
};


#endif
