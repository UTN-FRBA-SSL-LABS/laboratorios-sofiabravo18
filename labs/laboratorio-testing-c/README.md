# Laboratorio: Testing en C

## Verificación y calificación

Desde la raíz del repositorio ejecutá:

```bash
make grade LAB=laboratorio-testing-c
```

Si ya estás dentro de esta carpeta también podés usar `make test`. Ambos
comandos ejecutan los mismos checks y muestran el puntaje sin necesidad de hacer
push. La verificación oficial se ejecuta cuando cambia
`labs/laboratorio-testing-c/`; la nota queda en el resumen y en los artefactos
de GitHub Actions.

No modifiques `.github/`, `scripts/` ni `test_local.sh`: son infraestructura
docente y el workflow restaura automáticamente su versión oficial.

---

## Antes de empezar

Para trabajar local necesitás GCC:
- **Linux:** `sudo apt install gcc`
- **macOS:** `xcode-select --install` o `brew install gcc`
- **Windows:** WSL con Ubuntu recomendado

Si querés trabajar sin instalar nada, usá **GitHub Codespaces**: hacé click en el botón verde `<> Code` → pestaña `Codespaces` → `Create codespace on main`.

---

## Introducción

Tenés que implementar el backend de un carrito de compras simple. El código ya está escrito en `src/carrito.c`, pero **tiene bugs**. Tu trabajo es escribir tests que los descubran y luego corregirlos.

El módulo expone estas funciones:

```c
void carrito_init(Carrito *c);
int  carrito_agregar(Carrito *c, Producto p);  /* 1=ok, 0=lleno */
int  carrito_contar(Carrito *c);
int  carrito_total(Carrito *c);
int  carrito_descuento(int total, int porcentaje);
```

Y estas estructuras (en `src/carrito.h`):

```c
typedef struct {
    char nombre[32];
    int  precio;    /* precio unitario en pesos */
    int  cantidad;
} Producto;

typedef struct {
    Producto items[MAX_ITEMS];  /* MAX_ITEMS = 4 */
    int      cantidad;
} Carrito;
```

---

## Parte 1: Correr el programa

Compilá y ejecutá el programa principal:

```bash
make programa
./programa
```

Vas a ver el carrito con tres productos y su total.

**P1** — Calculá a mano el total esperado: Leche ($350 x2) + Pan ($200 x3) + Queso ($1500 x1).

> R:2800

**P2** — ¿El total que imprime el programa coincide con lo que calculaste? Si no coincide, ¿cuánto muestra?

> R:No

```
TOTAL_PROGRAMA=2050
```
_(escribí el número que imprimió el programa)_

---

## Parte 2: El framework de testing

Antes de seguir, abrí `tests/minunit/minunit.h`. Es el framework que usamos — apenas 60 líneas de C.

Los macros principales son:

```c
ASSERT_IGUAL(esperado, obtenido)  /* verifica que dos enteros sean iguales */
ASSERT_VERDADERO(condicion)       /* verifica que la condicion sea verdadera */
RESUMEN()                         /* imprime cuantos tests pasaron/fallaron */
```

Cuando todos los tests pasan, el programa termina con **exit code 0**. Si alguno falla, termina con **exit code 1**.

---

## Parte 3: Tests ya escritos

Compilá y corré los tests unitarios tal como están:

```bash
make test_unitarios
./test_unitarios
```

Deberías ver dos tests pasando:

```
=== Tests unitarios ===

[carrito nuevo]
  [OK]   carrito_contar(&c) == 0

[agregar un producto]
  [OK]   carrito_agregar(&c, p) == 1
  [OK]   carrito_contar(&c) == 1

--- 3/3 tests OK ---
```

Mirá el código en `tests/test_unitarios.c` para entender la estructura de un test.

**P3** — ¿Qué hace `carrito_init` y por qué es importante llamarla antes de usar el carrito?

