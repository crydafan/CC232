# Actividad 6 - Semana 6

Estudiante:

- Leonardo Gabriel Estacio Huaricapcha

## Bloque 1 - Núcleo conceptual

1. Una cola de prioridad es un tipo abstracto de datos que, a diferencia de una cola FIFO o una pila LIFO, no entrega el elemento por orden de llegada sino por orden de prioridad: la operación central es obtener (y a veces retirar) el elemento de mayor prioridad sin importar cuándo se insertó. En la semana esa interfaz vive en PQ, que declara las operaciones esenciales sin comprometerse con una representación: `insert` para agregar un elemento, `getMax` para consultar el de mayor prioridad y `delMax` para retirarlo. Lo importante es que PQ describe el qué, no el cómo.

2. Conviene separar tres ideas que suelen confundirse. Una cola de prioridad es un concepto, un contrato de comportamiento. Un heap es una representación concreta y eficiente con la que se implementa ese contrato. Y un árbol de búsqueda es otra estructura, pensada para búsqueda ordenada, que no es lo mismo que una cola de prioridad. La semana hace explícita esta distinción: PQ es la interfaz, PQ_ComplHeap y PQ_LeftHeap son dos representaciones distintas del mismo contrato, y Treap es un árbol de búsqueda que toma prestada la idea de prioridad para otra cosa.

3. Un heap binario completo es la forma más común de implementar una cola de prioridad porque aprovecha que el árbol está siempre lleno nivel por nivel de izquierda a derecha. Esa rigidez de forma permite guardarlo en un arreglo implícito sin punteros, calcular padres e hijos con aritmética de índices, y resolver `getMax` en tiempo constante mirando la raíz. Es el caso de PQ_ComplHeap, que vive sobre un arreglo y mantiene la propiedad de que cada padre es mayor o igual que sus hijos.

4. La propiedad que define a un max-heap es vertical y parcial: para todo nodo distinto de la raíz, el valor del padre es mayor o igual que el del hijo. No dice nada sobre el orden entre hermanos ni entre primos, solo sobre la relación padre-hijo. De ahí salen dos consecuencias: el máximo global queda siempre en la raíz, y no existe un recorrido del arreglo que entregue los elementos ordenados sin retirarlos uno por uno. Esto contrasta con la propiedad de orden de un BST, que es total y horizontal.

5. PQ_ComplHeap y PQ_LeftHeap implementan la misma interfaz con representaciones distintas. PQ_ComplHeap usa un arreglo implícito y restaura la propiedad con `percolateUp` tras insertar y `percolateDown` tras retirar el máximo. PQ_LeftHeap usa nodos enlazados y construye casi todo sobre una sola operación, `merge`, que mezcla dos heaps respetando la propiedad de prioridad. La ventaja de PQ_LeftHeap es que mezclar dos colas de prioridad es barato, algo que en un heap completo sobre arreglo es costoso.

6. La operación `getMax` consulta el máximo sin retirarlo. En PQ_ComplHeap es inmediata porque el máximo está siempre en el índice 0 del arreglo, así que `getMax` solo devuelve ese primer elemento en tiempo constante. En PQ_LeftHeap el máximo está en la raíz del árbol enlazado, también accesible de inmediato. La razón de fondo es la misma en ambas: la propiedad de heap garantiza que ningún elemento puede ser mayor que la raíz.

7. La operación `insert` agrega un elemento y restaura la propiedad. En PQ_ComplHeap el elemento nuevo se coloca al final del arreglo, donde la forma de árbol completo lo admite sin huecos, y luego `percolateUp` lo hace subir mientras sea mayor que su padre. En PQ_LeftHeap insertar es un caso particular de `merge`: se crea un heap de un solo nodo y se mezcla con el heap existente. Esa es justamente la elegancia del heap izquierdista, que insertar y retirar se reducen a mezclar.

8. La operación `delMax` retira el máximo. En PQ_ComplHeap no se puede simplemente borrar la raíz porque dejaría un hueco en medio del arreglo; en su lugar se lleva el último elemento a la raíz para conservar la forma compacta y luego `percolateDown` lo hunde hasta su lugar. En PQ_LeftHeap se descarta la raíz y se mezclan sus dos subárboles con `merge`, que vuelve a producir un heap válido. Otra vez la misma idea: lo que en el heap completo se resuelve con percolación, en el izquierdista se resuelve mezclando.

9. Huffman es un consumidor de la interfaz, no de una representación. El algoritmo necesita repetidamente extraer los dos elementos de menor frecuencia, combinarlos y reinsertar el resultado, y eso es exactamente lo que una cola de prioridad ofrece. Huffman_PQ se apoya en la interfaz PQ sin depender de si por debajo hay un heap completo o uno izquierdista. Esto demuestra el valor de separar el contrato de la representación: el código de Huffman no cambia aunque se cambie el heap.

10. Treap reutiliza la palabra prioridad pero para algo distinto. No es una cola de prioridad: es un árbol de búsqueda que mantiene dos invariantes a la vez, orden BST por clave y propiedad de heap por una prioridad aleatoria asignada a cada nodo. La prioridad aquí no sirve para entregar el máximo, sino para que el árbol quede balanceado en promedio sin tener que mantener reglas de balance explícitas. Es un buen cierre de la semana porque muestra que la misma idea de prioridad puede usarse con un propósito completamente diferente.

