#include "snake_widget.h"
#include <QPainter>
#include <unistd.h>
#include <QTimer>
#include <iostream>
#include <stdlib.h>
#include <QResizeEvent>

Snake_widget::Snake_widget(QWidget *parent)
    : QWidget(parent)
{
}

Snake_widget::~Snake_widget()
{
    std::exit(0);
}

void Snake_widget::start_game()
{
    for (int i = 0; i < snake_list.size(); i++)
    {
        print_pixel(snake_list[i], true);
    }
    print_pixel(apple, true);

    snake_list = {{domain/4, range/2, Qt::green}, {domain/4 - 1, range/2, Qt::green}, {domain/4 - 2, range/2, Qt::green}};
    apple = {domain/4*3, range/2, Qt::red};

    char direction = 'e';

    print_pixel(snake_list[0], false);
    print_pixel(snake_list[1], false);
    print_pixel(apple, false);

    next_frame();
}

void Snake_widget::print_pixel(Pixel pixel, bool clear)
{
    if (clear)
    {
        for (int i = 0; i < m_pixels.size(); i++)
        {
            if (m_pixels[i].x == pixel.x && m_pixels[i].y == pixel.y)
            {
                m_pixels.erase(m_pixels.begin() + i);
                //m_pixels[i].color = QColor(0, 0, 0, 255);
            }
        }
    }
    else
    {
        m_pixels.append(pixel);
    }

    update();
}

inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

char Snake_widget::get_direction(int idx)
{
    Pixel snake_part1 = snake_list[idx];
    Pixel snake_part2 = snake_list[idx + 1];

    if (snake_part1.x == snake_part2.x)
    {
        if (snake_part1.y > snake_part2.y)
        {
            return 'n';
        }
        else
        {
            return 's';
        }
    }
    else
    {
        if (snake_part1.x > snake_part2.x)
        {
            return 'e';
        }
        else
        {
            return 'w';
        }
    }
}

void Snake_widget::paintEvent(QPaintEvent *)
{

    //NEXT TIME OPTIMIZE TO DRAW LINES FOR SNAKE INSTEAD OF PIXELS
    QPainter p(this);
    if (scale > 5)
    {
        p.setPen(m_pixels[0].color);
        p.setBrush(m_pixels[0].color);
        p.drawRect(QRect(m_pixels[0].x*scale + scale/5, m_pixels[0].y*scale + scale/5, scale - scale/5, scale - scale/5));

        p.setPen(m_pixels[1].color);
        p.setBrush(m_pixels[1].color);

        int snake_idx = 0;
        std::cout << "FRAME\n";
        while (snake_idx < m_pixels.size() - 2)
        {
            std::cout << "MAKING A LINE\n";
            char current_line_direction = get_direction(snake_idx);
            std::vector<int> line_points = {snake_idx, snake_idx};
            while (snake_idx < snake_list.size() - 1 && current_line_direction == get_direction(snake_idx))
            {
                snake_idx++;
                line_points[1] = snake_idx;
            }

            int line_points_difference = line_points[1] - line_points[0];

            //picks which point is farther to the top-left and should be the anchor for the print
            //still need to make it figure out wether to print down or right after it gets to each case.
            if (snake_list[line_points[0]].x == snake_list[line_points[1]].x || snake_list[line_points[0]].y == snake_list[line_points[1]].y)
            {
                if (snake_list[line_points[0]].x == snake_list[line_points[1]].x)
                {
                    if (snake_list[line_points[0]].y > snake_list[line_points[1]].y)
                    {
                        p.drawRect(QRect(m_pixels[line_points[1] + 1].x*scale + scale/5, m_pixels[line_points[1] + 1].y*scale + scale/5, scale - scale/5, scale * line_points_difference));
                    }
                    else
                    {
                        p.drawRect(QRect(m_pixels[line_points[0] + 1].x*scale + scale/5, m_pixels[line_points[0] + 1].y*scale + scale/5, scale - scale/5, scale * line_points_difference));
                    }
                }
                else
                {
                    if (snake_list[line_points[0]].x > snake_list[line_points[1]].x)
                    {
                        p.drawRect(QRect(m_pixels[line_points[1] + 1].x*scale + scale/5, m_pixels[line_points[1] + 1].y*scale + scale/5, scale * line_points_difference, scale - scale/5));
                    }
                    else
                    {
                        p.drawRect(QRect(m_pixels[line_points[0] + 1].x*scale + scale/5, m_pixels[line_points[0] + 1].y*scale + scale/5, scale * line_points_difference, scale - scale/5));
                    }
                }
            }
            else
            {
                if (snake_list[line_points[0]].x > snake_list[line_points[1]].x && snake_list[line_points[0]].y > snake_list[line_points[1]].y)
                {
                    p.drawRect(QRect(m_pixels[line_points[1] + 1].x*scale + scale/5, m_pixels[line_points[1] + 1].y*scale + scale/5, scale - scale/5, scale - scale/5));
                }
                else
                {
                    p.drawRect(QRect(m_pixels[line_points[0] + 1].x*scale + scale/5, m_pixels[line_points[0] + 1].y*scale + scale/5, scale - scale/5, scale - scale/5));
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < m_pixels.size(); i++)
        {
            p.setBrush(m_pixels[i].color);
            p.setPen(m_pixels[i].color);
            p.drawRect(QRect(m_pixels[i].x*scale, m_pixels[i].y*scale, scale, scale));
        }
    }
}

void Snake_widget::resizeEvent(QResizeEvent *event)
{
    domain = width()/scale;
    range = height()/scale;
}

bool Snake_widget::in_snake(int x, int y)
{
    for (int i = 0; i < snake_list.size(); i++)
    {
        if (snake_list[i].x == x && snake_list[i].y == y)
        {
            return true;
        }
    }

    return false;
}

bool Snake_widget::in_list(char charactar, std::vector<char> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i] == charactar)
        {
            return true;
        }
    }
    return false;
}