> R:Inicializa la cantidad de productos en 0.Es importante llamarla porque en C las variables locales (como Carrito c) no se inicializan solas, el campo cantidad arranca con basura de memoria.Sin carrito_init el for de carrito_total o la condicion de carrito_agregar podrian comportarse de forma impredecible.

---

## Parte 4: Agregar un test (copy-paste)

Ahora vas a agregar un test que verifica el cálculo del total para un producto con cantidad = 1.

Abrí `tests/test_unitarios.c` y pegá esta función en el lugar marcado como `/* PARTE A */`:

```c
void test_total_precio_unitario(void) {
    printf("\n[total: un producto, cantidad 1]\n");
    Carrito c;
    carrito_init(&c);
    Producto p = {"Leche", 350, 1};
    carrito_agregar(&c, p);
    ASSERT_IGUAL(350, carrito_total(&c));
}
```

Luego, en el `main()`, descomentá la línea:
```c
/* test_total_precio_unitario(); */
```

Compilá y corré:

```bash
make test_unitarios
./test_unitarios
```

**P4** — ¿El nuevo test pasa o falla?

> R:Este nuevo test pasa

```
TEST_PRECIO_UNITARIO_PASA=SI
```
_(SI o NO)_

---

## Parte 5: Completar un test con blancos

El siguiente test verifica que el total tiene en cuenta la **cantidad** de cada producto.
Copialo en `tests/test_unitarios.c` (en el lugar `/* PARTE B */`) y completá los blancos:

```c
void test_total_con_cantidad(void) {
    printf("\n[total: un producto, cantidad 2]\n");
    Carrito c;
    carrito_init(&c);
    Producto p = {"Leche", 350, 2};  /* 350 x 2 = 700 */
    carrito_agregar(&c, p);
    ASSERT_IGUAL(____, carrito_total(&c));  /* <-- completar el valor esperado */
}
```

Descomentá `/* test_total_con_cantidad(); */` en el `main()`, compilá y corré.

**P5** — ¿Este test pasa o falla? ¿Qué valor esperaba y qué obtuvo?

> R:Este test falla.Esperaba 700 y obtuvo 350.

```
TEST_TOTAL_CANTIDAD_PASA=NO
```
_(SI o NO)_

---

## Parte 6: Encontrar y corregir el bug

El test anterior encontró un bug en `carrito_total`. Abrí `src/carrito.c` y buscá esa función.

**P6** — ¿En qué línea está el bug y qué dice ese código?

> R:El bug esta en la linea 24 del archivo "carrito.c" que dice total += c->items[i].precio. Dentro de carrito_total, suma solo el precio unitario.

**P7** — ¿Qué debería hacer esa línea para calcular el total correctamente?

> R:debería multiplicar por la cantidad: total += c->items[i].precio * c->items[i].cantidad;

Corregí el bug. Luego volvé a compilar y correr:

```bash
make test_unitarios
./test_unitarios
```

Guardá la salida:

```bash
mkdir -p salidas
./test_unitarios > salidas/test_unitarios.txt 2>&1
cat salidas/test_unitarios.txt
```

```
TESTS_UNITARIOS_PASAN=SI
```
_(escribí SI si todos los tests pasan ahora)_

---

## Parte 7: Escribir un test propio

Ahora escribís un test completamente vos. El carrito tiene capacidad para `MAX_ITEMS = 4` productos.
Cuando está lleno, `carrito_agregar` debería devolver `0` (fallo) en vez de `1` (éxito).

Escribí una función `test_carrito_lleno()` en `tests/test_unitarios.c` (en el lugar `/* PARTE C */`) que:

1. Inicialice un carrito
2. Agregue 4 productos (usá el mismo `Producto` cuatro veces, está bien)
3. Intente agregar un 5to producto
4. Verifique con `ASSERT_IGUAL` que ese último intento devuelve `0`

Descomentá `/* test_carrito_lleno(); */` en el `main()`, compilá y corré.

**P8** — ¿El test pasa o falla? Si falla, ¿qué devuelve ese 5to `carrito_agregar`?

