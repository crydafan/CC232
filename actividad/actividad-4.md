# Actividad 4 - Semana 4

Estudiante:

- Leonardo Gabriel Estacio Huaricapcha

## Bloque 1

1. El acceso LIFO significa que el último elemento que entró es el primero que sale: todas las operaciones ocurren por un mismo extremo, el tope. En Stack esto se ve directamente en que `push` hace `data_.push_back` y `pop`/`top` operan sobre `data_.back()`, así que insertar y extraer tocan siempre el final del vector. El acceso FIFO significa que el primero en entrar es el primero en salir: se inserta por un extremo y se extrae por el otro. En Queue esto se ve en que `enqueue` hace `data_.push_back` (entra por atrás) mientras `dequeue`/`front` operan sobre `data_.front()` (sale por adelante). La diferencia esencial es que LIFO invierte el orden de procesamiento respecto al de llegada y FIFO lo conserva.

2. Stack resuelve naturalmente los problemas donde importa "lo último pendiente" porque su disciplina LIFO es exactamente la de una agenda de tareas que se suspenden y se retoman en orden inverso. Cuando un cómputo abre un subproblema que debe resolverse antes de continuar con el anterior, ese subproblema es justamente lo más reciente que quedó pendiente, y la pila garantiza que será lo primero en volver a atenderse. Lo vemos en `divideParentheses`/`parenIterative`, donde una apertura `(` debe esperar a su cierre y el cierre que llega debe casar con la apertura más reciente, y en NQueens o Maze, donde la última decisión tomada es la primera que se deshace al retroceder. El tope de la pila representa siempre "el asunto que dejé a medias hace menos tiempo".

3. Queue modela naturalmente los procesos de espera y atención porque su disciplina FIFO reproduce la noción de justicia temporal: quien llegó primero es atendido primero. En la simulación bancaria cada `Queue<Customer>` representa la fila frente a una ventanilla; un cliente nuevo se agrega con `enqueue` al final, y la ventanilla siempre trabaja sobre `front()`, el cliente que más tiempo lleva esperando. Esto refleja que en una atención real no tiene sentido invertir el orden de llegada: el orden de servicio debe coincidir con el orden de arribo, y eso es precisamente lo que conserva una cola.

4. Reemplazar recursión implícita por una estructura explícita significa dejar de apoyarse en la pila de llamadas que el lenguaje administra automáticamente y administrar nosotros mismos, en una Stack declarada en el código, la información que de otro modo guardaría cada marco de activación. Cuando una función se llama a sí misma, el sistema apila por debajo la dirección de retorno y las variables locales; al hacerlo explícito, esa misma información de "dónde iba y qué me falta" se guarda como datos en una pila visible. Lo vemos al comparar `convertRecursive` (que se apoya en el _call stack_) con `convertIterative` (que mantiene una `Stack<char>` propia), y de forma más clara en NQueens y Maze, donde la búsqueda con retroceso, que naturalmente se escribiría recursiva, se implementa con un bucle `do/while` sobre una Stack explícita. La ventaja es control: el estado deja de ser invisible y se vuelve inspeccionable y acotado por nosotros.

5. Para que una pila permita reconstruir una solución parcial debe guardar, por cada decisión tomada, la información mínima que la identifica de forma única y permite tanto continuar como deshacerla. En NQueens la pila `solution` guarda objetos Queen con su columna `y` en cada fila `x`: con eso basta para imprimir la solución (recorriendo y tomando cada `queen.y`) y para retroceder (al hacer `pop` se recupera la última reina y se reintenta desde `++q.y`). En Maze, la pila `path` guarda los punteros a las celdas del camino actual y cada celda recuerda su dirección de salida `outgoing`, de modo que la pila reconstruye la ruta de inicio a fin y la dirección permite reanudar la exploración exactamente donde se quedó. La regla es: guardar el estado suficiente para reanudar y para retroceder, ni más ni menos.

6. La conversión recursiva (`convertRecursive`) y la iterativa (`convertIterative`) comparten la aritmética del algoritmo: ambas calculan en cada paso el residuo `n % base`, que es el dígito menos significativo, y avanzan al cociente `n / base`, y ambas apilan los dígitos en una `Stack<char>` para luego vaciarla con `popAll`. Lo que cambia es el control del proceso. En la versión recursiva el avance al cociente se expresa como una llamada a sí misma y es el _call stack_ del lenguaje quien lleva la cuenta de cuántos pasos faltan; en la iterativa ese avance es un `while (n > 0)` con `n /= base`, y el control de la repetición es explícito en el bucle. El resultado es idéntico porque la lógica matemática es la misma; solo difiere quién gestiona la repetición.

7. La verificación iterativa de paréntesis necesita almacenar las aperturas pendientes porque, cuando llega un símbolo de cierre, hay que saber con qué apertura debe casar, y la apertura correcta es siempre la más reciente que aún no se ha cerrado. Sin memoria de las aperturas no habría forma de comprobar ni el balance (que cada cierre tenga su apertura) ni el anidamiento correcto (que los tipos `()`, `[]`, `{}` no se crucen). En `parenIterative` cada apertura se hace `push` en la `Stack<char>`, y cada cierre hace `pop` y compara: un `)` exige que el tope sea `(`, un `]` exige `[`, etc. La pila es precisamente el registro de "aperturas que siguen esperando su cierre", y al final debe quedar vacía para que la expresión esté balanceada.

8. El evaluador de expresiones necesita dos pilas porque maneja dos clases de información con disciplinas de resolución distintas que no pueden mezclarse en una sola estructura. La `Stack<double> operands` guarda los valores numéricos a la espera de ser combinados, y la `Stack<char> operators` guarda los operadores a la espera de poder aplicarse según su prioridad. La separación es necesaria porque la decisión de cuándo aplicar un operador no depende solo de él, sino de su relación de precedencia con el siguiente símbolo (`orderBetween`): mientras esa relación sea `<` el operador se queda esperando en su pila, y solo cuando es `>` se extrae y se consume de la pila de operandos. Con una sola pila no se podría distinguir "lo que está listo para operar" de "lo que decide cuándo operar", ni respetar la prioridad sin un segundo registro independiente.

