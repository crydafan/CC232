# PC-2 — Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit

## Problema

Dado un arreglo de enteros `nums` y un entero `limit`, retornar la longitud del subarreglo continuo más largo tal que la diferencia absoluta entre cualquier par de elementos sea menor o igual a `limit`.

- **Entrada:** `nums` — arreglo de enteros, `limit` — entero no negativo
- **Salida:** entero — longitud del subarreglo más largo que satisface la condición
- **Restricción clave:** la diferencia entre el máximo y el mínimo del subarreglo debe ser `≤ limit`

## Soluciones implementadas

### Solución ingenua — O(n²)

Recorre todos los pares `(i, j)` con `i ≤ j`, manteniendo el mínimo y máximo acumulado del subarreglo `nums[i..j]`. En cuanto la diferencia supera `limit` se rompe el bucle interno. Es correcta pero inviable para entradas grandes.

### Solución óptima — O(n)

Aplica **ventana deslizante** con dos **deques monotónicos**: uno de máximos (decreciente) y uno de mínimos (creciente). El frente de cada deque guarda el índice del máximo y mínimo actuales de la ventana. Cuando la diferencia supera `limit`, el puntero izquierdo avanza y se expulsan los índices que quedan fuera.

**Invariante:** en todo momento, `nums[maxDq.front()] - nums[minDq.front()] <= limit`.

## Complejidad

| Solución | Tiempo | Espacio |
|----------|--------|---------|
| Ingenua  | O(n²)  | O(1)    |
| Óptima   | O(n)   | O(n)    |

## Compilacion y ejecucion

```bash
cmake -Bbuild -GNinja
ninja -C build

./build/pc-2/main
./build/pc-2/demos/demo_longest_subarray
./build/pc-2/bench/bench_longest_subarray

ctest --test-dir build/pc-2 --output-on-failure
```
