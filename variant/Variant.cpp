#include "Variant.h"
#include <iostream>
#include <stdexcept>
#include "json11.hpp"

// Constructor por defecto
Variant::Variant(variant_type type) : type(type), env(nullptr), proc(nullptr) {}

// Constructor con tipo y valor
Variant::Variant(variant_type type, const std::string& val) : type(type), val(val), env(nullptr), proc(nullptr) {}

// Constructor con procedimiento
Variant::Variant(proc_type proc) : type(Proc), proc(proc), env(nullptr) {}

// Implementación de los métodos
std::string Variant::to_string() {
    switch (type) {
        case Symbol:
            return val; // Devolver el valor del símbolo
        case Number:
            return val; // Asumiendo que val es el número en formato string
        case Cadena:
            return "\"" + val + "\""; // Devolver el valor de la cadena entre comillas
        case List: {
            if (list.empty()) return "()"; // Manejo de listas vacías
            std::string r = "(";
            for (size_t i = 0; i < list.size(); i++) {
                r += list[i].to_string(); // Convertir cada elemento a string
                if (i < list.size() - 1) r += " "; // Añadir espacio entre elementos
            }
            r += ")";
            return r;
        }
        case Proc:
            return "<proc>"; // Devolver un marcador de posición para procedimientos
        case Lambda:
            return "<lambda>"; // Devolver un marcador de posición para lambdas
    }
    return "<unknown>"; // Devolver un marcador de posición para tipos desconocidos
}

std::string Variant::to_json_string() {
    using namespace json11;

    Json::object obj;
    obj["type"] = variant_type_to_string(type); // Añadir el tipo al objeto JSON

    switch (type) {
        case Symbol:
        case Number:
        case Cadena:
            obj["value"] = val; // Añadir el valor al objeto JSON
            break;
        case List: {
            std::vector<Json> jlist;
            for (auto &elem : list) {
                std::string elem_json = elem.to_json_string(); // Convertir cada elemento a string JSON
                std::string err;
                Json subjob = Json::parse(elem_json, err); // Parsear el string JSON
                if (!err.empty()) {
                    throw std::runtime_error("Error al parsear elemento de la lista a JSON: " + err);
                }
                jlist.push_back(subjob); // Añadir el JSON parseado a la lista
            }
            obj["list"] = jlist; // Añadir la lista al objeto JSON
            break;
        }
        case Proc:
        case Lambda:
            // No guardamos proc ni env, solo el tipo
            break;
    }

    Json j(obj);
    return j.dump(); // Devolver el string JSON
}

Variant Variant::from_json_string(std::string sjson) {
    std::string err;
    json11::Json job = json11::Json::parse(sjson, err); // Parsear el string JSON
    if (!err.empty()) {
        throw std::runtime_error("Error al parsear JSON: " + err);
    }
    return parse_json(job); // Convertir el objeto JSON a Variant
}

Variant Variant::parse_json(const json11::Json &job) {
    std::string t = job["type"].string_value();
    if (t.empty()) {
        throw std::runtime_error("El JSON no contiene el campo 'type' o no es una cadena");
    }

    variant_type vtype = string_to_variant_type(t); // Convertir el string de tipo a variant_type

    Variant var(vtype);
    switch (vtype) {
        case Symbol:
        case Number:
        case Cadena:
            var.val = job["value"].string_value(); // Establecer el valor
            break;
        case List: {
            auto arr = job["list"].array_items();
            for (auto &elem : arr) {
                Variant v = Variant::parse_json(elem); // Parsear cada elemento en la lista
                var.list.push_back(v); // Añadir el elemento a la lista
            }
            break;
        }
        case Proc:
        case Lambda:
            // No se guardan detalles de proc o lambda en JSON
            break;
    }

    return var; // Devolver el Variant construido
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
    return "Unknown"; // Devolver "Unknown" para tipos desconocidos
}

variant_type Variant::string_to_variant_type(const std::string &s) {
    if (s == "Symbol") return Symbol;
    if (s == "Number") return Number;
    if (s == "List") return List;
    if (s == "Proc") return Proc;
    if (s == "Lambda") return Lambda;
    if (s == "Cadena") return Cadena;
    throw std::runtime_error("Tipo desconocido: " + s); // Lanzar un error para tipos desconocidos
}