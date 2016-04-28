#include <iostream>
#include <utility>
#include <vector>
#include <string>

typedef std::pair<int, int> task;

int main() {
  int invokersQty, tasksQty;
  std::cin >> tasksQty >> invokersQty;
  std::vector<task> task_list(tasksQty);
  for (int i = 0; i < tasksQty; ++i) {
    std::cin >> task_list[i].first >> task_list[i].second;
  }
  std::vector<std::string> tasks(tasksQty);
  while (true) {
    int foo;
    std::cin >> foo;
    if (foo == -1) break;
    int taskId;
    std::cin >> taskId;
    int pos = tasks[taskId].length() + 1;
    tasks[taskId].append("\nOK");
    bool solved = true;
    // std::cerr << foo << ' ' << taskId << "\n";
    while (true) {
      int time;
      std::cin >> time;
      if (time == -1) break;
      std::string verdict;
      std::cin >> verdict;
      solved &= verdict == "OK";
      tasks[taskId] += "," + std::to_string(time);
    }
    if (!solved) {
      tasks[taskId][pos] = 'W';
      tasks[taskId][pos + 1] = 'A';
    }
  }
  for (auto &csv : tasks) {
    std::cout << csv << '\n';
  }
  return 0;
}
