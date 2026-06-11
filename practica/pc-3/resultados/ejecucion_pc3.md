# Evidencia de ejecución — PC3 Maximum Frequency Stack

_Generado el: 

## `./build/practica/pc-3/pc3_main`

```
push: 5 7 5 7 4 5
size = 6, maximum frequency = 3
pop : 5 7 5 4 7 5
expected: 5 7 5 4 ... (then the rest in frequency order)
```

## `./build/practica/pc-3/demos/demo_frequency_stack`

```

=== LeetCode 895 example ===
push 5  -> maxFreq=1 size=1
push 7  -> maxFreq=1 size=2
push 5  -> maxFreq=2 size=3
push 7  -> maxFreq=2 size=4
push 4  -> maxFreq=2 size=5
push 5  -> maxFreq=3 size=6
pops: 5 7 5 4 7 5   (expected: 5 7 5 4 1...)

=== Frequency tie: the most recent wins ===
pop = 2  (expected 2: ties at freq 2 but is more recent)
pop = 1  (expected 1)

=== Single repeated value ===
pops: 9 9 9 9   (expected: 9 9 9 9)
```