## Bloque 2 - Heap binario completo y sus operaciones

PQ_ComplHeap es un max-heap sobre arreglo implícito. Las utilidades de índice en PQ_ComplHeap_macro son funciones `constexpr`:

```cpp
parent(i) = (i - 1) / 2
left(i)   = 2*i + 1
right(i)  = 2*i + 2
```

1. El máximo de un max-heap queda siempre en la raíz porque la propiedad obliga a que cada nodo sea mayor o igual que sus hijos, y aplicada en cadena desde la raíz hacia abajo, eso significa que la raíz es mayor o igual que cualquier descendiente, es decir mayor o igual que todos. Como la raíz ocupa el índice 0, `getMax` devuelve `data[0]` en tiempo constante sin recorrer nada.

2. Las fórmulas de índice funcionan porque el árbol es completo y se numera por niveles empezando en 0. El nodo `i` tiene su hijo izquierdo en `2*i + 1` y el derecho en `2*i + 2`, y la inversa da el padre en `(i - 1) / 2` con división entera. Por ejemplo, el nodo en el índice 3 tiene hijos en 7 y 8, y su padre es `(3 - 1) / 2 = 1`. Que sean funciones `constexpr` y no macros evita los problemas clásicos de las macros (efectos colaterales, falta de tipado) manteniendo el costo constante.

3. `percolateUp(i)` restaura la propiedad después de insertar al final. Compara el elemento en `i` con su padre `parent(i)`: si el hijo es mayor que el padre, los intercambia y sube `i = parent(i)`, repitiendo; si el hijo no es mayor, se detiene porque ya está en su lugar. El elemento recién insertado sube mientras viole el orden con su padre. El costo es proporcional a la altura, es decir `O(log n)`.

4. `percolateDown(i)` restaura la propiedad después de llevar el último elemento a la raíz. Mira los dos hijos `left(i)` y `right(i)`, elige el mayor entre el nodo y sus hijos, y si el mayor es uno de los hijos, intercambia y baja a esa posición, repitiendo; si el propio nodo ya es el mayor, se detiene. El elemento se hunde intercambiándose siempre con el mayor de sus hijos. El costo también es `O(log n)`.

5. Traza de inserciones una por una en un max-heap inicialmente vacío con la secuencia `4, 3, 8, 1, 9, 2, 7`. Tras cada `insert` se aplica `percolateUp` al último índice. El estado final del arreglo es:

   ```
   índice: 0  1  2  3  4  5  6
   valor:  9  4  8  1  3  2  7
   ```

   que como árbol es:

   ```
           9
          / \
         4   8
        / \ / \
       1  3 2  7
   ```

   El `9`, al insertarse cuando el arreglo era `[8, 4, 3, 1]` más el nuevo, subió desde el final hasta una posición alta por ser mayor que sus padres sucesivos; ese es el efecto de `percolateUp`.

6. Traza de un `insert` que sube hasta la raíz. Partiendo del heap `[9, 4, 8, 1, 3, 2, 7]`, insertar `10` lo coloca al final en el índice 7, y `percolateUp(7)` hace: padre `(7-1)/2 = 3` con valor `1`, como `10 > 1` intercambia; ahora en el índice 3, padre `1` con valor `4`, como `10 > 4` intercambia; ahora en el índice 1, padre `0` con valor `9`, como `10 > 9` intercambia; llega a la raíz y se detiene. El arreglo queda `[10, 9, 8, 4, 3, 2, 7, 1]`. El nuevo máximo subió tres niveles, uno por cada comparación.

7. Traza de un `delMax`. Partiendo de `[9, 4, 8, 1, 3, 2, 7]`, `delMax` devuelve `9`, lleva el último elemento `7` a la raíz y reduce el tamaño a 6, quedando tentativamente `[7, 4, 8, 1, 3, 2]`. Luego `percolateDown(0)`: hijos `4` (índice 1) y `8` (índice 2), el mayor es `8 > 7`, intercambia; ahora en el índice 2, su único hijo es `2` (índice 5), como `7 > 2` se detiene. El arreglo queda `[8, 4, 7, 1, 3, 2]` y el nuevo máximo es `8`. La forma de árbol completo se conservó en todo momento.

8. Hay que mover el último elemento a la raíz antes de `percolateDown` porque retirar la raíz deja un hueco que debe llenarse sin romper la forma de árbol completo. El único elemento que se puede quitar sin dejar huecos intermedios es el último del arreglo; ponerlo en la raíz mantiene el arreglo compacto y solo queda corregir el orden hundiéndolo. Hacerlo al revés, corregir antes de tapar el hueco, rompería la representación implícita.

9. Costo de las operaciones de PQ_ComplHeap: `getMax` es `O(1)` porque solo lee la raíz; `insert` es `O(log n)` porque `percolateUp` recorre a lo más la altura del árbol; y `delMax` es `O(log n)` por la misma razón con `percolateDown`. La altura de un heap completo de `n` elementos es del orden de `log n`, y todas las reparaciones suben o bajan a lo largo de un solo camino raíz-hoja.

