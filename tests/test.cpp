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
        std::cout << "Di una frase: ";
        std::getline(std::cin, phrase);

        std::istringstream iss(phrase);
        std::vector<std::string> words;
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }

        // Prueba 1: Crear un Variant List con las palabras como simbolos y numeros
        try {
            Variant lst(List);
            for (const auto& w : words) {
                if (is_number(w)) {
                    lst.list.push_back(Variant(Number, w));
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
                } else {
                    lst.list.push_back(Variant(Symbol, w));
                }
            }

            std::string json_str = lst.to_json_string();
            Variant reconstructed = Variant::from_json_string(json_str);

            std::cout << "Reconstruido desde JSON: " << reconstructed.to_string() << "\n";
            log_test("Reconstruir Variant List desde JSON", true);
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