# PC-3 — Maximum Frequency Stack (LeetCode 895)

**Curso:** CC232 — Estructuras de Datos
**Modalidad:** Individual
**Autor:** Leonardo
**Problema asignado:** LeetCode 895 — Maximum Frequency Stack

---

## 1. Problema

Diseñar una estructura tipo pila con dos operaciones:

- `push(val)`: inserta el entero `val`.
- `pop()`: elimina y devuelve el valor **más frecuente** de la estructura. Si
  varios valores empatan en frecuencia, devuelve el **insertado más recientemente**
  entre ellos.

**Ejemplo (oficial):**

```
push 5, push 7, push 5, push 7, push 4, push 5
pop -> 5   (frecuencia 3)
pop -> 7   (empata con 5 en frecuencia 2; 7 es más reciente)
pop -> 5   (frecuencia 2)
pop -> 4   (empata en frecuencia 1; 4 es más reciente)
```

## 2. Estructura de datos central

`FrequencyStack` (en `include/frequency_stack.h`) — implementada y explicada por el autor.

Reutiliza y **extiende la idea de Pila / LIFO de `Libreria_cc232/Semana4`**: en
vez de una sola pila, mantiene un **arreglo de pilas indexado por frecuencia**.

| Campo      | Tipo                        | Rol                                              |
|------------|-----------------------------|--------------------------------------------------|
| `frequency`    | `unordered_map<int,int>`    | frecuencia actual de cada valor (apoyo)          |
| `group`   | `vector<stack<int>>`        | `group[f]` = pila de valores en el nivel `f`    |
| `maximum_frequency` | `int`                       | frecuencia máxima presente                       |

El desempate por recencia surge **gratis** de la disciplina LIFO de cada cubeta:
`pop()` saca del tope de `group[maximum_frequency]`. Detalle completo en
[`docs/frequency_stack.md`](docs/frequency_stack.md).

> **Sobre el uso de STL (sección 3.2 del enunciado):** la estructura central es
> propia y no es una caja negra. `std::stack` y `std::unordered_map` se usan como
> **apoyo secundario**; en `docs/frequency_stack.md` se describe cómo se reemplazarían
> por una Pila y una tabla hash propias (Semanas 4–6).

## 3. Invariante de clase

```
(I1) maximum_frequency == frecuencia máxima actual (0 si está vacía)
(I2) group[0] siempre vacía (no existe la frecuencia 0)
(I3) group[f] no vacía  <=>  1 <= f <= maximum_frequency
(I4) un valor con frecuencia c aparece una vez en group[1..c], en ninguna otra
(I5) count == suma de frecuencias == suma de tamaños de las cubetas
```

El método `checkInvariant()` verifica I1–I5 y se usa en `frequency_stack_invariant_test`.

## 4. Complejidad

| Operación | Tiempo          | Espacio |
|-----------|-----------------|---------|
| `push`    | $O(1)$ amortizado | $O(1)$    |
| `pop`     | $O(1)$ amortizado | $O(1)$    |
| Ingenua `pop` (referencia) | $O(n^2)$ | $O(n)$ |

## 5. Compilación

Desde una copia limpia del repositorio:

```bash
cmake -S . -B build
cmake --build build
```

## 6. Ejecución

```bash
./build/practica/pc-3/pc3_main                       # ejemplo mínimo
./build/practica/pc-3/demos/demo_frequency_stack          # demostración guiada
```

## 7. Pruebas

```bash
ctest --test-dir build/practica/pc-3 --output-on-failure
```

Cada prueba es un ejecutable independiente registrado en ctest (estilo PC2).
Son 7 archivos que cubren los 6 tipos exigidos:

1. Caso mínimo — `frequency_stack_minimal_case_test.cpp`
2. Caso vacío — `frequency_stack_empty_test.cpp`
3. Duplicados / desempate — `frequency_stack_duplicates_tie_break_test.cpp`
4. Extremo pequeño — `frequency_stack_single_value_test.cpp`
5. Comparación con la ingenua — `frequency_stack_naive_comparison_test.cpp`
6. Invariante — `frequency_stack_invariant_test.cpp`
7. (+) Ejemplo oficial LeetCode — `frequency_stack_leetcode_example_test.cpp`

Las pruebas aleatorias (5 y 6) usan `std::mt19937` con semilla fija (reproducibles).

## 8. Estructura del proyecto

```
practica/pc-3/
├── include/
│   ├── frequency_stack.h          # estructura central (header-only)
│   └── frequency_stack_naive.h    # referencia ingenua (oráculo de pruebas)
├── src/main.cpp              # ejecución mínima
├── demos/demo_frequency_stack.cpp # demostración guiada
├── tests/                    # 7 pruebas (1 ejecutable por archivo, estilo PC2)
├── docs/
│   ├── frequency_stack.md             # explicación conceptual
│   ├── respuestas_obligatorias.md
│   └── bitacora.md
├── resultados/
│   ├── ejecucion_pc3.md
│   ├── tests_pc3.md
│   └── casos_borde.md
├── CMakeLists.txt
└── README.md
```

## 9. Política anti-copia

- No se usa `#include <bits/stdc++.h>` ni dependencias ocultas.
- La lógica central (`FrequencyStack`) es propia, con invariante explícito y verificable.
- Solo cabeceras `.h`; el repositorio no contiene binarios ni `build/` (ver
  `.gitignore` en la raíz).
- Compila desde cero con CMake y pasa todas las pruebas.
