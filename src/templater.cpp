#include <iostream>
#include <tuple>
#include <map>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

using namespace std;
using json = nlohmann::json;

tuple<string, string> get_arg_pair(string arg) {
    string delimeter = "=";
    size_t pos = arg.find(delimeter);
    string key = arg.substr(2, pos - 2);
    arg.erase(0, pos + delimeter.length());

    return tuple<string, string>(key, arg);
}

bool replace(string &str, const string &from, const string &to) {
    size_t start_pos = str.find(from);
    if (start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

int main(int argc, char **argv) {
    static const string VARIABLES = "variables";
    static const string OUTPUT = "output";
    static const string INPUT = "input";

    map<string, string> arguments = {};

    for (int i = 1; i < argc; i++) {
        tuple<string, string> pair = get_arg_pair(argv[i]);
        arguments[get<0>(pair)] = get<1>(pair);
    }


    if (arguments.find(VARIABLES) == arguments.end()) {
        cout << "There is no variables object. Abort" << endl;
        return 0;
    } else if (arguments.find(OUTPUT) == arguments.end()) {
        cout << "There is no output path. Abort" << endl;
        return 0;
    } else if (arguments.find(INPUT) == arguments.end()) {
        cout << "There is no input path. Abort" << endl;
        return 0;
    }

    auto json_input = json::parse(arguments.at(VARIABLES));

    ifstream input(arguments.at(INPUT));
    stringstream buffer;
    buffer << input.rdbuf();

    string result = buffer.str();

    for (auto &element : json_input.items()) {
        string key = "${" + element.key() + "}";
        bool success = replace(result, key, element.value());

        while (success) {
            success = replace(result, key, element.value());
        }
    }

    ofstream output;
    output.open(arguments[OUTPUT]);
    output << result;
    output.close();

    return 0;
}