#include <bits/stdc++.h>
#include <cassert>

using namespace std;

long long rdtsc() {
    long long tmp;
    asm("rdtsc" : "=A"(tmp));
    return tmp;
}

int invokers; // кол-во инвокеров
int freeInvokers; // кол-во свободных инвокеров
int taskQty; // кол-во задач

int currentTime; // текущее время
int open = 1; // входной поток

struct Task {
  long long timeLimit;
  size_t tests;
  // int averageTestingTime;
  // TODO: replace with Fenwick tree on average testing time
  // (f.get(i) -- remaining average time for tests from ith)
  // ... or smth better ...

  Task(long long timeLimit, size_t tests): timeLimit(timeLimit), tests(tests) {
  }
};

vector<Task> tasks;

struct Solution {
  size_t taskId;
  size_t nextTest;
  size_t tested;
  size_t firstFailed;
  long long arrivedAt;
  long long testingTime;

  Solution(size_t taskId, long long arrivedAt):
      taskId(taskId), nextTest(0), tested(0), firstFailed(numeric_limits<size_t>::max()),
      arrivedAt(arrivedAt), testingTime(0) {
  }
};

vector<Solution> solutions;

struct SolutionCmp {
  bool operator()(const size_t lhs, const size_t rhs) const {
    auto lhsV = estimate(solutions[lhs]);
    auto rhsV = estimate(solutions[rhs]);
    return lhsV == rhsV ? lhs < rhs : lhsV < rhsV;
  }

  // sort by currentTime - arrivedAt + estimatedTime <=> sort by arrivedAt - estimatedTime
  // TODO: improve
  long long estimate(const Solution &solution) const {
    auto remainingTests = tasks[solution.taskId].tests - solution.nextTest;
    return solution.arrivedAt - (solution.tested ? (solution.testingTime * remainingTests + solution.tested / 2) / solution.tested : remainingTests);
  }
};

set<size_t, SolutionCmp> scheduledQueue;
map<pair<size_t, size_t>, long long> startTestingAt;

void readSolutions() {
    while (open) {
        int curP;
        if (!(cin >> curP))
            open = 0;
        if (!open || curP == -1)
            break;

        size_t solId = solutions.size();
        // cerr << "RS [" << solId << "]: " << curP << '\n';
        solutions.emplace_back(size_t(curP), currentTime);
        scheduledQueue.insert(solId);
    }
}

void readVerdicts() {
    while (open) {
        int solId, testId;
        cin >> solId >> testId;
        if (solId == -1)
            break;

        freeInvokers++;

        auto it = startTestingAt.find(make_pair(size_t(solId), size_t(testId)));
        long long startTime = it->second;
        startTestingAt.erase(it);

        auto it0 = scheduledQueue.find(solId);
        bool was = false;
        if (it0 != scheduledQueue.end()) {
          if (int(*it0) != solId) {
            cerr << "WAT?oO " << *it0 << " != " << solId << '\n';
            assert(false);
          }
          if (solutions[solId].nextTest >= tasks[solutions[solId].taskId].tests) {
            cerr << "failRV [" << solId << "]: " << solutions[solId].nextTest << ' ' << tasks[solutions[solId].taskId].tests << '\n';
            assert(false);
          }
          was = true;
          scheduledQueue.erase(it0);
        }

        string verd;
        cin >> verd;
        // cerr << "verdict: " << solId << ' ' << testId << ' ' << verd << '\n';

        if (verd[0] == 'O') {
          solutions[solId].tested++;
          solutions[solId].testingTime += currentTime - startTime;
          // TODO: update task fields
          if (was) scheduledQueue.insert(solId);
        } // else solution is already erased from scheduledQueue
    }
}

void writeTests() {
    if (!open)
        return;
    while (freeInvokers > 0 && scheduledQueue.size()) {
      auto it = scheduledQueue.begin();
      auto solId = *it;
      scheduledQueue.erase(it);
      size_t testId = solutions[solId].nextTest;
      ++solutions[solId].nextTest;
      --freeInvokers;
      startTestingAt.emplace(make_pair(solId, testId), currentTime);
      cout << solId << ' ' << testId << '\n';
      // cerr << "submit: " << solId << ' ' << testId << '\n';
      if (solutions[solId].nextTest < tasks[solutions[solId].taskId].tests) {
        // std::cerr << "add to queue [" << solId << "]: " << solutions[solId].nextTest << ' ' << solutions[solId].taskId << ' ' << tasks[solutions[solId].taskId].tests << '\n';
        scheduledQueue.insert(solId);
      }
    }
    cout << "-1 -1\n";
    cout << flush;
}

void tick() {
    // cerr << "--->" << currentTime << "<---\n";
    // cerr << "in queue " << scheduledQueue.size() << " elements:";
    // for (auto &i : scheduledQueue) cerr << ' ' << i; cerr << '\n';
    readSolutions();
    readVerdicts();
    writeTests();
}

void solve() {
    cin >> invokers >> taskQty;
    freeInvokers = invokers;
    tasks.reserve(taskQty);
    for (int i = 0; i < taskQty; i++) {
      int tl, tc;
      cin >> tl >> tc;
      tasks.emplace_back(tl, tc);
    }

    for ( ; open; currentTime += 10)
        tick();
}


int main() {
    // freopen("log", "w", stderr);
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    srand(rdtsc());
    solve();
    return 0;
}
