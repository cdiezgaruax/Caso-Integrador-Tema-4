#include "Variant.h"
#include <iostream>
#include <stdexcept>
#include "json11.hpp"

// Constructor definitions
Variant::Variant(variant_type type) : type(type), env(nullptr), proc(nullptr) {}

Variant::Variant(variant_type type, const std::string& val) : type(type), val(val), env(nullptr), proc(nullptr) {}

Variant::Variant(proc_type proc) : type(Proc), proc(proc), env(nullptr) {}

// Implementación de los métodos
std::string Variant::to_string() {
    switch (type) {
        case Symbol:
            return val;
        case Number:
            return val; // Asumiendo que val es el número en formato string
        case Cadena:
            return "\"" + val + "\"";
        case List: {
            if (list.empty()) return "()"; // Manejo de listas vacías
            std::string r = "(";
            for (size_t i = 0; i < list.size(); i++) {
                r += list[i].to_string();
                if (i < list.size() - 1) r += " ";
            }
            r += ")";
            return r;
        }
        case Proc:
            return "<proc>";
        case Lambda:
            return "<lambda>";
    }
    return "<unknown>";
}

std::string Variant::to_json_string() {
    using namespace json11;

    Json::object obj;
    obj["type"] = variant_type_to_string(type);

    switch (type) {
        case Symbol:
        case Number:
        case Cadena:
            obj["value"] = val;
            break;
        case List: {
            std::vector<Json> jlist;
            for (auto &elem : list) {
                std::string elem_json = elem.to_json_string();
                std::string err;
                Json subjob = Json::parse(elem_json, err);
                if (!err.empty()) {
                    throw std::runtime_error("Error al parsear elemento de la lista a JSON: " + err);
                }
                jlist.push_back(subjob);
            }
            obj["list"] = jlist;
            break;
        }
        case Proc:
        case Lambda:
            // No guardamos proc ni env, solo el tipo
            break;
    }

    Json j(obj);
    return j.dump();
}

Variant Variant::from_json_string(std::string sjson) {
    std::string err;
    json11::Json job = json11::Json::parse(sjson, err);
    if (!err.empty()) {
        throw std::runtime_error("Error al parsear JSON: " + err);
    }
    return parse_json(job);
}

Variant Variant::parse_json(const json11::Json &job) {
    std::string t = job["type"].string_value();
    if (t.empty()) {
        throw std::runtime_error("El JSON no contiene el campo 'type' o no es una cadena");
    }

    variant_type vtype = string_to_variant_type(t);

    Variant var(vtype);
    switch (vtype) {
        case Symbol:
        case Number:
        case Cadena:
            var.val = job["value"].string_value();
            break;
        case List: {
            auto arr = job["list"].array_items();
            for (auto &elem : arr) {
                Variant v = Variant::parse_json(elem);
                var.list.push_back(v);
            }
            break;
        }
        case Proc:
        case Lambda:
            // No se guardan detalles de proc o lambda en JSON
            break;
    }

    return var;
}

// Funciones auxiliares
std::string Variant::variant_type_to_string(variant_type t) {
    switch (t) {
        case Symbol: return "Symbol";
        case Number: return "Number";
        case List: return "List";
        case Proc: return "Proc";
        case Lambda: return "Lambda";
        case Cadena: return "Cadena";
    }
    return "Unknown";
}

variant_type Variant::string_to_variant_type(const std::string &s) {
    if (s == "Symbol") return Symbol;
    if (s == "Number") return Number;
    if (s == "List") return List;
    if (s == "Proc") return Proc;
    if (s == "Lambda") return Lambda;
    if (s == "Cadena") return Cadena;
    throw std::runtime_error("Tipo desconocido: " + s);
}


#include "Entorno.h"

// Evalúa una expresión dentro de un entorno
Variant evaluar(const Variant& expr, Entorno& env) {
    if (expr.type == Symbol) {
        // Busca el símbolo en el entorno
        return env.buscar(expr.val);
    } else if (expr.type == List) {
        if (expr.list.empty()) {
            throw std::runtime_error("Error: lista vacía no se puede evaluar.");
        }

        // El primer elemento de la lista debe ser una función
        Variant fn = evaluar(expr.list[0], env);

        if (fn.type != Proc) {
            throw std::runtime_error("Error: el primer elemento no es una función.");
        }

        // Evalúa los argumentos
        std::vector<Variant> args;
        for (size_t i = 1; i < expr.list.size(); ++i) {
            args.push_back(evaluar(expr.list[i], env));
        }

        // Llama al procedimiento
        return fn.proc(args);
    }

    // Para valores atómicos, simplemente se devuelve la instancia
    return expr;
}
