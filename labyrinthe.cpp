#include "labyrinthe.h"

using namespace std;

bool Coordonnee::operator==(Coordonnee const& a)const
{
    return ((m_x == a.m_x) && (m_y == a.m_y));
}


void Chemin::inverserSens() //je ne l'ai pas utilisé ici ...
{
    int n = m_chemin.size();
    for (int i(0); i <= n / 2; ++i)
    {
        Coordonnee reg = m_chemin[i];
        m_chemin[i] = m_chemin[n - 1 - i];
        m_chemin[n - 1 - i] = reg;
    };
}

bool Chemin::operator==(Chemin const& autreChemin)const //regarde si les chemins sont égaux ou en sens inverse l'un de l'autre ... les deux sont considérés équivalents (true)
{
    if (m_chemin.size() != autreChemin.m_chemin.size())
        return false;

    int n = m_chemin.size();
    bool ex = true;
    for (int i(0); i < n; ++i)//regarde dans un sens ...
    {
        if (!(m_chemin[i] == autreChemin.m_chemin[i]))
        {
            ex = false;
            break;
        };
    }
    if (ex) return ex;

    ex = true;
    for (int i(0); i < n; ++i) //regarde dans l'autre sens ...
    {
        if (!(m_chemin[i] == autreChemin.m_chemin[n - 1 - i]))
        {
            ex = false;
            break;
        };
    }
    return ex;
}


Chemin Chemin::operator+(Chemin const& autreChemin) const //si une extrémité de l'un des chemins est égal à l'extrémité de l'autre des chemins, écrit l'un suivit de l'autre (dans le bon sens !)
                                                            //attention : on ne répète pas deux fois l'extrémité, en collant les deux chemins !
{
    if (m_chemin[m_chemin.size() - 1] == autreChemin.m_chemin[0])  //a + b
    {
        Chemin retour(*this);
        for (int i(1); i < autreChemin.m_chemin.size(); ++i)
            retour.m_chemin.push_back(autreChemin.m_chemin[i]);
        return retour;
    }

    else if (m_chemin[m_chemin.size() - 1] == autreChemin.m_chemin[autreChemin.m_chemin.size() - 1])  //a + -b
    {
        Chemin retour(*this);
        for (int i(1); i < autreChemin.m_chemin.size(); ++i)
            retour.m_chemin.push_back(autreChemin.m_chemin[autreChemin.m_chemin.size() - 1 - i]);
        return retour;
    }

    else if (m_chemin[0] == autreChemin.m_chemin[autreChemin.m_chemin.size() - 1])  //b + a
    {
        Chemin retour(autreChemin);
        for (int i(1); i < m_chemin.size(); ++i)
            retour.m_chemin.push_back(m_chemin[i]);
        return retour;
    }

    else if (m_chemin[0] == autreChemin.m_chemin[0])  // -a + -b
    {
        Chemin retour;
        for (int i(0); i < autreChemin.m_chemin.size(); ++i)
            retour.m_chemin.push_back(autreChemin.m_chemin[autreChemin.m_chemin.size() - 1 - i]);
        for (int i(1); i < m_chemin.size(); ++i)
            retour.m_chemin.push_back(m_chemin[i]);
        return retour;
    }
    else  //si ça ne match pas
    {
        Chemin retour;
        return retour;
    }
}

Chemin::Chemin() //crée un chemin vide
{
    vector<Coordonnee> m_chemin(0);
}

Chemin::~Chemin()
{
}

