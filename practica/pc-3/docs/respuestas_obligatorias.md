# Respuestas obligatorias — PC3

> **Nota:** las 20 preguntas comunes están redactadas según el formato habitual de
> la PC. Si el enunciado oficial trae una redacción distinta, ajusta el texto pero
> conserva el contenido. Las respuestas específicas del problema (sección B) son
> sobre **Maximum Frequency Stack (LeetCode 895)**.

---

## A. Preguntas comunes (1–20)

**1. ¿Qué problema resolviste y cuál es su entrada/salida?**
Maximum Frequency Stack. Entrada: una secuencia de operaciones `push(val)` (entero)
y `pop()`. Salida: cada `pop()` devuelve el valor más frecuente actualmente; si hay
empate, el insertado más recientemente.

**2. ¿Qué estructura de datos de la librería del curso reutilizaste o extendiste?**
La **Pila / LIFO de Semana 4**. La extiendo a un **arreglo de pilas indexado por
frecuencia** (`group[f]`): cada nivel de frecuencia es una pila independiente.

**3. ¿Por qué esa estructura es la adecuada?**
Porque el desempate "más reciente entre los de máxima frecuencia" es precisamente
la semántica LIFO de una pila. Indexar pilas por frecuencia da `pop()` en O(1)
amortizado sin recalcular frecuencias.

**4. ¿Cuál es el invariante de tu estructura?**
Ver sección 3 del README (I1–I5). En esencia: `maximum_frequency` es la frecuencia máxima
real, las cubetas `1..maximum_frequency` están no vacías, y un valor de frecuencia `c`
aparece exactamente en las cubetas `group[1..c]`.

**5. ¿Cómo verificas el invariante?**
Con `checkInvariant()`, que recalcula las frecuencias desde las cubetas y las
compara con `frequency`, además de revisar el patrón de cubetas vacías/no vacías. Se
ejecuta tras cada operación en `frequency_stack_invariant_test` (5000 operaciones).

**6. ¿Cuál es la complejidad de cada operación y por qué?**
`push` y `pop`: O(1) amortizado (una operación de tabla hash + una de pila).
"Amortizado" por el rehashing de la tabla y el crecimiento de los vectores.
Espacio: O(n).

**7. ¿Qué solución ingenua implementaste y cuál es su complejidad?**
`FrequencyStackNaive`: guarda el historial completo y en cada `pop()` recalcula
frecuencias por escaneo → O(n²) por pop. Es obviamente correcta y sirve de oráculo.

**8. ¿Cómo comparas la solución óptima con la ingenua?**
En `frequency_stack_naive_comparison_test`: 40 secuencias aleatorias (semilla fija,
`std::mt19937`) de 600 operaciones mezclando push/pop; se exige que la secuencia de
`pop()` coincida exactamente en ambas estructuras.

**9. ¿Qué casos borde consideraste?**
Pila vacía, un solo push/pop, un único valor repetido muchas veces, múltiples
valores empatados en frecuencia, y vaciado total. Detalle en
`resultados/casos_borde.md`.

**10. ¿Qué pasa si llamas `pop()` sobre una pila vacía?**
Es una violación de precondición; está protegida con `assert(!empty())`. En el
video muestro este `assert` actuando.

**11. ¿Usaste STL? ¿Dónde y por qué está permitido?**
Sí, como **apoyo secundario** (permitido por la sección 3.2): `std::stack` realiza
cada cubeta y `std::unordered_map` la tabla de frecuencias. La estructura central
(`FrequencyStack`) es propia y está explicada; no es una caja negra.

**12. Si STL estuviera prohibido, ¿cómo lo reemplazarías?**
`std::stack` → Pila propia sobre arreglo dinámico (Semana 4). `std::unordered_map`
→ tabla hash con encadenamiento (Semana 5/6). Solo se usan operaciones básicas
(push/pop/top/empty; insertar/leer/actualizar contador).

**13. ¿El proyecto compila desde cero con CMake?**
Sí: `cmake -S . -B build && cmake --build build`. Evidencia en
`resultados/ejecucion_pc3.md` y `resultados/tests_pc3.md`.