10. Insertar `n` elementos uno por uno cuesta `O(n log n)` en el peor caso, porque cada `insert` puede ejecutar un `percolateUp` que recorra toda la altura, y sumando sobre las `n` inserciones se obtiene del orden de `n log n` operaciones. Esto importa porque, cuando se dispone del arreglo completo de antemano, conviene construir el heap de otra forma más barata, que es justo lo que hace `heapify` de Floyd, tratado en el bloque siguiente.

## Bloque 3 - heapify de Floyd y heapsort

1. `heapify` de Floyd construye un heap a partir de un vector arbitrario sin insertar elemento por elemento. La idea es recorrer los nodos internos desde el último hacia la raíz y aplicar `percolateDown` a cada uno. Empezar por abajo garantiza que, cuando se procesa un nodo, sus dos subárboles ya cumplen la propiedad de heap, así que un solo `percolateDown` basta para colocar ese nodo en su lugar. El último nodo interno está en el índice `n/2 - 1`; las hojas, de la mitad en adelante, no necesitan trabajo.

2. Traza de `heapify` sobre el vector `[4, 3, 8, 1, 9, 2, 7]` con `n = 7`. Los nodos internos son los índices `0, 1, 2` y se procesan en orden `2, 1, 0`.

   - `i = 2` (valor `8`): hijos `2` y `7`, el mayor es `7`, como `8 >= 7` no hay cambio.
   - `i = 1` (valor `3`): hijos `1` y `9`, el mayor es `9 > 3`, intercambia. El `3` baja al índice 4, que es hoja, y se detiene. Arreglo: `[4, 9, 8, 1, 3, 2, 7]`.
   - `i = 0` (valor `4`): hijos `9` y `8`, el mayor es `9 > 4`, intercambia. El `4` baja al índice 1, cuyos hijos son `1` y `3`, el mayor es `3`, como `4 >= 3` se detiene. Arreglo: `[9, 4, 8, 1, 3, 2, 7]`.

   El resultado `[9, 4, 8, 1, 3, 2, 7]` es un max-heap válido con el máximo `9` en la raíz.

3. `heapify` de Floyd cuesta `O(n)`, no `O(n log n)`, porque el costo de cada `percolateDown` depende de la altura del nodo, no de la del árbol entero. Hay muchísimos nodos cerca de las hojas (que se hunden poco o nada) y muy pocos cerca de la raíz. Al sumar el trabajo por niveles, la serie converge a una cantidad proporcional a `n`. Por eso construir de golpe es asintóticamente más barato que las `O(n log n)` de insertar uno por uno.

4. Heapsort reutiliza la propiedad de heap para ordenar en sitio. Primero construye un max-heap con `heapify`. Luego, repetidamente, intercambia la raíz (el máximo actual) con el último elemento activo del arreglo, reduce en uno la zona considerada heap y aplica `percolateDown(0)` para restaurar la propiedad. Cada paso fija el máximo en su posición definitiva al final, así que al terminar el arreglo queda ordenado de menor a mayor.

5. Traza de heapsort partiendo del heap `[9, 4, 8, 1, 3, 2, 7]`. La extracción sucesiva del máximo produce, en orden, `9, 8, 7, 4, 3, 2, 1`. Cada valor se va colocando desde el final hacia el inicio, de modo que el arreglo final ordenado de forma ascendente es:

   ```
   1 2 3 4 7 8 9
   ```

   La secuencia descendente de máximos extraídos es la evidencia directa de que la propiedad de heap se mantuvo tras cada `percolateDown`.

6. Heapsort tiene costo `O(n log n)` en total: la fase de construcción con `heapify` es `O(n)`, y la fase de extracción hace `n` veces un `percolateDown` de costo `O(log n)`, lo que domina. Su gran virtud es que ordena en sitio, con memoria auxiliar `O(1)`, sin necesitar un arreglo extra como sí ocurre en otras estrategias de ordenamiento.

7. `vector_heapSort` aplica esta idea sobre un `std::vector`. No necesita una clase de cola de prioridad completa: le basta con las fórmulas de índice y un `percolateDown` que opere sobre el rango activo del vector. Es un buen ejemplo de cómo la propiedad de heap puede usarse de forma directa, sin construir el ADT entero, cuando lo único que se quiere es ordenar.

8. La diferencia entre construir con `heapify` y construir con inserciones se ve comparando el trabajo. Con inserciones, cada elemento parte de una hoja y puede subir hasta la raíz, y los elementos abundantes (las hojas) son los que más lejos están de la raíz, lo que encarece el total a `O(n log n)`. Con `heapify`, esos mismos elementos abundantes están al final del recorrido y casi no se mueven, y solo los pocos nodos altos hacen trabajo largo, lo que abarata el total a `O(n)`.

## Bloque 4 - Heap izquierdista y la operación merge

