# Actividad 5 - Semana 5

Estudiante:

- Leonardo Gabriel Estacio Huaricapcha

## Bloque 1 - Núcleo conceptual

1. Un árbol binario enlazado guarda cada elemento en un nodo creado con `new` que conoce a sus vecinos por punteros (`parent`, `left`, `right`), de modo que la forma del árbol vive en esos enlaces y no en posiciones de memoria contiguas. Un árbol almacenado implícitamente en un arreglo no usa punteros: el lugar de cada nodo dentro del vector codifica su posición en el árbol, y las relaciones padre-hijo se calculan con aritmética de índices. En la semana esto se ve en la diferencia entre BinNode/BinTree, que enlazan nodos, y BinaryHeap, que vive en un std::vector y obtiene los hijos con `left(i) = 2*i+1` y `right(i) = 2*i+2`. La representación enlazada admite cualquier forma de árbol a costa de pagar punteros; la implícita ahorra punteros pero exige que el árbol sea casi completo para no desperdiciar espacio.

2. Un BinNode guarda cinco piezas de información: el dato `data` que almacena el nodo, un puntero `parent` al nodo padre (es `nullptr` en la raíz), un puntero `left` al hijo izquierdo, un puntero `right` al hijo derecho, y un entero `height` con la altura del subárbol que cuelga de ese nodo. Los tres punteros sostienen la estructura del árbol en ambos sentidos (hacia abajo con `left`/`right`, hacia arriba con `parent`), y `height` es información derivada que se cachea en el nodo para no recalcularla a cada consulta, manteniéndose con `updateHeight`.

3. El puntero `parent` permite subir por el árbol, y eso es lo que habilita operaciones que necesitan mirar a los ancestros. En `succ()`, cuando un nodo no tiene hijo derecho, el sucesor inorden no está debajo sino arriba: hay que subir mientras el nodo sea hijo derecho (`isRightChild()`) y devolver el primer padre al que se llega como hijo izquierdo; sin `parent` ese ascenso sería imposible. Lo mismo ocurre simétricamente en `pred()`. Y en el mantenimiento de alturas, `updateHeightAbove` arranca en un nodo y recorre `node = node->parent` hasta la raíz, recalculando cada altura en el camino: una inserción o borrado solo puede afectar a los ancestros, y `parent` es justamente el enlace que permite visitarlos.

4. BinTree es el dueño del árbol completo, mientras que BinNode solo conoce su vecindad local. BinTree guarda la raíz `root_` y el tamaño `size_`, y se encarga de las operaciones que afectan al árbol como un todo: crear la raíz (`insertAsRoot`), insertar hijos manteniendo el conteo y las alturas (`insertAsLC`/`insertAsRC` con `updateHeightAbove`), adjuntar y separar subárboles (`attachAsLC`, `attachAsRC`, `secede`, `removeSubtree`), liberar memoria (`clear`/`destroy`) y validar consistencia (`checkParentLinks`). En cambio BinNode solo sabe enlazar un hijo (`insertAsLC` a nivel de nodo), recorrer su propio subárbol y calcular su sucesor o predecesor. La responsabilidad de BinTree es la coherencia global (tamaño, raíz, alturas, memoria); la de BinNode es el comportamiento local de un nodo.

5. BinaryTree hereda de BinTree y agrega la capa de navegación y consulta al estilo Morin, sin cambiar la representación. Añade `depth(u)` (subiendo por `parent`), `height(u)` y `subtreeSize(u)` (bajando recursivamente), los accesos por orden inorden `firstNode`, `lastNode`, `nextNode`, `prevNode`, los iteradores `iterator` y `node_iterator` que recorren el árbol en inorden usando `succ()`, las utilidades `iterateBySuccessor`/`iterateByPredecessor` y la representación textual `asciiArt()`. Es decir, BinTree da la infraestructura enlazada y BinaryTree da las herramientas para moverse por ella y observarla.

6. Lo que convierte un árbol binario en un árbol binario de búsqueda es una propiedad de orden: para todo nodo, todas las claves de su subárbol izquierdo son menores que su clave y todas las del subárbol derecho son mayores. En BinarySearchTree esa propiedad se mantiene en la inserción (`findLast` localiza el lugar comparando con `comp_` y `addChild` cuelga el nodo del lado correcto) y se verifica con `isBST`, que comprueba recursivamente que cada nodo respete una cota inferior y una superior. La estructura del árbol no cambia respecto a un BinaryTree; lo que se añade es la disciplina de dónde puede ir cada valor.

7. Lo que convierte un arreglo en un heap mínimo es la propiedad de prioridad: cada nodo es menor o igual que sus dos hijos, donde los hijos del índice `i` son `2*i+1` y `2*i+2`. Esto implica que el mínimo global queda siempre en la raíz, el índice 0. En BinaryHeap la propiedad se restablece con `bubbleUp` tras insertar y con `trickleDown` tras extraer, se construye de golpe con `heapify`, y se verifica con `isHeap`/`isHeapArray`. No se exige ningún orden entre hermanos ni entre primos: solo la relación vertical padre-hijo.

8. La propiedad de orden de un BST es total y horizontal: relaciona cada nodo con todo su subárbol izquierdo y todo su derecho, lo que ordena las claves de izquierda a derecha y permite búsqueda dirigida. La propiedad de prioridad de un heap es parcial y vertical: solo relaciona cada nodo con sus hijos inmediatos, sin decir nada sobre el orden entre subárboles hermanos. Por eso un BST sabe encontrar cualquier clave en tiempo proporcional a su altura, mientras que un heap solo sabe responder de inmediato por el extremo (el mínimo en la raíz) pero no permite buscar una clave arbitraria mejor que recorriendo todo.

9. El recorrido inorden de un BST produce una secuencia ordenada porque el inorden visita, para cada nodo, primero todo su subárbol izquierdo, luego el nodo, y luego todo su subárbol derecho. Como la propiedad BST garantiza que el subárbol izquierdo contiene solo claves menores y el derecho solo mayores, ese orden de visita coincide exactamente con el orden creciente de las claves. La evidencia directa está en el demo: insertando `7, 3, 10, 1, 5, 8, 12, 4, 6` el inorden imprime `1 3 4 5 6 7 8 10 12`.

10. Un heap no permite recorrer sus elementos en orden sin destruirlo o copiarlo porque su propiedad de prioridad solo ordena verticalmente: saber que cada padre es menor que sus hijos no dice cuál de dos hermanos va antes, así que no existe un recorrido del arreglo que entregue las claves ordenadas. La única forma de obtener el orden es extraer repetidamente el mínimo con `remove()`, pero cada extracción reduce el heap, de modo que vaciarlo destruye la estructura. Si se quiere conservar el heap hay que trabajar sobre una copia. Esto contrasta con el BST, donde un simple inorden lee el orden sin tocar la estructura.

## Bloque 2 - Navegación, altura, profundidad y tamaño