9. N-Reinas y laberinto son ejemplos naturales de _backtracking_ porque ambos construyen una solución por decisiones sucesivas que pueden fallar, y ante un fallo deben deshacer la última decisión y probar la siguiente alternativa. En NQueens cada paso intenta colocar una reina en una columna válida de la fila actual; si ninguna columna sirve, se hace `pop` de la última reina y se retoma desde su siguiente columna (`++q.y`). En Maze cada paso intenta avanzar a una celda vecina disponible probando direcciones en orden; si la celda no tiene salida, se marca `BACKTRACKED` y se hace `pop` del camino. En los dos casos el patrón es idéntico (avanzar mientras se pueda, retroceder cuando no) y ese patrón de exploración con deshacer es la definición misma del backtracking, que aquí se sostiene con una pila explícita en lugar de recursión.

10. La simulación bancaria no se modela bien con pila porque una pila atendería primero al último cliente en llegar, lo que invertiría el orden de espera y dejaría indefinidamente atrás a quienes llegaron antes; eso contradice la noción de atención justa. Se modela bien con colas porque la disciplina FIFO garantiza que cada ventanilla atienda al cliente que más tiempo lleva esperando: en `simulate`, un cliente entra con `enqueue` al final de su fila y la ventanilla siempre trabaja sobre `front()`, liberándolo con `dequeue` cuando su tiempo llega a cero. El criterio temporal del problema, orden de llegada igual a orden de servicio, coincide exactamente con la semántica de Queue y choca con la de Stack.

11. La relación entre estructura auxiliar, estado parcial y correctitud es que la estructura auxiliar es el lugar físico donde vive el estado parcial, y mantener ese estado consistente en cada paso es lo que hace correcto al algoritmo. El estado parcial es "lo que llevo resuelto hasta ahora": las aperturas pendientes en `parenIterative`, los dígitos ya calculados en la conversión, las reinas ya colocadas en NQueens, las celdas del camino en Maze, los clientes en espera en cada Queue. La estructura auxiliar (pila o cola) impone una disciplina de acceso a ese estado, y la correctitud depende de que cada operación preserve el invariante asociado: que la pila contenga exactamente las aperturas sin cerrar, que `solution` contenga exactamente reinas no conflictivas, que la cola conserve el orden de llegada. Si el invariante se rompe en algún paso, el resultado final deja de ser confiable aunque el programa termine.

12. La diferencia conceptual entre "resolver un problema" y "simular un proceso" está en qué se busca como salida. Resolver un problema significa producir una respuesta correcta a una pregunta bien definida (la representación en otra base, si los paréntesis están balanceados, el valor de una expresión, una colocación válida de reinas, un camino en el laberinto) donde lo que importa es el resultado y existe un criterio de correctitud que decide si es válido. Simular un proceso significa reproducir paso a paso la evolución de un sistema en el tiempo para observar su comportamiento, sin que necesariamente exista una única "respuesta correcta": en `simulate` no se pregunta cuál es la solución, sino cómo crecen y se vacían las colas instante a instante según las llegadas aleatorias y la política de ventanilla. En el primer caso la pila/cola es una herramienta para llegar a un resultado; en el segundo la cola es parte del modelo del fenómeno que se está imitando.

## Bloque 2

| Archivo | Salida u observable importante | Estructura o técnica central | Qué concepto permite defender |
| ------- | ------------------------------ | ---------------------------- | ----------------------------- |
| `demo_stack_queue.cpp` | Tras `push(5,7,9)` y `enqueue(10,20,30)`: `top = 9`, `pop = 9`, `front = 10`, `dequeue = 10` | Stack LIFO sobre `data_.back()` y Queue FIFO sobre `data_.front()` | La pila devuelve lo último que entró y la cola lo primero, contraste directo entre LIFO y FIFO |
| `demo_base_conversion.cpp` | `12345 en base 8 (recursivo) = 30071` y `(iterativo) = 30071` | Conversión por residuos sucesivos apilados en `Stack<char>`, recursiva e iterativa | Recursión implícita e iteración con pila explícita producen la misma representación |
| `demo_paren_rpn.cpp` | `balanceados = true`, `RPN = 0 ! 1 + 2 3 ! 4 + ^ * 5 ! 67 - 8 9 + - -`, `valor = 2012` | `parenIterative` con pila de aperturas y `evaluateExpression` con doble pila | Una expresión bien formada se balancea, se traduce a RPN válida y se reduce a un valor |
| `demo_nqueens.cpp` | `N = 4, soluciones = 2, verificaciones = 84` y las filas `1 3 0 2` / `2 0 3 1` | Backtracking iterativo con `Stack<Queen>` explícita | Buscar con retroceso usando una pila en lugar de recursión, midiendo costo con `checks` |
| `demo_maze.cpp` | `Medida del camino = 5` y `(1,1) (1,2) (1,3) (2,3) (3,3)` | Búsqueda en laberinto con `Stack<Cell*>` y marcado de estados | La pila contiene la ruta viva de inicio a destino, sin las celdas ya abandonadas |
| `demo_bank.cpp` | `Llegadas = 10, atendidos = 0` y una línea por instante con `[tiempos]` por ventanilla | Simulación temporal con un vector de `Queue<Customer>` y política de cola más corta | Una cola por ventanilla modela la espera; el observable es la evolución, no un único resultado |
| `demo_capitulo4_panorama.cpp` | `Semana 4 cargada correctamente` seguido de una línea por componente (pila, cola, base, paréntesis, RPN, valor, reinas, laberinto, banco) | Header agregador `Capitulo4.h` que reúne todas las estructuras y aplicaciones | Una misma semana integra estructuras básicas y aplicaciones que conviven sin conflicto |

1. En `demo_stack_queue.cpp` lo que deja más clara la diferencia entre tope y frente es el contraste entre las dos parejas de líneas. La pila recibe `5`, `7`, `9` y reporta `top = 9` y `pop = 9`: devuelve el último valor que entró. La cola recibe `10`, `20`, `30` y reporta `front = 10` y `dequeue = 10`: devuelve el primer valor que entró. Comparar `9` contra `10` sobre dos estructuras llenadas con el mismo patrón de inserción es lo que hace visible que el tope mira al final de lo insertado y el frente mira al inicio.

2. En `demo_base_conversion.cpp` el observable que permite afirmar que ambas versiones producen la misma representación es que las dos líneas impresas son idénticas: `toBaseRecursive(12345, 8)` y `toBaseIterative(12345, 8)` arrojan exactamente `30071`. Como la entrada es la misma y las dos cadenas coinciden carácter por carácter, queda demostrado que el cambio de control (recursión contra bucle) no altera el resultado, solo la forma de recorrerlo.

