#ifndef DIAGNOSISSERVICE_H
#define DIAGNOSISSERVICE_H

#include <string>
#include "core/Kiosk.h"

class DiagnosisReport {
public:
    std::string fullReport;
};

class DiagnosisService {
public:
    DiagnosisReport runFullCheck(Kiosk* kiosk);
};

#endif

