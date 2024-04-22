#include "tdas/list.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOMBRE 50

typedef struct {
  int prioridad;
  char nombre[MAX_NOMBRE];
  int edad;
  char sintoma[MAX_NOMBRE];
  time_t hora;
} Paciente;

int totalPacientes = 0;

void limpiarPantalla() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void strMay(char *cadena) {
  while (*cadena) {
    *cadena = toupper(*cadena);
    cadena++;
  }
}

void presioneTeclaParaContinuar() {
  printf("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

void mostrarMenuPrincipal() {
  limpiarPantalla();
  printf("========================================\n");
  printf("     Sistema de Gestión Hospitalaria\n");
  printf("========================================\n");

  printf("1) Registrar paciente\n");
  printf("2) Asignar prioridad a paciente\n");
  printf("3) Mostrar lista de espera\n");
  printf("4) Atender al siguiente paciente\n");
  printf("5) Mostrar pacientes por prioridad\n");
  printf("6) Salir\n");
}

void ordenarPorPrioridad(List *listaPacientes) {
  bool intercambio;
  do {
    intercambio = false;
    ListIterator iter = list_begin(listaPacientes);
    while (!list_iterator_end(listaPacientes, iter)) {
      Paciente *actual = list_iterator_get(listaPacientes, iter);
      Paciente *siguiente = list_iterator_next(listaPacientes, &iter);

      if (siguiente != NULL && (actual->prioridad < siguiente->prioridad ||
                                (actual->prioridad == siguiente->prioridad &&
                                 actual->hora > siguiente->hora))) {
        // Intercambiar pacientes
        Paciente temp = *actual;
        *actual = *siguiente;
        *siguiente = temp;
        intercambio = true;
      }
    }
  } while (intercambio);
}

void registrarPaciente(List *listaPacientes) {
  printf("\nRegistrar nuevo paciente\n");
  Paciente *paciente = malloc(sizeof(Paciente));
  if (paciente == NULL) {
    printf("ERROR DE MEMORIA\n");
    return;
  }

  printf("Ingresa el nombre del paciente:\n");
  getchar();
  scanf(" %[^\n]", paciente->nombre);
  strMay(paciente->nombre);

  printf("Ingresa la edad del paciente:\n");
  scanf("%d", &paciente->edad);

  printf("Ingresa el síntoma del paciente:\n");
  getchar(); // Consumir el salto de línea pendiente en el buffer
  scanf(" %[^\n]", paciente->sintoma);
  strMay(paciente->sintoma);

  paciente->prioridad = 1; // Asignar prioridad baja por defecto
  time(&paciente->hora);

  list_pushBack(listaPacientes, paciente);
  totalPacientes++;
  ordenarPorPrioridad(listaPacientes);
}

// Resto de las funciones...

int main() {
  List *listaPacientes = list_create();
  char opcion;

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      registrarPaciente(listaPacientes);
      break;
      // Implementar el resto de las opciones del menú...
    }

    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos al finalizar
  list_destroy(listaPacientes);

  return 0;
}