#include <bits/stdc++.h>

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
  size_t tests;
  long long timeLimit;
  // int averageTestingTime;
  // TODO: replace with Fenwick tree on average testing time
  // (f.get(i) -- remaining average time for tests from ith)
  // ... or smth better ...

  Task(size_t tests, long long timeLimit): tests(tests), timeLimit(timeLimit) {
  }
};

vector<Task> tasks;

struct Solution {
  size_t taskId;
  size_t nextTest;
  size_t tested;
  long long arrivedAt;
  long long testingTime;

  Solution(size_t taskId, long long arrivedAt):
      taskId(taskId), nextTest(0), tested(0), arrivedAt(arrivedAt),
      testingTime(0) {
  }
};

vector<Solution> solutions;

struct SolutionCmp {
  bool operator()(const size_t lhs, const size_t rhs) const {
    return estimate(solutions[lhs]) < estimate(solutions[rhs]);
  }

  // sort by currentTime - arrivedAt + estimatedTime <=> sort by arrivedAt - estimatedTime
  // TODO: improve
  long long estimate(const Solution &solution) const {
    size_t remainingTests = tasks[solution.taskId].tests - solution.nextTest;
    return solution.arrivedAt - (solution.tested ? solution.testingTime * remainingTests / solution.tested : 0);
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

        size_t solutionId = solutions.size();
        solutions.emplace_back(solutionId, currentTime);
        scheduledQueue.insert(solutionId);
    }
}

void readVerdicts() {
    while (open) {
        int solId, testId;
        cin >> solId >> testId;
        if (solId == -1)
            break;

        auto it = startTestingAt.find(make_pair(solId, testId));
        long long startTime = it->second;
        startTestingAt.erase(it);

        bool was = scheduledQueue.erase(solId);
        freeInvokers++;

        string verd;
        cin >> verd;

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
      auto id = *it;
      scheduledQueue.erase(it);
      size_t testId = solutions[id].nextTest;
      ++solutions[id].nextTest;
      --freeInvokers;
      startTestingAt.emplace(make_pair(id, testId), currentTime);
      cout << id << ' ' << testId << '\n';
      if (solutions[id].nextTest < tasks[solutions[id].taskId].tests) {
        std::cerr << id << ": " << solutions[id].nextTest << ' ' << solutions[id].taskId << ' ' << tasks[solutions[id].taskId].tests << '\n';
        scheduledQueue.insert(id);
      }
    }
    cout << "-1 -1\n";
    cout << flush;
}

void tick() {
    readSolutions();
    readVerdicts();
    writeTests();
}

void solve() {
    cin >> invokers >> taskQty;
    freeInvokers = invokers;
    tasks.reserve(taskQty);
    for (int i = 0; i < taskQty; i++) {
      int tl, testQty;
      cin >> tl >> testQty;
      tasks.emplace_back(testQty, tl);
    }

    for ( ; open; currentTime += 10)
        tick();
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    srand(rdtsc());
    solve();
    return 0;
}
