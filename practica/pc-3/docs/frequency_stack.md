# FrequencyStack — arreglo de pilas indexado por frecuencia

## La idea central

El problema pide una pila donde `pop()` no devuelve el último elemento, sino el
**más frecuente**; y si varios valores empatan en frecuencia, el **insertado más
recientemente**. La clave es darse cuenta de que el desempate por recencia es
exactamente la disciplina **LIFO de una pila** (estructura de Semana 4).

La estructura mantiene tres piezas:

| Campo       | Tipo                          | Significado                                            |
|-------------|-------------------------------|--------------------------------------------------------|
| `frequency`     | `unordered_map<int,int>`      | frecuencia actual de cada valor (apoyo: tabla hash)    |
| `group`    | `vector<stack<int>>`          | `group[f]` = Pila de valores en el nivel de frecuencia `f` |
| `maximum_frequency`  | `int`                         | frecuencia máxima presente (0 si está vacía)           |

La idea no trivial: **cuando un valor `v` alcanza la frecuencia `f`, lo empujamos
a la pila `group[f]`**. Así, un valor con frecuencia `c` está presente una vez en
cada cubeta `group[1], group[2], ..., group[c]`.

## Por qué funciona el desempate

`pop()` siempre saca de `group[maximum_frequency]`. Esa cubeta contiene únicamente los
valores cuya frecuencia llegó hasta `maximum_frequency`. Como es una **pila**, su tope es el
valor que **más recientemente** alcanzó ese nivel — que coincide con el valor más
recientemente insertado entre los de frecuencia máxima. El desempate sale gratis.

## Por qué `maximum_frequency` baja de uno en uno

Cada `push` incrementa la frecuencia de un único valor en 1, así que la frecuencia
máxima crece a lo sumo en 1. Cada `pop` saca un elemento de `group[maximum_frequency]`; si
esa cubeta queda vacía, significa que ya nadie tiene esa frecuencia, y el nuevo
máximo es exactamente `maximum_frequency - 1` (porque cualquier valor que tenía frecuencia
`maximum_frequency` también está presente en `group[maximum_frequency-1]`). Por eso basta con
`--maximum_frequency`, sin recalcular.

## Invariante (resumen)

```
(I1) maximum_frequency == max frecuencia actual (0 si vacía)
(I2) group[0] siempre vacía
(I3) group[f] no vacía  <=>  1 <= f <= maximum_frequency
(I4) un valor con frecuencia c aparece en group[1..c], en ninguna otra
(I5) count == suma de frecuencias == suma de tamaños de las cubetas
```

## Flujo del algoritmo

```
push(val):
    c = ++frequency[val]
    si c no tiene cubeta: crear group[c]
    group[c].push(val)
    maximum_frequency = max(maximum_frequency, c)
    count++

pop():                       # precondición: no vacía
    val = group[maximum_frequency].top()
    group[maximum_frequency].pop()
    frequency[val]--
    si group[maximum_frequency] vacía: maximum_frequency--
    count--
    return val
```

## Complejidad

| Operación | Tiempo            | Espacio | Razón                                              |
|-----------|-------------------|---------|----------------------------------------------------|
| `push`    | O(1) amortizado   | O(1)    | una inserción en hash + una en pila                |
| `pop`     | O(1) amortizado   | O(1)    | tope + pop de pila + decremento en hash            |
| total     | O(n) para n ops   | O(n)    | cada elemento ocupa una posición en una cubeta     |

"Amortizado" por el rehashing de la tabla y el crecimiento de los vectores; ambos
son O(1) amortizado estándar.

## Comparación con la solución ingenua

La referencia `FrequencyStackNaive` guarda el historial completo y, en cada `pop`,
recalcula por escaneo la frecuencia de cada valor: O(n²) por pop. Para n grande es
inviable, pero es **obviamente correcta**, por lo que se usa como oráculo en las
pruebas (`frequency_stack_naive_comparison_test`).

## Si STL estuviera prohibido

- `std::stack<int>` → una **Pila** propia sobre arreglo dinámico (Semana 4): los
  métodos usados son solo `push`, `pop`, `top`, `empty`.
- `std::unordered_map<int,int>` → una **tabla hash con encadenamiento** (Semana
  5/6): solo necesitamos insertar/leer/actualizar un contador entero por clave.

Ninguna de las dos es una "caja negra": su interfaz mínima y su implementación
alternativa están descritas, tal como exige la sección 3.2 del enunciado.