> R:El test falla.El 5.º carrito_agregar devuelve 1 en vez de 0. Además, escribe fuera del array: items tiene 4 lugares, índices válidos 0–3, y este 5.º intento escribe en items[4], que no existe → buffer overflow.

Si el test falló, encontraste el segundo bug. Buscá en `src/carrito.c` la condición del `if` dentro de `carrito_agregar`.

**P9** — ¿Cuál es el operador incorrecto y cuál debería ser?

> R:El operador incorrecto es <=; debería ser <. Con <=, cuando cantidad ya vale 4 (carrito lleno), la condición 4 <= 4 sigue siendo verdadera y deja escribir un 5.º elemento fuera de los límites del array.

Corregí el bug, volvé a compilar y verificá que todos los tests pasan.

```
BUG_2_CORREGIDO=SI
```
_(SI o NO)_

---

## Parte 8: Test de integración guiado

Los tests de integración verifican que las funciones trabajan bien **en combinación**.

Abrí `tests/test_integracion.c`. El archivo ya tiene la estructura armada; vas a agregar una función.

Escribí `test_compra_con_descuento()` en el lugar `/* PARTE D */` siguiendo esta descripción:

- Inicializá un carrito
- Agregá Pan ($200, cantidad 3) → subtotal: $600
- Agregá Leche ($350, cantidad 2) → subtotal: $700
- Total esperado: **$1300**
- Con 10% de descuento: **$1170**
- Usá `ASSERT_IGUAL` para verificar tanto el total como el precio con descuento

Descomentá `/* test_compra_con_descuento(); */` en el `main()`, compilá y corré:

```bash
make test_integracion
./test_integracion
```

Guardá la salida:

```bash
./test_integracion > salidas/test_integracion.txt 2>&1
cat salidas/test_integracion.txt
```

```
TEST_INTEGRACION_PASA=SI
```
_(SI o NO)_

---

## Parte 9: Test de integración propio

Escribí `test_agregar_hasta_llenar()` en el lugar `/* PARTE E */`. Este test debe:

1. Llenar el carrito hasta su capacidad máxima (4 productos)
2. Verificar que el conteo es `MAX_ITEMS`
3. Verificar que intentar agregar uno más devuelve `0`
4. Verificar que el conteo sigue siendo `MAX_ITEMS` (no cambió)

Descomentá `/* test_agregar_hasta_llenar(); */` en el `main()`, compilá y corré.

```
TEST_LLENAR_PASA=SI
```
_(SI o NO)_

---

## Parte 10: Cobertura de código

¿Tus tests ejercitan todo el código que escribiste? `gcov` lo mide:

```bash
make cobertura
cat carrito.c.gcov
```

Las líneas con `#####` nunca se ejecutaron — no están cubiertas por los tests.

**P10** — ¿Hay alguna línea de `carrito.c` con `#####`? ¿Cuál y por qué no se ejecutó?

> R:Sí, las líneas 29 y 30 que corresponden a la función carrito_descuento tienen #####. No se ejecutaron porque el comando make cobertura solo evalúa el código usando los tests unitarios (test_unitarios.c), y nosotros recién usamos la función de descuento en las pruebas de integración (test_integracion.c).

