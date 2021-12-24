/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 * 
 */ 

#include <iostream>
#include <chrono>
#include <thread>

using namespace std; 

void playerGame() 
{
    while(/*!scacco matto*/) 
    {
        /*Struttura dati che tiene in memoria le mosse precedentemente fatte*/
        if(/*turno del giocatore*/)
        {
            
            /*Stampa della board*/
            
            cout << "\nMossa del giocatore : ";
            /*Inserire mossa */
            /*conversione*/
            if(/*mossa accettabile*/) 
            {
                /*converti i char inseriti in cordinate*/
                /*accedi alla board, vedi il pezzo*/
                if(/*pezzo.isLegalMove*/)
                {
                    if(/*controlli vari su mosse speciali anche usando la std per le mosse precedenti*/)

                    /*mangia o no then aggiorna board*/
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            system("cls");
        }
        else
        {
            /*Stampa della board*/
            
            /*generare mosse random*/
            if(/*mossa accettabile*/) 
            {
                /*converti i char inseriti in cordinate*/
                /*accedi alla board, vedi il pezzo*/
                if(/*pezzo.isLegalMove*/)
                {
                    if(/*controlli vari su mosse speciali anche usando la std per le mosse precedenti*/)

                    /*mangia o no then aggiorna board*/
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

void computersGame() {}

int main() 
{
    /* Scelta se giocare o fare cc*/

    if() 
    {

    } 
    else ;


    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    system("cls");

    return 0;
}


