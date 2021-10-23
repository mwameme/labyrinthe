#ifndef LABYRINTHE_H_INCLUDED
#define LABYRINTHE_H_INCLUDED

#include <vector>
#include <iostream>
#include <string>
#include <fstream>


//les cases du labyrinthe
class Coordonnee
{
public:
    int m_x;
    int m_y;

    std::vector<Coordonnee*> m_voisins; //liste des voisins

    bool m_place; //regarde si c'est une 'place' :
                //plus de trois voisins, ou une impasse, ou E, ou S :
                //on regardera les chemins entre les places.
                //si ce n'est pas une place, c'est un couloir (2 voisins) entre deux places ...

    bool operator==(Coordonnee const& a)const;
    //    bool operator!=(Coordonnee const & a)const;

    int iterListePlace; //utilisée pour savoir, si c'est une place, le numéro de cette place dans la liste des places !

    //Coordonnee(Coordonnee const& c);

};

//liste de cases : commence à une place, se termine à une place, entre les deux ce sont des couloirs ....
class Chemin
{
public:
    Chemin operator+(Chemin const& autreChemin) const; //réunion des chemins
//    Chemin(Chemin const& c); //copier un Chemin
    Chemin(); //
    void inverserSens(); //inverser l'ordre dans le Chemin

    bool operator==(Chemin const& autreChemin)const;//vérifie dans les deux sens

    Chemin(Coordonnee const& a, Coordonnee const& b); //ajouter un couloir : de a vers b
    //et en continuant jusqu'à ce qu'on tombe sur une place.

    ~Chemin();
    Coordonnee premier() const;
    Coordonnee dernier() const;
    int taille()const;

    std::string getString() const;

private:
    std::vector<Coordonnee> m_chemin; //juste la liste des cases du chemin ...
};


//contient : la carte du labyrinthe, les places, les chemins, la matrice des chemins entre places :
// M_(i,j) est le chemin de la place i à la place j ...
class CarteLabyrinthe
{
public:
    CarteLabyrinthe(std::string const& adressse); //crée m_carte depuis un fichier .txt
    //crée m_listeChemins et m_listePlaces
    //remplit m_Matrice
    //m_Matrice +1
    //itere les calculs
    //donne la solution, si elle existe !

    Chemin solution();



private:
    int m_largeur;
    int m_hauteur;

    Coordonnee m_entree;
    Coordonnee m_sortie;
    int ES; //vaut 3 si entree ET sortie

    Chemin place2place(Coordonnee const& a, Coordonnee const& b);// deux places adjancentes //Non utilisé
    Chemin couloir(Coordonnee const& a); //un long couloir entre deux places //non utilisé

    std::vector<std::vector<Coordonnee>> m_carte; //m_largeur * m_hauteur coordonnées : juste l'ensemble des coordonnées, avec leurs voisins

    std::vector<Coordonnee> m_listePlaces; //les croisements : parcours la carte !
    std::vector<Chemin> m_listeChemins; //premiere liste de Chemins

    int m_nbPlaces; //le nombre de places dans la carte
    std::vector<std::vector<Chemin>> m_Matrice; //matrice m_listePlace.size() ** m_listePlace.size(), : pour chaque (i,j), le plus court chemin i->j, à actualiser !
    //qui contient des Chemins;

    Chemin m_solution; //la solution : E->S


    //les afficheurs  ...
public:
    //    void afficherCarte()const;
    void afficherMatrice()const;
};

std::ostream& operator<<(std::ostream& flux, Chemin const& c);


#endif // LABYRINTHE_H_INCLUDED
