#pragma once

#include "planche.hh"


#include <random>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <iterator>
#include <iostream>
#include <algorithm>


class jeu {

    private:
        std::vector<carte> _jeu;
        std::vector<carte> _pile_defausse;
        int _nb_joueurs;
        std::vector<std::unique_ptr<planche>> _planche;
        int _tour_actuel = 1;
        std::string _etat;
        int _dernierjoueur = -1;
        void retournerDeuxCartesAleatoires(planche& j, std::mt19937& g);
        void verifierFinPartie();
        void jouertourC(const std::string& input, bool isFinalturn);
        void updateConsolePrompt(bool isFinalTurn = false);

        enum class ActionContext {
            None,
            Replace,
            Flip
        };

        ActionContext _actionContext = ActionContext::None;
        carte _currentCard;

        enum class GameState {
            ChooseAction,
            ChooseCardOrFlip,
            ChoosePosition
        };

        GameState _currentState = GameState::ChooseAction;
        carte tirerCarteDeLaPioche();
        carte prendreCarteDefausse();
        void defausserCarte(const carte& c);


    public:
        jeu(int nb_joueurs);
        int get_nb_joueurs() const { return _nb_joueurs; }
        std::vector<std::unique_ptr<planche>>& get_planche() { return _planche; }
        void setupJeu();
        void jouertour(bool isFinalTurn);

};