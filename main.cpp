#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <iostream>
using namespace std;

class Medicine {
public:
    double cost;
    int expiration;

    Medicine(double cost, int expiration) : cost(cost), expiration(expiration) {}
};

tuple<vector<Medicine>, vector<vector<pair<int, int>>>, vector<pair<int, int>>>
read_input_files(const string &medicine_file, const string &strategy_file, const string &discard_file) {
    vector<Medicine> medicines;
    vector<vector<pair<int, int>>> strategies;
    vector<pair<int, int>> discards;

    ifstream in;
    string line;

    // 读取药品文件
    in.open(medicine_file);
    while (getline(in, line)) {
        stringstream ss(line);
        double cost;
        int expiration;
        ss >> cost >> expiration;
        medicines.emplace_back(cost, expiration);
    }
    in.close();

    // 读取策略文件
    in.open(strategy_file);
    while (getline(in, line)) {
        stringstream ss(line);
        vector<pair<int, int>> strategy;
        string item;
        while (getline(ss, item, ' ')) {
            stringstream item_ss(item);
            int medicine_index, price_index;
            char delimiter;
            item_ss >> medicine_index >> delimiter >> price_index;
            strategy.emplace_back(medicine_index, price_index);
        }
        strategies.push_back(strategy);
    }
    in.close();

    // 读取丢弃药品文件
    in.open(discard_file);
    while (getline(in, line)) {
        stringstream ss(line);
        int day, medicine_index;
        ss >> day >> medicine_index;
        discards.emplace_back(day, medicine_index);
    }
    in.close();

    return make_tuple(medicines, strategies, discards);
}
double simulate_sales(const vector<Medicine>& medicines, const vector<vector<pair<int, int>>>& strategies, const vector<pair<int, int>>& discards) {
    double profit = 0;
    vector<int> remaining_days(medicines.size());
    for (size_t i = 0; i < medicines.size(); ++i) {
        remaining_days[i] = medicines[i].expiration;
    }

    for (size_t day = 0; day < strategies.size(); ++day) {
        // 处理丢弃药品
        for (const auto& discard : discards) {
            if (discard.first == static_cast<int>(day)) {
                remaining_days[discard.second] = 0;
            }
        }

        // 处理每天的销售策略
        for (const auto& display_medicine : strategies[day]) {
            if (display_medicine.first >= 0 && remaining_days[display_medicine.first] > 0) {
                profit += display_medicine.second - medicines[display_medicine.first].cost;
                remaining_days[display_medicine.first]--;
            }
        }
    }

    return profit;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <medicine_file> <strategy_file> <delete_file> <output_file>" << endl;
        return 1;
    }

    string medicine_file = argv[1];
    string strategy_file = argv[2];
    string discard_file = argv[3];
    string output_file = argv[4];

    vector<Medicine> medicines;
    vector<vector<pair<int, int>>> strategies;
    vector<pair<int, int>> discards;

    tie(medicines, strategies, discards) = read_input_files(medicine_file, strategy_file, discard_file);
    double profit = simulate_sales(medicines, strategies, discards);

    ofstream out(output_file);
    out << "Profit: " << profit << endl;
    out.close();

    cout << "Profit: " << profit << endl;
    return 0;
}
