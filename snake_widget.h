#ifndef SNAKE_WIDGET_H
#define SNAKE_WIDGET_H

#include <QWidget>
#include <QRegion>
#include <cstdlib>
#include <ctime>

class Snake_widget : public QWidget
{
    Q_OBJECT

public:
    explicit Snake_widget(QWidget *parent = nullptr);
    ~Snake_widget() override;

    const int scale = 40;
    int domain = width()/scale + 1;
    int range = height()/scale + 1;

    struct Pixel {
        int x;
        int y;
        QColor color;
    };

    std::vector<Pixel> snake_list;
    Pixel apple;

    char direction = 'e';

    void start_game();
    char next_move();
    void next_frame();
    void print_pixel(Pixel pixel, bool clear);
    void move_apple();
    bool in_snake(int x, int y);
    bool in_list(char charactar, std::vector<char> list);
    char get_direction(int idx);


private:
    QVector<Pixel> m_pixels;

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *event) override;
};
#endif // SNAKE_WIDGET_H