3. En `demo_paren_rpn.cpp` la relación que se observa es de dependencia encadenada: que `parenIterative` devuelva `true` confirma que la expresión está balanceada, y solo una expresión bien formada puede traducirse a una RPN coherente como `0 ! 1 + 2 3 ! 4 + ^ * 5 ! 67 - 8 9 + - -`, que a su vez se reduce a un valor finito `2012`. El balance es la condición previa, la RPN es la forma intermedia que linealiza la prioridad de operadores y el valor es el resultado de consumir esa RPN. Si la cadena no estuviera balanceada, la evaluación terminaría en error y no en un número.

4. En `demo_nqueens.cpp`, `soluciones = 2` es la cantidad de colocaciones completas y válidas de 4 reinas en el tablero (las filas `1 3 0 2` y `2 0 3 1`), mientras que `verificaciones = 84` es la cantidad de veces que `conflictsWithAny` comparó una reina candidata contra las ya colocadas. No miden lo mismo porque `soluciones` cuenta resultados finales hallados y `verificaciones` cuenta el trabajo de búsqueda: cuántas comprobaciones de conflicto costó llegar a esos resultados. Un tablero puede tener pocas soluciones y aun así requerir muchísimas verificaciones, porque la mayor parte del esfuerzo se gasta en ramas que se descartan.

5. En `demo_maze.cpp` la secuencia `(1,1) (1,2) (1,3) (2,3) (3,3)` muestra el camino encontrado como la lista ordenada de celdas que quedaron en la pila `path` al alcanzar el destino, de inicio `(1,1)` a meta `(3,3)`. Son exactamente las celdas en estado `ROUTE`, es decir el recorrido vivo; las celdas que se probaron y resultaron sin salida ya fueron marcadas `BACKTRACKED` y retiradas con `pop`, así que no aparecen. La longitud `5` cuenta los pasos del camino válido, no los intentos fallidos.

6. En `demo_bank.cpp` cada lista impresa en un instante `t` representa el contenido de una `Queue<Customer>`, una por ventanilla, mostrando los tiempos de atención restantes de los clientes que esperan o están siendo atendidos en esa fila en ese momento. Por ejemplo, en `t=3: [26,2] [34] [33]` la primera ventanilla tiene dos clientes (uno al que le faltan 26 unidades y otro 2), y las otras dos tienen un cliente cada una. Que `atendidos = 0` con `Llegadas = 10` se explica porque la simulación corre solo 10 instantes y los tiempos de servicio sorteados son mayores, de modo que ningún cliente alcanza a terminar dentro de la ventana observada.

7. En `demo_capitulo4_panorama.cpp` la salida que mejor resume que una misma semana reúne estructuras y aplicaciones es el bloque que sigue a `Semana 4 cargada correctamente`: una línea por cada componente, donde la pila reporta su tope, la cola su frente, la conversión de base su resultado, los paréntesis su balance, la expresión su RPN y su valor, las N-Reinas su número de soluciones, el laberinto la longitud de su camino y el banco sus llegadas y atendidos. Ver todos esos observables juntos en una sola corrida deja claro que las estructuras básicas (Stack, Queue) y las aplicaciones (conversión, paréntesis, RPN, backtracking, simulación) pertenecen al mismo cuerpo de la semana y funcionan de forma integrada.

## Bloque 3

1. Para Stack la prueba pública valida: `empty()` (verifica que una pila recién creada esté vacía), `push(x)`, `top()` (lectura del tope sin extraer, que debe dar `9`), `pop()` (extracción en orden LIFO: primero `9`, luego `5`) y de nuevo `empty()` tras vaciarla. Es decir, comprueba la disciplina LIFO completa sobre el mismo extremo.

2. Para Queue la prueba pública valida: `empty()` inicial, `enqueue(x)` de `1`, `2`, `3`, `front()` (lectura del frente, que debe dar `1`), `dequeue()` tres veces en orden FIFO (`1`, `2`, `3`) y `empty()` final. Comprueba que se inserta por un extremo y se extrae por el otro respetando el orden de llegada.

3. Sobre la conversión de base la prueba pública valida que `toBaseRecursive(12345, 8)` y `toBaseIterative(12345, 8)` devuelvan ambas exactamente `"30071"`. Es decir, comprueba a la vez la correctitud del resultado y la equivalencia entre la versión recursiva y la iterativa para el mismo caso.

4. Sobre paréntesis balanceados la prueba pública valida tres casos: `parenRecursive("a+(b*(c+d))")` debe ser `true` (anidamiento correcto solo con paréntesis redondos), `parenIterative("a+(b*[c-{d/e}])")` debe ser `true` (anidamiento correcto con los tres tipos de delimitadores) y `parenIterative("([)]")` debe ser `false` (cruce incorrecto). Comprueba tanto la aceptación de cadenas bien formadas como el rechazo de un cruce mal anidado.

5. Sobre evaluación de expresiones y RPN la prueba pública valida que `evaluateExpression("(0!+1)*2^(3!+4)-(5!-67-(8+9))")` produzca la RPN exacta `"0 ! 1 + 2 3 ! 4 + ^ * 5 ! 67 - 8 9 + - -"` y el valor `2012` (con tolerancia `1e-9`). Verifica simultáneamente la cadena en notación polaca inversa y el valor numérico final.

6. Sobre NQueens la prueba pública valida que `placeQueens(4)` devuelva `solutions == 2` y que `placements.size() == 2`, es decir que el número de soluciones contadas coincida con el número de soluciones efectivamente almacenadas.

7. Sobre Maze la prueba pública valida que `findPath` sobre el laberinto de 5x5 desde `(1,1)` hasta `(3,3)` devuelva un camino no vacío cuyo primer elemento sea `(1,1)` y cuyo último elemento sea `(3,3)`. Comprueba que el camino existe y que conecta correctamente inicio y destino.

8. Sobre `bestWindow` la prueba pública arma tres ventanillas con tamaños `2`, `1` y `0` (dos clientes en la primera, uno en la segunda, ninguna en la tercera) y valida que `bestWindow` devuelva `2`, el índice de la cola más corta.

9. La prueba interna cubre casos adicionales que la pública no expone explícitamente: el caso `n = 0` en conversión (`toBaseRecursive(0, 2) == "0"`) y la base hexadecimal con letras (`toBaseIterative(255, 16) == "FF"`); una cadena sin paréntesis (`parenRecursive("sin parentesis") == true`), un orden invertido (`parenRecursive(")()(") == false`) y un cruce mixto (`parenIterative("{[(])}") == false`); expresiones con prioridad (`"3+4*2"` da `11`), con factorial y potencia (`"5!+2^3"` da `128`) y con menos unario (`"-3+5"` da `2`); el caso mínimo `placeQueens(1)` con una solución `[0]`; un laberinto sin salida cuyo camino queda vacío; una simulación que verifica `totalArrivals >= totalServed` y `timeline.size() == 8`; y un caso de error donde `toBaseIterative(10, 1)` lanza `std::invalid_argument` por base fuera de rango.