1. Un heap izquierdista es un heap binario enlazado que, además de la propiedad de prioridad, mantiene una propiedad estructural pensada para que mezclar sea barato. A cada nodo se le asocia una longitud de camino nulo, `npl`, que es la distancia al descendiente nulo más cercano. La propiedad izquierdista exige que, en todo nodo, `npl(left) >= npl(right)`. Eso obliga al árbol a inclinarse hacia la izquierda y mantiene el borde derecho corto, de longitud `O(log n)`.

2. `merge` es la operación central porque casi todo lo demás se construye encima de ella. Para mezclar dos heaps izquierdistas se compara la raíz de cada uno, se toma la mayor como nueva raíz, y se mezcla recursivamente su subárbol derecho con el otro heap completo. Al volver de la recursión se restaura la propiedad izquierdista en cada nodo: si quedó `npl(left) < npl(right)`, se intercambian los hijos y se recalcula `npl`. Trabajar siempre por el borde derecho, que es corto, es lo que hace que `merge` cueste `O(log n)`.

3. Traza conceptual de un `merge`. Sean dos max-heaps izquierdistas, uno con raíz `9` y otro con raíz `7`. Como `9 > 7`, el `9` será la raíz del resultado y se mezcla su subárbol derecho con todo el heap de raíz `7`. Esa submezcla vuelve a comparar raíces, elige la mayor, y desciende otra vez por el borde derecho, hasta que uno de los heaps se vacía. Al regresar, cada nodo verifica `npl(left) >= npl(right)` y, si hace falta, intercambia sus hijos. El máximo global del resultado es `9`, como exige la propiedad de prioridad.

4. `insert` en un heap izquierdista es un caso particular de `merge`: se crea un heap de un solo nodo con el elemento nuevo y se mezcla con el heap existente. No hay un procedimiento de inserción separado, lo que reduce el código y la cantidad de casos a razonar. El costo es el de un `merge`, es decir `O(log n)`.

5. `delMax` en un heap izquierdista también se reduce a `merge`: se retira el nodo raíz, que contiene el máximo, y se mezclan sus dos subárboles izquierdo y derecho, que por sí mismos ya son heaps izquierdistas válidos. El resultado de esa mezcla es el nuevo heap sin el máximo. Otra vez el costo es `O(log n)`.

6. La ventaja del heap izquierdista frente al heap completo sobre arreglo es la mezcla. Unir dos heaps completos sobre arreglo obliga, en general, a reconstruir, lo que cuesta lineal en el tamaño total. En cambio dos heaps izquierdistas se mezclan en `O(log n)` recorriendo solo los bordes derechos. Cuando el problema requiere combinar colas de prioridad con frecuencia, el heap izquierdista es claramente preferible.

7. MeldableHeap aparece como apoyo conceptual: es un heap mezclable que ilustra la misma idea de que la operación de fusión puede ser la base de toda la estructura, sin comprometerse con la disciplina de `npl` del heap izquierdista. Sirve para reforzar que mezclar es una operación de primera clase y que insertar y retirar pueden definirse a partir de ella.

8. La razón de mantener `npl` y no otra medida es que `npl` controla exactamente la longitud del borde derecho, que es por donde `merge` desciende. Garantizar `npl(left) >= npl(right)` mantiene ese borde derecho acotado por `O(log n)`, que es la cota de costo de todas las operaciones. No se pide que el árbol esté balanceado en el sentido usual; basta con que el camino derecho sea corto.

## Bloque 5 - Huffman

1. Huffman construye un código de prefijo óptimo a partir de las frecuencias de los símbolos. La idea es combinar repetidamente los dos símbolos o subárboles de menor frecuencia en uno nuevo cuya frecuencia es la suma, hasta que quede un único árbol. Los símbolos más frecuentes quedan cerca de la raíz, con códigos cortos, y los raros quedan abajo, con códigos largos, lo que minimiza la longitud total esperada de la codificación.

2. Huffman necesita una cola de prioridad porque en cada paso debe extraer los dos elementos de menor frecuencia entre todos los disponibles, combinarlos y reinsertar el resultado, que pasa a competir con los demás. Hacer esto con búsqueda lineal costaría `O(n)` por paso; con una cola de prioridad cada extracción e inserción es `O(log n)`, así que el algoritmo completo queda en `O(n log n)`. Huffman_PQ se apoya en la interfaz PQ, sin depender de qué heap concreto esté por debajo.

3. Sobre la convención de menor frecuencia o mayor prioridad: combinar los dos de menor frecuencia equivale a tratar la cola como una cola de mínimos. Si la representación disponible es un max-heap, se consigue el mismo efecto usando como prioridad el negativo de la frecuencia, de modo que la menor frecuencia tiene la mayor prioridad. El árbol resultante es el mismo; solo cambia el signo con el que se ordena. Huffman_PQ_generateTree encapsula esa generación del árbol a partir de la cola.

