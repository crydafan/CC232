# Examen Parcial (Versión C)

Estudiante:

- Leonardo Gabriel Estacio Huaricapcha

---

## Pregunta 1

### 1(a)

El ADT `IndexedBag<T>` es el contrato que define las operaciones disponibles y sus precondiciones y postcondiciones, sin decir cómo se almacenan los datos internamente. La representación es la estrategia para mapear esa abstracción en memoria, en este caso las tres opciones son `ArrayStack`, `RootishArrayStack` o `SLList`. La implementación es la realización concreta en código de esa representación.

### 1(b)

| Operación   | ArrayStack    | RootishArrayStack | SLList   |
| ----------- | ------------- | ----------------- | -------- |
| `add`       | $O(1)$ amort. | $O(1)$ amort.     | $O(1)$   |
| `get(i)`    | $O(1)$        | $O(1)$            | $O(i+1)$ |
| `contains`  | $O(n)$        | $O(n)$            | $O(n)$   |
| `removeOne` | $O(n)$        | $O(n)$            | $O(n)$   |

`add` en ArrayStack y RootishArrayStack es $O(1)$ amortizado ya que ocasionalmente necesitan crecer (redimensionar el arreglo o agregar un bloque nuevo), mientras que en SLList siempre se agrega al final con el puntero de cola en $O(1)$. `get(i)` es $O(1)$ en ArrayStack por acceso directo y también en RootishArrayStack usando `i2b(i)` para calcular el bloque en tiempo constante; en SLList es $O(i+1)$ porque hay que recorrer los nodos desde la cabeza hasta el índice. `contains` y `removeOne` son $O(n)$ en los tres casos ya que en el peor caso hay que recorrer todos los elementos para encontrar el buscado.

### 1(c)

Para acceder a un elemento mediante RootishArrayStack se requiere dos indirecciones de puntero en vez de una sola en comparación de ArrayStack puesto que este último solo posee un arreglo que cuando se llena se redimensiona en comparación a los bloques que maneja RootishArrayStack.

### 1(d)

Para el SLList las operaciones más incómodas son las que iteran los elementos: `get()`, `contains()`, `removeOne()` y `uniqueStable()`, ya que al usar nodos enlazados no se aprovecha la cache como si los datos estuvieran en memoria contigua y la ejecución será más lenta en la práctica.

### 1(e)

```pseudo
FUNCIÓN uniqueStable()
    visto <- lista vacía
    PARA CADA índice, elemento EN lista_principal HACER
        SI elemento EN visto ENTONCES
            eliminar elemento en índice de lista_principal
        SINO
            agregar elemento a visto
        FIN SI
    FIN PARA
FIN FUNCIÓN
```

En el peor caso (sin duplicados), para cada uno de los $n$ elementos se hace un `contains` sobre `visto`, que crece hasta tamaño $n$, con lo que la complejidad temporal es $O(n^2)$. El espacio adicional es $O(n)$ por la lista `visto`.

## Pregunta 2

### 2(a)

```pseudo
sumRec([2,4,6,8], 4)
= sumRec([2,4,6,8], 3) + 8
    = sumRec([2,4,6,8], 2) + 6
        = sumRec([2,4,6,8], 1) + 4
            = sumRec([2,4,6,8], 0) + 2
                = 0
            = 0 + 2
            = 2
        = 2 + 4
        = 6
    = 6 + 6
    = 12
= 12 + 8
= 20
```

### 2(b)

Sea $n$ un entero mayor o igual a $0$ y $A$ un arreglo de enteros cualquiera. Para $n=0$ se tiene $sumRec(A,0)=0$ por la condición base. Hipótesis inductiva: supongamos que para $n=k$ se cumple $sumRec(A,k)=A[0]+A[1]+...+A[k-1]$. Para $n=k+1$ se tiene que $sumRec(A,k+1)=sumRec(A,k)+A[k]=A[0]+...+A[k-1]+A[k]$, lo que coincide con la suma de los primeros $k+1$ elementos del arreglo. Por inducción se comprueba que $sumRec(A,n)=A[0]+A[1]+...+A[n-1]$ para todo $n \geq 0$.

### 2(c)

La complejidad temporal es $O(n)$ ya que la función se llama exactamente $n+1$ veces, una por cada valor de $n$ hasta llegar al caso base. El espacio adicional es $O(n)$ por la pila de llamadas: hay $n$ llamadas activas simultáneamente esperando su retorno. La memoria del arreglo $A$ no es espacio adicional ya que existía antes de llamar a la función.

### 2(d)

```cpp
int sumIter(const int A[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += A[i];
    }
    return sum;
}
```

