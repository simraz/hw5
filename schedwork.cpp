#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

bool scheduleHelper(size_t day, size_t workerSlot, const AvailabilityMatrix& avail,
                const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, std::vector<size_t>& shiftsWorked)
{
    size_t nDays = avail.size();
    size_t kWorkers = avail[0].size();

    // Base case: All days scheduled
    if (day == nDays) {
        return true;
    }

    // Determine next position to fill
    size_t nextDay = (workerSlot + 1 == dailyNeed) ? day + 1 : day;
    size_t nextWorkerSlot = (workerSlot + 1) % dailyNeed;

    // Iterate through all possible workers for the current slot
    for (Worker_T workerId = 0; workerId < kWorkers; workerId++) {
        // Is worker available
        if (!avail[day][workerId]) {
            continue;
        }

        // Is worker already scheduled for today
        auto endFilledIt = sched[day].begin() + workerSlot;
        auto it = std::find(sched[day].begin(), endFilledIt, workerId);
        if (it != endFilledIt) {
            continue;
        }            

        // Has worker reached max shifts
        if (shiftsWorked[workerId] >= maxShifts) {
            continue;
        }

        // Assign worker to the current slot
        sched[day][workerSlot] = workerId;
        shiftsWorked[workerId]++;

        // Do recursion to fill in the next slot
        if (scheduleHelper(nextDay, nextWorkerSlot, avail, dailyNeed, maxShifts, sched, shiftsWorked)) {
            // return if successfully scheduled
            return true;
        }

        // Do back tracking if not successful
        shiftsWorked[workerId]--;
    }

    // No worker could be assigned to this slot
    return false;
}

bool schedule(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched)
{
    size_t nDays = avail.size();
    size_t kWorkers = avail[0].size();

    // Initialize schedule with empty values
    sched = std::vector<std::vector<Worker_T>>(nDays, std::vector<Worker_T>(dailyNeed));

    // Keep track of shifts worked by each worker
    std::vector<size_t> shiftsWorked(kWorkers, 0);

    return scheduleHelper(0, 0, avail, dailyNeed, maxShifts, sched, shiftsWorked);
}

