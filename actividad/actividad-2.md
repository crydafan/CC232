# Actividad 2 - Semana 2

Integrantes:

- Enrique Alejandro Paul Flores
- Leonardo Gabriel Estacio Huaricapcha

## Bloque 1

1. Significa que todos sus elementos se almacenan en posiciones de RAM consecutivas, sin huecos entre ellos. El bloque completo ocupa un segmento ininterrumpido de memoria: el elemento en la posición `i` está exactamente `i × sizeof(T)` bytes después del elemento en la posición `0`. Esto contrasta con estructuras enlazadas, donde cada nodo puede vivir en cualquier dirección y se conecta mediante punteros.
2. Porque la dirección del elemento `i` se calcula con aritmética pura: `dirección(A[i]) = dirección_base + i × sizeof(T)`. Es una multiplicación y una suma de enteros: tiempo constante, independientemente del tamaño del arreglo y del valor de `i`. No hay recorrido ni búsqueda.
3. `size` (`_size` / `n`) indica cuántos elementos lógicos hay actualmente en la estructura, mientras que `capacity` (`_capacity` / `a.length`) indica cuántos elementos _caben_ en el bloque de memoria reservado. Siempre se cumple `size ≤ capacity`; la diferencia es el espacio reservado pero aún vacío. En `DengVector.h` esto se ve en los campos `_size` y `_capacity`; en `ArrayStack.h`, `n` y `a.length` cumplen el mismo papel.
4. El bloque contiguo que el arreglo ocupa puede tener, inmediatamente después en memoria, datos pertenecientes a otro objeto. El sistema operativo y el allocator no garantizan que esas posiciones estén libres, por lo que extender el bloque sobreescribiría esa memoria. Por eso `resize()` reserva un bloque nuevo de mayor tamaño, copia los elementos existentes y libera el bloque anterior. En `ArrayStack::resize()` (línea 70) y en `DengVector::expand()` (línea 32) se ve exactamente este patrón: `new T[nueva_capacidad]`, copia, `delete[]`.
5. Sea `n` el número total de inserciones. Las expansiones ocurren en capacidades `c, 2c, 4c, 8c, …` y el costo de cada expansión es proporcional al tamaño en ese momento. El costo total de todas las copias es `c + 2c + 4c + … ≤ 2·(último tamaño) ≤ 2n`, es decir `O(n)`. Distribuyendo entre `n` inserciones: `O(n) / n = O(1)` amortizado. La clave es que duplicar hace que cada expansión sea cada vez más infrecuente.
6. `ArrayStack` y `DengVector` comparten arreglo dinámico con expansión al duplicar capacidad, reducción de capacidad cuando el arreglo queda muy vacío, acceso por índice en O(1), e inserción y eliminación. Sus diferencias son:

   | Aspecto | `ArrayStack` (Morin) | `DengVector` (Deng) |
   | ------- | -------------------- | ------------------- |
   | Interfaz | `add(i,x)`, `remove(i)`, `get(i)`, `set(i,x)` | `insert(r,e)`, `remove(r)`, `find(e)`, `traverse()`, `copyFrom()` |
   | Constructor de copia | No definido explícitamente | `copyFrom()` + constructor de copia + `operator=` |
   | Búsqueda | No incluida | `find(e, lo, hi)` — búsqueda secuencial |
   | Recorrido | No incluido | `traverse(visit)` — separa recorrido de operación |
   | Intención didáctica | Núcleo mínimo para listas basadas en arreglo (Morin cap. 2) | Puente hacia el capítulo de Vectores de Deng; muestra gestión de ciclo de vida completo |

7. `FastArrayStack` reemplaza los bucles manuales de copiado por llamadas a `std::copy` y `std::copy_backward` (líneas 37, 44, 53 de `FastArrayStack.h`). La biblioteca estándar puede usar instrucciones de hardware optimizadas (memcpy, instrucciones SIMD) cuando el tipo lo permite, reduciendo el factor constante de las operaciones con desplazamientos. La complejidad asintótica no cambia: insertar en posición `i` sigue siendo O(n − i) en el peor caso.
8. En lugar de reservar un único bloque grande que puede desperdiciar hasta la mitad de su capacidad, `RootishArrayStack` distribuye los elementos en múltiples bloques de tamaños crecientes: el bloque `b` tiene `b+1` celdas. Con `r` bloques la capacidad total es `r(r+1)/2`. El desperdicio máximo es el tamaño del último bloque: O(√n), mucho mejor que el O(n) posible en `ArrayStack`.
9. Con `r` bloques de tamaños `1, 2, …, r` la capacidad total es `r(r+1)/2`, por lo que para almacenar `n` elementos se necesitan aproximadamente `r ≈ √(2n)` bloques y el desperdicio es O(√n). Si todos los bloques tuvieran el mismo tamaño `k`, el desperdicio sería O(k) y el número de bloques `n/k`; minimizar ambos lleva a `k ≈ √n`, que equivale a bloques crecientes `1, 2, 3, …`. El mapeo de índice lógico `i` a `(bloque b, offset j)` se hace con la inversa de `r(r+1)/2` en `RootishArrayStack.h` (línea 58).
10. Las tres dimensiones representación, costo temporal y desperdicio espacial están en tensión:

    | Estructura | Representación | Costo de acceso | Costo de inserción | Desperdicio espacial |
    | ---------- | -------------- | --------------- | ------------------ | -------------------- |
    | `ArrayStack` | Un bloque contiguo | O(1) | O(n) peor, O(1) amortizado al final | O(n) posible |
    | `FastArrayStack` | Ídem, copias con `std::copy` | O(1) | O(n) peor, O(1) amortizado al final | O(n) posible, constante más pequeña |
    | `RootishArrayStack` | `r` bloques de tamaños 1…r | O(1) (con cálculo i2b) | O(n) peor caso | O(√n) |
    | `DengVector` | Un bloque contiguo | O(1) | O(n) peor, O(1) amortizado al final | O(n) posible |

    La memoria contigua dentro de cada bloque garantiza acceso O(1); la política de duplicación garantiza amortización O(1) en inserciones al final; y la organización en bloques crecientes de `RootishArrayStack` es el único mecanismo que reduce el desperdicio a O(√n) sin sacrificar el acceso en O(1).
