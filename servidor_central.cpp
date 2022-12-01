/*
    to do: marcas de Lamport   *y solución a solicitudes seguidas del mismo proceso*

DONE:
    Revisar codependencia entre clases - Errores de compilacion.  ✅
    simulacion de pedir token y revisar cómo se libera el token   ✅
    update token ✅
    aleatoriedad de solicitudes ✅
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

#include <thread>

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
    int getQueueSize();
    void upDateToken(proceso* P);
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
    std::cout <<"Proceso Inicial:  " << aleatorio << std::endl;


    //Se decide al "azar" el orden de las solicitudes. 
   
    for (size_t i = 0; i < 3; i++)
    {
        std::cout << "Proceso que pide entrar a SC: " << aleatorio << std::endl;
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

        aleatorio = std::rand() % 3;
        //std::cout <<"Proceso sig:  " << aleatorio << std::endl;

    }

    //Se manda a trabajar hasta que la cola esté vacia. 
    while( S->getQueueSize() > 0){
        std::thread T0 (trabajar, S, P1);
        std::thread T1 (trabajar, S, P2);
        std::thread T2 (trabajar, S, P3);

        T0.detach();
        T1.detach();
        T2.detach();

        sleep(2);    
    }

    

    return 0;
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
                    cola.push(P);
                }
                else    //en caso contrario se hace pop y se le asigna el token al proceso siguiente
                {
                    cola.push(P);
                }                
            }
            else //si el token no está disponible se encola :)
            {
                cola.push(P);
            }
        }

void servidor::liberarToken(proceso* P){
        //std::cout << "liberar token: : " << P->pid << std::endl; 
        proceso* tmp;
        this->disponible = true;
        P->token = nullptr;
        //tmp = this->cola.front();
        this->cola.pop();
        //std::cout <<"tmp: "  << tmp->pid << std::endl;
        //std::cout <<"disponible liberar: "  << this->disponible << std::endl;
        
        if(this->getQueueSize()>0){
            tmp = this->cola.front();
            this->upDateToken(tmp);
        }
    }

int servidor::getQueueSize(){
    //std::cout << "cola size: " << this->cola.size() << std::endl; 
    return this->cola.size();
}

void servidor::upDateToken(proceso* P){
    //std::cout << "update: " << P->pid << std::endl; 
        P->token = &token;
        this->disponible = false;

}


int trabajar(servidor* S, proceso* P){

    int trabajar_mas  = std::rand() % 100;
       std::cout << "ganas de trabajar extras de: " << P->pid << "  " << trabajar_mas << std::endl;
    

    if(P->token == nullptr){
        std::cout << "Estoy en espera :( soy: " << P->pid << std::endl; 
       
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

        /*if(S->getQueueSize()>0){
            S->upDateToken(P);
        }
        */

       

    }

    if (trabajar_mas > 87)
    {
        std::cout << "MAASSS >:D  soy: " << P->pid << std::endl; 
        S->requestToken(P);
    }

     return(1);
        
}