10. Pasar pruebas no reemplaza una explicación de invariantes, estado y complejidad porque las pruebas solo observan salidas para entradas concretas. No demuestran que la pila mantenga su invariante LIFO en toda secuencia posible, ni por qué el evaluador necesita dos pilas, ni que `findPath` recorra correctamente todas las formas de laberinto, ni el costo `O()` de cada operación. Confirman que el código funciona en los casos elegidos, no por qué funciona en general.

11. Un error conceptual que sobreviviría a los casos mínimos es la ceguera de `parenRecursive` ante `[]` y `{}`: la prueba pública solo usa `parenRecursive` sobre cadenas con paréntesis redondos y reserva los casos con corchetes y llaves para `parenIterative`, de modo que la versión recursiva pasa todas sus pruebas aunque ignore por completo los otros delimitadores. El experimento 2 lo confirma: `parenRecursive("([)]")` devuelve `true` (incorrecto), mientras que `parenIterative("([)]")` devuelve `false` (correcto). El error conceptual queda oculto si solo se ejecutan los casos mínimos.

## Bloque 4

1. En la conversión de base el cociente, el residuo y la pila cumplen papeles complementarios. El residuo `n % base` es el dígito menos significativo del número en esa base. El cociente `n / base` es lo que queda por convertir y gobierna cuántos pasos faltan: el proceso termina cuando llega a cero. La pila guarda los residuos en el orden en que se producen para poder invertirlos al final, porque se generan del dígito menos significativo al más significativo pero deben imprimirse al revés.

2. Los residuos se apilan antes de formar la cadena porque la división repetida entrega los dígitos en orden inverso al de lectura: el primer residuo es el dígito de las unidades (el de más a la derecha) y el último es el más significativo (el de más a la izquierda). Al apilarlos y luego vaciar la pila con `popAll`, la disciplina LIFO los devuelve en el orden correcto, del más significativo al menos significativo.

3. Entre dejar que el call stack haga el trabajo y manejar una pila explícita cambia quién administra la repetición y el almacenamiento intermedio, no el resultado. En `convertRecursive` la profundidad de la recursión equivale al número de dígitos y es el call stack del lenguaje quien guarda el estado de cada paso; en `convertIterative` un `while (n > 0)` y una `Stack<char>` propia hacen ese mismo trabajo de forma visible. Ambas tienen el mismo costo asintótico, pero la iterativa evita el costo por llamada y el riesgo de desbordar la pila del sistema en entradas muy grandes.

4. En `parenRecursive`, `divideParentheses` intenta capturar la idea de dividir y vencer: dado un bloque que empieza en un `(`, busca su `)` correspondiente avanzando con un contador (`crc`) que sube con cada `(` y baja con cada `)`, deteniéndose cuando el contador vuelve a cero. Ese punto medio separa "lo que está dentro del primer par de paréntesis" de "lo que viene después", de modo que la verificación se reduce recursivamente a comprobar esas dos partes por separado.

5. La limitación conceptual de la versión recursiva frente a la iterativa es que `parenRecursive` solo reconoce paréntesis redondos: su función `trimParentheses` recorta cualquier carácter que no sea `(` o `)`, ignorando por completo `[`, `]`, `{` y `}`. Por eso no puede detectar desbalances ni cruces que involucren corchetes o llaves. El experimento 2 lo evidencia: `parenRecursive("([)]")` devuelve `true` porque solo ve el par `()` balanceado, mientras que `parenIterative("([)]")` devuelve `false` porque rastrea los tres tipos de delimitadores y detecta el cruce.

6. En `parenIterative` un cierre incorrecto se detecta apenas aparece porque, al encontrar un símbolo de cierre, la función mira de inmediato el tope de la pila: si la pila está vacía o el tope no es la apertura correspondiente (`(` para `)`, `[` para `]`, `{` para `}`), devuelve `false` en el acto. No necesita seguir leyendo el resto de la cadena, porque un cierre que no casa con la apertura más reciente ya rompe el balance.

7. Comparando ambas parejas de funciones, la versión iterativa resulta más natural en la verificación de paréntesis: un único recorrido lineal con una pila maneja los tres tipos de delimitadores y detecta el primer error en el momento, mientras que la recursiva con divide y vencerás es más enrevesada y se queda corta. En cambio, la versión recursiva resulta más expresiva en la conversión de base, porque refleja directamente la definición matemática `n = (n / base) * base + (n % base)`: apilar el residuo y recurrir sobre el cociente es casi una transcripción de esa identidad.

### Experimento 1

| Número | Base | Salida recursiva | Salida iterativa | ¿Coinciden? | Comentario |
| ------ | ---- | ---------------- | ---------------- | ----------- | ---------- |
| 255 | 2 | `11111111` | `11111111` | Sí | Ocho unos, como esperaba para `2^8 - 1` |
| 255 | 16 | `FF` | `FF` | Sí | Usa los dígitos hexadecimales con letras |
| 100 | 8 | `144` | `144` | Sí | `1*64 + 4*8 + 4 = 100` |
| 31 | 2 | `11111` | `11111` | Sí | Cinco unos, `2^5 - 1` |
| 1000 | 16 | `3E8` | `3E8` | Sí | `3*256 + 14*16 + 8 = 1000` |
| 12345 | 8 | `30071` | `30071` | Sí | Mismo caso de la prueba pública |
| 42 | 5 | `132` | `132` | Sí | `1*25 + 3*5 + 2 = 42` |

En los siete casos la salida recursiva y la iterativa coinciden carácter por carácter, lo que respalda que ambas versiones implementan el mismo algoritmo y solo difieren en el control del proceso.

### Experimento 2