void Snake_widget::move_apple()
{
    std::vector<Pixel> available_pixels = {};
    for (int x = 0; x < domain; x++)
    {
        for (int y = 0; y < range; y++)
        {
            if (!in_snake(x, y))
            {
                available_pixels.push_back(Pixel {x, y, Qt::red});
            }
        }
    }

    int rand_idx = rand() % available_pixels.size();
    apple = available_pixels[rand_idx];
}

char Snake_widget::next_move()
{
    std::vector<char> available_directions = {};

    if (!in_snake(snake_list[0].x, snake_list[0].y - 1) && snake_list[0].y - 1 >= 0)
    {
        //std::cout << "CAN GO NORTH\n";
        available_directions.push_back('n');
    }

    if (!in_snake(snake_list[0].x, snake_list[0].y + 1) && snake_list[0].y + 1 <= range)
    {
        //std::cout << "CAN GO SOUTH\n";
        available_directions.push_back('s');
    }

    if (!in_snake(snake_list[0].x - 1, snake_list[0].y) && snake_list[0].x - 1 >= 0)
    {
        //std::cout << "CAN GO WEST\n";
        available_directions.push_back('w');
    }

    if (!in_snake(snake_list[0].x + 1, snake_list[0].y) && snake_list[0].x + 1 <= domain)
    {
        //std::cout << "CAN GO EAST\n";
        available_directions.push_back('e');
    }

    int x_difference = snake_list[0].x - apple.x;
    int y_difference = snake_list[0].y - apple.y;

    //std::cout << "DIIFS: " << x_difference << ", " << y_difference << "\n";

    if (std::abs(x_difference) > std::abs(y_difference))
    {
        if (x_difference < 0 && in_list('e', available_directions))
        {
            return 'e';
        }

        if (x_difference > 0 && in_list('w', available_directions))
        {
            return 'w';
        }
    }
    else
    {
        if (y_difference < 0 && in_list('s', available_directions))
        {
            return 's';
        }

        if (y_difference > 0 && in_list('n', available_directions))
        {
            return 'n';
        }
    }

    if (available_directions.size() == 0)
    {
        return direction;
    }
    else
    {
        return available_directions[rand() % available_directions.size()];
    }
}

void Snake_widget::next_frame()
{
    direction = next_move();
    //std::cout << "\n" << direction << "\n";

    Pixel new_head;

    switch(direction)
    {
        case 'n':
            new_head = {snake_list[0].x, snake_list[0].y - 1, Qt::green};
            break;
        case 's':
            new_head = {snake_list[0].x, snake_list[0].y + 1, Qt::green};
            break;
        case 'e':
            new_head = {snake_list[0].x + 1, snake_list[0].y, Qt::green};
            break;
        case 'w':
            new_head = {snake_list[0].x - 1, snake_list[0].y, Qt::green};
            break;
    }



    if (in_snake(new_head.x, new_head.y) || new_head.x < 0 || new_head.x > domain || new_head.y < 0 || new_head.y > range)
    {
        start_game();
    }
    else
    {
        //snake_list.insert(snake_list.begin(), new_head);

        if (new_head.x == apple.x && new_head.y == apple.y)
        {
            snake_list.insert(snake_list.begin(), new_head);
            move_apple();
            m_pixels[0] = apple;
        }
        else
        {
            print_pixel(snake_list[snake_list.size() - 1], true);

            for (int i = snake_list.size() - 1; i > 0; i--)
            {
                snake_list[i].x = snake_list[i - 1].x;
                snake_list[i].y = snake_list[i - 1].y;
                snake_list[i].color = Qt::green;
            }

            snake_list[0].x = new_head.x;
            snake_list[0].y = new_head.y;
        }

        print_pixel(snake_list[0], false);
    }
    delay(500);
    next_frame();
}
