#include "jeuconsole.hh"

jeu::jeu(int nombre_de_joueurs)
    :_etat(""), _nb_joueurs(nombre_de_joueurs)
{

    setupJeu();
    std::cout << "=== Début de la partie ===" << std::endl;
    std::cout << "Tour N°" << _tour_actuel << std::endl;
    if (!_pile_defausse.empty()) {
        std::cout << "Defausse : " << _pile_defausse.back().get_value() << std::endl;
    }
    for (int i = 0; i < _nb_joueurs; ++i) {
        _planche[i]->afficherMain();
    }
    updateConsolePrompt(false);
    jouertour(false);

}


void jeu::setupJeu() {
    for (int value = -2; value <= 12; ++value) {
        int nbCartes = (value == -2) ? 5 : (value == 0) ? 15 : 10;
        for (int i = 0; i < nbCartes; ++i) {
            _jeu.push_back(carte(value));
        }
    }
    std::random_device rd;  // Source d'entropie (peut être lent)
    std::mt19937 g(rd());   // Générateur basé sur Mersenne Twister
    std::shuffle(_jeu.begin(), _jeu.end(), g);

    for (int i = 0; i < _nb_joueurs; ++i) {
        _planche.push_back(std::make_unique<planche>());
    }

    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < _nb_joueurs; ++j) {
            _planche[j] -> addCarte(_jeu.back());
            _jeu.pop_back();
        }
    }

    for (auto& planche : _planche) {
        retournerDeuxCartesAleatoires(*planche, g);
    }

    _pile_defausse.push_back(_jeu.back());
    _jeu.pop_back();
}

void jeu::jouertourC(const std::string& input, bool isFinalTurn) {
    switch (_currentState) {
        case GameState::ChooseAction:
            if (input == "piocher") {
                _currentCard = tirerCarteDeLaPioche();
                std::cout << "Carte tirée : " << _currentCard.get_value() << std::endl;
                _currentState = GameState::ChooseCardOrFlip;
                updateConsolePrompt(isFinalTurn);
            } else if (input == "prendre_defausse") {
                _currentCard = prendreCarteDefausse();
                _actionContext = ActionContext::Replace;
                _currentState = GameState::ChoosePosition;
                updateConsolePrompt(isFinalTurn);
            }
            break;

        case GameState::ChooseCardOrFlip:
            if (input == "defausser") {
                _pile_defausse.push_back(_currentCard);
                _actionContext = ActionContext::Flip;
                _currentState = GameState::ChoosePosition;
                updateConsolePrompt(isFinalTurn);
            } else if (input == "remplacer") {
                _actionContext = ActionContext::Replace;
                _currentState = GameState::ChoosePosition;
                updateConsolePrompt(isFinalTurn);
            }
            break;
    case GameState::ChoosePosition: {
        bool ok = false;
        int position;
        try {
            position = std::stoi(input);
            ok = true;
        } 
        catch (const std::invalid_argument&) {
            ok = false;
        }
        if (ok) {
            if (_actionContext == ActionContext::Replace) {
                int joueurIndex = _tour_actuel % _nb_joueurs;
                planche& joueurActuel = *_planche[joueurIndex];
                carte ancienne = joueurActuel.remplacerCarte(position, _currentCard);
                _pile_defausse.push_back(ancienne);
                if (!isFinalTurn) {
                    verifierFinPartie();
                }
            } else if (_actionContext == ActionContext::Flip) {
                int joueurIndex = _tour_actuel % _nb_joueurs;
                _planche
                [joueurIndex]->retournercarte(position);
            }
            _currentCard = carte();
            _currentState = GameState::ChooseAction;
            if (!isFinalTurn) {
            verifierFinPartie();
            }
            _tour_actuel++;
            std::cout << "Tour N°" << _tour_actuel << std::endl;
            if (!_pile_defausse.empty()) {
                std::cout << "Defausse : " << _pile_defausse.back().get_value() << std::endl;
            }
            for (int i = 0; i < _nb_joueurs; ++i) {
                _planche[i]->afficherMain();
            }
            updateConsolePrompt(isFinalTurn);
            if (_tour_actuel%_nb_joueurs != _dernierjoueur && _dernierjoueur != -1) {
                jouertour(isFinalTurn);
            }
        }
    }
        break;
    }

}

void jeu::jouertour(bool isFinalTurn) {
    std::string input;
    while (_dernierjoueur == -1) {
        // Priorité au buffer IA
        if (_aiBuffer >> input) {
            jouertourC(translateCommand(input), isFinalTurn);
        }
        // Sinon lecture console + traduction
        else {
            std::cout << "> ";
            if (std::cin >> input) {
                jouertourC(translateCommand(input), isFinalTurn);
            }
            else {
                std::cin.clear();
                std::cin.ignore();
            }
        }

        // Vérification fin de tour
        if (_currentState == GameState::TurnEnd) {
            _currentState = GameState::ChooseAction;
            break;
        }
    }
}

carte jeu::tirerCarteDeLaPioche() {
    if (!_jeu.empty()) {
        carte carte = _jeu.back();
        carte.set_faceUp(true);
        _jeu.pop_back();
        return carte;
    }
    return carte();
}

carte jeu::prendreCarteDefausse() {
    if (!_pile_defausse.empty()) {
        carte carte = _pile_defausse.back();
        _pile_defausse.pop_back();
        return carte;
    }
    return carte();
}

void jeu::defausserCarte(const carte& carte) {
    _pile_defausse.push_back(carte);
}

void jeu::updateConsolePrompt(const bool isFinalTurn) {
    std::string prompt;
    switch (_currentState) {
        case GameState::ChooseAction:
            prompt = "Choisissez une action (piocher, prendre_defausse): ";
            break;
        case GameState::ChooseCardOrFlip:
            prompt = "Choisissez une action (remplacer, defausser): ";
            break;
        case GameState::ChoosePosition:
            prompt = "Choisissez la position de la carte: ";
            break;
    }
    if (isFinalTurn) {
        prompt += " (Dernier tour)";
    }
    std::cout << prompt << std::endl;
}

void jeu::retournerDeuxCartesAleatoires(planche& j, std::mt19937& g) {
    std::vector<int> indices(12);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), g);
    for (int i = 0; i < 2; ++i) {
        j.retournercarte(indices[i]);
    }
}


void jeu::verifierFinPartie() {
    int joueur_fini = -1;
    for (size_t i = 0; i < _planche.size(); ++i) {
        const auto& main = _planche[i]->get_main();
        if (main.size() == 12 && std::all_of(main.begin(), main.end(),
            [](const carte& c) { return c.get_faceUp(); })) {
            joueur_fini = static_cast<int>(i);
            break;
        }
    }

    bool piocheVide = (_jeu.empty() && _pile_defausse.empty());
    bool partieTerminee = (joueur_fini != -1 || piocheVide);

    if (partieTerminee) {
        if (joueur_fini != -1) {
            std::cout << "Le joueur " << (joueur_fini + 1) << " a terminé !" << std::endl;
            _dernierjoueur = joueur_fini;
        }

        std::string scores;
        for (const auto& planche : _planche) {
            planche->set_score();
            scores += planche->get_nom() + " : " 
                    + std::to_string(planche->get_score()) 
                    + " points\n";
        }
        std::cout << "\n=== Scores finaux ===\n" << scores << std::endl;
    }
}
