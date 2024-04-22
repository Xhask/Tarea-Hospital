#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define MAX 100

typedef struct tipoPaciente {
  int prioridad;
  int nFicha;
  char nombre[MAX];
  int edad;
  char sintoma[MAX];
  time_t hora;
} tipoPaciente;

int totalPacientes = 0;

// Declaración de funciones
void limpiarPantalla();
void strMay(char *cadena);
void presioneTeclaParaContinuar();
void mostrarMenuPrincipal();
void ordenarPorPrioridad(tipoPaciente *listaPacientes);
void registrarPaciente(tipoPaciente *listaPacientes);
void mostrarListaPacientes(tipoPaciente *listaPacientes);
int contadorPrioridad(tipoPaciente *listaPacientes, int prioridad);
void mostrarListaPrioridad(tipoPaciente *listaPacientes);
void asignarPrioridad(tipoPaciente *listaPacientes);
void siguientePaciente(tipoPaciente *listaPacientes);

// Función para limpiar la pantalla
void limpiarPantalla() {
  system("clear");
}

// Convierte una cadena a mayúsculas
void strMay(char *cadena) {
  while (*cadena != '\0') {
    *cadena = toupper(*cadena);
    cadena++;
  }
}

// Espera a que el usuario presione una tecla para continuar
void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Muestra el menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}

// Ordena la lista de pacientes por prioridad y hora
void ordenarPorPrioridad(tipoPaciente *listaPacientes) {
  bool huboIntercambio;
  do {
    huboIntercambio = false;
    tipoPaciente *pacienteActual = listaPacientes;
    tipoPaciente *siguientePaciente = listaPacientes + 1;

    while (siguientePaciente != NULL) {
      if (pacienteActual->prioridad < siguientePaciente->prioridad ||
          (pacienteActual->prioridad == siguientePaciente->prioridad &&
           pacienteActual->hora > siguientePaciente->hora)) {
        // Intercambiar los pacientes
        tipoPaciente temp = *pacienteActual;
        *pacienteActual = *siguientePaciente;
        *siguientePaciente = temp;
        huboIntercambio = true;
      }

      pacienteActual = siguientePaciente;
      siguientePaciente++;
    }
  } while (huboIntercambio);
}

// Registra un nuevo paciente
void registrarPaciente(tipoPaciente *listaPacientes) {
  printf("\nRegistrar nuevo paciente \n");

  tipoPaciente *paciente = (tipoPaciente *)malloc(sizeof(tipoPaciente));
  if (paciente == NULL) {
    printf("ERROR DE MEMORIA");
    return;
  }

  printf("Ingresa el nombre del paciente: \n");
  getchar();
  char nombreIngresado[MAX];
  scanf(" %[^\n]", nombreIngresado);
  strMay(nombreIngresado);

  bool encontrado = false;
  tipoPaciente *pacienteActual = listaPacientes;
  while (pacienteActual != NULL && !encontrado) {
    if (strcmp(pacienteActual->nombre, nombreIngresado) == 0) {
      puts("El paciente ya se encuentra registrado");
      encontrado = true;
      free(paciente);
      return;
    }
    pacienteActual++;
  }

  if (!encontrado) {
    strcpy(paciente->nombre, nombreIngresado);
    printf("Ingresa la edad del paciente: \n");
    scanf("%i", &paciente->edad);

    printf("Ingresa el síntoma del paciente:\n");
    getchar();
    scanf(" %[^\n]", paciente->sintoma);
    strMay(paciente->sintoma);

    paciente->prioridad = 1; // Se asigna prioridad baja automáticamente
    time(&paciente->hora);

    *(listaPacientes + totalPacientes) = *paciente;
    totalPacientes++;
    paciente->nFicha = totalPacientes;

    ordenarPorPrioridad(listaPacientes);
  }
}

// Muestra la lista de pacientes en espera
void mostrarListaPacientes(tipoPaciente *listaPacientes) {
  printf("Pacientes en espera: \n");
  tipoPaciente *pacienteActual = listaPacientes;

  if (pacienteActual == NULL) {
    puts("No hay pacientes registrados");
    return;
  }

  puts("======================================================================="
       "======");
  printf("|                          Pacientes en espera: %d                   "
         "        |\n",
         totalPacientes);
  puts("======================================================================="
       "======");
  printf("| %-20s | %-10s | %-25s | %-8s |\n", "Nombres", "Edad", "Síntoma",
         "Prioridad");
  puts("======================================================================="
       "======");
  while (pacienteActual != NULL) {
    char nombre[MAX];
    strcpy(nombre, pacienteActual->nombre);

    int edad = pacienteActual->edad;

    char sintoma[MAX];
    strcpy(sintoma, pacienteActual->sintoma);

    int prioridad = pacienteActual->prioridad;

    printf("| %-20s | %-10d | %-25s |", nombre, edad, sintoma);
    if (prioridad == 1)
      printf(" %-9s |\n", "Baja");
    else if (prioridad == 2)
      printf(" %-9s |\n", "Media");
    else if (prioridad == 3)
      printf(" %-9s |\n", "Alta");

    pacienteActual++;
  }
}

// Contador de pacientes con cierta prioridad
int contadorPrioridad(tipoPaciente *listaPacientes, int prioridad) {
  int cont = 0;
  tipoPaciente *pacienteActual = listaPacientes;
  while (pacienteActual != NULL) {
    if (pacienteActual->prioridad == prioridad)
      cont += 1;
    pacienteActual++;
  }
  return cont;
}