**14. ¿Qué archivos de cabecera usas y qué extensión tienen?**
`frequency_stack.h` y `frequency_stack_naive.h`, ambas con extensión `.h` (no `.hpp`/`.hh`).

**15. ¿El repositorio contiene binarios o carpetas de build?**
No. El `.gitignore` de la raíz excluye `build/` y artefactos de compilación
(`*.o`, `*.out`, `*.exe`, `CMakeCache.txt`, etc.).

**16. ¿Cuántos commits tienes y en cuántos días?**
Al menos 5 commits en 5 días distintos con mensajes coherentes (ver
`docs/bitacora.md` y `git log`).

**17. ¿Cómo están organizadas tus pruebas?**
Un archivo `.cpp` por prueba (estilo PC2), cada uno con su propio `main()` y
`assert`, registrado como ejecutable independiente en `ctest`. 7 archivos cubren
los 6 tipos exigidos.

**18. ¿Qué herramientas de verificación adicional usaste?**
Flags `-Wall -Wextra` (sin warnings) y se puede activar AddressSanitizer/UBSan con
`-DENABLE_SANITIZERS=ON` desde el CMake raíz.

**19. ¿La solución es determinista? ¿Cómo lo garantizas en las pruebas?**
Sí. El generador de las pruebas aleatorias es `std::mt19937` con semilla fija, así
que los resultados son reproducibles.

**20. ¿Por qué tu solución es original y no copiada?**
La idea general (grupos por frecuencia) es estándar, pero la implementación,
nombres, invariante explícito `checkInvariant()`, el oráculo ingenuo y las pruebas
son propios. En el README hay una sección de política anti-copia.

---

## B. Preguntas específicas — Maximum Frequency Stack

**B1. ¿Por qué un arreglo de pilas resuelve el desempate por recencia?**
Porque `pop()` siempre opera sobre `group[maximum_frequency]`, que contiene exactamente los
valores que alcanzaron la frecuencia máxima. Como es una pila (LIFO), su tope es el
que más recientemente llegó a ese nivel, es decir, el más reciente entre los
empatados.

**B2. ¿Por qué empujas un valor a `group[c]` cuando su frecuencia llega a `c`?**
Para que un valor de frecuencia `c` esté "vivo" en todos los niveles `1..c`. Así,
al hacer `pop()` y bajar su frecuencia a `c-1`, sigue disponible en `group[c-1]`
sin trabajo extra.

**B3. ¿Por qué `maximum_frequency` solo baja de uno en uno?**
Cada `push` sube la frecuencia de un valor en 1 (el máximo crece a lo sumo 1). Al
vaciarse `group[maximum_frequency]`, todo valor que tenía esa frecuencia también está en
`group[maximum_frequency-1]`, así que el nuevo máximo es `maximum_frequency-1`. Basta `--maximum_frequency`.

**B4. ¿Qué ocurre con valores grandes (hasta 10⁹)?**
No se pueden indexar directamente, por eso `frequency` es una tabla hash
(`valor → conteo`) y no un arreglo. Las cubetas, en cambio, se indexan por
frecuencia, que sí es densa y pequeña (`1..maximum_frequency`).

**B5. ¿Cómo probaste la correctitud del desempate?**
Con `frequency_stack_duplicates_tie_break_test` (caso dirigido) y con
`frequency_stack_naive_comparison_test`, que compara contra una referencia O(n²)
obviamente correcta sobre miles de operaciones aleatorias.

**B6. ¿Cuál es el costo total de procesar `n` operaciones?**
O(n) tiempo amortizado y O(n) espacio: cada `push` agrega un elemento a una cubeta
y cada `pop` lo retira; ningún elemento se reprocesa.

**B7. ¿Qué modificación harás en vivo en el video?**
Agregar (o reforzar) una verificación del invariante: por ejemplo, un
`assert(checkInvariant())` dentro de `pop()`, o imprimir el estado interno
(`maximum_frequency`, tamaños de cubetas) tras cada operación, recompilar y mostrar que
sigue pasando. (Ver guion en la respuesta del asistente / bitácora.)
