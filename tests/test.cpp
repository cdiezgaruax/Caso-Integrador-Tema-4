#include "test.h"
#include "../variant/Variant.h"
#include <sstream>
#include <cctype>
#include <algorithm>

void log_test(const std::string& test_name, bool success) {
    if (success) {
        std::cout << "[PRUEBA SUPERADA] " << test_name << std::endl;
    } else {
        std::cout << "[PRUEBA FALLIDA] " << test_name << std::endl;
    }
}

bool is_number(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

int ejecutar_test() {
    try {
        std::string phrase;
        std::cout << "Di una frase y entre [ pon usasa simbolos para symbol numeros para number \"\" para cadena y () para listas: ";
        std::getline(std::cin, phrase);

        std::istringstream iss(phrase);
        std::vector<std::string> words;
        std::string word;
        bool in_quotes = false;
        std::string quoted_word;
        bool in_list = false;
        std::string list_word;

        while (iss >> word) {
            if (word.front() == '"' && word.back() == '"') {
                words.push_back(word);
            } else if (word.front() == '"') {
                in_quotes = true;
                quoted_word = word;
            } else if (word.back() == '"') {
                in_quotes = false;
                quoted_word += " " + word;
                words.push_back(quoted_word);
            } else if (in_quotes) {
                quoted_word += " " + word;
            } else if (word.front() == '(') {
                in_list = true;
                list_word = word;
            } else if (word.back() == ')') {
                in_list = false;
                list_word += " " + word;
                words.push_back(list_word);
            } else if (in_list) {
                list_word += " " + word;
            } else {
                words.push_back(word);
            }
        }

        // Prueba 1: Crear un Variant List con las palabras como simbolos y numeros
        try {
            Variant lst(List);
            for (const auto& w : words) {
                if (is_number(w)) {
                    lst.list.push_back(Variant(Number, w));
                } else if (w.front() == '"' && w.back() == '"') {
                    lst.list.push_back(Variant(Cadena, w.substr(1, w.size() - 2)));
                } else if (w.front() == '(' && w.back() == ')') {
                    std::istringstream list_iss(w.substr(1, w.size() - 2));
                    Variant sublist(List);
                    std::string subword;
                    while (list_iss >> subword) {
                        if (is_number(subword)) {
                            sublist.list.push_back(Variant(Number, subword));
                        } else {
                            sublist.list.push_back(Variant(Symbol, subword));
                        }
                    }
                    lst.list.push_back(sublist);
                } else {
                    lst.list.push_back(Variant(Symbol, w));
                }
            }

            log_test("Crear Variant List con palabras como simbolos y numeros", true);
        } catch (const std::exception& e) {
            log_test("Crear Variant List con palabras como simbolos y numeros", false);
            std::cerr << "Error: " << e.what() << std::endl;
        }

        // Prueba 2: Convertir a string
        try {
            Variant lst(List);
            for (const auto& w : words) {
                if (is_number(w)) {
                    lst.list.push_back(Variant(Number, w));
                } else if (w.front() == '"' && w.back() == '"') {
                    lst.list.push_back(Variant(Cadena, w.substr(1, w.size() - 2)));
                } else if (w.front() == '(' && w.back() == ')') {
                    std::istringstream list_iss(w.substr(1, w.size() - 2));
                    Variant sublist(List);
                    std::string subword;
                    while (list_iss >> subword) {
                        if (is_number(subword)) {
                            sublist.list.push_back(Variant(Number, subword));
                        } else {
                            sublist.list.push_back(Variant(Symbol, subword));
                        }
                    }
                    lst.list.push_back(sublist);
                } else {
                    lst.list.push_back(Variant(Symbol, w));
                }
            }

            std::cout << "to_string: " << lst.to_string() << "\n";
            log_test("Convertir Variant List a string", true);
        } catch (const std::exception& e) {
            log_test("Convertir Variant List a string", false);
            std::cerr << "Error: " << e.what() << std::endl;
        }

        // Prueba 3: Convertir a JSON
        try {
            Variant lst(List);
            for (const auto& w : words) {
                if (is_number(w)) {
                    lst.list.push_back(Variant(Number, w));
                } else if (w.front() == '"' && w.back() == '"') {
                    lst.list.push_back(Variant(Cadena, w.substr(1, w.size() - 2)));
                } else if (w.front() == '(' && w.back() == ')') {
                    std::istringstream list_iss(w.substr(1, w.size() - 2));
                    Variant sublist(List);
                    std::string subword;
                    while (list_iss >> subword) {
                        if (is_number(subword)) {
                            sublist.list.push_back(Variant(Number, subword));
                        } else {
                            sublist.list.push_back(Variant(Symbol, subword));
                        }
                    }
                    lst.list.push_back(sublist);
                } else {
                    lst.list.push_back(Variant(Symbol, w));
                }
            }

            std::string json_str = lst.to_json_string();
            std::cout << "to_json_string: " << json_str << "\n";
            log_test("Convertir Variant List a JSON", true);
        } catch (const std::exception& e) {
            log_test("Convertir Variant List a JSON", false);
            std::cerr << "Error: " << e.what() << std::endl;
        }

        // Prueba 4: Reconstruir desde JSON
        try {
            Variant lst(List);
            for (const auto& w : words) {
                if (is_number(w)) {
                    lst.list.push_back(Variant(Number, w));
                } else if (w.front() == '"' && w.back() == '"') {
                    lst.list.push_back(Variant(Cadena, w.substr(1, w.size() - 2)));
                } else if (w.front() == '(' && w.back() == ')') {
                    std::istringstream list_iss(w.substr(1, w.size() - 2));
                    Variant sublist(List);
                    std::string subword;
                    while (list_iss >> subword) {
                        if (is_number(subword)) {
                            sublist.list.push_back(Variant(Number, subword));
                        } else {
                            sublist.list.push_back(Variant(Symbol, subword));
                        }
                    }
                    lst.list.push_back(sublist);
                } else {
                    lst.list.push_back(Variant(Symbol, w));
                }
            }

            std::string json_str = lst.to_json_string();
            Variant reconstructed = Variant::from_json_string(json_str);

            std::cout << "Reconstruido desde JSON: " << reconstructed.to_string() << "\n";
            log_test("Reconstruir Variant List desde JSON", true);

            // Mostrar el resultado final
            std::cout << "Resultado final: " << reconstructed.to_string() << "\n";
        } catch (const std::exception& e) {
            log_test("Reconstruir Variant List desde JSON", false);
            std::cerr << "Error: " << e.what() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error fatal en el programa: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}