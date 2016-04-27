#include <bits/stdc++.h>

using namespace std;

long long rdtsc() {
    long long tmp;
    asm("rdtsc" : "=A"(tmp));
    return tmp;
}

const int MAXP = 10000;
const int MAXT = 1000;
const int MAXS = 100000;

int T; // кол-во инвокеров
int t; // кол-во свободных инвокеров
int p; // кол-во задач

int tl[MAXP]; // TL задач
int tests[MAXP]; // кол-во тестов задач
int aver[MAXP]; // среднее время тестирования задачи

int s; // количество присланных решений
int curS; // количество непротестированных решений

int solP[MAXS]; // задача, по которой пришло решение
int testN[MAXS]; // номер следующего теста, на котором надо запустить решение
int testT[MAXS]; // время тестирование задачи на тестах [0; testN)
int tested[MAXS]; // кол-во завершённых тестов по решению
int start[MAXS]; // время, когда запустилось тестирование на текущем тесте
int failT[MAXS]; // номер теста, на котором повалилось решение
set<int> sols; // множество ещё не протестированных решений

int curT; // текущее время
int open = 1; // входной поток

void readSols() {
    while (1) {
        int curP;

        if (!(cin >> curP))
            open = 0;

        if (!open || curP == -1)
            break;

        solP[s] = curP;
        sols.insert(s++);
    }
}

void readVerds() {
    while (open) {
        int solId, testId;
        cin >> solId >> testId;

        if (solId == -1)
            break;

        t++;

        string verd;
        cin >> verd;

        if (verd[0] == 'R' && sols.find(solId) != sols.end())
            sols.erase(solId);
    }
}

void writeTests() {
    if (!open)
        return;
    while (t > 0 && sols.size()) {
        int i = rand() % sols.size(), j = 0;
        for (int solId : sols)
            if (j == i) {
                cout << solId << ' ' << testN[solId]++ << '\n';
                if (tests[solP[solId]] == testN[solId])
                    sols.erase(solId);
                t--;
                break;
            } else j++;
    }
    cout << "-1 -1\n";
    cout << flush;
}

void tick() {
    readSols();
    readVerds();
    writeTests();
}

void solve() {
    cin >> t >> p;
    T = t;
    for (int i = 0; i < p; i++)
        cin >> tl[i] >> tests[i];

    for ( ; open; curT += 10)
        tick();
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    srand(rdtsc());
    solve();
    return 0;
}
