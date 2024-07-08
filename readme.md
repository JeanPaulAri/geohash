# EDA PROJECT

### This is the master branch

# Proyecto de Busqueda de Arista Mas Cercana usando Geohash

Este proyecto se encarga de encontrar la arista mas cercana a un punto geografico dado (latitud, longitud) utilizando el algoritmo de geohash para la busqueda espacial. Los datos utilizados incluyen nodos y aristas representados en archivos CSV (dataset).

## Requisitos

- Un compilador de C++ compatible con C++11 o superior (por ejemplo, g++)
- Archivos CSV de nodos (`nodes.csv`) y aristas (`edges.csv`)

## Archivos del Proyecto

- `main.cpp`: Archivo principal con la logica del programa.
- `geohash.h` : Implementación de la codificación de geohash y sus vecinos.
- `nodes.csv`: Archivo CSV que contiene la información de los nodos (ID, latitud, longitud).
- `edges.csv`: Archivo CSV que contiene la información de las aristas (nodo origen, nodo destino, nombre de la calle).

## Compilación y Ejecucion

### Compilacion

Para compilar el proyecto, abre una terminal y luego, ejecuta el siguiente comando:

```sh
g++ -o geohash_project main.cpp
```
### Ejecucion

Para ejecutar el proyecto, usa el siguiente comando:

```sh
./geohash_project
```
### Exportacion de Resultados

El programa exportara dos archivos CSV con los siguientes contenidos:

1. `query_exported.csv`: Contiene la latitud y longitud del punto consultado y los nodos `u` y `v` de la arista mas cercana encontrada.
   - Columnas: `query_lat, query_lon, result_u, result_v`

2. `nodes_geohashed.csv`: Contiene los nodos con sus IDs, latitudes, longitudes y el geohash correspondiente.
   - Columnas: `osmid, lat, lon, geohash`


![mapa de arequipa](https://github.com/JeanPaulAri/geohash/blob/master/img1.png)

![localizacion](https://github.com/JeanPaulAri/geohash/blob/master/img2.png)