1. En BinNode esos predicados consultan la vecindad del nodo: `hasLeft()` y `hasRight()` indican si el nodo tiene hijo izquierdo o derecho (comparan `left`/`right` contra `nullptr`); `isRoot()` indica si el nodo no tiene padre (`parent == nullptr`); `isLeaf()` indica si no tiene ningún hijo (ni `left` ni `right`); `isLeftChild()` indica que el nodo tiene padre y además es el hijo izquierdo de ese padre (`parent->left == this`); e `isRightChild()` es lo simétrico (`parent->right == this`). Son la base con la que se deciden los casos de `succ()`, `pred()` y las rotaciones.

2. `succ()` baja al subárbol derecho cuando el nodo sí tiene hijo derecho. En ese caso el sucesor inorden es la menor de las claves mayores que el nodo, y esa menor clave es el nodo más a la izquierda del subárbol derecho. El código entra a `right` y luego avanza `s = s->left` mientras pueda, devolviendo ese extremo izquierdo. Por ejemplo, en el árbol del demo, `succ(7)` baja a `10` y luego al más a la izquierda de su subárbol, que es `8`.

3. `succ()` sube por los ancestros cuando el nodo no tiene hijo derecho. En ese caso ya se visitaron todas las claves menores o iguales que están debajo, y el sucesor es el primer ancestro hacia el que el nodo aparece "por la izquierda". El código sube mientras el nodo actual sea hijo derecho (`while (s->isRightChild()) s = s->parent`) y al detenerse devuelve `s->parent`, es decir el primer ancestro donde se hizo un giro hacia la izquierda. Por ejemplo `succ(6)` no tiene derecho: `6` es hijo derecho de `5`, `5` es hijo derecho de `3`, y `3` es hijo izquierdo de `7`, así que el sucesor es `7`.

4. `pred()` es la imagen especular de `succ()`. Si el nodo tiene hijo izquierdo, el predecesor es la mayor de las claves menores, es decir el nodo más a la derecha del subárbol izquierdo: el código entra a `left` y avanza `s = s->right` hasta el final. Si el nodo no tiene hijo izquierdo, sube por los ancestros mientras sea hijo izquierdo (`while (s->isLeftChild()) s = s->parent`) y devuelve `s->parent`, el primer ancestro al que llega "por la derecha". Por ejemplo `pred(5)` baja a `4`, y `pred(6)`, que no tiene izquierdo, sube y devuelve `5`.

5. Árbol de prueba (9 nodos, el mismo del demo):

   ```
           7
          / \
         3    10
        / \   / \
       1   5 8   12
          / \
         4   6
   ```

   Inorden: `1 3 4 5 6 7 8 10 12`.

   | Nodo | Predecesor inorden | Sucesor inorden |
   | ---- | ------------------ | --------------- |
   | 3    | 1                  | 4               |
   | 7    | 6                  | 8               |
   | 6    | 5                  | 7               |

   - `3` tiene hijo izquierdo `1` (rightmost = `1`) y hijo derecho `5` (leftmost = `4`).
   - `7` baja a la derecha hasta `8` para el sucesor, y a la izquierda hasta `6` para el predecesor.
   - `6` no tiene hijos: sube como hijo derecho hasta `7` para el sucesor, y para el predecesor su padre directo `5`.

6. `depth(u)` calcula la profundidad de un nodo, es decir el número de aristas desde la raíz hasta `u`. Puede implementarse subiendo por `parent` porque desde cualquier nodo solo hay un camino hacia la raíz, y ese camino tiene exactamente tantas aristas como la profundidad. En BinaryTree::depth el código hace `node = node->parent` contando pasos hasta llegar a `root_`. Para el nodo `5` del demo, `depth(5) = 2` (`5 -> 3 -> 7`).

7. `height(u)` calcula la altura del subárbol que cuelga de `u`, es decir la longitud del camino más largo desde `u` hasta una hoja descendiente. Suele implementarse bajando recursivamente porque la altura de un nodo se define en términos de la de sus hijos: `height(u) = 1 + max(height(left), height(right))`, con `height(nullptr) = -1`. Cada hijo necesita resolverse antes que el padre, y la recursión sobre `left` y `right` expresa exactamente esa dependencia.

8. `subtreeSize(u)` cuenta cuántos nodos hay en el subárbol con raíz en `u`, incluido `u`. Se calcula recursivamente como `1 + subtreeSize(left) + subtreeSize(right)`, con `subtreeSize(nullptr) = 0`. Para la raíz del árbol de prueba devuelve 9, que es el total de nodos.

9. Sea `u` un nodo cualquiera. Por definición existe una hoja `L` descendiente de `u` cuya distancia a `u` es exactamente `height(u)` aristas (el camino más largo hacia abajo desde `u`). La distancia de la raíz a esa hoja es entonces `depth(u) + height(u)`, porque el camino raíz-a-`L` pasa por `u` y se compone del tramo raíz-a-`u` (de `depth(u)` aristas) más el tramo `u`-a-`L` (de `height(u)` aristas). Ahora bien, `height(T)` es la mayor profundidad alcanzada por cualquier hoja del árbol, así que `height(T) >= depth(L) = depth(u) + height(u)`. Por lo tanto `depth(u) + height(u) <= height(T)` para todo nodo `u`.

10. La igualdad `depth(u) + height(u) = height(T)` se alcanza si y solo si `u` está sobre algún camino raíz-a-hoja de longitud máxima, es decir un camino que realiza la altura del árbol. La condición es necesaria porque la suma iguala a `height(T)` solo cuando la hoja más profunda bajo `u` es también la hoja más profunda de todo el árbol; y es suficiente porque, si `u` pertenece a un camino más largo, su tramo de bajada llega hasta la hoja que define `height(T)`, completando la suma. En particular la raíz siempre cumple la igualdad (`depth = 0`, `height(raíz) = height(T)`) y toda hoja del nivel más profundo también.

## Bloque 3 - Recorridos y trazado guiado

Árbol de prueba (el del demo): inorden `1 3 4 5 6 7 8 10 12`.

| Recorrido | Versión revisada | Estructura auxiliar usada | Secuencia producida | Argumento de correctitud y costo |
| --------- | ---------------- | ------------------------- | ------------------- | -------------------------------- |
| Preorden recursivo | `travPre` | Pila de llamadas implícita | `7 3 1 5 4 6 10 8 12` | Visita raíz, luego izquierdo, luego derecho; cada nodo se visita una vez en `O(n)` |
| Preorden iterativo | `travPreIterative2` | std::stack explícita | `7 3 1 5 4 6 10 8 12` | Apila derecho antes que izquierdo para que el izquierdo salga primero; `O(n)` |
| Inorden recursivo | `travInRecursive` | Pila de llamadas implícita | `1 3 4 5 6 7 8 10 12` | Izquierdo, raíz, derecho; un `visit` por nodo, `O(n)` |
| Inorden iterativo #1 | `travInIterative1` | std::stack explícita | `1 3 4 5 6 7 8 10 12` | Desciende apilando izquierdos, visita al desapilar y gira a la derecha; `O(n)` |
| Inorden iterativo #2 | `travInIterative2` | Punteros `parent` y `prev`/`curr` (sin pila) | `1 3 4 5 6 7 8 10 12` | Usa de dónde se viene (`prev`) para decidir el siguiente; espacio `O(1)`, tiempo `O(n)` |
| Inorden iterativo #3 | `travInIterative3` | `succ()` y punteros `parent` (sin pila) | `1 3 4 5 6 7 8 10 12` | Va de `leftmost()` a `rightmost()->succ()` usando sucesor; `O(n)` amortizado |
| Postorden recursivo | `travPost` | Pila de llamadas implícita | `1 4 6 5 3 8 12 10 7` | Izquierdo, derecho, raíz; `O(n)` |
| Postorden iterativo | `travPostIterative` | Dos std::stack (`s1`, `s2`) | `1 4 6 5 3 8 12 10 7` | Produce un preorden invertido (raíz-derecha-izquierda) en `s2` y lo vacía; `O(n)` |
| Recorrido por niveles | `travLevel` | std::queue | `7 3 10 1 5 8 12 4 6` | Procesa por capas encolando hijos; cada nodo entra y sale una vez, `O(n)` |