4. Traza con las frecuencias `a:5, b:9, c:12, d:13, e:16, f:45` (total `100`). Combinando siempre los dos menores:

   | Paso | Se combinan | Frecuencia del nuevo nodo | Cola restante (frecuencias) |
   | ---- | ----------- | ------------------------- | --------------------------- |
   | 1 | `a(5)`, `b(9)` | `14` | `12, 13, 14, 16, 45` |
   | 2 | `c(12)`, `d(13)` | `25` | `14, 16, 25, 45` |
   | 3 | `14`, `e(16)` | `30` | `25, 30, 45` |
   | 4 | `25`, `30` | `55` | `45, 55` |
   | 5 | `f(45)`, `55` | `100` | `100` (raíz) |

5. Códigos resultantes de esa traza, asignando `0` a la rama izquierda y `1` a la derecha:

   | Símbolo | Frecuencia | Código | Longitud |
   | ------- | ---------- | ------ | -------- |
   | `f` | 45 | `0` | 1 |
   | `c` | 12 | `100` | 3 |
   | `d` | 13 | `101` | 3 |
   | `e` | 16 | `111` | 3 |
   | `a` | 5 | `1100` | 4 |
   | `b` | 9 | `1101` | 4 |

   El total de bits es `45*1 + 12*3 + 13*3 + 16*3 + 5*4 + 9*4 = 224`, frente a los `300` bits que costaría un código fijo de 3 bits para seis símbolos. El ahorro es la evidencia de que Huffman produce un código eficiente.

6. Que el código sea de prefijo (ningún código es prefijo de otro) está garantizado por la construcción: los símbolos quedan siempre en las hojas del árbol, nunca en nodos internos, así que el camino raíz-hoja de un símbolo nunca es el comienzo del camino de otro. Esto permite decodificar sin ambigüedad leyendo bit a bit y bajando por el árbol hasta llegar a una hoja.

7. La optimalidad de Huffman descansa en un argumento voraz: en un código óptimo, los dos símbolos de menor frecuencia pueden situarse como hermanos en el nivel más profundo, así que combinarlos primero nunca empeora la solución. Repetir ese paso sobre el problema reducido mantiene la optimalidad. La cola de prioridad es justo lo que hace eficiente esa elección voraz repetida del par de menor frecuencia.

## Bloque 6 - Treap, rotaciones y comparación con BST y heap

1. Un Treap es un árbol de búsqueda que mantiene dos invariantes simultáneos. Por las claves cumple la propiedad BST: para cada nodo, las claves a la izquierda son menores y las de la derecha mayores. Por las prioridades, asignadas al azar a cada nodo al insertarlo, cumple la propiedad de max-heap: la prioridad de un nodo es mayor o igual que la de sus hijos. La clave decide la posición horizontal y la prioridad decide la posición vertical.

2. La prioridad aleatoria sirve para mantener el árbol balanceado en promedio sin reglas de balance explícitas. Dado un conjunto de claves, una vez fijadas las prioridades el Treap queda determinado de forma única, y como las prioridades son aleatorias, el árbol que resulta es, en distribución, el mismo que se obtendría insertando las claves en orden aleatorio en un BST. Eso da altura esperada `O(log n)` y, por tanto, búsquedas, inserciones y borrados esperados en `O(log n)`.

3. La búsqueda en un Treap ignora por completo la prioridad: es una búsqueda BST normal que compara la clave buscada con la del nodo y baja a la izquierda o a la derecha. Las prioridades no participan porque solo gobiernan la forma, no el orden de las claves. Por eso el recorrido inorden de un Treap, igual que el de cualquier BST, entrega las claves ordenadas de menor a mayor.

4. La inserción en un Treap tiene dos fases. Primero se inserta la clave como en un BST cualquiera, descendiendo hasta la hoja correspondiente, y se le asigna una prioridad aleatoria. Eso puede violar la propiedad de heap si el nuevo nodo tiene más prioridad que su padre. La segunda fase la repara con rotaciones: mientras la prioridad del nodo supere la de su padre, se rota para subirlo, sin alterar nunca el orden de las claves. El nodo asciende hasta que su prioridad ya no excede la del padre.

5. El borrado es la imagen espejo de la inserción. Para eliminar un nodo se lo rota hacia abajo, eligiendo en cada paso subir al hijo de mayor prioridad, hasta que el nodo queda como hoja, y entonces se desconecta. Rotar hacia abajo mantiene la propiedad de heap en el resto del árbol y, como toda rotación, conserva el orden de las claves.

6. Ejemplo de Treap con claves `1, 3, 5, 8` y prioridades `1:20, 3:40, 5:60, 8:30` (max-heap por prioridad):

   ```
           5 (pri 60)
          /      \
       3 (40)   8 (30)
       /
     1 (20)
   ```

   Por clave es un BST válido (inorden `1 3 5 8`) y por prioridad es un max-heap (`60 >= 40, 30` y `40 >= 20`). El nodo de mayor prioridad, la clave `5`, quedó en la raíz, y dentro de cada subárbol se repite la regla.

7. Una rotación reestructura localmente tres bloques manteniendo su orden relativo, por eso preserva el inorden. En `rotateRight(u)` con `w = u->left`, si llamamos `A` al subárbol izquierdo de `w`, `B` al derecho de `w` y `C` al derecho de `u`, antes se cumple `A < w < B < u < C`. Después de la rotación `w` sube y `u` baja a su derecha, pero la relación sigue siendo `A < w < B < u < C`: ningún elemento cruzó a un lado prohibido. El inorden es por tanto invariante ante rotaciones, y eso es lo que permite usarlas tanto en un BST balanceado como en un Treap sin romper el orden.

