# Bitácora

> Registro de avance día a día: decisiones, errores y commits. 
> El repositorio en conjunto ya acumula commits en más de 5 días distintos.

---

## Día 1 — Comprensión del problema

- Leí el enunciado de LeetCode 895 y reproduje a mano el ejemplo
  `5,7,5,7,4,5 -> 5,7,5,4`.
- Identifiqué las dos reglas: (a) sacar el más frecuente; (b) desempatar por
  recencia.
- Decisión: el desempate por recencia es semántica LIFO donde la estructura base es la **Pila** de semana 4.

## Día 2 — Diseño de la estructura central

- Diseñé `FrequencyStack` con `frequency` (tabla hash), `group` (arreglo de pilas) y `maximum_frequency`.
- Escribí el invariante I1–I5 antes de programar.
- Duda resuelta: por qué empujar el valor a `group[c]` cuando llega a frecuencia `c` (mantiene el valor vivo en los niveles inferiores).

## Día 3 — Implementación y referencia ingenua

- Implementé `push`/`pop` y verifiqué la complejidad $O(1)$ amortizada.
- **Error encontrado:** al inicio creaba la cubeta con `if (count == group.size())` y en un caso límite faltaba la cubeta; lo cambié por un `while (count >= size)` para ser robusto.
- Implementé `FrequencyStackNaive` $O(n^2)$ como oráculo.

## Día 4 — Pruebas

- Primer borrador: un único `test_pc3.cpp` monolítico con una macro `CHECK` y un
  generador `xorshift64` propio. Funcionaba, pero no seguía el estilo de la PC2.
- **Reorganización:** lo dividí en 7 archivos de prueba (un ejecutable + un
  `add_test` por archivo, estilo PC2), cada uno con su `main()` y `assert`. Eliminé
  la macro `CHECK`.
- **Cambio de RNG:** reemplacé el `xorshift64` casero por `std::mt19937` con semilla
  fija (más estándar y reproducible) en las pruebas aleatorias.
- **Error encontrado:** mi primer desempate en la ingenua usaba `>=` y rompía el empate al revés; lo corregí a `>` recorriendo de más reciente a más antiguo.
- `checkInvariant()` me ayudó a detectar que no estaba decrementando `count` en un borrador previo.
- Comparación contra la ingenua: 40x600 operaciones en verde. Invariante: 5000 operaciones en verde.

## Día 5 — Documentación y evidencia

- Redacté `README.md`, `docs/frequency_stack.md` y `docs/respuestas_obligatorias.md`.
- Creé los resultados a partir de la ejecución real; decidí guardarlos en
  **Markdown** (`resultados/ejecucion_pc3.md`, `tests_pc3.md`, `casos_borde.md`) en
  lugar de `.txt`, para que se lean mejor en el repo.
- Simplifiqué los `CMakeLists.txt` al mínimo estilo PC2 (un `add_executable` /
  `add_test` explícito por prueba; sin `foreach` ni variables auxiliares).
- Verifiqué compilación desde copia limpia: `cmake -S . -B build && cmake --build build`.

## Día 6 — Refactor de consistencia y cierre

- **Renombrado a inglés completo** para uniformar el código: `FreqStack` →
  `FrequencyStack`, `maxFrequency()` → `maximumFrequency()`, miembros
  (`freq_`/`group_`/`maxFreq_`/`count_`) → `frequency`/`group`/`maximum_frequency`/
  `count`, cabeceras `freq_stack*.h` → `frequency_stack*.h`, demo y archivos de
  prueba `freq_stack_*` → `frequency_stack_*`. La documentación se mantiene en
  español; el código y los prints, en inglés.
- **Error encontrado (CMake):** al nombrar el ejecutable del `main` como `main`, la
  configuración falló con **CMP0002** porque la PC2 ya define un target `main`. Los
  nombres de target son globales: lo renombré a `pc3_main`.
- Volví a crear los `resultados/*.md` tras el renombrado para que la evidencia refleje
  los nombres finales (`demo_frequency_stack`, `frequency_stack_*_test`).
- Verificación final: `cmake -S . -B build && cmake --build build` y
  `ctest` → **7/7 pruebas en verde**.
- Organicé la entrega en 6 commits lógicos (estilo PC2): scaffold → header central
  → referencia ingenua → pruebas → demo y main → documentación y evidencia.

---

## Resumen de errores y aprendizajes

| Error                                   | Síntoma                         | Corrección                          |
|-----------------------------------------|---------------------------------|-------------------------------------|
| Creación de cubeta con `==`             | cubeta inexistente en un límite | `while (count >= group.size())`    |
| Desempate invertido en la ingenua       | pops no coincidían con FrequencyStack| recorrer de reciente a antiguo, `>` |
| Olvido de `--count` en `pop`           | `checkInvariant()` fallaba (I5) | decrementar `count` en `pop`       |
| Target CMake llamado `main`             | configuración falla con CMP0002 (colisión con PC2) | renombrar el target a `pc3_main`    |
| Test monolítico con macro y RNG propio  | no seguía el estilo PC2         | 7 archivos separados + `std::mt19937` |
