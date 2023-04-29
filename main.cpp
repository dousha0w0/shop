#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <array>

using namespace std;

struct Medicine {
    int id;
    double price;
    int expiry_days;
};

tuple<vector<Medicine>, vector<array<pair<int, int>, 3>>, vector<pair<int, int>>>
read_input_files(const string &medicine_file, const string &strategy_file, const string &discard_file) {
    // 读取medicine.txt
    ifstream m_file(medicine_file);
    vector<Medicine> medicines;
    double price;
    int expiry_days;
    int id = 0;
    while (m_file >> price >> expiry_days) {
        medicines.push_back({id++, price, expiry_days});
    }
    m_file.close();

    // 读取strategy.txt
    ifstream s_file(strategy_file);
    vector<array<pair<int, int>, 3>> strategies(10);
    int day = 0;
    string line;
    while (getline(s_file, line)) {
        stringstream ss(line);
        int id1, index;
        for (int i = 0; i < 3; ++i) {
            ss >> id1 >> index;
            strategies[day][i] = {id1, index};
        }
        ++day;
    }
    s_file.close();

    // 读取delete.txt
    ifstream d_file(discard_file);
    vector<pair<int, int>> discards;
    int discard_day, discard_id;
    while (d_file >> discard_day >> discard_id) {
        discards.emplace_back(discard_day, discard_id);
    }
    d_file.close();

    return {medicines, strategies, discards};
}

double simulate_sales(const vector<Medicine> &medicines, const vector<array<pair<int, int>, 3>> &strategies,
                      const vector<pair<int, int>> &discards) {
    const vector<double> price_offsets = {-1.5, -1, -0.5, 0, 2, 4, 6};
    double total_profit = 0.0;
    vector<Medicine> remaining_medicines = medicines;

    for (int day = 0; day < 10; ++day) {
        // 应用策略
        vector<Medicine> displayed_medicines;
        for (const auto &[id, index]: strategies[day]) {
            if (id != -1) {
                Medicine m = remaining_medicines[id];
                m.price += price_offsets[index];
                displayed_medicines.push_back(m);
            }
        }

        // 顾客购买
        sort(displayed_medicines.begin(), displayed_medicines.end(), [](const Medicine &a, const Medicine &b) {
            if (a.price == b.price) {
                return a.expiry_days > b.expiry_days;
            }
            return a.price < b.price;
        });

        int customers = 3;
        for (const Medicine &m: displayed_medicines) {
            if (customers > 0) {
                total_profit += m.price - m.price;
                remaining_medicines[m.id].expiry_days = -1;
                --customers;
            }
        }

        // 管理费用
        for (Medicine &m: remaining_medicines) {
            if (m.expiry_days > 0) {

                if (m.expiry_days < 5) {
                    total_profit -= 1;
                } else {
                    {
                        total_profit -= 0.5;
                    }
                    m.expiry_days -= 1;
                }
            }

            // 丢弃过期药品
            for (const auto &[discard_day, discard_id]: discards) {
                if (discard_day == day) {
                    Medicine &m = remaining_medicines[discard_id];
                    if (m.expiry_days == 0) {
                        total_profit -= m.price;
                        m.expiry_days = -1;
                    }
                }
            }
        }

        return total_profit;
    }
}

int main(int argc, char *argv[]) {
    string medicine_file, strategy_file, discard_file;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-m") {
            medicine_file = argv[++i];
        } else if (arg == "-s") {
            strategy_file = argv[++i];
        } else if (arg == "-d") {
            discard_file = argv[++i];
        }
    }
    auto input_data = read_input_files(medicine_file, strategy_file, discard_file);
    vector<Medicine> medicines = get<0>(input_data);
    vector<array<pair<int, int>, 3>> strategies = get<1>(input_data);
    vector<pair<int, int>> discards = get<2>(input_data);

    double profit = simulate_sales(medicines, strategies, discards);
    cout << "Total profit: " << fixed << setprecision(2) << profit << endl;

    return 0;
}