8. Las rotaciones son el puente entre el BST de la semana anterior y el Treap. El demo de rotaciones lo muestra sobre un caso pequeño: rotar la raíz de un BST cambia qué nodo está arriba pero deja el inorden idéntico. El Treap aprovecha exactamente esa propiedad: usa rotaciones para acomodar las prioridades sin tocar nunca el orden de las claves, que es lo que garantiza que sigue siendo un árbol de búsqueda correcto.

9. La diferencia entre heap, BST y Treap se resume así. Un heap solo mantiene prioridad vertical y responde rápido por el máximo, pero no permite búsqueda ordenada. Un BST mantiene orden total entre claves y permite búsqueda dirigida y recorrido ordenado, pero su forma depende del orden de inserción y puede degenerar. Un Treap combina ambas cosas: conserva la búsqueda ordenada del BST y usa una prioridad de tipo heap, pero aleatoria, para evitar la degeneración sin reglas de balance complicadas.

10. Treap reutiliza la idea de prioridad de un modo distinto al de una cola de prioridad. En una cola de prioridad la prioridad es el dato que importa y sirve para entregar el máximo. En un Treap la prioridad es un valor auxiliar y aleatorio, invisible para el usuario, cuyo único propósito es dar forma al árbol. Es la misma propiedad de heap, pero puesta al servicio del balance en lugar de al servicio de la extracción del extremo.

## Bloque 7 - Demos

| Archivo | Salida u observable importante | Idea estructural | Argumento de costo, espacio o diseño |
| ------- | ------------------------------ | ---------------- | ------------------------------------ |
| `demo_pq_complheap_basico.cpp` | `getMax: 9` tras insertar `4 3 8 1 9 2 7`, y secuencia de `delMax`: `9 8 7 4 3 2 1` | Cola de prioridad máxima sobre arreglo implícito | `getMax` en `O(1)`, `insert` y `delMax` en `O(log n)`; sin punteros, todo por aritmética de índices |
| `demo_heapify_floyd.cpp` | `Heapify: 9 4 8 1 3 2 7` desde el vector `4 3 8 1 9 2 7` | Construcción de heap de abajo hacia arriba | `heapify` en `O(n)`, más barato que las `O(n log n)` de insertar uno por uno |
| `demo_heapsort.cpp` | `Ordenado: 1 2 3 4 7 8 9` | Reuso de la propiedad de heap para ordenar en sitio | Construcción `O(n)` más `n` extracciones `O(log n)`, total `O(n log n)`, memoria auxiliar `O(1)` |
| `demo_left_heap_merge.cpp` | `Maximo tras merge: 9`, y secuencia de `delMax` decreciente | Heap izquierdista con `merge` como operación base | `merge`, `insert` y `delMax` en `O(log n)` recorriendo solo el borde derecho corto |
| `demo_huffman.cpp` | Total de bits `224` para las frecuencias `a:5 b:9 c:12 d:13 e:16 f:45`, códigos de prefijo en las hojas | Construcción voraz con cola de prioridad uniforme | Cada paso extrae e inserta en `O(log n)`, total `O(n log n)`; código de prefijo óptimo |
| `demo_compare_with_semana5.cpp` | El máximo desde PQ_ComplHeap coincide con la raíz de BinaryHeap (visto como max-heap), el BST entrega inorden ordenado | Coexistencia de las estructuras de prioridad con las de Semana5 | El heap responde por el extremo en `O(1)`, el BST entrega orden con un inorden sin destruirse |
| `demo_bst_rotations.cpp` | Rotar la raíz cambia qué nodo está arriba pero el inorden no cambia | Rotaciones que preservan el orden de las claves | Rotación local en `O(1)`; el inorden es invariante, lo que habilita el balance |
| `demo_treap_basico.cpp` | Inorden por clave ordenado e invariante de prioridad de max-heap respetado tras insertar y borrar | Árbol que mezcla orden BST y prioridad aleatoria | Operaciones esperadas en `O(log n)` por la altura esperada logarítmica |
| `demo_capitulo6_panorama.cpp` | Recorrido panorámico que conecta cola de prioridad, heap, Huffman y Treap | Vista integradora de la semana bajo la idea común de prioridad | Cada estructura muestra dónde la prioridad ayuda y dónde no alcanza |

1. En `demo_pq_complheap_basico.cpp` lo que defiende que el máximo queda en la raíz es la línea `getMax: 9` justo después de insertar `4 3 8 1 9 2 7`: el `9` subió hasta la posición 0 al insertarse, y la posterior secuencia de `delMax` decreciente `9 8 7 4 3 2 1` confirma que cada extracción entrega el máximo actual y restaura la propiedad.

