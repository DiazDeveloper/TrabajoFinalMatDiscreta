#include "pch.h"
#include <iostream>
#include <vector>
#include <iomanip>  // Para usar setw
#include <set>      // Para usar set (para evitar duplicados)
using namespace std;
using namespace System;

void crearMatriz(int N, vector<vector<int>>& matriz)
{
    // Creamos una instancia de Random
    Random^ random = gcnew Random();

    // Llenamos la matriz con relaciones aleatorias (1) con la condici�n de m�ximo 5 relaciones por empleado
    for (int i = 0; i < N; i++) {
        int relaciones = 0;  // Contador de relaciones de cada empleado
        for (int j = i + 1; j < N; j++) {  // Solo recorremos la parte superior de la matriz para evitar duplicados
            if (relaciones < 5 && random->Next(2) == 0) {  // Generamos un n�mero aleatorio entre 0 y 1
                matriz[i][j] = 1;  // E[i] est� en contacto con E[j]
                matriz[j][i] = 1;  // E[j] est� en contacto con E[i] (relaci�n bidireccional)
                relaciones++;  // Aumentamos el contador de relaciones de E[i]

                // Tambi�n aseguramos que E[j] no tenga m�s de 5 relaciones
                int relaciones_j = 0;
                for (int k = 0; k < N; k++) {
                    if (matriz[j][k] == 1) relaciones_j++;
                }
                if (relaciones_j >= 5) {
                    break;  // Si E[j] ya tiene 5 relaciones, salimos de este bucle
                }
            }
        }
    }
}

void imprimirMatrizConInfectado(int N, vector<vector<int>>& matriz, vector<bool>& infectados)
{
    // Imprimimos los encabezados de las columnas (E1, E2, ..., En)
    cout << "      "; // 5 espacios antes de E1
    for (int j = 0; j < N; j++) {
        cout << "E" << j + 1 << "  ";  // Imprime E1, E2, ..., En
    }
    cout << endl;

    // Imprimimos las filas
    for (int i = 0; i < N; i++) {
        // Imprimimos el identificador del empleado en cada fila
        cout << "E" << i + 1 << " ";  // Imprime E1, E2, ..., En en las filas
        for (int j = 0; j < N; j++) {
            // Si estamos en la celda de un empleado infectado, imprimimos "X"
            if (infectados[i] && i == j)
                cout << setw(4) << "X";  // Marca con "X" la posici�n del empleado infectado
            else
                cout << setw(4) << matriz[i][j];  // Imprime los valores de la matriz con alineaci�n
        }
        cout << endl;
    }
}

