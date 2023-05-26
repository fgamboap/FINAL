#include <iostream>
#include <stdlib.h>

using namespace std;

struct corredor {
    int numero;
    string nombre;
    int vueltas;
    corredor* sig;
};

struct promedio {
    int numero;
    float tiempoPromedio;
    promedio* izq;
    promedio* der;
    int altura;
};

corredor* cab = NULL;
corredor* aux = NULL;
corredor* aux2 = NULL;
promedio* raiz = NULL;

int obtenerAltura(promedio* nodo) {
    if (nodo == NULL) {
        return 0;
    }
    return nodo->altura;
}


int obtenerFactorEquilibrio(promedio* nodo) {
    if (nodo == NULL) {
        return 0;
    }
    return obtenerAltura(nodo->izq) - obtenerAltura(nodo->der);
}


void actualizarAltura(promedio* nodo) {
    int alturaIzq = obtenerAltura(nodo->izq);
    int alturaDer = obtenerAltura(nodo->der);
    nodo->altura = max(alturaIzq, alturaDer) + 1;
}


promedio* rotarDerecha(promedio* nodo) {
    promedio* nuevoPadre = nodo->izq;
    nodo->izq = nuevoPadre->der;
    nuevoPadre->der = nodo;
    actualizarAltura(nodo);
    actualizarAltura(nuevoPadre);
    return nuevoPadre;
}


promedio* rotarIzquierda(promedio* nodo) {
    promedio* nuevoPadre = nodo->der;
    nodo->der = nuevoPadre->izq;
    nuevoPadre->izq = nodo;
    actualizarAltura(nodo);
    actualizarAltura(nuevoPadre);
    return nuevoPadre;
}


promedio* balancearNodo(promedio* nodo) {
    actualizarAltura(nodo);
    int factorEquilibrio = obtenerFactorEquilibrio(nodo);

    if (factorEquilibrio > 1) {
        if (obtenerFactorEquilibrio(nodo->izq) < 0) {
            nodo->izq = rotarIzquierda(nodo->izq);
        }
        return rotarDerecha(nodo);
    }

    if (factorEquilibrio < -1) {
        if (obtenerFactorEquilibrio(nodo->der) > 0) {
            nodo->der = rotarDerecha(nodo->der);
        }
        return rotarIzquierda(nodo);
    }

    return nodo;
}


promedio* insertarPromedio(promedio* nodo, promedio* nuevo) {
    if (nodo == NULL) {
        return nuevo;
    }

    if (nuevo->tiempoPromedio < nodo->tiempoPromedio) {
        nodo->izq = insertarPromedio(nodo->izq, nuevo);
    } else {
        nodo->der = insertarPromedio(nodo->der, nuevo);
    }

    return balancearNodo(nodo);

}

void registrar() {
    aux = new corredor;
    aux->sig = NULL;
    cout << "Proporcione el nombre del corredor: ";
    cin >> aux->nombre;
    cout << "Proporcione el número del corredor: ";
    cin >> aux->numero;
    cout << "Proporcione la cantidad de vueltas realizadas por el corredor: ";
    cin >> aux->vueltas;

    if (aux->vueltas >= 2) {
        if (cab == NULL) {
            cab = aux;
        } else {
            aux2 = cab;
            while (aux2->sig != NULL) {
                aux2 = aux2->sig;
            }
            aux2->sig = aux;
        }

    
        float tiempoTotal = 0.0;
        for (int i = 0; i < aux->vueltas; i++) {
            float tiempo;
            cout << "Ingrese el tiempo de la vuelta " << (i + 1) << ": ";
            cin >> tiempo;
            tiempoTotal += tiempo;
        }
        float tiempoPromedio = tiempoTotal / aux->vueltas;

        promedio* nuevo = new promedio;
        nuevo->numero = aux->numero;
        nuevo->tiempoPromedio = tiempoPromedio;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        nuevo->altura = 1; 

        raiz = insertarPromedio(raiz, nuevo);
    } else {
        delete aux;
        cout << "El corredor no ha realizado al menos dos vueltas. No se registra." << endl;
    }
}

void mostrarCorredores() {
    cout << "Lista de corredores registrados con dos o más vueltas:" << endl;
    aux = cab;
    bool corredoresEncontrados = false;

    while (aux != NULL) {
        if (aux->vueltas >= 2) {
            cout << "Número: " << aux->numero << " - Nombre: " << aux->nombre << " - Vueltas: " << aux->vueltas << endl;
            corredoresEncontrados = true;
        }
        aux = aux->sig;
    }

    if (!corredoresEncontrados) {
        cout << "No se encontraron corredores con dos o más vueltas registradas." << endl;
    }
}

void mostrarPromedios(promedio* nodo) {
    if (nodo != NULL) {
        mostrarPromedios(nodo->izq);  
        cout << "Número: " << nodo->numero << " - Tiempo promedio: " << nodo->tiempoPromedio << endl;
        mostrarPromedios(nodo->der);  
    }
}

int main() {
    int opcion = 0;
    do {
        cout << endl << "1. Registrar corredor" << endl;
        cout << "2. Mostrar corredores" << endl;
        cout << "3. Mostrar promedios de tiempo" << endl;
        cout << "4. Salir" << endl;
        cout << "Ingrese una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrar();
                break;
            case 2:
                mostrarCorredores();
                break;
            case 3:
                cout << "Promedios de tiempo registrados (de menor a mayor):" << endl;
                mostrarPromedios(raiz);
                break;
            case 4:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opción inválida. Intente nuevamente." << endl;
                break;
        }
    } while (opcion != 4);

    
    aux = cab;
    while (aux != NULL) {
        aux2 = aux;
        aux = aux->sig;
        delete aux2;
    }

    return 0;
}