2. En `demo_heapify_floyd.cpp` la evidencia de que `heapify` construye un heap válido es comparar el vector de entrada `4 3 8 1 9 2 7` con la salida `9 4 8 1 3 2 7`: la entrada estaba desordenada y, tras un solo recorrido de abajo hacia arriba, el máximo `9` quedó en la raíz y se cumple la propiedad en todos los nodos, sin haber insertado uno por uno.

3. En `demo_heapsort.cpp` el observable es directamente la línea `Ordenado: 1 2 3 4 7 8 9`. Que un arreglo desordenado quede creciente reutilizando solo la propiedad de heap (construir y extraer máximos hacia el final) muestra que heapsort ordena en sitio sin estructuras auxiliares.

4. En `demo_left_heap_merge.cpp` lo que distingue al heap izquierdista es que mezclar dos heaps produce de inmediato un heap válido con `Maximo tras merge: 9`, y que tanto insertar como retirar se ven como casos de `merge`. La secuencia decreciente de `delMax` confirma que la prioridad se mantuvo tras cada mezcla.

5. En `demo_huffman.cpp` la cifra que resume la optimalidad es el total `224` bits frente a los `300` de un código fijo de 3 bits. Que los símbolos más frecuentes (`f` con código `0`) tengan los códigos más cortos y los raros (`a`, `b` con 4 bits) los más largos es lo que produce el ahorro.

6. En `demo_compare_with_semana5.cpp` la comparación clave es que la cola de prioridad y la estructura de búsqueda responden preguntas distintas sobre los mismos datos: el heap entrega el extremo de inmediato pero no sabe recorrer en orden, mientras que el BST entrega el inorden ordenado sin destruirse. Esa dualidad es la idea central que conecta la semana con la anterior.

7. En `demo_bst_rotations.cpp` y `demo_treap_basico.cpp` el observable compartido es que el inorden por clave no cambia. En el primero, rotar la raíz reordena la estructura pero deja el inorden intacto; en el segundo, tras insertar y borrar en el Treap, el inorden sigue ordenado y además se respeta la prioridad de max-heap. Juntos muestran que las rotaciones son seguras para el orden y que el Treap aprovecha justo eso.

## Bloque 8 - Pruebas e invariantes

1. La prueba pública valida sobre PQ_ComplHeap: que `insert` seguido de `getMax` devuelva siempre el máximo presente, que `delMax` retire ese máximo y deje un heap válido, y que retirar repetidamente hasta vaciar produzca la secuencia decreciente de los elementos. El invariante de fondo que se comprueba es la propiedad de max-heap tras cada operación.

2. El caso que valida `heapify` es construir el heap desde un vector desordenado y comprobar, con una verificación de la propiedad, que cada padre es mayor o igual que sus hijos, y que `getMax` coincide con el máximo del conjunto. Esto separa la corrección de la construcción de golpe de la corrección de las inserciones una a una.

3. La prueba de heapsort verifica que, para una entrada desordenada, la salida quede ordenada de forma creciente y sea una permutación de la entrada (mismos elementos, ningún valor perdido ni duplicado de más). Ordenar en sitio sin perder elementos es el invariante que se valida.

4. Sobre el heap izquierdista, la prueba comprueba dos cosas: que `merge` de dos heaps válidos produzca un heap cuyo máximo sea el mayor de los dos máximos de entrada, y que retirar repetidamente entregue los elementos en orden decreciente. De paso valida que la propiedad izquierdista `npl(left) >= npl(right)` se mantenga, que es lo que garantiza el costo logarítmico.

5. La prueba de Huffman valida que el resultado sea un código de prefijo (ningún código es prefijo de otro, porque los símbolos están en hojas) y que el total de bits coincida con el esperado para las frecuencias dadas, por ejemplo `224` para el conjunto del bloque 5. También puede comprobar que codificar y luego decodificar recupere el texto original.

6. La prueba del Treap valida los dos invariantes a la vez: que el recorrido inorden por clave quede ordenado (propiedad BST) y que cada nodo tenga prioridad mayor o igual que la de sus hijos (propiedad de heap). Verifica además que tras insertar y borrar ambos invariantes se conserven, porque son justo las operaciones que más reestructuran el árbol con rotaciones.

7. La prueba de rotaciones comprueba que rotar a la izquierda y a la derecha cambie la raíz local pero preserve el inorden y la propiedad BST, y que una rotación seguida de su inversa devuelva el árbol al estado original. El invariante validado es que el orden de las claves no se altera nunca.

8. Las pruebas internas agregan cobertura que la pública no expone de forma directa: el comportamiento de `percolateUp` y `percolateDown` por separado, la construcción con `heapify` frente a la construcción por inserciones sobre el mismo conjunto para comprobar que producen heaps con el mismo máximo, mezclas encadenadas de varios heaps izquierdistas, y árboles Treap construidos con prioridades fijas para verificar una forma esperada concreta.

9. Pasar las pruebas públicas demuestra que, para los casos concretos probados, las operaciones producen los resultados correctos: el máximo es el esperado, las extracciones salen en orden, Huffman da el total de bits previsto y el Treap mantiene sus dos invariantes. Es evidencia real de que la implementación funciona en escenarios representativos.

