/**
*********************************************************************
*********
* @project_name :Tetris
* @file : tetris.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/8/5 
*********************************************************************
*********
*/
//

// You may need to build the project (run Qt uic code generator) to get "ui_tetris.h" resolved
#include <qevent.h>

#include <QVector>
#include <QPushButton>
#include <iostream>
#include "tetris.hpp"
#include "QFontDatabase"

namespace tetris_game {

    Tetris::Tetris() {
        area.resize(lines);
        for (auto &i:area) {
            i.resize(cols);
        }
        forms = {
                {1,3,5,7}, // I
                {2,4,5,7}, // Z
                {3,5,4,6}, // S
                {3,5,4,7}, // T
                {2,3,5,7}, // L
                {3,5,7,6}, // J
                {2,3,4,5}, // O
        };

        setFixedSize(360,720);
        setWindowTitle("Tetris (remix)");

        //分割图片
        originalImage.load("../resources/img/squares.png");

        split_image_func();
        //加载背景图片
        bg_image_.load("../resources/img/background.png");

        palette_.setBrush(QPalette::Window,QBrush(bg_image_));

        setPalette(palette_);

        setAutoFillBackground(true);

        dirx_=score_=0;
        rotate_=game_over_={false};
        timer_count_={0};
        delay={move_down_interval};
        color_={1};

        auto number=std::rand()%shapes;
        auto offset=std::rand()%9;
        for (int i = 0; i < squares; ++i) {
            z[i].setX(int(forms[number][i]%2)+offset);
            z[i].setY(int(forms[number][i]/2)-1);
        }

        load_font();
        customFont_.setPointSize(30);
        label_palette_.setColor(QPalette::WindowText,Qt::white);

        text_score=new QLabel(this);
        text_score->setFont(customFont_);
        text_score->move(100,80);
        auto temp_score=QString("%1").arg(score_);
        text_score->setPalette(label_palette_);
        text_score->setText("SCORE:"+ temp_score);

        text_game_over=new QLabel(this);
        customFont_.setPointSize(40);
        text_game_over->setFont(customFont_);
        text_game_over->move(25,300);
        text_game_over->setPalette(label_palette_);
        text_game_over->setText("GAME OVER");
        text_game_over->setVisible(false);




        game_running_timer_= new QTimer (this);
        connect(game_running_timer_,&QTimer::timeout,this,&Tetris::game_running);
    }
    void Tetris::move_down() {
        if( timer_count_ > delay ){
            for (std::size_t i {}; i < squares; ++i) {
                k[i] = z[i];

                z[i].setY(z[i].y()+1);
            }

            if( max_limit() ){

                for (std::size_t i {}; i < squares; ++i) {
                    area[ k[i].y() ][ k[i].x() ] = color_;
                }

                color_ = std::rand() % shapes;
                auto offset= std::rand() % 9 ;
                uint32_t number = rand() % shapes;
                for (qsizetype i {}; i < squares; ++i) {
                    z[i].setX((int) (forms[number][i] % 2)+offset);
                    z[i].setY((int) (forms[number][i] / 2)-1);
                }
            }

            timer_count_ = 0;
        }

    }

    void Tetris::set_rotate() {
        if( rotate_ ){
            auto coords = z[1];
            for (std::size_t i {}; i < squares; ++i) {
                int x = z[i].y() - coords.y();
                int y = z[i].x() - coords.x();

                z[i].setX(coords.x() - x);
                z[i].setY( coords.y() + y);
            }

            if( max_limit() ){
                for (std::size_t i {}; i < squares; ++i) {
                    z[i] = k[i];
                }
            }
        }
    }

    void Tetris::reset_values() {
        dirx_ = 0;
        rotate_ = false;
        delay = {move_down_interval};
    }

    void Tetris::change_position() {
        for (std::size_t i {}; i < squares; ++i) {
            k[i] = z[i];
//            z[i].x += dirx;
            z[i].setX(z[i].x()+dirx_);
        }

        if( max_limit() ){
            for (std::size_t i {}; i < squares; ++i) {
                z[i] = k[i];
            }
        }
    }

    bool Tetris::max_limit() {
        for (std::size_t i {}; i < squares; ++i) {
            if( z[i].x() < 0 ||
                z[i].x() >= cols ||
                z[i].y() >= lines ||
                area[ z[i].y() ][ z[i].x() ]){
                return true;
            }
        }
        return false;
    }

