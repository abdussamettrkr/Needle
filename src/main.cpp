#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <solver.cpp>

using namespace std::chrono;

int main()
{
    int windows_w = 1500;
    int windows_h = 1000;
    sf::RenderWindow window(sf::VideoMode(windows_w, windows_h), "Simple circle physics!!!");
    Solver solver = Solver(windows_w, windows_h);

    auto beg = high_resolution_clock::now();
    auto solver_time = high_resolution_clock::now();
    auto last_spawn = high_resolution_clock::now();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - beg);
    auto render_time = duration_cast<microseconds>(end - beg);
    auto physics_time = duration_cast<microseconds>(end - beg);

    int circle_idx = 1;
    float p_time_ema = 0;

    sf::Font font;
    if (!font.loadFromFile("/home/tr/Desktop/turker/Needle/fonts/arial.ttf"))
    {
        exit(1);
    }

    sf::Text text, text2, text3;
    text.setFont(font);
    text2.setFont(font);
    text3.setFont(font);
    text.setCharacterSize(20);
    text2.setCharacterSize(20);
    text3.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text2.setFillColor(sf::Color::White);
    text3.setFillColor(sf::Color::White);
    text.setPosition(20.f, 20.f);
    text2.setPosition(20.f, 40.f);
    text3.setPosition(20.f, 60.f);
    text.setString("Hello, SFML!");

    sf::CircleShape shape_temp{1};
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        beg = high_resolution_clock::now();
        solver.update();
        solver_time = high_resolution_clock::now();

        window.clear();
        for (const auto &object : solver.objects)
        {
            shape_temp.setPosition(object->position);
            shape_temp.setScale(object->radius, object->radius);
            shape_temp.setFillColor(object->color);
            window.draw(shape_temp);
        }

        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(50, 50)),
                sf::Vertex(sf::Vector2f(50, 950))};
        sf::Vertex line2[] =
            {
                sf::Vertex(sf::Vector2f(50, 950)),
                sf::Vertex(sf::Vector2f(1450, 950))};

        sf::Vertex line3[] =
            {
                sf::Vertex(sf::Vector2f(1450, 950)),
                sf::Vertex(sf::Vector2f(1450, 50))};

        window.draw(line, 4, sf::Lines);
        window.draw(line2, 5, sf::Lines);
        window.draw(line3, 9, sf::Lines);
        p_time_ema = p_time_ema * 0.9 + 0.1 * (physics_time.count() / 1000);
        text.setString("Physics time: " + std::to_string(static_cast<int>(p_time_ema)) + " ms");
        text2.setString("Render time:" + std::to_string(static_cast<int>(render_time.count()) / 1000) + " ms");
        text3.setString("N objects: " + std::to_string(circle_idx));
        window.draw(text);
        window.draw(text2);
        window.draw(text3);

        window.display();
        end = high_resolution_clock::now();
        physics_time = duration_cast<microseconds>(solver_time - beg);
        render_time = duration_cast<microseconds>(solver_time - solver_time);
        duration = duration_cast<microseconds>(end - beg);

        if (duration.count() < 1000000.0 / 30)
        {
            usleep(1000000.0 / 30 - duration.count());
        }
        else
        {
            std::cout << circle_idx << std::endl;
            //break;
        }

        duration = duration_cast<microseconds>(end - last_spawn);
        if (duration.count() / 1000 > 50)
        {
            solver.add_object(sf::Vector2f(800, 400), sf::Vector2f(798, 400), 10, sf::Color(rand() % 240 + 15, rand() % 240 + 15, rand() % 240 + 15), circle_idx++);
            last_spawn = high_resolution_clock::now();
        }
    }

    return 0;
}