1. Visitar un nodo en preorden significa procesarlo antes que a sus dos subárboles: primero la raíz, después todo el subárbol izquierdo y después todo el derecho. Es el orden en que se "descubre" cada nodo al bajar.

2. Visitar un nodo en inorden significa procesarlo entre sus dos subárboles: primero todo el subárbol izquierdo, luego el nodo, luego el derecho. En un BST este orden entrega las claves ordenadas.

3. Visitar un nodo en postorden significa procesarlo después de sus dos subárboles: primero el izquierdo, luego el derecho y al final el nodo. Es el orden natural para liberar o agregar de abajo hacia arriba, porque cuando se visita un nodo sus hijos ya fueron tratados.

4. Visitar un árbol por niveles significa recorrerlo capa por capa, de arriba hacia abajo y dentro de cada nivel de izquierda a derecha: primero la raíz, luego sus hijos, luego los nietos, y así. Es un recorrido en anchura que se apoya en una cola.

5. Los recorridos recursivos son `O(n)` porque cada nodo se alcanza exactamente una vez y en cada visita se hace una cantidad constante de trabajo (comparar punteros, llamar `visit`, descender). Con `n` nodos y trabajo constante por nodo, el total es proporcional a `n`.

6. Las versiones iterativas también son `O(n)` porque, aunque cambian la pila implícita por una pila o cola explícita o por punteros, no alteran la cantidad de visitas: cada nodo se apila y desapila (o se alcanza por sucesor) un número acotado de veces. El recuento total de operaciones sigue siendo proporcional a `n`.

7. La memoria auxiliar de un recorrido recursivo en un árbol balanceado es `O(log n)`, porque la profundidad de la pila de llamadas iguala a la altura del árbol, y un árbol balanceado tiene altura `O(log n)`.

8. La memoria auxiliar de un recorrido recursivo en un árbol degenerado es `O(n)`, porque el árbol se comporta como una lista: su altura es `n-1`, así que la pila de llamadas acumula un marco por cada nodo del único camino largo.

9. Usar una pila explícita y usar la pila de llamadas dan el mismo orden y la misma complejidad asintótica, pero difieren en el control. La pila de llamadas la administra el lenguaje y su tamaño máximo lo fija el sistema, por lo que un árbol muy profundo puede provocar desbordamiento de pila; la pila explícita la administramos nosotros sobre el heap del programa, podemos inspeccionarla, pausar el recorrido o reanudarlo, y solo está limitada por la memoria disponible. La explícita cambia recursión por iteración haciendo visible el estado que antes estaba oculto.

10. La cola del recorrido por niveles crece según el ancho máximo del árbol, no su altura. En un árbol completo el último nivel concentra cerca de la mitad de los nodos, así que en algún momento la cola llega a contener del orden de `n/2` nodos, es decir `O(n)`. En un árbol degenerado cada nivel tiene un solo nodo, de modo que la cola nunca guarda más de uno o dos a la vez, es decir `O(1)`. Por eso el costo en memoria del recorrido por niveles depende de la forma del árbol y es opuesto al de la recursión: la recursión sufre con los árboles profundos y la cola sufre con los anchos.

## Bloque 4 - Demos

| Archivo | Salida u observable importante | Idea estructural | Argumento de costo, espacio o diseño |
| ------- | ------------------------------ | ---------------- | ------------------------------------ |
| `demo_binary_tree.cpp` | Los cuatro recorridos, `Sucesor de 5: 6`, `Predecesor de 5: 4`, `Altura estructural: 3`, `Profundidad de 5: 2`, `Parent links OK: si` | Árbol binario enlazado navegable con `succ`/`pred`, alturas y `asciiArt` | Recorridos en `O(n)`; alturas cacheadas en cada nodo; `checkParentLinks` valida la consistencia de los enlaces |
| `demo_bst.cpp` | `BST inorden: 1 3 4 5 6 7 8 10 12`, `findEQ(5): 5`, `lowerBound(9): 10`, `upperBound(8): 10`, `Tras remove(3): 1 4 5 6 7 8 10 12`, `isBST: si` | BST con búsqueda dirigida, borrado por sucesor y rotaciones | Búsqueda y borrado en `O(altura)`; el inorden ordenado evidencia la propiedad de orden |
| `demo_heap.cpp` | `Heapify: 1 3 2 7 5 8 10`, `Tras add(0): 0 1 2 3 5 8 10 7`, `remove() -> 0`, `Secuencia ordenada por extraccion: 1 2 3 5 7 8 10` | Heap mínimo implícito en std::vector | `heapify` en `O(n)`, `add`/`remove` en `O(log n)`; sin punteros, todo por aritmética de índices |
| `demo_capitulo5_panorama.cpp` | `Heap minimo actual: 1`, `Raiz BST: 9`, `Altura BST: 2`, `Recorrido STL-like: 2 4 7 9 10 12 15` | Coexistencia de heap y BST sobre la misma base de árbol | El heap responde por el mínimo en `O(1)`; el BST entrega orden con un inorden, sin destruir la estructura |

1. En `demo_binary_tree.cpp` lo que permite verificar el orden de visita son las nueve líneas de recorridos impresas sobre el mismo árbol: el preorden da `7 3 1 5 4 6 10 8 12`, el inorden `1 3 4 5 6 7 8 10 12`, el postorden `1 4 6 5 3 8 12 10 7` y los niveles `7 3 10 1 5 8 12 4 6`. Que las versiones recursivas e iterativas de cada recorrido produzcan la misma secuencia confirma que todas visitan los nodos en el orden esperado.

2. La parte que permite defender que `succ()` y `pred()` respetan el orden inorden son las líneas `Sucesor de 5: 6` y `Predecesor de 5: 4`, junto con `Iteracion por sucesor: 1 3 4 5 6 7 8 10 12`. En el inorden, `5` aparece entre `4` y `6`, exactamente como reportan `pred` y `succ`; y recorrer todo el árbol saltando de `succ` en `succ` reproduce el inorden completo, lo que muestra que el sucesor implementa el paso siguiente del orden.

3. La representación ASCII (`asciiArt()`) produce evidencia de la forma real del árbol: muestra que `7` es la raíz, que su subárbol izquierdo cuelga de `3` y el derecho de `10`, y dónde quedan las hojas. Sirve para confrontar la estructura que uno cree haber construido con la que efectivamente quedó en memoria, lo que es útil tanto para depurar como para sustentar oralmente por qué un recorrido da cierta secuencia.

