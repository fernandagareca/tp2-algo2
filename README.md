<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Marianela fernanda Gareca Janko - 109606 - fernanda.gareca321@gmail.com

- Para compilar:

```bash
make main
```

- Para ejecutar:

```bash
./main
```

- Para ejecutar con valgrind:
```bash
make
```
---
##  Funcionamiento



### Diagrama de memoria de juego

<div align="center">
<img width="70%" src="img/juego.jpeg">
</div>

---
### Diagrama de memoria de adversario
<div align="center">
<img width="70%" src="img/adversario.jpeg">
</div>

---
### Diagrama de memoria de menu

<div align="center">
<img width="70%" src="img/menu.jpeg">
</div>


## Implementacion de juego



En la función `juego cargar Pokémon` verificó que el juego y el archivo no sean null, después cargo el campo info de la estructura de juego verificando que la función `Pokémon cargar archivo` devuelva una información válida, en caso de que la cantidad de pokemones sea menor a la mínima ,liberoa la memoria reservada y  retorno POKEMONES INSUFICIENTES.
### complejidad
 La complejidad de esta funcion es:
 la funcion `pokemon cargar archivo` lee too el archivo de pokemones por lo que hace un trabajo O(n) y en el caso de tene que destruir la informacion de pokemones porque no son suficientes se eliminan todos los elementos que tambien seria un trabajo O(n) .por lo que la complejidad es O(n)+O(n) = O(n).

---
 En la función `juego listar pokemon` verifico que el juego exista, y cargo la lista de la estructura juego con todos los pokemones que se encuentran en la info de pokemones y devuelvo la lista.

### complejidad

La funcion `con cada pokemon` recorre todos los elementos cargados y le aplica la funcion enviada por lo que seria O(n) ,despues en la fucnion que
se les aplica `funcion insertar pokes` hago una insercion O(1) ya que enserta al final segun mi inplemtacion por lo que estoy haciendo un trabajo n*O(1) ya que 
`lista insertar` inserta al final teniendo un puntero al ultimo elmento.por  lo que en toda la funcion la complejidad seria O(n).

---

En la función `juego seleccionar Pokémon` verifico que el juego y los nombres recibidos no sean null , verifico que los nombres no se repitan y que dichos pokemones existan, segun el tipo de jugador inserto en su lista los primeros 2 pokemones, pero el 3.er Pokémon lo inserto en la lista del otro jugador, cargo los ataques en el vector del jugador correspondiente para usarlo despues,use listas para guaradar los pokemones de cada jugador porque quise preservar el orden de 
seleccion y porque son solo 3 pokemones.

### complejidad
En la función se utiliza `strcmp` para comparar los nombres ingresados lo cual haría un trabajo O(n) porque recorrer todos los caracteres, pero como estamos evaluando con respecto a la cantidad de pokemones lo tomare como constante, buscar los pokemones en el peor caso es lineal, ya que deberíamos recorrer toda la lista 1 vez para cada Pokémon, lo que seria n+n+n =3*n, después para insertar los pokemones en sus correspondientes listas hacemos un trabajo constante, porque la funcion `lista insertar` inserta al final de la lista y la función `lista insertar en posición`, siempre inserto en la posicion 0 lo que también es constante 3*O(1), pero para cargar los ataques en la función`con cada ataque` recorro todos los ataques de cada Pokémon y le aplico la función `función insertar` que inserta los ataques en el vector del jugador lo que termina teniendo un costo lineal por lo que la complejidad de todo la función seria  3*n + 3*O(1) + 3*(n) = O(n).

---

En la función `juego jugar turno` verifico que los pokemones existan en la lista de pokemones, en caso contrario retorno el resultado, después buscó los ataques de cada jugada recibida y en la función `se encuentra` recorro el vector de ataque que cargue previamente, buscando el ataque de la jugada, y cada vez que lo encuentra cambio el nombre del ataque ha usado para que cuando lo vuelva a buscar ese ataque no lo encuentre y no pueda usar una ataque que ya fue usado, en caso que el ataque no se encuentre salgo de la función retornando el resultado vacio, caso contrario calculo que si el ataque de cada jugador es efectivo en la función `ataque efectivo` y por último sumo los puntos correspondientes en el campo puntaje de cada jugador, devuelvo el resultado.

### complejidad 
El costo de buscar los pokemones en la lista en la función `lista buscar elemento` en lineal, ya que debería recorrer toda la lista en el peor caso, después busco el ataque ingresado lo cual realiza un trabajo lineal para cada Pokémon y en la función `se encuentra` busco el ataque recibido en el peor caso tendría que recorrer todo el vector, la función `ataque efectivo` es contante` porque solo compara los tipos, la función `puntos` también es constante porque solo hace cuentas por lo que la complejidad termina siendo 2*O(n)+2*O(n)+2*O(n)+ 2*O(1)+2*O(1) = O(n).

---

En la función `juego obtener puntaje` devuelvo el puntaje según el jugador recibido, si el juego lo existe devuelvo 0,los puntajes los guarde en una campo en la estructura de jugador
### complejidad 
La funcion es constante ya que guarde los puntajes en un campo por lo que el trabajo es O(1).

---

En la función `juego finalizado` devuelve false mientras los 9 turnos en total no se hayan acabado, si los turnos que se van restando en `juego jugar turno` legan a 0 el juego la función devuelve true.
### complejidad
la funciones contante ya que guarade los turnos permitidos en un campo de cada jugador.

---

En la funcion `juego_destruir` si el juego no existe salgo caso contrario libero la lista de todos los pokemones en la funcion`lista destruir` 
libero las listas de cada jugador ,libero la informacion de todos los pokemones y libero el juego.

### complejidad
La complejidad de destruir las 3 listas es n+n+n = 3*n, al liberar la información también recorro todos los elementos por lo que es O(n) y al final libero el juego por lo que la complejidad de toda función es O(n).

---
