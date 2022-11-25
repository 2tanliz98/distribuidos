#include <iostream>
#include <thread>
#include <queue>
#include <cstdlib>
#include <ctime>

class proceso{
    public:
    int pid;
    int* token;

    void liberarToken(servidor* S, int* tk){
        S->disponible = true;
        free(token);
    }
};

class servidor{
    private:
    int token;
    int timeout;
    std::queue <proceso*> cola;

    public:
        bool disponible;

        servidor(){
            this -> token = 123;
            this -> disponible = true;
            this -> timeout = 100;
        }

        void requestToken(int* tk, proceso* P){
            proceso* sigpid;
            if (this->disponible)
            {
                if (cola.empty()) //la cola está vacía, se toma directamente el token
                {
                    *P->token = token;
                    this->disponible = false;
                }
                else    //en caso contrario se hace pop y se le asigna el token al proceso siguiente
                {
                    sigpid = cola.front();
                    cola.pop();
                    *sigpid->token = token;
                    this->disponible = false;
                }                
            }
            else //si el token no está disponible se encola :)
            {
                cola.push(P);
            }
        }

        void actualizar(){
            proceso* sigpid;
            if (!cola.empty())
            {
                sigpid = cola.front();
                cola.pop();
            }
        }

};


int main (){

    servidor* S = new servidor();

    proceso* P1 = new proceso();
    proceso* P2 = new proceso();
    proceso* P3 = new proceso();

    P1 ->pid = 0;
    P2 ->pid = 1;
    P3 ->pid = 2;

    std::srand(std::time(nullptr));
    int aleatorio = std::rand() % 3;
    std::cout <<"hola:  " << aleatorio;

/*
to do: simulacion de pedir token y revisar cómo se libera el token
*/
    switch (aleatorio)
    {
    case 0:
        
        break;
    case 1:
        /* code */
        break;
    case 2:
        /* code */
        break;
    default:
        break;
    }

}