4. En `demo_bst.cpp` el observable que defiende que el inorden queda ordenado es directamente la línea `BST inorden: 1 3 4 5 6 7 8 10 12`: las claves se insertaron en desorden (`7, 3, 10, 1, 5, 8, 12, 4, 6`) y aun así el inorden las entrega crecientes, lo que solo puede ocurrir si la propiedad BST se mantuvo en cada inserción. La línea `Tras remove(3): 1 4 5 6 7 8 10 12` refuerza que el orden se conserva incluso después de borrar.

5. Las búsquedas que mejor se distinguen en el demo son `lowerBound` y `upperBound` frente a `findEQ`. Para una clave presente, `findEQ(5)` devuelve `5`. Para una clave ausente, `findEQ(9)` fallaría, pero `lowerBound(9)` devuelve `10` (el primer valor mayor o igual). Y `upperBound(8)` devuelve `10` aunque `8` sí exista, porque busca el primer valor estrictamente mayor. El contraste entre `lowerBound(9) = 10`, `upperBound(8) = 10` y `findEQ(5) = 5` deja claro que `findEQ` exige coincidencia exacta, `lowerBound` admite el igual, y `upperBound` siempre salta por encima.

6. En `demo_heap.cpp` lo que defiende que el mínimo queda en la raíz son dos cosas: tras `heapify` el arreglo empieza en `1` (`Heapify: 1 3 2 7 5 8 10`), y tras `add(0)` el arreglo empieza en `0` (`Tras add(0): 0 1 2 3 5 8 10 7`), es decir el nuevo mínimo subió hasta la posición 0. La línea `remove() -> 0` confirma que extraer devuelve precisamente ese mínimo de la raíz.

7. La evidencia que distingue insertar con `add()` de construir con `heapify()` es comparar las salidas. `heapify` toma el arreglo desordenado `{7, 3, 10, 1, 5, 8, 2}` y lo reordena de una sola vez a `1 3 2 7 5 8 10`, trabajando de abajo hacia arriba en `O(n)`. En cambio `add(0)` parte de un heap ya válido y solo inserta un elemento al final, dejándolo subir con `bubbleUp` hasta `0 1 2 3 5 8 10 7`, en `O(log n)`. Uno construye el heap completo; el otro lo extiende en un elemento.

8. En `demo_capitulo5_panorama.cpp` la comparación que mejor resume la semana es heap contra BST sobre los mismos datos. El heap responde `Heap minimo actual: 1` de inmediato pero no ofrece sus elementos ordenados, mientras que el BST permite el `Recorrido STL-like: 2 4 7 9 10 12 15`, que sale ordenado sin destruir el árbol. Esa dualidad (el heap optimiza la consulta del extremo, el BST optimiza la búsqueda y el recorrido ordenado) es la idea central que conviene defender de la semana.

## Bloque 5 - Pruebas e invariantes