Invariante de ciclo: al inicio de cada iteración con índice $i$, `sum` contiene la suma $A[0]+A[1]+...+A[i-1]$. Al terminar el ciclo con $i=n$, `sum` es $A[0]+...+A[n-1]$, que es el resultado esperado.

### 2(e)

Pasar `const int A[]` indica que la función no modificará los elementos del arreglo, solo los leerá. Esto es útil para la correctitud ya que el compilador puede detectar si hay un intento de modificación dentro de la función y el lector del código sabe que `A` es de solo lectura para `sumRec`.

### 2(f)

Dos casos borde: que el arreglo sea `nullptr` y que $n$ sea un número negativo. Para el primer caso habría que agregar una prueba que verifique el comportamiento definido cuando $A$ es nulo. Para el segundo, una prueba que verifique qué ocurre con $n<0$, ya que la condición `n==0` nunca se cumpliría y habría recursión infinita, por lo tanto también debería manejarse explícitamente.

## Pregunta 3

### 3(a)

Con $r=5$ la capacidad total es $5 \times 6/2 = 15$, por lo que los índices lógicos van de $0$ a $14$:

- Bloque 0: $[0]$
- Bloque 1: $[1, 2]$
- Bloque 2: $[3,4,5]$
- Bloque 3: $[6,7,8,9]$
- Bloque 4: $[10,11,12,13,14]$

### 3(b)

| Índice | Bloque | Desplazamiento |
| ------ | ------ | -------------- |
| 0      | 0      | 0              |
| 1      | 1      | 0              |
| 2      | 1      | 1              |
| 5      | 2      | 2              |
| 9      | 3      | 3              |
| 14     | 4      | 4              |

El desplazamiento se calcula como $i−b(b+1)/2$, donde $b$ es el bloque que contiene el índice $i$.

### 3(c)

Como los bloques tienen tamaños distintos, dado un índice lógico $i$ no es inmediato saber en qué bloque está. Hay que encontrar $b$ tal que $b(b+1)/2 \leq i \le (b+1)(b+2)/2$. La función `i2b(i)` calcula esto en $O(1)$ con la fórmula $b=\lfloor (−1 + \sqrt{1+8i})/2 \rfloor$, que es la solución analítica de esa desigualdad. Sin ella habría que recorrer los bloques uno por uno acumulando tamaños, lo que haría el acceso $O(\sqrt{n})$.

### 3(d)

Cuando hay $n$ elementos, se necesitan $r$ bloques donde $r(r+1)/2 \approx n$, con lo que $r \approx \sqrt{2n}$. El espacio desperdiciado es el que queda vacío en el último bloque, cuyo tamaño es $r = O(\sqrt{n})$. En el peor caso el último bloque está completamente vacío, por lo que el desperdicio es $O(\sqrt{n})$, mucho menor que el $O(n)$ de ArrayStack.

### 3(e)

Lo que se conserva es el $O(1)$ para `get(i)`, ya que calcular `i2b(i)` es $O(1)$ y el acceso al elemento dentro del bloque también es directo. El costo adicional respecto a ArrayStack es que se necesitan dos indirecciones de puntero: primero al arreglo de punteros de bloques y luego al bloque correspondiente y la complejidad matemática al momento de calcular el bloque en el que se encuentra el índice.

### 3(f)

Para crecer: cuando el último bloque se llena y se necesita más espacio, se asigna un nuevo bloque de tamaño $r+1$ y se incrementa $r$. El costo es $O(1)$ ya que solo se asigna el nuevo bloque. Para reducir: cuando el último bloque queda vacío, se desasigna y se decrementa $r$ en $O(1)$.

## Pregunta 4

### 4(a)

```pseudo
FUNCIÓN get(i)
    SI i < longitud(front) ENTONCES
        RETORNAR front[longitud(front) - i - 1]
    FIN SI
    RETORNAR back[i - longitud(front)]
FIN FUNCIÓN
```

Para $i=0$: $0 \lt |front|$ por tanto $front[|front|−0−1]=front[2]=10$
Para $i=2$: $2 \lt |front|$ por tanto $front[|front|−2−1]=front[0]=30$
Para $i=3$: $3 \geq |front|$ por tanto $back[3−|front|]=back[0]=40$
Para $i=6$: $6 \geq |front|$ por tanto $back[6−|front|]=back[3]=70$

### 4(b)

Para `add(1, 15)`: $i=1<n/2=3.5$, por lo que se inserta en `front` en la posición $|front|−i=3−1=2$. Se tiene `front` $= [30,20,15,10]$. La secuencia lógica pasa a ser $[10,15,20,30,40,50,60,70]$.