Chemin::Chemin(Coordonnee const& a, Coordonnee const& b)
{
    vector<Coordonnee> m_chemin(0);
    if (!a.m_place)
        return;


    bool correct = false;
    for (int i(0); i < a.m_voisins.size(); ++i)//regarde si b est bien un voisin de a !
    {
        if (b == *(a.m_voisins[i]))
        {
            correct = true;
            break;
        }
    }

    if (!correct) //verifie si a pointe vers b
        return;

    //cout << "correct" << endl;

    //iter tant qu'on n'est pas dans une place ...
    m_chemin.push_back(a);//le chemin commence par a
    m_chemin.push_back(b);//puis b

    Coordonnee a1(a), b1(b), c1;//puis on prend LE voisin c de b (c'est un couloir entre deux places)
                            //qui, évidemment, ne doit pas être a !
                            //et on translate : b=c;a=b; et on continue !
    while (!b1.m_place)  //tant que b est un couloir (place=2)
    {
        //trouver c
        for (int i(0); i < b1.m_voisins.size(); ++i)
        {
            if (!(*(b1.m_voisins[i]) == a1))
            {
                c1 = *(b1.m_voisins[i]);
                a1 = b1;
                b1 = c1;
                m_chemin.push_back(b1);
                break;
                //              cout<<"ok ";
            }
        }
        //        cout<<"...";
    };

    /*
    cout << "taille " << m_chemin.size() << endl;
    cout << "afficher Chemin avant ";
    for (int i(0); i < m_chemin.size(); ++i)
        cout << m_chemin[i].m_x << "," << m_chemin[i].m_y << " ; ";
    cout << endl;
    */

    this->m_chemin = m_chemin;
}

Coordonnee Chemin::premier()const //premiere coordonnée du chemin
{
    if (m_chemin.size() > 0)
        return m_chemin[0];
    else
    {
        Coordonnee retour;
        return retour;
    }
}

Coordonnee Chemin::dernier()const //derniere coordonnée du chemin
{
    if (m_chemin.size() > 0)
        return m_chemin[m_chemin.size() - 1];
    else
    {
        Coordonnee retour;
        return retour;
    }
}

int Chemin::taille()const //longueur du chemin
{
    return m_chemin.size();
}

string Chemin::getString() const //utilisé pour afficher un chemin sur la console !
{
    string s;
    for (int i(0); i < m_chemin.size(); ++i)
    {
        s = s + "(" + to_string(m_chemin[i].m_x) + "," + to_string(m_chemin[i].m_y) + ") ; ";
    }
    return s;
}