1. La prueba pública valida sobre el BST: inserción de claves nuevas (`add`), rechazo de duplicados, los cuatro inorden (recursivo e iterativos #1, #2, #3), `findEQ`, `find` (que actúa como `lowerBound`), `lowerBound`, `upperBound`, `minNode`, `maxNode`, la validación `isBST`, el borrado `remove` con `contains` y `checkParentLinks`, y la construcción `buildBalancedFromSorted` con su iterador inorden.

2. El caso que valida que el BST no acepta duplicados es `expect(!bst.add(5), ...)` después de haber insertado `5`: la segunda inserción de una clave ya existente debe devolver `false`. A nivel de código esto ocurre en `addChild`, que cuando `comp_` no distingue el nuevo valor del de un nodo existente cae en la rama `else` y retorna `false` sin insertar.

3. Al comparar el inorden recursivo con las versiones iterativas se verifica que las cuatro estrategias (`Recursive`, `Iterative1`, `Iterative2`, `Iterative3`) producen exactamente la misma secuencia `1 3 4 5 6 7 8 10 12`. Eso confirma que cambiar el mecanismo de recorrido (pila implícita, pila explícita, punteros `parent`, sucesor) no cambia el resultado: las cuatro implementan el mismo orden inorden.

4. De `findEQ(8)` la prueba espera que no sea `nullptr` y que el nodo encontrado tenga `data == 8`: `findEQ` debe ubicar la clave exacta `8` que sí existe en el árbol.

5. De `lowerBound(9)` se espera que devuelva el nodo con `data == 10`, porque `9` no está y `10` es el menor valor mayor o igual a `9`. De `upperBound(8)` se espera también `10`, porque `upperBound` busca el primer valor estrictamente mayor que `8`, y aunque `8` exista, lo salta.

6. `isBST()` valida la propiedad de orden completa: que para cada nodo todas las claves de su subárbol izquierdo sean menores y las del derecho mayores, recorriendo con cotas (`low`, `high`). Además, como `isBST` también llama a `checkParentLinks`, valida de paso que los enlaces `parent` sean consistentes con `left`/`right`.

7. Después de `remove(3)` se verifica que el borrado haya devuelto `true`, que el inorden siga ordenado (`1 4 5 6 7 8 10 12`), que la clave `3` ya no esté (`!contains(3)`) y que los enlaces `parent` sigan consistentes (`checkParentLinks`). Es decir, se comprueba que borrar no rompió ni el orden ni la estructura de enlaces.

8. `checkParentLinks()` valida que cada nodo apunte correctamente a su padre: recorre el árbol comprobando que `node->parent` sea exactamente el nodo desde el cual se descendió. Tras borrar, separar (`secede`) o adjuntar (`attachAsRC`) subárboles es la garantía de que los punteros hacia arriba quedaron coherentes con los punteros hacia abajo, algo que esas operaciones modifican y que un error dejaría inconsistente sin alterar necesariamente los valores.

9. Del heap la prueba pública valida: que `heapify` deje una estructura válida (`isHeap`), que extraer repetidamente con `remove()` hasta vaciar produzca la secuencia ordenada `1 2 3 5 7 8 10`, y que `isHeapArray` reconozca como válido un arreglo que ya cumple la propiedad de heap.

10. Extraer repetidamente de un min-heap hasta vaciarlo demuestra que la raíz siempre contiene el mínimo actual: como cada `remove()` devuelve ese mínimo y luego restaura la propiedad, los valores salen en orden creciente. La secuencia ordenada resultante es la evidencia de que la propiedad de prioridad se mantuvo en cada extracción.

11. De BinTree se validan: `attachAsRC` (adjuntar un subárbol y comprobar que el tamaño suma y que el subárbol fuente queda vacío), `secede` (separar un subárbol, comprobar que el árbol original descuenta los nodos, que el desprendido conserva su tamaño y que mantiene `parent links` válidos) y `removeSubtree` (eliminar y liberar el subárbol restante, dejando el árbol vacío).

12. Las pruebas internas agregan cobertura que la pública no expone de forma directa: rotaciones (`rotateRight` y `rotateLeft` sobre `{3,2,1}` comprobando que cambian la raíz pero preservan el inorden y la propiedad BST), `bubbleUp` (insertando `5,4,3,2,1` uno por uno y verificando que el mínimo sube a la raíz), `trickleDown` (a través de `remove` comprobando el nuevo mínimo y que sigue siendo heap), profundidad y altura (`depth`, `height`), tamaño de subárbol (`subtreeSize`), y sucesor y predecesor a través de `iterateBySuccessor` e `iterateByPredecessor`.

13. Pasar las pruebas públicas demuestra que, para los casos concretos probados, las operaciones producen los resultados correctos: el BST mantiene el orden tras insertar y borrar, las búsquedas devuelven los nodos esperados, el heap extrae en orden, los recorridos coinciden entre versiones y los enlaces quedan consistentes. Es evidencia real de que la implementación funciona en escenarios representativos.

14. Pasar las pruebas públicas no demuestra que la implementación sea correcta en todos los casos: no cubre todos los tamaños y formas de árbol, no prueba todos los caminos de borrado (por ejemplo borrar la raíz con dos hijos en muchas configuraciones), no garantiza ausencia de fugas de memoria, no mide la complejidad real de las operaciones, ni verifica el comportamiento ante árboles degenerados o entradas extremas.

15. Una defensa correcta debe mencionar invariantes y complejidad además de los resultados observables porque las pruebas solo muestran salidas para entradas puntuales, mientras que la corrección general descansa en las propiedades que se mantienen siempre (la propiedad BST, la propiedad de heap, la consistencia de `parent`, las alturas actualizadas) y en el costo de las operaciones (`O(altura)` en el BST, `O(log n)` en el heap, `O(n)` en los recorridos y en `heapify`). Sin razonar sobre invariantes y costo, un conjunto de pruebas que pasa podría ocultar errores conceptuales en casos no probados.

## Bloque 6 - Lectura cercana de código

1. En BinNode los invariantes entre `parent`, `left` y `right` son de reciprocidad: si un nodo tiene `left`, entonces `left->parent` debe apuntar de vuelta a ese nodo, y lo mismo con `right`; la raíz debe tener `parent == nullptr`; y un nodo no puede ser a la vez hijo izquierdo y derecho de su padre. `isLeftChild()` e `isRightChild()` dependen de que esos enlaces sean coherentes, y `checkParentLinks` existe precisamente para verificarlos.

2. `insertAsLC` e `insertAsRC` rechazan la inserción cuando el hijo correspondiente ya existe (lanzan `std::logic_error`) porque sobrescribir `left` o `right` sin más perdería el subárbol anterior: sus nodos quedarían inalcanzables, provocando una fuga de memoria y dejando enlaces `parent` colgando. Al exigir que el hijo esté vacío, la operación garantiza que solo se enlaza donde no se destruye nada.

3. `size()` de BinNode recorre el subárbol de forma recursiva: calcula el tamaño del subárbol izquierdo (`0` si no hay `left`), el del derecho (`0` si no hay `right`), y devuelve `1 + ls + rs` para contarse a sí mismo. Cada nodo se visita una vez, de modo que el costo es `O(k)` siendo `k` el tamaño del subárbol.

4. `leftmost()` parte del nodo y desciende por `left` mientras pueda, devolviendo el nodo sin hijo izquierdo, que es el de menor clave en un BST. `rightmost()` es simétrico: desciende por `right` hasta el nodo sin hijo derecho, el de mayor clave. Ambos son el extremo del subárbol en cada dirección y se usan para `firstNode`, `lastNode`, `succ` y `pred`.

5. `succ()` funciona en dos casos. Si el nodo tiene hijo derecho, baja a `right` y luego sigue por `left` hasta el fondo, devolviendo el `leftmost` del subárbol derecho (la menor de las claves mayores). Si no tiene hijo derecho, sube por `parent` mientras el nodo sea hijo derecho (`isRightChild()`) y devuelve el primer `parent` al que llega siendo hijo izquierdo; si llega hasta arriba sin ese giro, devuelve `nullptr` porque el nodo era el máximo.

6. `pred()` es el espejo de `succ()`. Si el nodo tiene hijo izquierdo, baja a `left` y luego sigue por `right` hasta el fondo, devolviendo el `rightmost` del subárbol izquierdo (la mayor de las claves menores). Si no tiene hijo izquierdo, sube por `parent` mientras el nodo sea hijo izquierdo (`isLeftChild()`) y devuelve el primer `parent` al que llega siendo hijo derecho; si llega a la raíz sin ese giro, devuelve `nullptr` porque el nodo era el mínimo.

7. En BinTree, `root_` apunta al nodo raíz del árbol (es `nullptr` si está vacío) y `size_` guarda el número total de nodos. Juntos definen el estado global del árbol: todas las operaciones que insertan, adjuntan, separan o eliminan deben actualizar `size_` y, cuando corresponde, `root_`, para que el árbol se mantenga consistente.

8. `updateHeight(Node*)` recalcula la altura de un solo nodo a partir de las alturas ya almacenadas en sus hijos: `height = 1 + max(stature(left), stature(right))`, donde `stature` devuelve `-1` para un hijo nulo. Es una operación local de costo `O(1)` que asume que las alturas de los hijos ya son correctas.

9. `updateHeightAbove(Node*)` recalcula la altura del nodo dado y luego sigue subiendo por `parent` hasta la raíz, aplicando `updateHeight` en cada paso. Sube hacia la raíz porque una inserción, borrado o adjunción solo puede cambiar la altura de los ancestros del punto modificado: ningún otro nodo se ve afectado, así que basta con propagar el recálculo por la cadena de padres.

10. `attachAsLC` (o `attachAsRC`) transfiere un subárbol completo desde otro BinTree hacia un nodo del árbol actual sin copiar nodos: enlaza `parent->left = subtree.root_`, ajusta `subtree.root_->parent = parent`, suma `subtree.size_` al tamaño propio, actualiza las alturas con `updateHeightAbove(parent)` y finalmente vacía el árbol fuente (`subtree.root_ = nullptr`, `subtree.size_ = 0`) para que no haya dos dueños de los mismos nodos. Exige que el hijo destino esté libre, para no perder un subárbol existente.

11. La diferencia entre `removeSubtree` y `secede` está en qué pasa con los nodos desprendidos. `removeSubtree` desconecta el subárbol y lo destruye con `destroy`, liberando su memoria, y devuelve cuántos nodos se eliminaron. `secede` desconecta el subárbol pero lo conserva, devolviéndolo como un nuevo BinTree independiente con su propia raíz y tamaño. Ambos descuentan los nodos del árbol original y actualizan alturas; uno libera, el otro transfiere la propiedad.

12. `secede` no debe destruir los nodos desprendidos porque su propósito es entregarlos vivos dentro de un nuevo BinTree: el árbol devuelto pasa a ser el dueño de esos nodos y el llamador podrá seguir usándolos. Liberarlos dejaría al BinTree resultante apuntando a memoria ya destruida.

13. `removeSubtree` sí debe liberar nodos porque su propósito es eliminarlos definitivamente del programa: nadie va a quedarse con ellos. Si no llamara a `destroy`, esos nodos quedarían sin ningún puntero que los alcance y su memoria se perdería (fuga).

14. `checkParentLinks()` verifica recursivamente que cada nodo tenga el `parent` correcto: que `node->parent` coincida con el nodo desde el cual se llegó en el descenso. Recorre el árbol comparando ese enlace en cada nodo y devuelve `false` ante la primera inconsistencia. Es la red de seguridad de todas las operaciones que reescriben punteros.

15. En BinaryTree, `firstNode` devuelve `root_->leftmost()` (el mínimo inorden) y `lastNode` devuelve `root_->rightmost()` (el máximo inorden). `nextNode(node)` delega en `node->succ()` y `prevNode(node)` en `node->pred()`. Es decir, la navegación por orden inorden se construye sobre los extremos del árbol y sobre el sucesor y predecesor de cada nodo.

16. Un iterador basado en `succ()` produce recorrido inorden porque empieza en `firstNode()` (el menor) y en cada `++` avanza a `succ()`, que por definición devuelve el siguiente nodo en orden inorden. Encadenar sucesores desde el mínimo recorre todas las claves en orden creciente, que es exactamente el inorden; el demo lo confirma con `Recorrido STL-like: 2 4 7 9 10 12 15`.

17. `asciiArt()` aporta una representación textual de la forma real del árbol, dibujando para cada nodo su posición relativa, sus hijos y sus hojas. Para depuración permite ver de inmediato si la estructura quedó como se esperaba tras inserciones, borrados o rotaciones; para sustentación permite mostrar visualmente por qué un recorrido da cierta secuencia o por qué una rotación cambió la raíz.

## Bloque 7 - BST

1. La propiedad BST se define así: para todo nodo `u` con clave `k`, toda clave del subárbol izquierdo de `u` es menor que `k` y toda clave del subárbol derecho de `u` es mayor que `k`. Con la relación `comp_` del código y sin duplicados, esto equivale a que `isBST` se cumpla con cotas: cada nodo respeta una cota inferior (su menor ancestro por la derecha) y una superior (su mayor ancestro por la izquierda).

2. El inorden de un BST produce una secuencia no decreciente porque visita izquierdo, nodo, derecho; como por la propiedad BST el subárbol izquierdo solo tiene claves menores y el derecho solo mayores, cada nodo se emite justo después de todas las claves menores que él y antes de todas las mayores. Aplicado recursivamente a todo el árbol, el resultado es el orden creciente de las claves.

3. Las cuatro búsquedas se diferencian así: `findEQ(x)` devuelve el nodo con clave exactamente `x`, o `nullptr` si no existe; `lowerBound(x)` devuelve el nodo con la menor clave mayor o igual a `x`; `upperBound(x)` devuelve el nodo con la menor clave estrictamente mayor que `x`; y `find(x)` es un alias de `lowerBound(x)`. La diferencia clave es que `findEQ` exige igualdad, `lowerBound` admite el igual y si no lo halla devuelve el siguiente mayor, y `upperBound` nunca devuelve el igual sino siempre el primer mayor.

4. `findEQ(x)` puede fallar aunque `lowerBound(x)` no falle porque hacen preguntas distintas. `findEQ` solo tiene éxito si `x` está en el árbol; si no, devuelve `nullptr`. `lowerBound` puede tener éxito aun cuando `x` no exista, porque devuelve el primer valor mayor o igual: en el demo, `findEQ(9)` fallaría pero `lowerBound(9)` devuelve `10`. Solo cuando `x` es mayor que todas las claves ambos fallan.

5. BST construido insertando `7, 3, 10, 1, 5, 8, 12, 4, 6` en ese orden:

   ```
           7
          / \
         3    10
        / \   / \
       1   5 8   12
          / \
         4   6
   ```

   Paso a paso: `7` es la raíz; `3 < 7` va a la izquierda; `10 > 7` a la derecha; `1 < 3` a la izquierda de `3`; `5 > 3` a la derecha de `3`; `8 < 10` a la izquierda de `10`; `12 > 10` a la derecha de `10`; `4` (`< 7, > 3, < 5`) a la izquierda de `5`; `6` (`< 7, > 3, > 5`) a la derecha de `5`.

6. Recorridos de ese árbol:

   - Inorden: `1 3 4 5 6 7 8 10 12`
   - Preorden: `7 3 1 5 4 6 10 8 12`
   - Postorden: `1 4 6 5 3 8 12 10 7`
   - Por niveles: `7 3 10 1 5 8 12 4 6`

7. `lowerBound(9)` paso a paso: arranca en la raíz `7`; como `9 > 7` no entra a la izquierda, va a la derecha a `10`; como `9 < 10`, guarda `10` como candidato y baja a la izquierda a `8`; como `9 > 8`, va a la derecha, que es `nullptr`; al terminar devuelve el candidato `10`. `upperBound(8)` paso a paso: arranca en `7`; como `8 > 7` va a la derecha a `10`; como `8 < 10`, guarda `10` como candidato y baja a `8`; en `8` la condición `comp_(8, 8)` es falsa, así que va a la derecha (sin actualizar candidato), que es `nullptr`; devuelve el candidato `10`.

8. Los casos de eliminación en un BST son tres. Hoja: el nodo no tiene hijos, se desconecta directamente. Un hijo: el nodo tiene un solo hijo, que ocupa su lugar enganchándose al padre del nodo borrado. Dos hijos: no se puede quitar el nodo sin reorganizar, así que se reemplaza su clave por la de su sucesor inorden (el `leftmost` del subárbol derecho) y luego se elimina ese sucesor, que por construcción tiene a lo más un hijo, reduciéndose a uno de los dos casos anteriores.

9. `splice` cumple el papel de quitar un nodo que tiene a lo más un hijo, "cosiendo" el árbol: toma el único hijo del nodo (o `nullptr`), lo engancha al padre del nodo en la posición que ocupaba el nodo (raíz, hijo izquierdo o derecho), ajusta el `parent` de ese hijo, descuenta el tamaño y actualiza alturas con `updateHeightAbove`. En `remove`, cuando el nodo tiene dos hijos, `splice` se aplica al sucesor (que sí tiene a lo más un hijo) después de copiar su clave.

10. Después de eliminar una clave deben seguir siendo ciertos: la propiedad BST (el inorden sigue ordenado), la consistencia de los enlaces `parent` (`checkParentLinks`), el tamaño correcto (`size_` decrementado en uno), las alturas actualizadas en los ancestros afectados, y que la clave eliminada ya no se encuentre con `findEQ`/`contains`.

11. `remove(3)` debe conservar el inorden ordenado porque `3` tiene dos hijos, así que el borrado lo reemplaza por su sucesor inorden `4` (el `leftmost` del subárbol derecho, que cuelga de `5`) y luego elimina ese `4` que era una hoja. El nodo conserva su lugar en la estructura pero ahora con la clave `4`, de modo que el orden global no se rompe: el inorden pasa de `1 3 4 5 6 7 8 10 12` a `1 4 5 6 7 8 10 12`, que sigue creciente, justo como muestra el demo.

12. `rotateLeft(u)` sube al hijo derecho `w` de `u` para que ocupe el lugar de `u`. Reasigna el padre de `w` al antiguo padre de `u` (o lo hace raíz), convierte el subárbol izquierdo de `w` en el nuevo hijo derecho de `u`, cuelga `u` como hijo izquierdo de `w`, y recalcula las alturas de `u`, `w` y sus ancestros. Es una reestructuración local que baja a `u` un nivel y sube a `w`.

13. `rotateRight(u)` es la operación espejo: sube al hijo izquierdo `w` de `u`. Reasigna el padre de `w` al de `u` (o lo hace raíz), convierte el subárbol derecho de `w` en el nuevo hijo izquierdo de `u`, cuelga `u` como hijo derecho de `w`, y recalcula alturas. Baja a `u` y sube a `w` por el otro lado.

14. Una rotación preserva la propiedad BST porque solo reordena tres bloques manteniendo su orden relativo. En `rotateLeft(u)` con `w = u->right`, llamemos `A` al subárbol izquierdo de `u`, `B` al izquierdo de `w` y `C` al derecho de `w`. Antes se cumple `A < u < B < w < C`. Después de la rotación `w` queda arriba con `u` como hijo izquierdo, `u` conserva `A` a su izquierda y recibe `B` a su derecha, y `w` conserva `C` a su derecha; la relación sigue siendo `A < u < B < w < C`. Como ningún elemento cruzó a un lado donde violaría el orden, la propiedad BST se mantiene; el inorden es invariante ante rotaciones, lo que la prueba interna confirma sobre `{1,2,3}`.

15. Construir un BST balanceado desde un arreglo ordenado (`buildBalancedFromSorted`) sirve para obtener un árbol de altura mínima `O(log n)` sin pasar por inserciones una a una: se elige el elemento central como raíz y se construyen recursivamente las mitades izquierda y derecha. Esto evita el peor caso de insertar datos ya ordenados (que degeneraría el árbol en una lista) y garantiza búsquedas eficientes. El demo lo muestra: desde `{1..9}` produce un árbol con `isBST: si`, y la prueba pública verifica que con 7 nodos la altura es exactamente `2`.

16. En un BST balanceado la búsqueda cuesta `O(log n)` porque la altura es logarítmica y cada paso de `findLast`/`findEQ` desciende un nivel. En un BST degenerado (por ejemplo insertando claves ya ordenadas) el árbol se vuelve una cadena de altura `n-1`, así que la búsqueda degrada a `O(n)`, recorriendo en el peor caso todos los nodos. El costo de búsqueda es proporcional a la altura, y por eso el balance importa tanto.

## Bloque 8 - Heap

1. Un heap binario puede almacenarse en un std::vector sin punteros porque siempre es un árbol casi completo: se llena nivel por nivel de izquierda a derecha, sin huecos. Esa forma rígida permite asignar a cada nodo un índice fijo (la raíz en 0, sus hijos en 1 y 2, y así) de modo que la posición en el arreglo codifica la posición en el árbol. Las relaciones padre-hijo se vuelven aritmética de índices, así que no hace falta gastar memoria en enlaces `parent`, `left` ni `right`.

2. Demostración de las fórmulas. Si numeramos los nodos por niveles empezando en 0, un nodo en el índice `i` tiene antes de él `i` nodos, y los hijos del árbol se colocan de forma consecutiva. La raíz (índice 0) tiene hijos en 1 y 2; el nodo 1 en 3 y 4; el nodo 2 en 5 y 6; en general el hijo izquierdo del nodo `i` está en `2*i + 1` y el derecho en `2*i + 2`. Se verifica por inducción: cada nodo `i` aporta dos hijos, y la cantidad de nodos hasta `i` inclusive deja al hijo izquierdo justo en la posición `2*i + 1`. Invirtiendo la fórmula, si un hijo está en `j`, su padre está en `(j - 1) / 2` con división entera: para `j = 2*i + 1` da `(2*i) / 2 = i`, y para `j = 2*i + 2` da `(2*i + 1) / 2 = i`. Por eso `parent(i) = (i - 1) / 2`.

   ```cpp
   left(i)   = 2*i + 1
   right(i)  = 2*i + 2
   parent(i) = (i - 1) / 2
   ```

3. La propiedad de min-heap es: para todo índice `i > 0`, el valor del padre es menor o igual que el del hijo, es decir `data[parent(i)] <= data[i]`. Equivalentemente, ningún hijo es menor que su padre. Esto la verifica `isHeapArray` comprobando para cada `i` que ni `left(i)` ni `right(i)` sean menores que `data[i]`.

4. `top()` devuelve el mínimo porque la propiedad de min-heap obliga a que cada nodo sea menor o igual que sus descendientes; aplicado a la raíz, esto significa que la raíz es menor o igual que todos los demás nodos. Como la raíz está en el índice 0, `top()` simplemente devuelve `data_.front()`, que es el mínimo global, en `O(1)`.

5. `bubbleUp(i)` restaura la propiedad tras insertar un elemento al final del arreglo. Compara el elemento en `i` con su padre `parent(i)`: si el hijo es menor que el padre (`comp_(data[i], data[p])`), los intercambia y sube `i = p`, repitiendo; si el hijo no es menor, se detiene porque la propiedad ya se cumple. El nuevo elemento "burbujea" hacia arriba mientras viole el orden con su padre, deteniéndose en su lugar correcto. El costo es `O(log n)` porque a lo más recorre la altura del árbol.

6. `trickleDown(i)` restaura la propiedad tras mover un elemento a la raíz. Mira los dos hijos `left(i)` y `right(i)` y elige el menor entre el nodo y sus hijos (`best`); si el menor es uno de los hijos, intercambia y baja a esa posición (`i = best`), repitiendo; si el propio nodo ya es el menor, se detiene. El elemento "se hunde" hacia abajo intercambiándose siempre con el menor de sus hijos, hasta quedar en una posición donde no es mayor que ninguno. El costo es `O(log n)`.

7. `remove()` debe mover el último elemento a la raíz antes de aplicar `trickleDown(0)` porque al extraer la raíz queda un hueco que debe llenarse manteniendo la forma de árbol casi completo. El único elemento que se puede quitar sin dejar huecos en medio es el último del arreglo; al ponerlo en la raíz, el arreglo sigue siendo compacto y solo falta corregir el orden, lo que hace `trickleDown(0)` hundiéndolo a su lugar. Hacerlo al revés (corregir antes de tapar el hueco) rompería la representación implícita.

8. `isHeap()` verifica que el arreglo cumpla la propiedad de min-heap: recorre cada índice `i` y comprueba que ni su hijo izquierdo ni el derecho (cuando existen) sean menores que `data[i]`. Si encuentra un hijo menor que su padre, devuelve `false`. Es la validación del invariante que `bubbleUp`, `trickleDown` y `heapify` deben preservar.

9. Construir un heap insertando `n` elementos uno por uno aplica `add` (y por tanto `bubbleUp`) `n` veces, con costo total `O(n log n)`. Construirlo con `heapify` parte del arreglo completo y aplica `trickleDown` desde la mitad hacia la raíz, con costo total `O(n)`. `heapify` es asintóticamente más barato porque la mayoría de los nodos están cerca de las hojas y se hunden poco.

10. Insertar `n` elementos uno por uno cuesta `O(n log n)` en el peor caso porque cada inserción ejecuta `bubbleUp`, que puede recorrer toda la altura del árbol, es decir `O(log k)` para el `k`-ésimo elemento. Sumando sobre las `n` inserciones se obtiene del orden de `n log n` operaciones; en el peor caso (por ejemplo insertar en orden decreciente para un min-heap) cada nuevo elemento sube hasta la raíz.

11. `heapify` se ejecuta en `O(n)` porque, aunque hace `trickleDown` en cada nodo interno, el costo de `trickleDown` depende de la altura del nodo, no de la del árbol entero. Hay muchos nodos cerca de las hojas (altura pequeña) y muy pocos cerca de la raíz (altura grande). Al sumar el trabajo de todos los niveles, la serie `sum (n / 2^(h+1)) * h` converge a una cantidad proporcional a `n`, por lo que el total es lineal.

12. Extracción completa del heap construido con `{7, 3, 10, 1, 5, 8, 2}`. Tras `heapify` el arreglo queda `1 3 2 7 5 8 10` (mínimo `1` en la raíz). Las extracciones sucesivas con `remove()` devuelven: `1`, luego `2`, `3`, `5`, `7`, `8` y `10`, es decir la secuencia ordenada `1 2 3 5 7 8 10`, como confirma el demo. Sale ordenada porque cada `remove()` entrega el mínimo actual de la raíz y luego `trickleDown` deja el siguiente mínimo en la raíz; extraer mínimos sucesivos de un conjunto produce sus elementos en orden creciente.

13. El heap conviene cuando se necesita consultar y extraer el mínimo repetidamente: ofrece `top()` en `O(1)` y `remove()` en `O(log n)`, sin pagar por mantener todo ordenado. El BST conviene cuando se necesitan búsquedas ordenadas y consultas de rango: permite `findEQ`, `lowerBound`, `upperBound` y recorrido inorden ordenado en `O(altura)`, algo que el heap no puede hacer sin destruirse. En resumen, prioridad por un extremo favorece al heap; búsqueda y orden total favorecen al BST.

## Bloque 9 - Cierre comparativo

¿Qué cambia cuando pasamos de estudiar listas, pilas y colas a diseñar árboles binarios, heaps y árboles binarios de búsqueda?

El cambio central es pasar de estructuras lineales, donde cada elemento tiene a lo más un sucesor, a estructuras jerárquicas con ramificación, donde un nodo tiene dos hijos y la forma del conjunto deja de ser una secuencia para volverse una relación de niveles. Eso obliga a decidir cómo se representa la jerarquía y qué propiedad se le impone.

- Representación enlazada con nodos y punteros: en BinNode y BinTree cada nodo se crea con `new` y conoce a su padre y a sus dos hijos por punteros, de modo que la estructura admite cualquier forma de árbol; a cambio paga tres punteros por nodo y exige mantener invariantes de reciprocidad (`checkParentLinks`) cada vez que se reescriben enlaces.

- Representación implícita en arreglo: en BinaryHeap el árbol vive en un std::vector y las relaciones padre-hijo se calculan con `left(i) = 2*i+1`, `right(i) = 2*i+2` y `parent(i) = (i-1)/2`, sin gastar un solo puntero; el precio es que solo funciona para árboles casi completos.

- Diferencia entre propiedad estructural y propiedad de orden: un árbol binario por sí solo es una propiedad estructural (quién es hijo de quién), mientras que un BST añade una propiedad de orden total entre claves y un heap añade una propiedad de prioridad parcial entre padre e hijos; la misma forma de árbol sirve de soporte a disciplinas distintas según qué invariante se exija.

- Recorridos como forma de convertir un árbol en una secuencia: preorden, inorden, postorden y por niveles son maneras distintas de aplanar la jerarquía en una lista, y cada una revela algo (el inorden de un BST entrega el orden creciente, el de niveles revela la forma por capas); recorrer es traducir la estructura ramificada a la secuencia lineal de la que veníamos.

- Mantenimiento de alturas y enlaces `parent`: a diferencia de una lista, un árbol necesita información derivada que hay que sostener tras cada cambio; `updateHeightAbove` propaga el recálculo de alturas hacia la raíz y el puntero `parent` habilita tanto ese ascenso como `succ()` y `pred()`, que son imposibles en una estructura que solo mira hacia adelante.

- BST, búsqueda ordenada y recorrido inorden: la propiedad de orden convierte la búsqueda en un descenso dirigido de costo `O(altura)` (`findEQ`, `lowerBound`, `upperBound`) y hace que un simple inorden entregue todas las claves ordenadas sin destruir el árbol; por eso conviene mantenerlo balanceado, ya que su eficiencia depende de la altura.

- Heap, prioridad y costo de `add`, `remove` y `heapify`: la propiedad de prioridad concentra el mínimo en la raíz para responder `top()` en `O(1)`, mantiene el orden con `bubbleUp` y `trickleDown` en `O(log n)` por inserción o extracción, y permite construir el heap completo en `O(n)` con `heapify`, más barato que las `O(n log n)` de insertar uno por uno.

- Evidencia para defender correctitud: usaría las pruebas públicas e internas (que pasan: `OK test_public_week5_final` y `OK test_internal_week5_final`) como verificación de casos concretos, los demos como observación del comportamiento (inorden ordenado, mínimo en la raíz, secuencia de extracción ordenada), los invariantes (`isBST`, `isHeap`, `checkParentLinks`) como garantía de propiedades que se mantienen siempre, los trazados manuales de `succ`, `lowerBound` y `heapify` como explicación paso a paso, y el análisis de complejidad (`O(altura)`, `O(log n)`, `O(n)`) como argumento de costo. Las pruebas muestran que funciona en los casos probados; los invariantes y la complejidad explican por qué funciona en general.

## Autoevaluación breve

- Qué puedo defender con seguridad: la diferencia entre representación enlazada (BinNode/BinTree) e implícita (BinaryHeap), los dos casos de `succ()` y `pred()` con el papel del puntero `parent`, por qué el inorden de un BST sale ordenado, el borrado por sucesor con `splice`, y por qué `heapify` es `O(n)` frente a las `O(n log n)` de insertar uno por uno.
- Qué todavía confundo: los detalles finos de `travInIterative2`, que recorre en inorden sin pila usando solo `prev`/`curr` y `parent`, y la suma exacta de la serie que justifica el costo lineal de `heapify`.
- Qué evidencia usaría en una sustentación: la salida de `demo_bst.cpp` con el inorden ordenado y `lowerBound(9) = 10` frente a `upperBound(8) = 10`, la secuencia de extracción `1 2 3 5 7 8 10` del heap, y el `asciiArt()` antes y después de `rotateLeft(root)` para mostrar que el inorden no cambia.
- Qué parte del código me parece más importante para revisar otra vez: BinarySearchTree::remove junto con `splice` (el caso de dos hijos y la actualización de alturas y `parent`) y las rotaciones, porque son las operaciones que más punteros reescriben y donde es más fácil romper un invariante.
