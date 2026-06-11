# Casos borde — PC3 Maximum Frequency Stack

Cada caso indica entrada, salida observada y por qué es relevante. Las salidas
provienen de los ejecutables de `tests/` y de `demo_frequency_stack`.

## 1. Caso vacío — `frequency_stack_empty_test`

- **Entrada:** `FrequencyStack` recién construido.
- **Estado:** `empty()==true`, `size()==0`, `maximumFrequency()==0`, `checkInvariant()==true`.
- **Relevante:** estado inicial válido; tras `push`+`pop` vuelve a vacío sin residuos
  (`maximum_frequency` regresa a 0). `pop()` sobre vacío está protegido por `assert`.

## 2. Caso mínimo — `frequency_stack_minimal_case_test`

- **Entrada:** `push(42)`, `pop()`.
- **Salida:** `pop()==42`, luego `empty()==true`.
- **Relevante:** una sola operación de cada tipo; la cubeta `group[1]` se crea y se
  vacía, `maximum_frequency` baja de 1 a 0.

## 3. Extremo pequeño — un solo valor repetido — `frequency_stack_single_value_test`

- **Entrada:** `push(9)` ×5, luego 5 `pop`.
- **Salida:** `9 9 9 9 9` (`maximumFrequency()==5` antes de los pops).
- **Relevante:** un mismo valor ocupa las cubetas `group[1..5]`; cada `pop` baja la
  frecuencia en 1 y el valor reaparece en el nivel inferior.

## 4. Duplicados / desempate por recencia — `frequency_stack_duplicates_tie_break_test`

- **Entrada:** `push(1)`, `push(1)`, `push(2)`, `push(2)`.
- **Salida:** `pop()==2`, `pop()==1`, `pop()==2`, `pop()==1`.
- **Relevante:** 1 y 2 empatan en frecuencia 2; gana el insertado más recientemente
  (2). Verifica que el desempate LIFO de la cubeta `group[2]` funciona.

## 5. Ejemplo oficial LeetCode 895 — `frequency_stack_leetcode_example_test`

- **Entrada:** `push 5,7,5,7,4,5`; luego 4 `pop`.
- **Salida:** `5 7 5 4` (coincide con la salida esperada por LeetCode).
- **Relevante:** mezcla frecuencias 3/2/1 con empates; caso de referencia del problema.

## 6. Comparación contra solución ingenua — `frequency_stack_naive_comparison_test`

- **Entrada:** 40 secuencias aleatorias (`std::mt19937`, semilla fija) de 600
  operaciones mezclando `push` (dominio 0..7) y `pop`.
- **Salida:** todas las secuencias de `pop` coinciden con `FrequencyStackNaive` (O(n²)).
- **Relevante:** valida correctitud, incluido el desempate, contra un oráculo simple.

## 7. Invariante bajo estrés — `frequency_stack_invariant_test`

- **Entrada:** 5000 operaciones aleatorias (`std::mt19937`, semilla fija), dominio 0..11.
- **Salida:** `checkInvariant()==true` después de CADA operación.
- **Relevante:** confirma I1..I5 (`maximum_frequency` correcto, cubetas `1..maximum_frequency`
  no vacías, recuento de frecuencias consistente con `frequency`, `count == total`).

---

**Resultado global:** 7/7 pruebas en verde, 0 fallos.