int main() {
    int N;
    int opcion;

    do {
        // Limpiar la pantalla y mostrar el men�
        system("cls");
        cout << "/////////////////////////////////////////////\n";
        cout << "//              MENU DE CONTAGIOS          // \n";
        cout << "////////////////////////////////////////////\n";
        cout << "\nQue accion deseas hacer?\n";
        cout << "1.- Iniciar simulacion\n";
        cout << "2.- Salir\n";
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        // Procesar la opci�n seleccionada
        switch (opcion) {
        case 1:
            // Iniciar simulaci�n
            cout << "\nIngresa el n�mero de empleados (N entre 5 y 120): ";
            cin >> N;

            if (N < 5 || N > 120) {
                cout << "El n�mero de empleados debe estar entre 5 y 120." << endl;
            }
            else {
                // Creamos la matriz
                vector<vector<int>> matriz(N, vector<int>(N, 0));
                vector<bool> infectados(N, false);  // Vector para saber si un empleado est� infectado

                // Creamos y mostramos la matriz de relaciones
                crearMatriz(N, matriz);

                // Imprimimos la matriz inicial de relaciones
                cout << "\nMatriz inicial de relaciones:\n";
                imprimirMatrizConInfectado(N, matriz, infectados);  // -1 significa que no hay infectados a�n

                // Pedimos al usuario que ingrese el n�mero del empleado a infectar
                int infectado;
                cout << "\nIngresa el n�mero del empleado a infectar (de 1 a " << N << "): ";
                cin >> infectado;

                // Validamos que el n�mero del empleado est� dentro del rango
                if (infectado < 1 || infectado > N) {
                    cout << "N�mero de empleado inv�lido. Debe estar entre 1 y " << N << "." << endl;
                }
                else {
                    infectado -= 1;  // Ajustamos el �ndice para que coincida con la matriz (de 0 a N-1)
                    infectados[infectado] = true;  // Marcamos al empleado infectado
                    int empleadosInfectados = 1;  // Contador de los empleados infectados

                    set<int> infectadosDia2;  // Usamos un set para evitar duplicados
                    bool contagiosPosibles = true;  // Para verificar si a�n hay contagios posibles
                    int diasTotales = 1;  // Contador de d�as

                    // Empezamos el ciclo de contagio por d�as
                    while (contagiosPosibles) {
                        cout << "\nD�a " << diasTotales << ":\n";
                        cout << "Se infect� al empleado n�mero " << infectado + 1 << ".\n";

                        // Imprimimos la matriz con la "X" en el lugar del empleado infectado
                        imprimirMatrizConInfectado(N, matriz, infectados);

                        // Encontramos los empleados infectados por contacto
                        vector<bool> infectadosNuevoDia(N, false);  // Para almacenar los infectados del d�a siguiente

                        // Recorremos todos los empleados infectados del d�a anterior
                        for (int i = 0; i < N; i++) {
                            if (infectados[i]) {
                                // Buscamos los empleados con los que tienen relaci�n
                                for (int j = 0; j < N; j++) {
                                    if (matriz[i][j] == 1 && !infectados[j]) {
                                        infectadosNuevoDia[j] = true;  // Marcamos como infectados a los relacionados
                                        infectadosDia2.insert(j);  // Guardamos los nuevos infectados
                                        empleadosInfectados++;  // Incrementamos el contador de empleados infectados
                                    }
                                }
                            }
                        }

                        // Verificamos si hay nuevos infectados en este d�a
                        if (infectadosDia2.empty()) {
                            contagiosPosibles = false;  // Si no hay nuevos infectados, terminamos el ciclo
                            cout << "\nYa no hay m�s contagios posibles." << endl;
                        }

                        // Actualizamos el estado de los infectados
                        for (int i : infectadosDia2) {
                            infectados[i] = true;  // Marcamos a los nuevos infectados
                        }

                        // Imprimimos los empleados que se infectaron
                        cout << "\nLos empleados infectados en el D�a " << diasTotales << " son: ";
                        for (int i : infectadosDia2) {
                            cout << "E" << i + 1 << " ";
                        }
                        cout << endl;

                        // Espera para el siguiente d�a
                        int continuar;
                        cout << "Presione 0 para salir, o cualquier otra tecla para continuar al siguiente d�a: ";
                        cin >> continuar;  // Leer la entrada del usuario

                        if (continuar == 0) {
                            cout << "\nEl programa ha terminado.\n";
                            break;  // Sale del ciclo y termina el programa
                        }

                        diasTotales++;  // Avanzamos al siguiente d�a
                    }

                    // Recuento de los infectados (contamos las "X" en la �ltima matriz)
                    int infectadosRecuento = 0;
                    for (int i = 0; i < N; i++) {
                        if (infectados[i]) {
                            infectadosRecuento++;
                        }
                    }

                    // Reporte final
                    double porcentaje = ((double)infectadosRecuento / N) * 100;  // Calculamos el porcentaje de empleados infectados
                    cout << "\nD�as transcurridos: " << diasTotales << endl;
                    cout << "N�mero de empleados contagiados: " << infectadosRecuento << endl;
                    cout << "Porcentaje de empleados contagiados: " << fixed << setprecision(2) << porcentaje << "%" << endl;
                    cin.ignore();  
                    cin.get(); 
             
                }
            }
            break;  // Volver al men� despu�s de iniciar la simulaci�n

        case 2:
            // Salir del programa
            cout << "\nEl programa ha terminado. �Hasta luego!\n";
            break;  // Fin del programa

        default:
            cout << "\nOpci�n no v�lida. Intenta de nuevo.\n";
            break;
        }

    } while (opcion != 2);  // Repetir el men� hasta que se seleccione la opci�n 2 (salir)

    return 0;
}