```
COBERTURA_COMPLETA=NO
```
_(SI si todas las líneas están cubiertas, NO si hay alguna con #####)_

---

## Preguntas de reflexión

**P11** — ¿Qué diferencia hay entre un test unitario y uno de integración? ¿Cuál de los dos detectó primero el bug de `carrito_total`?

> R:un test unitario prueba una función aislada (por ejemplo, carrito_total sola); uno de integración prueba varias funciones trabajando juntas (agregar productos + calcular total + aplicar descuento). El bug de carrito_total lo detectó primero un test unitario.

**P12** — El bug de capacidad en `carrito_agregar` causa un **buffer overflow**: se escribe más allá del array. ¿Por qué esto es peligroso en C pero no ocurriría en un lenguaje como Python o Java?

> R:en C, escribir en items[4] cuando el array tiene tamaño 4 no lanza ningún error: el compilador y el runtime no verifican límites de array. Esa escritura cae en memoria que pertenece a otra variable (o queda simplemente indefinida), lo que puede corromper datos, provocar comportamiento impredecible o crashear el programa más adelante, lejos de donde ocurrió el error real. En Python o Java, en cambio, acceder fuera de los límites de una lista/array lanza una excepción inmediatamente (IndexError, ArrayIndexOutOfBoundsException) porque esos lenguajes chequean los límites en cada acceso.

**P13** — En este laboratorio encontraste los bugs escribiendo tests. ¿Qué tiene de mejor este enfoque frente a mirar el código directamente?

> R:un test da un resultado objetivo y automático (pasa/falla) en vez de depender de que una persona note el error a simple vista.Además, una vez escrito, el test queda para siempre como red de seguridad: si alguien reintroduce el bug en el futuro, se detecta al instante.

**P14** — El test `test_total_precio_unitario` (cantidad = 1) **pasó** a pesar del bug, mientras que `test_total_con_cantidad` (cantidad = 2) **falló**. ¿Por qué el primer test no detectó el bug?

> R:porque usaba cantidad = 1, y precio × 1 da exactamente lo mismo que solo precio. El bug (no multiplicar por cantidad) solo se manifiesta cuando la cantidad es distinta de 1 — por eso hizo falta el test con cantidad = 2 para exponerlo.

```
BUG_EN_FUNCION_1=carrito_total
```
_(nombre de la función con el primer bug)_

```
BUG_EN_FUNCION_2=carrito_agregar
```
_(nombre de la función con el segundo bug)_

---

## Entrega

### Checklist

- [ ] Ambos bugs corregidos en `src/carrito.c`
- [ ] Tests en `tests/test_unitarios.c` y `tests/test_integracion.c` completos y pasando
- [ ] Salidas guardadas en `salidas/`
- [ ] Respuestas y claves completadas en este archivo
- [ ] `make test` pasa localmente
- [ ] Todo pusheado a `main`

### Verificación local

Antes de hacer push, verificá tu puntaje con:

```bash
make test
```

**Flujo recomendado:** hacé commits frecuentes mientras avanzás, usá `make test` para verificar tu progreso, y dejá el push para cuando una parte esté realmente lista.

Cuando estés listo/a:

```bash
git add .
git commit -m "Agrego tests y corrijo bugs en carrito"
git push
```

### Integración continua

Cuando pusheás cambios dentro de `labs/laboratorio-testing-c/`, el workflow central del monorepo ejecuta los mismos checks y valida el puntaje mínimo.

> ⚠️ **Evitá pushes innecesarios.** Cada ejecución consume cómputo en servidores de GitHub — un recurso compartido. `make test` te da el mismo resultado en tu terminal sin costo.

Para ver los resultados:

1. Entrá a tu repositorio en GitHub
2. Hacé click en la pestaña **Actions**
3. Hacé click en la ejecución más reciente → job **Grading · laboratorio-testing-c**
4. Al final del job vas a ver la tabla con el resultado de cada check y el puntaje total

---

## Ejercitación extra

**E1** — Agregá una función `int carrito_buscar(Carrito *c, char *nombre)` que devuelva el índice del producto con ese nombre, o `-1` si no existe. Escribí al menos 3 tests unitarios para ella.

**E2** — Modificá `carrito_total` para que ignore productos con `cantidad <= 0`. Agregá un test que lo verifique.

**E3** — Deliberadamente escribí un test que falle (ponés un valor incorrecto en `ASSERT_IGUAL`). Corré los tests y observá el mensaje de error. ¿Qué información da el framework? Luego revertí el cambio.

**E4** — Investigá qué es **TDD** (Test Driven Development). ¿En qué orden se escribe el código? ¿En qué se diferencia de lo que hiciste en este laboratorio?