    void Tetris::set_score() {
        uint32_t match = lines - 1;
        for (qsizetype i = match; i >= 1; --i) {
            uint32_t sum {};
            for (qsizetype j {}; j < cols; ++j) {
                if( area[i][j] ){
                    if( i == 1 ){
                        game_over_ = true;
                    }
                    ++sum;
                }
                area[match][j] = area[i][j];
            }
            if( sum < cols ){
                --match;
            }else{
//                txtScore.setString("SCORE: " + std::to_string(++score));
                text_score->setText("SCORE:"+QString::number(++score_));
            }
        }
    }

    void Tetris::game_running() {
        auto time=game_running_timer_->remainingTime();
        timer_count_+=time;
        if (isVisible()) { // 检查窗口是否可见
            if (!game_over_) {
                change_position();
                set_rotate();
                move_down();
                set_score();
                reset_values();
                update(); // 调用update()来触发paintEvent
            }
        } else {
            // 如果窗口不可见，可以停止定时器
            game_stop();
        }


    }

    void Tetris::load_font() {
        auto font_id=QFontDatabase::addApplicationFont("../resources/font/font.ttf");
        if (font_id==-1){
            qWarning("failed to load font");
            return;
        }
        auto fontFamily=QFontDatabase::applicationFontFamilies(font_id);
        if (fontFamily.isEmpty()){
            qWarning("No font families found");
            return;
        }
        customFont_.setFamily(fontFamily.first());
        customFont_.setPointSize(12);

    }

    void Tetris::keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
            case Qt::Key_Up:
                rotate_ = true;
                qDebug()<<"Key_Up";
                break;
            case Qt::Key_Down:
                qDebug()<<"Key_Down";
                delay = move_down_interval/4;
                break;
            case Qt::Key_Left:
                qDebug()<<"Key_Left";
                --dirx_;
                break;
            case Qt::Key_Right:
                qDebug()<<"Key_Right";
                ++dirx_;
                break;
            case Qt::Key_Escape:
                this->close();
                break;
        }
    }

    void Tetris::paintEvent(QPaintEvent *event) {

        auto painter_ = QPainter(this);

//        painter_.fillRect(event->rect(),Qt::black);
//        painter_.drawPixmap(0,0,bg_image_);


        for (int i = 0; i < lines; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (area[i][j]!=0){
                    painter_.drawImage(j * 36, i * 36,split_image_vector[area[i][j]]);
                }
            }
        }

        for (std::size_t i {}; i < squares; ++i) {
                painter_.drawImage(z[i].x()*36,z[i].y()*36,split_image_vector[color_]);
        }

        if (game_over_){
            text_game_over->setVisible(true);
        }
    }

    void Tetris::split_image_func() {
        auto num=originalImage.size().width()/36;
        for (int i = 0; i < num; ++i) {
            QImage part= originalImage.copy(i*36,0,36,36);
            split_image_vector.append(part);
        }
    }

    void Tetris::game_init() {
        area.resize(lines);
        for (auto &i:area) {
            i.resize(cols);
        }
        forms = {
                {1,3,5,7}, // I
                {2,4,5,7}, // Z
                {3,5,4,6}, // S
                {3,5,4,7}, // T
                {2,3,5,7}, // L
                {3,5,7,6}, // J
                {2,3,4,5}, // O
        };

        setFixedSize(360,720);
        setWindowTitle("Tetris (remix)");

        //分割图片
        originalImage.load("../resources/img/squares.png");

        split_image_func();
        //加载背景图片
        bg_image_.load("../resources/img/background.png");

        palette_.setBrush(QPalette::Window,QBrush(bg_image_));

        setPalette(palette_);

        setAutoFillBackground(true);

        dirx_=score_=0;
        rotate_=game_over_={false};
        timer_count_={0};
        delay={move_down_interval};
        color_={1};

        auto number=std::rand()%shapes;
        auto offset=std::rand()%9;
        for (int i = 0; i < squares; ++i) {
            z[i].setX(int(forms[number][i]%2)+offset);
            z[i].setY(int(forms[number][i]/2)-1);
        }

        load_font();
        customFont_.setPointSize(30);
        label_palette_.setColor(QPalette::WindowText,Qt::white);

        text_score=new QLabel(this);
        text_score->setFont(customFont_);
        text_score->move(100,80);
        auto temp_score=QString("%1").arg(score_);
        text_score->setPalette(label_palette_);
        text_score->setText("SCORE:"+ temp_score);

        text_game_over=new QLabel(this);
        customFont_.setPointSize(40);
        text_game_over->setFont(customFont_);
        text_game_over->move(25,300);
        text_game_over->setPalette(label_palette_);
        text_game_over->setText("GAME OVER");
        text_game_over->setVisible(false);


        game_running_timer_= new QTimer (this);
        connect(game_running_timer_,&QTimer::timeout,this,&Tetris::game_running);
    }
} // tetris_game