| Expresión | Recursivo | Iterativo | ¿Coinciden? | Qué explica el caso |
| --------- | --------- | --------- | ----------- | ------------------- |
| `""` (vacía) | `true` | `true` | Sí | Una cadena sin símbolos está trivialmente balanceada |
| `a+b-c` | `true` | `true` | Sí | Sin delimitadores no hay nada que cerrar |
| `(a+(b*c))` | `true` | `true` | Sí | Anidamiento correcto solo con paréntesis redondos |
| `((a+b)` | `false` | `false` | Sí | Desbalance: queda una apertura sin cerrar |
| `([)]` | `true` | `false` | No | Cruce incorrecto: la recursiva ignora `[]` y solo ve `()` balanceado; la iterativa detecta el cruce |
| `{[a]+(b)}` | `true` | `true` | Sí | Coinciden, pero por motivos distintos: la recursiva solo revisa los `()`; la iterativa valida los tres tipos |
| `((((a+b)*(c-d))/((e+f)*(g-h))))` | `true` | `true` | Sí | Cadena larga con anidamiento profundo bien balanceado |
| `a+[b*(c-{d})]` (inventada) | `true` | `true` | Sí | La iterativa valida los tres delimitadores; la recursiva solo ve el par `()` interno, también balanceado |

El caso decisivo es `([)]`: es el único donde las dos versiones discrepan, y deja al descubierto la limitación de `parenRecursive` para detectar cruces entre delimitadores distintos. En `{[a]+(b)}` y `a+[b*(c-{d})]` ambas dan `true`, pero conviene notar que la recursiva acierta por casualidad, ya que solo está mirando los paréntesis redondos.

## Bloque 5

1. `EvaluationResult` guarda dos cosas: `value`, un `double` con el valor numérico final de la expresión, y `rpn`, un `std::string` con la expresión reescrita en notación polaca inversa. Es decir, el resultado del cálculo y la traza de cómo se ordenaron los operadores.

2. Primero se eliminan los espacios (con `removeSpaces`) para que el resto del análisis pueda asumir que los tokens están contiguos y no tener que tratar espacios en cada decisión. Eso simplifica `isUnaryMinus`, `readNumber` y la comparación de prioridades, que de otro modo tendrían que saltar espacios constantemente.

3. El menos unario se detecta con `isUnaryMinus`: el carácter debe ser `-`, el siguiente debe ser un dígito o un punto, y además el `-` debe estar al inicio de la expresión (`i == 0`) o venir justo después de otro operador o de un paréntesis de apertura (`(`, `+`, `-`, `*`, `/`, `^`). En esas condiciones el `-` no es resta sino el signo del número que sigue, y se lee como parte del operando.

4. El factorial se trata como operador unario porque actúa sobre un solo operando, el que tiene a su izquierda, y se aplica en cuanto su prioridad lo permite (`calcu(op, b)` con `op == '!'`). La restricción que impone el código es que el operando sea un entero no negativo: `calcu` redondea el valor y, si difiere del redondeo en más de `1e-9`, lanza "el factorial requiere un operando entero"; y `factorialInt` lanza un error para enteros negativos. Así `3!` es válido pero `2.5!` o `(-1)!` fallan.

5. La RPN se va construyendo durante la evaluación y no al final porque `appendRpn` se llama en dos momentos: cada vez que se lee un operando se agrega de inmediato a la cadena, y cada vez que un operador se resuelve (relación `>`, cuando se saca de la pila para aplicarlo) se agrega ese operador. De esa forma la notación polaca inversa emerge en el mismo recorrido en que se calcula el valor, reflejando el orden real en que se aplicaron las operaciones.

6. La relación entre el operador del tope y el símbolo actual la da `orderBetween(operators.top(), current)`, que devuelve `<`, `=` o `>`. Un `<` significa que el símbolo actual tiene mayor prioridad y debe esperar: se apila y se avanza. Un `=` significa un par que se cancela (un `(` con su `)`, o el `\0` final con el `\0` inicial): se descarta el tope y se avanza. Un `>` significa que el operador del tope tiene prioridad mayor o igual y ya puede aplicarse: se saca, se agrega a la RPN y se opera sobre la pila de operandos.

7. Una expresión mal formada debe terminar en error y no en un valor arbitrario porque devolver un número cualquiera ocultaría el problema y el llamador confiaría en un resultado sin sentido. Por eso el evaluador lanza excepciones ante operandos faltantes, división entre cero, fin inesperado de la expresión o un orden de operadores inválido: es preferible informar que la entrada no es válida a producir silenciosamente una respuesta incorrecta.

8. La ventaja conceptual de obtener a la vez el valor y la RPN es que en un solo recorrido se consigue tanto el resultado como la evidencia de cómo se llegó a él: la RPN muestra el orden en que se aplicaron los operadores, es decir cómo se resolvió la prioridad y la asociatividad. Eso permite verificar y defender que el cálculo respetó las reglas, y resulta muy útil para depurar, porque si el valor está mal la RPN suele revelar en qué punto se ordenó mal la expresión.

### Experimento 3

| Expresión | RPN esperada | RPN obtenida | Valor/error esperado | Valor/error obtenido | Explicación |
| --------- | ------------ | ------------ | -------------------- | -------------------- | ----------- |
| `3+4*2` | `3 4 2 * +` | `3 4 2 * +` | `11` | `11` | El `*` tiene mayor prioridad y se aplica antes que el `+` |
| `10-2*3` | `10 2 3 * -` | `10 2 3 * -` | `4` | `4` | Otra vez la multiplicación se resuelve primero |
| `(1+2)*3` | `1 2 + 3 *` | `1 2 + 3 *` | `9` | `9` | El paréntesis fuerza la suma antes que el producto |
| `2*(3+(4-1))` | `2 3 4 1 - + *` | `2 3 4 1 - + *` | `12` | `12` | Anidamiento: se resuelve de adentro hacia afuera |
| `-5+3*2` | `-5 3 2 * +` | `-5 3 2 * +` | `1` | `1` | El `-5` se lee como menos unario, no como resta |
| `(3+*2)` | (error) | (error) | error: faltan operandos | `faltan operandos para el operador binario` | El `+` no tiene operando izquierdo válido antes del `*` |

Las dos expresiones válidas sin paréntesis, las dos con anidamiento y la del menos unario coinciden con lo esperado tanto en RPN como en valor. La inválida termina en excepción, como debe ser, sin devolver un número arbitrario.

### Extensión opcional

Extendí el evaluador con un operador adicional, probado y compilado contra los headers de la semana.

