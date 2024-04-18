#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool validPlacement(
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t n_days,
    size_t worker,
    vector<size_t>& days_booked);

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t n_days,
    size_t d_workers,
    vector<size_t>& days_booked);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // resize sched to be a vector of n vectors
    sched.resize(avail.size());

    // create a vector to store how many shifts each worker has been scheduled
    int k = avail[0].size();
    vector<size_t> days_booked(k, 0);

    return schedule(avail, dailyNeed, maxShifts, sched, 0, 0, days_booked);
}

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t n_days,
    size_t d_workers,
    vector<size_t>& days_booked)
{
  // check if valid schedule
  if(n_days == avail.size()) {
    return true;
  }
  
  //cout << "day " << n_days << ", shift " << d_workers << "/" << maxShifts << endl;

  for(size_t i = 0; i < days_booked.size(); i++) {
    if(avail[n_days][i] == 1) {
      //cout << "incrementing days_booked for worker " << i << endl;
      days_booked[i]++;
      sched[n_days].push_back(i);
      d_workers++;
      //cout << "did it" << endl;

      if(validPlacement(dailyNeed, maxShifts, sched, n_days, i, days_booked)) {
        //cout << "valid placement" << endl;
        //cout << "booked worker " << i << " for shift " << d_workers-1 << "/" << maxShifts;
        //cout << " on day " << n_days << endl;
        bool status;

        if(d_workers == dailyNeed) {
          status = schedule(avail, dailyNeed, maxShifts, sched, n_days+1, 0, days_booked);
        }
        else {
          status = schedule(avail, dailyNeed, maxShifts, sched, n_days, d_workers, days_booked);
        }

        if(status) return true;
      }

      d_workers--;
      sched[n_days].pop_back();
      days_booked[i]--;
    }
  }

  return false;
}

bool validPlacement(
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t n_days,
    size_t worker,
    vector<size_t>& days_booked)
{
  //cout << "called validPlacement()" << endl;
  if(days_booked[worker] > maxShifts) {
    return false;
  }
  //cout << "passed check 1" << endl;
  if(sched[n_days].size() > dailyNeed) {
    return false;
  }
  //cout << "passed check 2" << endl;
  for(size_t i = 0; i < sched[n_days].size()-1; i++) {
    if(sched[n_days][i] == worker) {
      return false;
    }
  }
  //cout << "passed check 3" << endl;
  return true;
}