// Muestra la lista de pacientes según una prioridad especificada
void mostrarListaPrioridad(tipoPaciente *listaPacientes) {
  int prioridadIngresada;
  bool validPrioridad = false;

  while (!validPrioridad) {
    printf("\nIngrese el número de la prioridad que desea mostrar: \n");
    printf("1) Baja\n");
    printf("2) Media\n");
    printf("3) Alta\n");
    scanf("%i", &prioridadIngresada);
    if (prioridadIngresada >= 1 && prioridadIngresada <= 3)
      validPrioridad = true;
    else
      puts("Prioridad no válida, ingrese nuevamente");
  }

  int cantidadPrioridad = contadorPrioridad(listaPacientes, prioridadIngresada);

  tipoPaciente *pacienteActual = listaPacientes;

  if (pacienteActual == NULL || cantidadPrioridad == 0) {
    puts("No hay pacientes registrados con esa prioridad");
    return;
  }

  puts("======================================================================="
       "======");
  printf("|                          Pacientes en espera: %d                   "
         "        |\n",
         cantidadPrioridad);
  puts("======================================================================="
       "======");
  printf("| %-20s | %-10s | %-25s | %-8s |\n", "Nombres", "Edad", "Síntoma",
         "Prioridad");
  puts("======================================================================="
       "======");
  while (pacienteActual != NULL) {
    char nombre[MAX];
    strcpy(nombre, pacienteActual->nombre);

    int edad = pacienteActual->edad;

    char sintoma[MAX];
    strcpy(sintoma, pacienteActual->sintoma);

    int prioridad = pacienteActual->prioridad;
    if (prioridad == prioridadIngresada) {
      printf("| %-20s | %-10d | %-25s |", nombre, edad, sintoma);
      if (prioridad == 1)
        printf(" %-9s |\n", "Baja");
      else if (prioridad == 2)
        printf(" %-9s |\n", "Media");
      else if (prioridad == 3)
        printf(" %-9s |\n", "Alta");
    }

    pacienteActual++;
  }
}

// Asigna una nueva prioridad a un paciente
void asignarPrioridad(tipoPaciente *listaPacientes) {
  if (totalPacientes == 0) {
    puts("No hay pacientes registrados");
    return;
  }

  printf("Ingrese el nombre del paciente del cual quiere cambiar la prioridad:\n");
  getchar();
  char nombrePaciente[MAX];
  scanf(" %[^\n]", nombrePaciente);
  strMay(nombrePaciente);

  tipoPaciente *pacienteActual = listaPacientes;

  while (pacienteActual != NULL) {
    if (strcmp(pacienteActual->nombre, nombrePaciente) == 0) {
      break;
    }
    pacienteActual++;
  }

  if (pacienteActual == NULL) {
    puts("No se ha encontrado al paciente");
    return;
  }

  printf("El paciente tiene prioridad ");
  if (pacienteActual->prioridad == 1)
    printf("baja");
  else if (pacienteActual->prioridad == 2)
    printf("media");
  else if (pacienteActual->prioridad == 3)
    printf("alta");

  int nuevaPrioridad;
  bool validPrioridad = false;

  while (!validPrioridad) {
    printf("\nIngrese el número de la nueva prioridad: \n");
    printf("1) Baja\n");
    printf("2) Media\n");
    printf("3) Alta\n");
    scanf("%i", &nuevaPrioridad);
    if (nuevaPrioridad >= 1 && nuevaPrioridad <= 3)
      validPrioridad = true;
    else
      puts("Prioridad no válida, ingrese nuevamente");
  }

  if (pacienteActual->prioridad == nuevaPrioridad) {
    puts("No se ha hecho ningún cambio");
  } else {
    pacienteActual->prioridad = nuevaPrioridad;
    puts("Se han guardado los cambios");
  }

  ordenarPorPrioridad(listaPacientes);
}

// Atiende al siguiente paciente en la lista
void siguientePaciente(tipoPaciente *listaPacientes) {
  if (totalPacientes == 0) {
    puts("No hay pacientes registrados");
    return;
  }

  puts("¿Desea pasar al siguiente en la lista de espera?");
  puts("Y/N");
  getchar();

  char confirmacion;
  bool confirmado = false;

  while (!confirmado) {
    scanf(" %c", &confirmacion);
    confirmacion = toupper(confirmacion);

    if (confirmacion != 'Y' && confirmacion != 'N') {
      puts("Respuesta no válida");
      puts("Intente de nuevo:");
      while (getchar() != '\n');
    } else if (confirmacion == 'N') {
      puts("Acción cancelada");
      return;
    } else {
      confirmado = true;
    }
  }

  // Mostrar el primer paciente en espera
  tipoPaciente *pacienteActual = listaPacientes;

  if (pacienteActual == NULL) {
    puts("No hay pacientes registrados");
    return;
  }

  puts("\nPaciente atendido:");
  printf("Nombre: %s\n", pacienteActual->nombre);
  printf("Edad: %i\n", pacienteActual->edad);
  printf("Síntoma: %s\n", pacienteActual->sintoma);

  // Eliminar al paciente atendido moviendo los elementos hacia adelante
  for (int i = 0; i < totalPacientes - 1; i++) {
    *(listaPacientes + i) = *(listaPacientes + i + 1);
  }

  // Reducir el contador de pacientes
  totalPacientes--;

  printf("Se ha continuado con el siguiente paciente\n");
  printf("Pacientes restantes: %i\n", totalPacientes);
}

int main() {
  char opcion;
  tipoPaciente listaPacientes[MAX];

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el newline anterior

    switch (opcion) {
    case '1':
      registrarPaciente(listaPacientes);
      break;
    case '2':
      asignarPrioridad(listaPacientes);
      break;
    case '3':
      mostrarListaPacientes(listaPacientes);
      break;
    case '4':
      siguientePaciente(listaPacientes);
      break;
    case '5':
      mostrarListaPrioridad(listaPacientes);
      break;
    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }

    presioneTeclaParaContinuar();

  } while (opcion != '6');

  return 0;
}