- **Símbolo agregado:** `%` (módulo).
- **Aridad:** binario (opera sobre dos operandos, el de la izquierda y el de la derecha).
- **Prioridad:** la misma que `*` y `/`, con asociatividad por la izquierda. En la tabla de precedencias, `%` se comporta como `MUL`/`DIV`: cede ante `^` y `!` (relación `<`) y se aplica antes que `+` y `-` (relación `>`), y frente a `*`, `/` y a sí mismo la relación es `>` para que se evalúen de izquierda a derecha.
- **Casos válidos probados:** `10%3` da `1` con RPN `10 3 %`; `17%5+2` da `4` con RPN `17 5 % 2 +` (el módulo antes que la suma); `(9+1)%4` da `2` con RPN `9 1 + 4 %`; y `2*10%3` da `2` con RPN `2 10 * 3 %`, confirmando que `*` y `%` tienen igual prioridad y se resuelven de izquierda a derecha (`(2*10)%3 = 20%3 = 2`).
- **Caso inválido probado:** `10%` lanza "faltan operandos para el operador binario" porque falta el operando derecho; adicionalmente `10%0` lanza "modulo entre cero", protegiendo contra el módulo por cero.
- **Qué parte del código tuve que modificar:** en `OperatorPriority.h` agregué `MOD` al `enum Operator`, amplié la tabla `pri` de 9x9 a 10x10 con la fila y la columna de `%` (copiando el patrón de `MUL`/`DIV`) y añadí el caso `case '%': return MOD;` en `optr2rank`. En `ExpressionEvaluator.h` agregué `case '%':` a `isOperatorChar` y el caso `case '%':` en la función `calcu` binaria, calculando `std::fmod` sobre los operandos redondeados y lanzando error si el divisor es cero.

## Bloque 6

1. En Queen, que dos reinas entren en conflicto significa que se atacan: el `operator==` devuelve `true` si comparten fila (`x == other.x`), columna (`y == other.y`) o alguna de las dos diagonales (`x + y == other.x + other.y` o `x - y == other.x - other.y`). Es decir, "iguales" en el sentido del código quiere decir "en conflicto".

2. En NQueens, la pila `solution` representa la solución parcial en construcción: contiene las reinas ya colocadas, una por fila, cada una con su columna `y`. La profundidad de la pila indica en qué fila va la búsqueda, y al mismo tiempo funciona como la estructura de backtracking, porque retroceder es hacer `pop` de la última reina colocada.

3. Avanzar en columna dentro de una fila es incrementar `q.y` para probar la siguiente posición de la fila actual, saltando las columnas donde la reina entraría en conflicto (`while (q.y < n && conflictsWithAny(...)) ++q.y`). Toca retroceder cuando ya no se puede avanzar: cuando se agotaron las columnas de la fila (`q.y >= n`) o cuando la pila ya completó las `n` filas; en ese caso se hace `pop` de la última reina y se reanuda desde su siguiente columna (`q = solution.pop(); ++q.y`).

4. `checks` es una métrica útil porque cuenta cuántas verificaciones de conflicto se hicieron (`conflictsWithAny` la incrementa en cada comparación), es decir mide el esfuerzo de la búsqueda con independencia de cuántas soluciones haya. Permite analizar cómo crece el costo del backtracking al aumentar `n`, algo que el número de soluciones por sí solo no revela.

5. Cuando `collectPlacements` vale `false`, el algoritmo deja de guardar el vector de columnas de cada solución encontrada: sigue contando `solutions` y `checks`, pero no llena `result.placements`. Es útil cuando solo interesan los conteos y no las configuraciones concretas, porque ahorra la memoria de almacenar todos los tableros.

6. En Maze, los estados representan la situación de cada celda durante la búsqueda: `AVAILABLE` es una celda libre todavía no visitada; `ROUTE` es una celda que forma parte del camino actual (está en la pila); `BACKTRACKED` es una celda que se visitó, se exploró por completo y resultó un callejón sin salida, por lo que se abandonó; y `WALL` es un obstáculo intransitable.

7. `incoming` y `outgoing` codifican direcciones cardinales (`EAST`, `SOUTH`, `WEST`, `NORTH`). `incoming` guarda la dirección por la que se entró a la celda, y `outgoing` la dirección que se está probando para salir de ella. `advance` usa `outgoing` para moverse a la celda vecina y le fija a esa vecina su `incoming` opuesto, y al retomar una celda se sigue girando `outgoing` con `nextESWN` hasta agotar las cuatro direcciones (`NO_WAY`).

8. El algoritmo marca y desmarca estado en lugar de solo "moverse" porque el backtracking necesita memoria de por dónde ya pasó y qué ya falló. Marcar `ROUTE` evita volver a entrar a una celda que ya está en el camino actual (impide ciclos), y marcar `BACKTRACKED` evita reintentar un callejón sin salida que ya se exploró. Sin esas marcas el algoritmo daría vueltas o repetiría trabajo; con ellas, cada celda se intenta de forma ordenada y se descarta definitivamente cuando se agota.

9. N-Reinas y laberinto comparten que ambos son búsquedas con retroceso sostenidas por una pila explícita: avanzan mientras encuentran una opción válida y deshacen la última decisión cuando se quedan sin salida. Lo que cambia es la representación del estado. En N-Reinas el estado es la pila de objetos Queen (la fila viene dada por la profundidad y la columna por `y`) y la validación es global, comparando la candidata contra todas las reinas ya colocadas. En el laberinto el estado vive en las propias celdas (su `status`, su `incoming` y `outgoing`) más la pila de punteros del camino, y la validación es local, mirando solo los cuatro vecinos inmediatos.

### Experimento 4

| `n` | Soluciones | `checks` | ¿Crece rápido o lento? | Comentario |
| --- | ---------- | -------- | ---------------------- | ---------- |
| 1 | 1 | 0 | base | Una reina, sin conflictos que verificar |
| 4 | 2 | 84 | rápido | Primer caso no trivial |
| 5 | 10 | 405 | rápido | Más soluciones y casi 5x más verificaciones |
| 6 | 4 | 2016 | muy rápido | Menos soluciones que `n=5` pero muchas más verificaciones |
| 8 | 92 | 46752 | explosivo | El tablero clásico, casi 50 mil verificaciones |
| 10 | 724 | 1297558 | explosivo | Más de un millón de verificaciones |

1. El patrón en el crecimiento de `checks` es de explosión combinatoria: crece muchísimo más rápido que de forma lineal, aproximadamente exponencial. De `n=4` a `n=10` los `checks` pasan de 84 a más de 1.29 millones, mientras que `n` solo se multiplicó por algo más de dos.

2. Contar verificaciones no es lo mismo que contar soluciones porque las soluciones son los tableros completos válidos, mientras que `checks` mide el trabajo total, incluidas todas las ramas que se exploraron y se descartaron. El caso `n=6` lo demuestra de forma clara: tiene menos soluciones que `n=5` (4 frente a 10) pero muchas más verificaciones (2016 frente a 405). El esfuerzo de búsqueda no sigue al número de soluciones.

