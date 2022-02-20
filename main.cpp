/* Zadanie o19e3pen - Pensje
 * Autor: Daniel Kobiałka
 */

#include <iostream>
#include <vector>
#include <set>
#include <queue>

struct Employee {
    int id, salary, managerId, childrenCount, unknownDescendantsCount, onlyChild;
    Employee() = default;
    Employee(int i, int s, int m) : id(i), salary(s), managerId(m), childrenCount(0),
        unknownDescendantsCount(0), onlyChild(0) { }
};

/* Find out how many descendants with unknown salary each employee has */
void calculateUnknownDescendants(std::vector<Employee>& employees) {
    for (auto& emp : employees)
        employees[emp.managerId].childrenCount++;
    std::queue<Employee> leavesQueue;
    for (auto& emp : employees)
        if (emp.childrenCount == 0)
            leavesQueue.push(emp);
    while (!leavesQueue.empty()) {
        Employee emp = leavesQueue.front();
        leavesQueue.pop();
        if (emp.salary == 0) {
            employees[emp.managerId].unknownDescendantsCount += emp.unknownDescendantsCount + 1;
            if (employees[emp.managerId].onlyChild == 0)
                employees[emp.managerId].onlyChild = emp.id;
            else
                employees[emp.managerId].onlyChild = -1;

            if (--employees[emp.managerId].childrenCount == 0)
                leavesQueue.push(employees[emp.managerId]);
        }
    }
}

/* Fill table with salaries and corresponding employee. If employee is unknown at the moment, set it to 0 */
void assignKnownSalaries(std::vector<int>& salariesTable, std::vector<Employee>& employees) {
    for (int& sal : salariesTable)
        sal = 0;
    for (const auto& emp : employees)
        if (emp.salary != 0)
            salariesTable[emp.salary] = emp.id;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int employees_num;
    std::cin >> employees_num;

    std::vector<Employee> employees(employees_num + 1);

    for (int i = 1; i <= employees_num; i++) {
        int manager, salary;
        std::cin >> manager >> salary;
        if (i == manager)
            salary = employees_num;
        employees[i] = Employee(i, salary, manager);
    }
    calculateUnknownDescendants(employees);
    std::vector<int> salariesTable(employees_num + 1);
    assignKnownSalaries(salariesTable, employees);

    std::set<int> possibleSalaries;
    int freeSalariesCount = 0;
    for (int i = 1; i < salariesTable.size(); i++) {
        if (salariesTable[i] == 0) {
            freeSalariesCount++;
            possibleSalaries.insert(i);
        } else {
            Employee* employee = &employees[salariesTable[i]];
            freeSalariesCount -= employee->unknownDescendantsCount;
            if (freeSalariesCount == 0) {
                while (!possibleSalaries.empty() && employee->onlyChild > 0) {
                    employee = &employees[employee->onlyChild];
                    /* should assign highest of possible salaries */
                    employee->salary = *possibleSalaries.rbegin();
                    possibleSalaries.erase(employee->salary);
                }
                possibleSalaries.clear();
            }
            /* if employee is not 'leaf', we should reset possible salaries */
            if (employee->unknownDescendantsCount != 0)
                possibleSalaries.clear();
        }
    }

    for (int i = 1; i < employees.size(); i++)
        std::cout << employees[i].salary << "\n";

    return 0;
}
