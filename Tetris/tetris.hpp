/**
*********************************************************************
*********
* @project_name :Tetris
* @file : tetris.hpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/8/5 
*********************************************************************
*********
*/
//

#ifndef TETRIS_TETRIS_HPP
#define TETRIS_TETRIS_HPP
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>


namespace tetris_game {
    class Tetris:public QWidget{
        static const uint32_t lines   {20};
        static const uint32_t cols    {10};
        static const uint32_t squares {4};
        static const uint32_t shapes  {7};
        static const uint32_t move_down_interval=500;
        QVector<QVector<uint32_t>> area;
        QVector<QVector<uint32_t>> forms;

        QVector<QImage> split_image_vector;

        QImage originalImage;

        QSharedPointer<QGraphicsPixmapItem> sprite;

        QPoint z[squares],k[squares];

//        QPainter painter_;

        QPixmap tiles;

        QGraphicsScene scene;

        QPixmap bg_image_;

        QFont customFont_;

        QPalette palette_;

        QPalette label_palette_;



        QTimer * game_running_timer_;

        QLabel * text_score;

        QLabel * text_game_over;

        int  dirx_ , color_ , score_ ;
        bool rotate_ , game_over_;
        int timer_count_, delay;
    protected:
        void load_font();

        void split_image_func();

        void keyPressEvent(QKeyEvent *event) override;

        void paintEvent(QPaintEvent *event) override;

        void move_down();

        void set_rotate();

        void reset_values();

        void change_position();

        bool max_limit();

        void set_score();

        void game_running();

        void game_init();
    public:
           explicit Tetris();

           inline void game_start(){
               game_running_timer_->start(30);
           }
           inline void game_stop(){
               game_running_timer_->stop();
           }

  };
} // tetris_game

#endif //TETRIS_TETRIS_HPP