3. La poda aparece en el `while (q.y < n && conflictsWithAny(solution, q, result.checks)) ++q.y` y en el hecho de que solo se hace `push` cuando la columna no genera conflicto. Al rechazar de inmediato una columna conflictiva y al colocar una reina solo en posiciones válidas, el algoritmo nunca llega a explorar los subárboles de tableros que ya son inválidos por una colocación parcial. No hay una función llamada "podar", pero ese rechazo temprano es exactamente una poda del espacio de búsqueda.

### Experimento 5

Laberinto 1, camino claro (inicio `(1,1)`, destino `(3,3)`):

```
#####
#...#
#.#.#
#...#
#####
```

Longitud del camino: 5, celdas en estado `BACKTRACKED`: 0. El algoritmo llega directo de inicio a destino sin abandonar ninguna celda, porque desde cada punto la primera dirección probada ya conduce hacia la meta.

Laberinto 2, sin salida (inicio `(1,1)`, destino `(3,3)`):

```
#####
#.#.#
###.#
#...#
#####
```

Longitud del camino: 0 (no existe), celdas en estado `BACKTRACKED`: 1. El destino queda aislado del inicio por muros, así que `findPath` agota las opciones disponibles, abandona la única celda transitable que pudo intentar y devuelve un camino vacío.

Laberinto 3, con varios retrocesos (inicio `(1,1)`, destino `(5,1)`):

```
#######
#...#.#
#.#.#.#
#.#...#
#.###.#
#...#.#
#######
```

Longitud del camino: 5, celdas en estado `BACKTRACKED`: 10, con camino final `(1,1) (2,1) (3,1) (4,1) (5,1)`. Aquí está la evidencia de retroceso: el algoritmo, que prueba primero la dirección este, se interna en toda la región de la derecha del tablero, que es un callejón sin salida respecto al destino, y la abandona celda por celda (10 celdas quedan marcadas `BACKTRACKED`) antes de volver a `(1,1)` y bajar limpio por la columna izquierda hasta `(5,1)`. El camino final es corto, pero el número de celdas retrocedidas revela todo el trabajo descartado.

## Bloque 7

1. Cada `Queue<Customer>` dentro del vector `windows` representa la fila de una ventanilla: la secuencia de clientes que esperan o están siendo atendidos en esa ventanilla, cada uno con su `time` (tiempo de atención restante) y su `window` (la ventanilla asignada). Hay tantas colas como ventanillas tenga la simulación.

2. `bestWindow` usa el criterio de la cola más corta: recorre las ventanillas y se queda con la de menor `size()`. Ante un empate toma la decisión de quedarse con la de menor índice, porque solo actualiza `opt` cuando encuentra una cola estrictamente más corta (`windows[i].size() < minSize`); si una cola posterior tiene el mismo tamaño, no la prefiere. Así, con varias colas igual de cortas, elige la primera de izquierda a derecha.

3. Que la simulación use una semilla (`seed`) significa que el generador aleatorio `std::mt19937` se inicializa con un valor fijo, de modo que produce siempre la misma secuencia de llegadas y de tiempos de servicio. Es lo que hace la simulación reproducible: con la misma semilla, los mismos parámetros y el mismo código, el resultado es idéntico.

4. La relación que debe cumplirse es `totalServed <= totalArrivals`: no se puede atender a más clientes de los que han llegado. La prueba interna lo verifica con `totalArrivals >= totalServed`. La diferencia entre ambos números son los clientes que al final de la simulación siguen en las colas sin haber terminado su atención.

5. La línea de tiempo (`timeline`) representa la evolución del sistema instante a instante: para cada `now` guarda una foto del estado de todas las colas, es decir los tiempos de atención restantes de los clientes en cada ventanilla en ese momento. Recorrer la `timeline` muestra cómo crecen y se vacían las colas a lo largo de la simulación.

6. Esta aplicación necesita colas y no pilas porque el criterio de atención es por orden de llegada: el cliente que más tiempo lleva esperando debe ser atendido primero, y eso es exactamente la disciplina FIFO de una cola. Una pila atendería primero al último en llegar, lo que invertiría el orden de espera y sería injusto.

7. El simulador introduce varias simplificaciones del mundo real: el tiempo es discreto y avanza de a una unidad; llega a lo más un cliente por instante (cuando `arrival(rng) != 0`); los tiempos de servicio se sortean de manera uniforme entre 1 y 98 sin relación con el tipo de trámite; un cliente, una vez asignado a una ventanilla, no se cambia de fila; y no hay prioridades, abandonos ni descansos de las ventanillas.

8. Si la política ya no fuera "cola más corta" cambiaría el reparto de la carga entre ventanillas. Por ejemplo, asignar siempre a la primera ventanilla libre, hacerlo al azar o por turnos rotativos (round-robin) produciría colas más desiguales o menos balanceadas; una política basada en el tiempo de atención restante total (y no solo en la cantidad de clientes) podría reducir mejor la espera real. La elección de la política determina cómo se distribuye la congestión.

### Experimento 6

| `nWin` | `servTime` | `seed` | `totalArrivals` | `totalServed` | Tamaño final de cada cola | Observación |
| ------ | ---------- | ------ | --------------- | ------------- | ------------------------- | ----------- |
| 3 | 10 | 12345 | 10 | 0 | `[4, 3, 3]` | Ventana corta: nadie termina y las colas quedan pequeñas |
| 3 | 30 | 12345 | 26 | 1 | `[8, 9, 8]` | Más tiempo, más llegadas y colas más largas |
| 3 | 30 | 999 | 18 | 0 | `[6, 6, 6]` | Misma config, otra semilla: menos llegadas, colas parejas |
| 5 | 30 | 12345 | 27 | 2 | `[5, 5, 5, 5, 5]` | Más ventanillas reparten la carga: colas más cortas |
| 2 | 30 | 12345 | 22 | 1 | `[10, 11]` | Menos ventanillas: colas mucho más largas |
| 3 | 30 | 12345 | 26 | 1 | `[8, 9, 8]` | Repetición exacta del caso 2: resultado idéntico |

1. El parámetro que más influye en la congestión observable es una combinación de `servTime` y `nWin`. Alargar `servTime` deja entrar más clientes y, como los tiempos de servicio son grandes, las colas se acumulan (de `[4,3,3]` con `servTime=10` a `[8,9,8]` con `servTime=30`). Reducir `nWin` concentra esa carga en menos filas (`[10,11]` con 2 ventanillas frente a `[5,5,5,5,5]` con 5). En conjunto, `servTime` determina cuánta carga total entra y `nWin` cómo se reparte.