10. Pasar las pruebas no demuestra corrección en todos los casos: no cubre todos los tamaños ni todas las distribuciones de prioridades aleatorias del Treap, no agota todas las formas de mezcla de heaps izquierdistas, no garantiza ausencia de fugas de memoria en las estructuras enlazadas, ni mide la complejidad real. Por eso la defensa debe apoyarse también en los invariantes (propiedad de heap, propiedad BST, `npl`) y en el análisis de costo (`O(1)` para `getMax`, `O(log n)` para `insert`, `delMax` y `merge`, `O(n)` para `heapify`).

## Bloque 9 - Cierre comparativo

¿Qué cambia cuando pasamos de estudiar árboles binarios, heaps y árboles de búsqueda a diseñar colas de prioridad, heaps especializados, Huffman y Treap?

El cambio central es pasar de mirar estructuras por su forma a mirarlas por el servicio que prestan. La cola de prioridad no es una estructura concreta sino un contrato, y la semana se organiza alrededor de separar ese contrato de las muchas formas de cumplirlo.

- Interfaz frente a representación: PQ define qué se puede pedir (insertar, consultar el máximo, retirarlo) y PQ_ComplHeap y PQ_LeftHeap son dos maneras distintas de cumplirlo. Poder cambiar la representación sin tocar al cliente, como hace Huffman_PQ, es la lección de diseño más importante de la semana.

- Heap completo sobre arreglo: PQ_ComplHeap aprovecha la forma de árbol completo para vivir en un arreglo sin punteros, con `getMax` en `O(1)`, `insert` y `delMax` en `O(log n)` mediante `percolateUp` y `percolateDown`, y construcción de golpe en `O(n)` con `heapify` de Floyd. Es la representación por defecto cuando no se necesita mezclar.

- Heapsort como reuso de la propiedad: la misma propiedad que da el máximo en la raíz permite ordenar en sitio, construyendo el heap y extrayendo máximos hacia el final del arreglo. Es un ejemplo de cómo un invariante diseñado para una cosa (prioridad) resuelve otra (ordenamiento) sin memoria extra.

- Heap izquierdista y la centralidad de `merge`: cuando el problema pide combinar colas de prioridad, el arreglo implícito ya no conviene y entra el heap izquierdista, que mantiene corto el borde derecho con `npl` y reduce insertar y retirar a casos de `merge`, todo en `O(log n)`. Es un cambio de representación motivado por una operación nueva.

- Huffman como cliente de la cola de prioridad: el algoritmo voraz de Huffman necesita extraer repetidamente los dos elementos de menor frecuencia, y eso es exactamente lo que una cola de prioridad ofrece barato. El resultado, un código de prefijo óptimo con los símbolos en las hojas, conecta una estructura de datos con un problema clásico de compresión.

- Treap como reutilización distinta de la prioridad: el Treap toma la propiedad de heap pero la pone al servicio del balance, no de la extracción. Mantiene orden BST por clave y prioridad aleatoria de tipo heap, y usa rotaciones (que preservan el inorden) para conservar ambos invariantes. Es el puente hacia los árboles balanceados de las semanas siguientes.

- Evidencia para defender correctitud: usaría las pruebas públicas e internas como verificación de casos concretos, los demos como observación del comportamiento (máximo en la raíz, secuencia de extracción decreciente, total de bits de Huffman, inorden ordenado del Treap), los invariantes (propiedad de heap, propiedad BST, `npl`) como garantía de lo que se mantiene siempre, las trazas manuales de `percolateUp`, `heapify` y la construcción de Huffman como explicación paso a paso, y el análisis de costo (`O(1)`, `O(log n)`, `O(n)`, `O(n log n)`) como argumento de eficiencia. Las pruebas muestran que funciona en los casos probados; los invariantes y la complejidad explican por qué funciona en general.

## Autoevaluación breve

- Qué puedo defender con seguridad: la diferencia entre la interfaz PQ y sus dos representaciones, por qué el máximo de un max-heap queda en la raíz, cómo `percolateUp` y `percolateDown` reparan la propiedad, por qué `heapify` de Floyd es `O(n)` frente a las `O(n log n)` de insertar uno por uno, por qué Huffman necesita una cola de prioridad, y cómo el Treap combina orden BST con prioridad aleatoria sin romper el inorden gracias a las rotaciones.
- Qué todavía confundo: los detalles finos del restablecimiento de la propiedad izquierdista dentro de `merge` (cuándo exactamente se intercambian los hijos y cómo se recalcula `npl` al regresar de la recursión), y la suma exacta de la serie que justifica el costo lineal de `heapify`.
- Qué evidencia usaría en una sustentación: la salida de `demo_pq_complheap_basico.cpp` con la secuencia de extracción decreciente, el total de `224` bits de `demo_huffman.cpp` frente a los `300` de un código fijo, y el inorden ordenado del Treap junto con la verificación de su propiedad de prioridad tras insertar y borrar.
- Qué parte del código me parece más importante para revisar otra vez: la operación `merge` del heap izquierdista y la inserción y borrado del Treap, porque son las que más punteros y rotaciones reescriben y donde es más fácil romper un invariante sin que las pruebas más simples lo noten.