CarteLabyrinthe::CarteLabyrinthe(std::string const& m_adresse)
{

    //ouvrir la carte
    ifstream monFlux(m_adresse);

    if (!monFlux)
        return;

    monFlux >> m_largeur;
    monFlux >> m_hauteur;

    vector<vector<Coordonnee>> m_carte(m_largeur, vector<Coordonnee>(m_hauteur));
    //bool continuer;

    //récupérer la carte du labyrinthe
    char a;
    int i(0), j(0);
    int ES = 0;
    do
    {
    label:
        monFlux.get(a);
        if (!a) break;
        m_carte[i][j].m_x = i;
        m_carte[i][j].m_y = j;
        if ((a == 'B') && (j < m_hauteur - 1))
        {
            m_carte[i][j].m_voisins.push_back(&m_carte[i][j + 1]); //ajoute un chemin a->b, et aussi l'inverse : b->a
            m_carte[i][j + 1].m_voisins.push_back(&m_carte[i][j]);
        }
        else if ((a == 'D') && (i < m_largeur - 1))
        {
            m_carte[i][j].m_voisins.push_back(&m_carte[i + 1][j]);
            m_carte[i + 1][j].m_voisins.push_back(&m_carte[i][j]);
        }
        else if (a == ',') //passe à la case suivante !
        {
            ++i;
            if (i >= m_largeur) break;
        }
        else if (a == ';') //passe à la ligne suivante !
        {
            ++j;
            i = 0;
            if (j >= m_hauteur) break;
        }
        else if (a == 'E') //est-ce l'entrée ?
        {
            m_entree.m_x = i;
            m_entree.m_y = j;
            ES = ES | 1;
        }
        else if (a == 'S') //est-ce la sortie ??
        {
            m_sortie.m_x = i;
            m_sortie.m_y = j;
            ES = ES | 2;
        }
        else goto label; //sinon recommencer à la case d'après
    } while ((i < m_largeur) && (j < m_hauteur)); //tant qu'on est dans la taille de la carte !


    //est-ce une place ? si 1, 3, ou 4 voisins
    //ou si c'est une extremité du labyrinthe
    int nbPlaces = 0;
    for (int i(0); i < m_largeur; ++i)
        for (int j(0); j < m_hauteur; ++j)
        {
            m_carte[i][j].m_place = false;
            int nbVoisins = m_carte[i][j].m_voisins.size();
            if (nbVoisins >= 3)
            {
                m_carte[i][j].m_place = true;
                ++nbPlaces;
            }
            if (nbVoisins == 1)
            {
                m_carte[i][j].m_place = true;
                ++nbPlaces;
            }
        }

    if (!m_carte[m_entree.m_x][m_entree.m_y].m_place) //ajoute l'entrée si besoin
    {
        m_carte[m_entree.m_x][m_entree.m_y].m_place = true;
        ++nbPlaces;
    }
    if (!m_carte[m_sortie.m_x][m_sortie.m_y].m_place) //ajoute la sortie si besoin
    {
        m_carte[m_sortie.m_x][m_sortie.m_y].m_place = true;
        ++nbPlaces;
    }


    //creer liste des places
    vector<Coordonnee> m_listePlaces(nbPlaces); //compte le nombre de places, tout simplement
    int iter = 0;
    for (int i(0); i < m_largeur; ++i)
        for (int j(0); j < m_hauteur; ++j)
        {
            if (m_carte[i][j].m_place)
            {
                m_listePlaces[iter] = m_carte[i][j];
                ++iter;
            }
        }

    /*
    cout << "listeplaces" << endl;
    bool continuer;
    for (int i(0);i<nbPlaces;++i)
        cout << m_listePlaces[i].m_x << "," << m_listePlaces[i].m_y << endl;
    cin >> continuer;
*/

    //creer la premiere liste de chemins :
    //parcourir les places, et ajouter des couloirs : pour une place, vers ses voisins, jusqu'à une autre place ...
    vector<Chemin> m_listeChemins2(0);
    for (int i(0); i < nbPlaces; ++i)
    {
        for (int j(0); j < m_listePlaces[i].m_voisins.size(); ++j)
        {
            Chemin a(m_listePlaces[i], *m_listePlaces[i].m_voisins[j]);
            m_listeChemins2.push_back(a);
            //cout << "chemin a : " << a << endl;
            //cout <<"taille 2"<<a.taille() << endl;
        };
    }

    /*
    cin >> continuer;

    cout << "taille liste chemins" << m_listeChemins2.size() << endl;

    for (int i(0); i < m_listeChemins2.size(); ++i)
        cout << m_listeChemins2[i].getString() << endl;


    cin >> continuer;
    */


    vector<Chemin> m_listeChemins(0);

    //on enleve les doublons, grâce à == : on recopie dans une autre liste !
    //remarque : on pourrait enlever les chemins qui arrivent sur une impasse, à condition que cette impasse ne soit ni l'entrée ni la sortie !
    for (int i(0); i < m_listeChemins2.size(); ++i)
    {
        bool trouve = false;
        for (int j(0); j < m_listeChemins.size(); ++j)
        {
            if (m_listeChemins[j] == m_listeChemins2[i])
            {
                trouve = true;
                break;
            }
        }
        if (!trouve) //si le chemin n'est pas encore dans la deuxieme liste, on l'y ajoute !
        {
            m_listeChemins.push_back(m_listeChemins2[i]);
        }
    }


    //cout << "taille liste chemins" << m_listeChemins.size() << endl;




    //on indique, pour chaque place, dans sa coordonnee de m_carte, sa position dans listePlace :
    //pour avoir la carte réciproque ... iter (dans la case qui est une place), c'est le numéro tel que m_place[iter] vaut cette coordonnée
    for (int i(0); i < nbPlaces; ++i)
    {
        m_carte[m_listePlaces[i].m_x][m_listePlaces[i].m_y].iterListePlace = i;
    }


    //on initialise la matrice des chemins :
    //on parcourt les chemins et on les ajoute au bon endroit (dans la Matrice) ...
    //matrice [i][j] où i,j est l'iter de listePlace ...
    vector<vector<Chemin>> m_Matrice(nbPlaces, vector<Chemin>(nbPlaces));
    for (int i(0); i < m_listeChemins.size(); i++)
    {
        Coordonnee debut = m_listeChemins[i].premier();
        Coordonnee fin = m_listeChemins[i].dernier();
        //trouver "debut" dans listePlace
        int iterDebut = m_carte[debut.m_x][debut.m_y].iterListePlace;
        int iterFin = m_carte[fin.m_x][fin.m_y].iterListePlace;
        if (m_Matrice[iterDebut][iterFin].taille() > m_listeChemins[i].taille())
        {
            m_Matrice[iterDebut][iterFin] = m_listeChemins[i];
            m_Matrice[iterFin][iterDebut] = m_listeChemins[i];
        }
        if (m_Matrice[iterDebut][iterFin].taille() == 0)
        {
            m_Matrice[iterDebut][iterFin] = m_listeChemins[i];
            m_Matrice[iterFin][iterDebut] = m_listeChemins[i];
        }
    }

    /*
    cout << "nb Places" << nbPlaces << endl;
    for (int i(0); i < nbPlaces; ++i)
        for (int j(0); j < nbPlaces; ++j)
            cout << m_Matrice[i][j].taille()<<" ";
 
    cin >> continuer;
*/

    //on calcule n+1 de la matrice ... jusqu'à ce qu'il n'y ait plus de modifications à ajouter !!! c'est le centre du programme : recherche du plus court chemin !
    int compteurBoucle = 0;
    bool modifie;
    do
    {
        modifie = false;
        for (int i(0); i < nbPlaces; ++i) //comme M_ij est symétrique, on s'occupe juste de i<j (i==j est trivial);
            for (int k(0); k < i; ++k)
            {
                int taille = m_Matrice[i][k].taille();
                if (taille > 0)
                {
                    //on regarde i -> j + j-> k, et on compare à i->k;
                    for (int j(0); j < nbPlaces; ++j)
                    {
                        if ((m_Matrice[i][j].taille() > 0) && (m_Matrice[j][k].taille() > 0) && (m_Matrice[i][j].taille() + m_Matrice[j][k].taille() < taille)) //si M_ij +M_jk < M_ik ...
                        {
                            m_Matrice[i][k] = m_Matrice[i][j] + m_Matrice[j][k];
                            m_Matrice[k][i] = m_Matrice[i][k];
                            //taille = m_Matrice[i][k].taille();
                            modifie = true;
                        }
                    }
                }
                else //si le chemin i->k n'existe pas encore, on l'y ajoute !
                {
                    for (int j(0); j < nbPlaces; ++j)
                    {
                        if ((m_Matrice[i][j].taille() > 0) && (m_Matrice[j][k].taille() > 0))
                        {
                            m_Matrice[i][k] = m_Matrice[i][j] + m_Matrice[j][k];
                            m_Matrice[k][i] = m_Matrice[i][k];
                            //taille = m_Matrice[i][k].taille();
                            modifie = true;
                            break;
                        }
                    }
                }
            }
        //cout << ++compteurBoucle << ","; //un compteur de boucle, histoire de voir si ça tourne en rond !
        //if(compteurBoucle<10) modifie=true; //

    } while (modifie); //si aucun changement, on s'arrête !

    //cout << endl;

    //cout << "matrice" << endl;


    Chemin m_solution; //solution




    //trouver le Chemin de E à S
    if (ES == 3)
    {
        int iterE = m_carte[m_entree.m_x][m_entree.m_y].iterListePlace; //on utilise la carte réciproque : dans la coordonnée (entree_x,entree_y)
                                                    //on regarde son indice dans la liste des places (et donc des chemins)
        int iterS = m_carte[m_sortie.m_x][m_sortie.m_y].iterListePlace;
        m_solution = m_Matrice[iterE][iterS];

        //cout << "iterE" << iterE << endl;
        //cout << "iterS" << iterS << endl;

    }
    /*
    cin >> continuer;

    cout << nbPlaces << endl;
    cout << ES << endl;
    */

    cout << m_solution << endl; //imprimer la solution ! et c'est fini !

    cout << "taille " << m_solution.taille() << endl;
    bool continuer;
    cin >> continuer;

}

Chemin CarteLabyrinthe::solution()
{
    return m_solution;
}

void CarteLabyrinthe::afficherMatrice()const //afficher la liste des chemins trouvés, histoire de vérifier tout le labyrinthe !!
{
    cout << "MATRICE" << endl;

    for (int i(0); i < m_nbPlaces; ++i)
        for (int j(0); j < i; ++j)
        {
            cout << i << "," << j << " : " << m_Matrice[i][j] << endl;
        }
    cout << endl << endl;



}




ostream& operator<<(ostream& flux, Chemin const& c) //afficher un Chemin ... pour le style ;-)
{
    return flux << c.getString();
}