2. Cuando repito exactamente la misma semilla (caso 2 y caso 6, ambos `nWin=3`, `servTime=30`, `seed=12345`) el resultado es idéntico: 26 llegadas, 1 atendido y colas finales `[8, 9, 8]`. Esto confirma que la simulación es determinista respecto a la semilla.

3. Cuando mantengo `nWin` y `servTime` pero altero la semilla (caso 2 con `seed=12345` frente a caso 3 con `seed=999`) cambian las llegadas y los tiempos de servicio: de 26 llegadas y colas `[8, 9, 8]` paso a 18 llegadas y colas `[6, 6, 6]`. La forma general del sistema es parecida, pero los conteos concretos dependen del azar que fija la semilla.

4. Para defender que la cola más corta es una política razonable usaría como evidencia el balanceo observado en los tamaños finales: con 5 ventanillas las colas quedan en `[5, 5, 5, 5, 5]` y con 3 en `[8, 9, 8]`, es decir muy parejas, lo que muestra que la política distribuye la carga de forma uniforme y barata de calcular. No es necesariamente óptima porque solo mira la cantidad de clientes y no el tiempo de atención restante de cada fila: una cola corta podría tener al frente un cliente con un servicio larguísimo. Aun así, balancear por longitud es una heurística simple que evita que una ventanilla se sature mientras otra está ociosa.

## Bloque 8

**¿Qué cambia cuando pasamos de "usar pilas y colas como ADTs básicos" a "usarlas como mecanismos de control para resolver problemas"?**

Lo que cambia es el papel de la estructura: deja de ser un simple almacén de datos con una interfaz y pasa a ser el motor que dirige el algoritmo, decidiendo en cada paso qué se hace a continuación. La disciplina de acceso se convierte en la lógica del control.

- Sobre LIFO y FIFO: la disciplina de acceso deja de ser un detalle de la interfaz y se vuelve la decisión de diseño central. LIFO sirve cuando lo que importa es "lo último pendiente" (deshacer la última decisión, casar el cierre con la apertura más reciente, invertir el orden de los dígitos), y FIFO sirve cuando importa "lo primero que llegó" (atender en orden de arribo). Elegir una u otra es elegir cómo se resuelve el problema, no solo cómo se guardan los datos.

- Sobre recursión implícita frente a estructura explícita: una pila explícita puede reemplazar la recursión implícita porque el call stack del lenguaje no es más que una pila de estados pendientes; al declarar nuestra propia Stack hacemos visible y controlable ese mismo estado, como se ve al pasar de `convertRecursive` a `convertIterative` o al implementar N-Reinas y el laberinto con un bucle sobre una pila en vez de llamadas recursivas. Ganamos control y evitamos el riesgo de desbordar la pila del sistema.

- Sobre evaluación de expresiones: el evaluador usa dos pilas como mecanismo de control, una de operandos y otra de operadores, y la relación de prioridad entre el tope y el símbolo actual (`orderBetween`) decide en cada paso si apilar, descartar o aplicar. La estructura no solo guarda los operandos y operadores: gobierna cuándo se ejecuta cada operación, y de paso produce la RPN como traza de ese control.

- Sobre backtracking: una pila convierte la búsqueda con retroceso en un proceso explícito de avanzar y deshacer. En N-Reinas y en el laberinto, hacer `push` es comprometerse con una decisión y hacer `pop` es retractarse de la última; la poda temprana (rechazar columnas en conflicto, marcar celdas como `BACKTRACKED`) recorta el espacio de búsqueda sin necesidad de una estructura formal de poda.

- Sobre simulación: una cola por ventanilla no resuelve un problema con una única respuesta correcta, sino que modela un proceso que evoluciona en el tiempo. La FIFO es parte del modelo del fenómeno (la espera justa), y la `timeline` deja observar cómo el sistema se congestiona o se alivia según los parámetros, que es una pregunta distinta de "cuál es la solución".

- Sobre correctitud experimental: las pruebas y los experimentos respaldan la defensa, pero no la reemplazan. Los experimentos mostraron evidencia concreta (recursivo e iterativo coinciden en conversión de base, `parenRecursive` falla ante `([)]`, los `checks` de N-Reinas explotan de 84 a más de un millón, el laberinto 3 retrocede 10 celdas, la simulación es determinista por semilla), pero defender por qué funciona exige hablar de invariantes (qué guarda cada pila), de estado parcial y de complejidad, no solo de salidas.

- Comparación entre los cinco casos: la conversión de base usa una pila para invertir el orden de los dígitos; la verificación de paréntesis usa una pila para recordar las aperturas pendientes y casar cierres; N-Reinas usa una pila para sostener la solución parcial y retroceder ante conflictos; el laberinto usa una pila para mantener el camino vivo y abandonar callejones; y el banco usa colas para atender en orden de llegada. Los cuatro primeros son pilas porque el problema exige volver sobre lo más reciente (inversión o retroceso), y el último es cola porque el proceso exige respetar el orden de llegada. La misma idea, la disciplina de acceso como control, se manifiesta de forma distinta según si el problema pide deshacer o pide esperar.

## Autoevaluación breve

- Qué puedo defender con seguridad: por qué Stack resuelve inversión y retroceso y Queue modela espera, la equivalencia entre conversión recursiva e iterativa, por qué el evaluador necesita dos pilas y la prioridad decide cuándo aplicar, la ceguera de `parenRecursive` ante `[]` y `{}`, y por qué los `checks` de N-Reinas miden esfuerzo y no soluciones.
- Qué todavía confundo: el detalle fino de `divideParentheses` con el contador `crc` cuando hay anidamientos profundos, y la mecánica exacta de `incoming`/`outgoing` con `nextESWN` al reintentar direcciones en el laberinto.
- Qué experimento me dio mejor evidencia: el experimento 2, porque el caso `([)]` expuso de forma rotunda la limitación de la versión recursiva frente a la iterativa; y el experimento 4, por el crecimiento explosivo de `checks`.
- Qué evidencia usaría en una sustentación: la tabla de N-Reinas con el salto de 84 a 1.29 millones de `checks`, el laberinto 3 con 10 celdas `BACKTRACKED` y camino final limpio, y la discrepancia `recursivo=true` / `iterativo=false` en `([)]`.
