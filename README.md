Aquí tienes ambos mensajes unidos en un solo README extendido:

---

# Caso Final Integrador Tema 4

[Link del repositorio](https://github.com/cdiezgaruax/Caso-Integrador-Tema-4.git)

## Enunciado del Proyecto
Para este proyecto, se te pide que implementes un intérprete "tiny-lisp" basado en la clase `Variant` y las capacidades de la Standard Template Library (STL) de C++. Deberás trabajar en CLion para este proyecto.

### Requisitos del proyecto:

- Definir e implementar una clase `Variant`. Esta clase deberá ser capaz de representar diferentes tipos de datos, incluyendo símbolos, números, listas y procedimientos.
- Implementar un método `to_string()` para la clase `Variant` que convierta una instancia de la clase a una cadena de texto.
- Implementar un método `to_json_string()` para la clase `Variant` que convierta una instancia de la clase a una representación en formato JSON.
- Implementar un método estático `from_json_string()` que tome una cadena en formato JSON y la convierta a una instancia de la clase `Variant`.
- Implementar un método `parse_json()` que tome una cadena en formato JSON y la convierta a una instancia de la clase `Variant`.

---

### **Explicación de la Implementación de `Variant`**

#### **Clase `Variant`**
La clase `Variant` es el núcleo del proyecto y permite representar múltiples tipos de datos usados en el intérprete "tiny-lisp". Cada instancia puede representar uno de los siguientes tipos:
- **Symbol**: Representa un símbolo (nombre de una variable o una función).
- **Number**: Representa un número en formato de texto.
- **List**: Una lista que contiene otras instancias de `Variant`.
- **Proc**: Una referencia a una función que puede ser ejecutada.
- **Lambda**: Una función anónima o definida dinámicamente.
- **Cadena**: Una cadena de texto encerrada entre comillas.

El atributo principal de la clase es `type`, que indica qué tipo de dato está representando.

#### **Constructor**
```cpp
Variant::Variant(variant_type type) : type(type), env(nullptr), proc(nullptr) {}
```
Este constructor permite inicializar una instancia de `Variant` con un tipo específico, dejando los valores adicionales como punteros nulos (`nullptr`).

#### **Método `to_string()`**
Este método convierte una instancia de `Variant` en una representación de texto legible. Su implementación varía dependiendo del tipo:
- Para **Symbol** y **Number**, devuelve directamente el valor como texto.
- Para **Cadena**, devuelve el texto encerrado en comillas.
- Para **List**, recorre todos los elementos de la lista y los convierte a texto, separados por espacios y encerrados entre paréntesis `( ... )`.
- Para **Proc** y **Lambda**, devuelve una cadena descriptiva (`<proc>` o `<lambda>`).

#### **Método `to_json_string()`**
Este método convierte una instancia de `Variant` en una representación JSON. Utiliza la biblioteca `json11` para crear un objeto JSON y lo devuelve como texto. Los detalles son:
- Los atributos `type` y `val` se almacenan en el objeto JSON.
- Para las listas, recorre sus elementos y convierte cada uno a JSON de forma recursiva.

Ejemplo:
```cpp
Variant v(Number, "42");
std::cout << v.to_json_string();
// Resultado: {"type": "Number", "value": "42"}
```

#### **Método `from_json_string()`**
Este método es estático y toma una cadena JSON como entrada. Parsea la cadena usando `json11` y convierte el resultado en una instancia de `Variant`. Si la cadena es inválida, lanza una excepción.

Ejemplo:
```cpp
std::string json_input = "{\"type\": \"Number\", \"value\": \"42\"}";
Variant v = Variant::from_json_string(json_input);
std::cout << v.to_string();
// Resultado: 42
```

#### **Método `parse_json()`**
Este método interno realiza la conversión recursiva de un objeto JSON (`json11::Json`) a una instancia de `Variant`. Es utilizado por `from_json_string()` para procesar datos anidados como listas o estructuras complejas.

---

### **Explicación de los Tests**

El archivo de test contiene pruebas diseñadas para garantizar que la clase `Variant` y sus métodos se comporten de acuerdo a lo esperado. Estas pruebas verifican casos comunes y límites de cada función implementada.

#### **Estructura General**
El archivo de tests (`tests/test.cpp`) se basa en una serie de casos que validan la funcionalidad clave de la clase `Variant`. Incluye:
1. **Pruebas unitarias básicas** para cada método (`to_string`, `to_json_string`, `from_json_string`).
2. **Casos de prueba combinados** para asegurar que los métodos funcionan correctamente en conjunto.
3. **Verificación de tipos y conversiones**, como listas anidadas y cadenas JSON complejas.

---

#### **Ejemplo de Test: Método `to_string()`**
**Objetivo:** Asegurarse de que las instancias de `Variant` se conviertan correctamente en representaciones de texto según su tipo.

**Prueba:**
```cpp
void test_to_string() {
    Variant v1(Number, "42");
    Variant v2(Cadena, "Hello, World!");
    Variant v3(Symbol, "x");

    assert(v1.to_string() == "42");
    assert(v2.to_string() == "\"Hello, World!\"");
    assert(v3.to_string() == "x");

    std::cout << "Método to_string() pasó todas las pruebas.\n";
}
```

**Explicación:**
- Se crea un número (`v1`) y se espera que `to_string()` devuelva `"42"`.
- Se crea una cadena (`v2`) y se espera que `to_string()` devuelva `"Hello, World!"` encerrado entre comillas.
- Se crea un símbolo (`v3`) y se verifica que devuelva su valor textual.

---

#### **Ejemplo de Test: Método `to_json_string()`**
**Objetivo:** Validar que las instancias de `Variant` se conviertan correctamente a una representación JSON.

**Prueba:**
```cpp
void test_to_json_string() {
    Variant v1(Number, "42");
    Variant v2(Cadena, "Hello");
    Variant v3(List);
    v3.list.push_back(Variant(Symbol, "x"));
    v3.list.push_back(Variant(Number, "5"));

    assert(v1.to_json_string() == "{\"type\":\"Number\",\"value\":\"42\"}");
    assert(v2.to_json_string() == "{\"type\":\"Cadena\",\"value\":\"Hello\"}");
    assert(v3.to_json_string() == "{\"type\":\"List\",\"list\":[{\"type\":\"Symbol\",\"value\":\"x\"},{\"type\":\"Number\",\"value\":\"5\"}]}");

    std::cout << "Método to_json_string() pasó todas las pruebas.\n";
}
```

**Explicación:**
- Se crea un número (`v1`) y se espera que devuelva un JSON con el tipo `Number` y valor `"42"`.
- Se verifica una cadena (`v2`) con el tipo `Cadena`.
- Para una lista (`v3`), se prueban elementos anidados para asegurar que se procesen correctamente.

---

#### **Ejemplo de Test: Método `from_json_string()`**
**Objetivo:** Comprobar que una cadena JSON se pueda convertir de vuelta a una instancia válida de `Variant`.

**Prueba:**
```cpp
void test_from_json_string() {
    std::string json1 = "{\"type\":\"Number\",\"value\":\"42\"}";
    std::string json2 = "{\"type\":\"Cadena\",\"value\":\"Hola\"}";
    std::string json3 = "{\"type\":\"List\",\"list\":[{\"type\":\"Symbol\",\"value\":\"y\"}]}";

    Variant v1 = Variant::from_json_string(json1);
    Variant v2 = Variant::from_json_string(json2);
    Variant v3 = Variant::from_json_string(json3);

    assert(v1.type == Number && v1.val == "42");
    assert(v2.type == Cadena && v2.val == "Hola");
    assert(v3.type == List && v3.list[0].type == Symbol && v3.list[0].val == "y");

    std::cout << "Método from_json_string() pasó todas las pruebas.\n";
}
```

**Explicación:**
- Se prueba la conversión de un JSON simple que representa un número, verificando que el tipo y valor coincidan.
- Se prueba una cadena JSON que representa un texto.
- Se verifica una lista con un símbolo anidado para validar la conversión recursiva.

---

#### **Ejemplo de Test: Método `parse_json()`**
**Objetivo:** Verificar que un objeto JSON complejo pueda ser transformado en un objeto `Variant`.

**Prueba:**
```cpp
void test_parse_json() {
    json11::Json job = json11::Json::object{
        {"type", "List"},
        {"list", json11::Json::array{
            json11::Json::object{{"type", "Number"}, {"value", "123"}},
            json11::Json::object{{"type", "Cadena"}, {"value", "Hola"}}
        }}
    };

    Variant v = Variant::parse_json(job);

    assert(v.type == List);
    assert(v.list.size() == 2);
    assert(v.list[0].type == Number && v.list[0].val == "123");
    assert(v.list[1].type == Cadena && v.list[1].val == "Hola");

    std::cout << "Método parse_json() pasó todas las pruebas.\n";
}
```

**Explicación:**
- Se construye un objeto JSON con una lista que contiene un número y una cadena.
- Se procesa este JSON y se verifica que los elementos se conviertan correctamente en objetos `Variant`.
