# Deque monotónico — técnica utilizada en la solución óptima

## Qué es un deque monotónico

Un deque monotónico es un deque (cola de doble extremo) en el que los elementos están siempre ordenados de mayor a menor (monotónico decreciente) o de menor a mayor (monotónico creciente). Se mantiene este invariante al insertar: antes de agregar un nuevo elemento por la cola, se eliminan todos los que violarían el orden.

El costo total de todas las inserciones y eliminaciones es O(n) amortizado, porque cada elemento entra y sale del deque exactamente una vez.

## Por qué se necesitan dos deques

El problema exige conocer en todo momento el **máximo** y el **mínimo** de la ventana actual `nums[l..r]`. Un solo deque no puede rastrear ambos a la vez, por lo que se usan dos:

- `maxDq`: deque monotónico decreciente → `maxDq.front()` es el índice del máximo.
- `minDq`: deque monotónico creciente → `minDq.front()` es el índice del mínimo.

## Invariante de la ventana

Antes de cada iteración y al terminar:

```
nums[maxDq.front()] - nums[minDq.front()] <= limit
```

Si esta diferencia supera `limit` después de incorporar `nums[r]`, el puntero izquierdo `l` avanza y se expulsan del frente los índices que quedan fuera de la ventana.

## Flujo del algoritmo

```
para r = 0 hasta n-1:
    1. Mantener maxDq: sacar por cola los índices con valor <= nums[r]
       Insertar r por la cola de maxDq.
    2. Mantener minDq: sacar por cola los índices con valor >= nums[r]
       Insertar r por la cola de minDq.
    3. Mientras la diferencia max - min > limit:
           avanzar l
           si maxDq.front() < l: sacar por frente de maxDq
           si minDq.front() < l: sacar por frente de minDq
    4. Actualizar respuesta: ans = max(ans, r - l + 1)
```

## Complejidad

| Aspecto | Valor | Razón |
|---------|-------|-------|
| Tiempo  | O(n)  | Cada índice entra y sale de cada deque exactamente una vez |
| Espacio | O(n)  | En el peor caso los dos deques almacenan todos los índices |

## Comparación con la solución ingenua

La solución ingenua revisa todos los subarreglos `nums[i..j]` con dos bucles anidados, lo que resulta en O(n²). Para n = 100 000 esto representa ~10⁹ operaciones, mientras que la solución con deques monotónicos las reduce a ~10⁵.