Luego, con `add(6, 55)`: ahora $n=8$, $|front|=4$. $i=6 \geq n/2=4$, por lo que se inserta en `back` en la posición $i−|front|=6−4=2$. Se tiene `back` $= [40,50,55,60,70]$. La secuencia lógica pasa a ser $[10,15,20,30,40,50,55,60,70]$.

### 4(c)

`front` guarda los elementos en orden inverso para que las operaciones en el extremo lógico izquierdo (índice $0$) correspondan al final del ArrayStack de `front`, que es $O(1)$ amortizado. Si `front` estuviera en orden normal, agregar o eliminar en el índice 0 implicaría desplazar todos los elementos de `front`, lo que sería $O(n)$.

### 4(d)

Una condición razonable de balance es que ninguno de los dos arreglos tenga más del doble de elementos del otro: $\max(|front|,|back|) \leq 2 \times \min(|front|,|back|)+1$. Cuando se viola, `balance()` redistribuye los $n$ elementos de manera que cada arreglo tenga aproximadamente $n/2$, transfiriendo elementos del lado más grande al más chico.

### 4(e)

Después de un rebalanceo ambos lados tienen aproximadamente $n/2$ elementos. Para que la condición de balance se vuelva a violar se necesitan al menos $\approx n/4$ operaciones consecutivas sobre el mismo lado, ya que hay que llegar a una proporción de $2:1$. Por lo tanto el costo $O(n)$ del rebalanceo se amortiza en al menos $n/4$ operaciones, con lo que el costo amortizado por operación es $O(n)/(n/4) = O(1)$.

## Pregunta 5

### 5(a)

SEList almacena los elementos en una lista doblemente enlazada de nodos, donde cada nodo es un pequeño ArrayDeque de capacidad $b+1$. A diferencia de DLList que tiene un nodo por elemento con dos punteros de overhead, SEList agrupa aproximadamente $b$ elementos por nodo, por lo que el overhead de punteros se reparte entre $b$ elementos y el uso de memoria se reduce.

### 5(b)

El invariante sobre el tamaño de los bloques es que cada bloque, salvo quizá el primero y el último, debe tener entre $\lfloor b/2 \rfloor$ y $b+1$ elementos. Esto garantiza que ningún bloque intermedio esté casi vacío, lo que generaría muchos nodos con pocos elementos y perdería el beneficio de la estructura.

### 5(c)

Al insertar en un bloque lleno se busca primero espacio en los bloques vecinos, recorriendo la lista hacia adelante o atrás hasta $b$ pasos. Si algún vecino tiene espacio, se desplaza un elemento hacia él mediante `add`/`remove` locales en cada bloque del camino. Si no hay espacio en ningún vecino dentro de esos $b$ pasos, se crea un nuevo bloque y se redistribuyen los elementos. El costo total es $O(b)$ por los desplazamientos locales.

### 5(d)

Para muchas inserciones cerca del centro, ArrayDeque requiere desplazar aproximadamente $n/2$ elementos por inserción, con lo que el costo es $O(n)$. SEList en cambio solo desplaza hasta $b$ elementos dentro del bloque y sus vecinos inmediatos, lo que da $O(b)$ por inserción. Con $b=\sqrt{n}$ el costo baja a $O(\sqrt{n})$ por inserción, bastante mejor que ArrayDeque para $n$ grande, aunque el acceso aleatorio por índice pasa de $O(1)$ a $O(n/b)$.

### 5(e)

La interfaz expone `get(i)`, `add(i,x)` y `remove(i)` igual que cualquier lista indexada. El usuario no necesita saber que internamente los elementos están agrupados en bloques, la implementación oculta ese detalle y todas las operaciones están definidas en términos de índices lógicos, por lo que el comportamiento observable es el de una lista simple.

### 5(f)

Primero inserta $n$ elementos siempre en la posición central de la lista, luego verifica que los elementos y el tamaño sean correctos. Después elimina repetidamente el elemento del centro hasta vaciar la estructura. Su objetivo es comprobar el correcto funcionamiento de las operaciones de inserción, acceso y eliminación bajo múltiples modificaciones internas. Si la estructura pierde elementos, duplica alguno o reporta un tamaño incorrecto después de muchas inserciones y eliminaciones, las verificaciones lo detectarán.

## Pregunta 6

### 6(a)

Prueba 1: capacidad $4$, $j=3$, $n=3$ con elementos $[10,20,30]$ en posiciones físicas $3,0,1$. Llamar `remove(1)`, el desplazamiento de elementos para cerrar el hueco cruza el borde del arreglo.

