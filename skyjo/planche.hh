#pragma once

#include "carte.hh"

#include <vector>
#include <string>

class planche {


    private :
        std::string _nom_joueur;
        std::vector<carte> _main;
        int _score;

    public :
        planche();

        std::vector<carte> get_main() const { return _main; }
        int get_score() const { return _score; }
        std::string get_nom() const { return _nom_joueur; }

        void set_nom(std::string const & nom) { _nom_joueur = nom; }
        void set_score();
        void setUpCarte();
        void addCarte(carte const & c) { _main.push_back(c); }
        void afficherMain();

        carte remplacerCarte(int index, const carte &carte_a_ajouter);
        void verifierColonnesVerrouillees();
        void retournercarte(int const & index);
};