
/*
to do: simulacion de pedir token y revisar cómo se libera el token ✅
to do later: marcas de Lamport          
Revisar codependencia entre clases - Errores de compilacion. 
*/
#ifdef _WIN32
#include <Windows.h>
#else 
#include <unistd.h>
#endif

#include <iostream>
#include <thread>
#include <queue>
#include <cstdlib>
#include <ctime>

class proceso{
    public:
    int pid;
    int* token;

    //void liberarToken(servidor* S);
    //int trabajar(servidor* S);


};

class servidor{
    private:
    int token;
    int timeout;
    std::queue <proceso*> cola;

    public:
    bool disponible;
    void requestToken(proceso* P);
    void liberarToken(proceso* P);
    servidor();
};

int trabajar(servidor* S, proceso* P);

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

    for (size_t i = 0; i < 3; i++)
    {
        switch (aleatorio)
        {
        case 0:
            S->requestToken(P1);
            break;
        case 1:
            S->requestToken(P2);
            break;
        case 2:
            S->requestToken(P3);
            break;
        default:
            break;
        }

        int var =   trabajar(S, P1);
        var  =      trabajar(S, P2);
        var  =      trabajar(S, P3);
    }
    
    

}

 servidor::servidor(){
            this -> token = 123;
            this -> disponible = true;
            this -> timeout = 100;
        }

void servidor::requestToken(proceso* P){
            proceso* sigpid;
            if (this->disponible)
            {
                if (cola.empty()) //la cola está vacía, se toma directamente el token
                {
                    P->token = &token;
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

void servidor::liberarToken(proceso* P){
        this->disponible = true;
        free(P->token);
    }

int trabajar(servidor* S, proceso* P){
    if(P->token == nullptr){
        std::cout << "Estoy en espera :( soy: " << P->pid << std::endl; 
        return(0);
    }
    else {
        std::cout << "Estoy trabajando :) soy: " << P->pid << std::endl; 

        //Bloque condicional del preprocesador horrible porque compilo con WSL (Linux) y el Debugger es de Windows XD
        #ifdef _WIN32
            Sleep(2);
        #else 
            sleep(2);
        #endif

        S->liberarToken(P);
        std::cout << "Ya termine de trabajar >.<  soy: " << P->pid << std::endl; 
        return(1);
    }
        
}
