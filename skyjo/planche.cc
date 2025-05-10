#include "planche.hh"
#include <iostream>
#include <stdexcept>

planche::planche()
    : _nom_joueur(""), _score(0) {
}



void planche::afficherMain() {
    for (size_t i = 0; i < _main.size(); ++i) {
        if (_main[i].get_faceUp()) {
            std::cout << "[" << _main[i].get_value() << "] ";
        } else {
            std::cout << "[X] ";
        }
    }
    std::cout << std::endl;
}

void planche::set_score() {
    int aux_score = 0;
    for (auto c : _main) {
        if (c.get_faceUp() && !c.get_delete()) {
            aux_score += c.get_value();
        }
    }
    _score = aux_score;
}

carte planche::remplacerCarte(int index, const carte& carte_a_ajouter) {
    if (index < 0 || static_cast<size_t>(index) >= _main.size()) {
        throw std::out_of_range("Index invalide pour remplacer la carte");
    }
    carte carte_remplacee = _main[index];
    _main[index] = carte_a_ajouter;
    _main[index].set_faceUp(true);

    verifierColonnesVerrouillees();
    return carte_remplacee;
}
void planche::verifierColonnesVerrouillees() {
    for (int i = 0; i<4; i++) {
        if (_main[i].get_faceUp() && _main[i + 4].get_faceUp() && _main[i + 8].get_faceUp()) {
            if(_main[i].get_value() == _main[i + 4].get_value() && _main[i].get_value() == _main[i + 8].get_value()) {
                _main[i].set_delete(true);
                _main[i + 4].set_delete(true);
                _main[i + 8].set_delete(true);
            }
        }
    }
}

void planche::retournercarte(int const & index) {
    if (index < 0 || index >= _main.size()) {
        throw std::out_of_range("Index invalide");
    }
    _main[index].set_faceUp(true);
    verifierColonnesVerrouillees();
}
