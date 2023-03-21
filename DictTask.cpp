#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <cmath>

std::vector < std::string > split_string(std::string row) {
    std::stringstream ss(row);
    std::string s;
    std::vector<std::string> res;

    while (getline(ss, s, ' ')) {
        if(s.size())
            res.push_back(s);
    }
    return res;
}

bool check_string(std::string dict_word, std::string word) {
    if (abs(int(dict_word.size() - word.size())) > 1 or dict_word.size() == 1)
        return 0;

    int length = dict_word.size(), count = 0;
    bool flag = 0;
    for (int i = 0; i < length; ++i) {
        flag = 0;
        if (count > 1)
            return 0;
        if (dict_word[i] != word[i]) {
            flag = 0;
            if (dict_word.size() == word.size()) {
                word[i] = dict_word[i];
                count++;
                flag = 1;
            }
            if (dict_word.size() > word.size()) {
                word.insert(i, &dict_word[i], 1);
                count++;
                flag = 1;
            }
            if (dict_word.size() < word.size()) {
                word.erase(i, 1);
                count++;
                flag = 1;
            }
            if (!flag)
                count++;
        }
    }
    if (count > 1)
        return 0;
    if (word != dict_word)
        return 0;
    return true;
}

int main() {
    
    std::ifstream text_file("text.txt");
    std::ifstream dict_file("dict.txt");
    std::ofstream new_text_file("new_text.txt");
    std::string row_string, new_string;
    std::vector<std::string> words;
    std::set<std::string> dictionary;
    std::string user_command;
    std::string synt_char;
    int check_index;


    while (getline(dict_file, row_string, '\n')) {
        dictionary.insert(row_string);
    }

    dict_file.close();
    std::ofstream dict_file_out;
    dict_file_out.open("dict.txt", std::ofstream::app);

    while (getline(text_file, row_string, '\n')) {
        words = split_string(row_string);
        for (std::string word : words) {
            check_index = word.find_first_of(".,!?:;");
            synt_char = "";
            if (check_index != -1) {
                synt_char = word.back();
                word.pop_back();
            }
            
            if (dictionary.find(word) != dictionary.end()) {
                new_string += word + synt_char + ' ';
                continue;
            }
            
            user_command = "N";
            for (auto& dict_word : dictionary) {
                if (dict_word == word) {
                    user_command = "Y";
                    break;
                }
                if (check_string(dict_word, word)) {
                    std::cout << "Replace *" + word + "* with *" + dict_word + "*? [Y/N]\n>";
                    std::cin >> user_command;
                    if (user_command == "Y") {
                        new_string += dict_word + synt_char + ' ';
                        break;
                    }
                }
            }
            if (user_command == "N") {
                std::cout << "Add *" + word + "* to the dict? [Y/N]\n>";
                std::cin >> user_command;
                if (user_command == "Y") {
                    dictionary.insert(word);
                    dict_file_out << word << '\n';
                }
                new_string += word + synt_char + ' ';
            }
        }
        new_text_file << new_string << '\n';
        new_string = "";
    }

    text_file.close();
    new_text_file.close();
    dict_file_out.close();
    
    return 0;
}