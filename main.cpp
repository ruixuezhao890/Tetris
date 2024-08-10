#include <QApplication>
#include <QPushButton>
#include "tetris.hpp"
int main(int argc, char *argv[]) {
    std::srand( std::time( nullptr ) );
    QApplication a(argc, argv);
    auto game=tetris_game::Tetris();
    game.show();
    game.game_start();
    return QApplication::exec();
}