Prueba 2: capacidad $4$, $j=2$, $n=4$ con elementos $[10,20,30,40]$ en posiciones físicas $2,3,0,1$. Llamar `remove(0)`, se elimina el head y hay que incrementar $j$ con módulo; si se hace sin módulo $j$ queda fuera de rango.

### 6(b)

Tamaño 1: insertar un elemento y llamar `remove(0)`, luego verificar que $n=0$ y que un `add` posterior funciona correctamente.

Tamaño 2, eliminar primero: insertar dos elementos con `j = capacidad - 1`, llamar `remove(0)` y verificar que el elemento restante es el correcto y que $n=1$.

### 6(c)

Las pruebas públicas suelen construir el deque desde cero con $j=0$, por lo que nunca hay wrap-around. La implementación puede ser correcta para ese caso y fallar cuando $j \neq 0$, ya que los cálculos de índice circular difieren. Al no haber pruebas que fuercen $j$ a estar en el borde del arreglo, los errores en la aritmética modular pasan desapercibidos.

### 6(d)

Después de cada eliminación debería verificarse que $n$ disminuyó en 1, que todos los elementos en posiciones lógicas $[0, n−1]$ son accesibles con `get(i)` y retornan los valores correctos, y que el elemento eliminado ya no aparece en ninguna posición de la secuencia.

### 6(e)

ASan detecta accesos fuera de los límites del arreglo físico (buffer overflow) y uso de memoria liberada. Lo que no detecta son errores lógicos donde se accede a una posición dentro del arreglo pero que corresponde al índice lógico equivocado, por ejemplo por un cálculo de módulo incorrecto. Ese tipo de bug produce resultados incorrectos sin violar límites de memoria, con lo que ASan no lo reporta.

## Pregunta 7

### 7(a)

Para `apply(x)`: No tiene precondición alguna. La postcondición es que $x$ es el nuevo estado actual, `size` aumenta en 1.

Para `undo()`: La precondición es que `size > 0`. La postcondición es que el estado anterior pasa a ser el actual, `size` disminuye en 1.

Para `current()`: La precondición es que `size > 0`. La postcondición es que retorna el estado actual sin modificar el historial.

Para `size()`: Retorna la cantidad de estados almacenados.

Para `clear()`: La postcondición es que `size = 0`, no hay estado actual.

### 7(b)

Para la representación con arreglo dinámico: se almacena un arreglo `states[0..n-1]` donde `states[n-1]` es el estado actual. `apply(x)` agrega x al final y `undo()` decrementa n. El invariante es $n \geq 0$ y cuando $n > 0$, `states[n-1]` es el estado actual.

Para la representación con lista enlazada: cada nodo almacena un estado y un puntero `prev` al estado anterior. `head` apunta al estado actual. `apply(x)` crea un nodo nuevo con `prev = head` y actualiza `head`. `undo()` hace `head = head->prev`. El invariante es que `head == nullptr` si y solo si `size == 0`.

### 7(c)

| Operación | Arreglo dinámico | Lista enlazada |
| --------- | ---------------- | -------------- |
| apply     | $O(1)$ amort.    | $O(1)$         |
| undo      | $O(1)$           | $O(1)$         |
| current   | $O(1)$           | $O(1)$         |
| clear     | $O(1)$           | $O(n)$         |

### 7(d)

Si `size == 1` y se llama `undo()`, el historial queda vacío: `size = 0` y no hay estado actual, con lo que `current()` deja de ser válida. Si se llama `undo()` con `size == 0` se viola la precondición, la implementación debería lanzar una excepción o retornar un indicador de error según lo que defina el ADT. Lo importante es que este comportamiento esté documentado en el contrato para que el usuario sepa qué esperar.

### 7(e)

Para secuencia larga: llamar `apply(i)` para `i` de 0 a 999, verificar que `size() == 1000` y `current() == 999`, luego llamar `undo()` 1000 veces y verificar que `size() == 0`.

Para estados repetidos: llamar `apply(x)` tres veces con el mismo valor, verificar que `size() == 3`, luego `undo()` y verificar que `current() == x` y `size() == 2`.

Para operación inválida: llamar `undo()` con `size() == 0` y verificar que se produce el error definido en el ADT.

### 7(f)

Si se agrega `get(i)` para consultar cualquier estado por índice, la lista enlazada requiere $O(i)$ para llegar al i-ésimo nodo desde la cabeza ya que no hay acceso directo. El arreglo dinámico permite `states[i]` en $O(1)$. Por lo tanto, si se necesita acceso por índice la representación con arreglo dinámico es la adecuada y la lista enlazada deja de ser una opción razonable.
