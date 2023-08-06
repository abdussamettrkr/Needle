#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <collision_grid.cpp>

class VerletObject
{
private:
    sf::Vector2f position_last;
    sf::Vector2f acceleration{0, 0};

public:
    sf::Vector2f position;
    int grid_x = 0;
    int grid_y = 0;
    sf::Color color;
    float radius;
    int id;
    VerletObject(sf::Vector2f position_, sf::Vector2f position_last, float radius_, int id_, sf::Color color_) : position{position_}, position_last{position_last}, radius{radius_}, id{id_}, color{color_}
    {
        grid_x = (int)(position.x / 50);
        grid_y = (int)(position.y / 50);
    }

    void accelerate(sf::Vector2f acceleration_)
    {
        acceleration += acceleration_;
    }
    void update(float dt, Grid *grid)
    {
        sf::Vector2f displacement = (position - position_last);
        position_last = position;
        position = position + displacement + acceleration * (dt * dt);
        int n_grid_x = (int)(position.x / 50);
        int n_grid_y = (int)(position.y / 50);

        if (n_grid_x != grid_x || n_grid_y != grid_y)
        {
            grid->move_object(id, grid_x, grid_y, n_grid_x, n_grid_y);
            grid_x = n_grid_x;
            grid_y = n_grid_y;
        }

        acceleration.x = 0;
        acceleration.y = 0;
    }
};

class Solver
{
public:
    std::vector<VerletObject *> objects;
    sf::Vector2f gravity{0, 500};
    int sub_steps = 3;
    int grid_size = 50;
    int grid_depth = 12;
    int windows_w;
    int windows_h;

    Solver(int windows_w_, int windows_h_) : windows_w{windows_w_}, windows_h{windows_h_}
    {

        grid = new Grid(windows_w / grid_size, windows_h / grid_size, grid_depth);
    }

    void update()
    {
        for (int i = 0; i < sub_steps; i++)
        {
            apply_gravity();
            update_objects();
            handle_collisions();
            apply_constrain();
            update_objects();
        }
    }
    void add_object(sf::Vector2f position, sf::Vector2f position_last, float radius, sf::Color color, int id)
    {
        VerletObject *n_object = new VerletObject(position, position_last, radius, id, color);
        objects.push_back(n_object);

        grid->add_object(id, position.x / grid_size, position.y / grid_size);
    }

private:
    Grid *grid;
    int **grid_counter;
    void apply_gravity()
    {
        for (auto &object : objects)
        {
            object->accelerate(gravity);
        }
    }

    void update_objects()
    {
        for (auto &object : objects)
        {
            object->update((1.0 / 60) / sub_steps, grid);
        }
    }

    void apply_constrain()
    {
        for (auto &object : objects)
        {
            if (object->position.x + object->radius * 2 >= 1450)
                object->position.x = 1450 - object->radius * 2;

            if (object->position.x <= 50)
                object->position.x = 50;

            if (object->position.y + object->radius * 2 >= 950)
                object->position.y = 950 - object->radius * 2;
        }
    }

    void handle_collisions()
    {
        int w_grid = windows_w / grid_size;
        int h_grid = windows_h / grid_size;
        for (int i = 0; i < w_grid; i++)
        {
            for (int j = 0; j < h_grid; j++)
            {
                int *c_cell = grid->get_cell(i, j);

                int *c_block = grid->get_cell_block(i, j);
                for (int k = 0; k < grid_depth; k++)
                {
                    if (c_cell[k] == -1)
                        continue;
                    for (int l = 0; l < grid_depth * 9; l++)
                    {
                        if (c_cell[k] == c_block[l])
                            continue;
                        if (c_block[l] == -1)
                            break;

                        auto object1 = objects[c_cell[k] - 1];
                        auto object2 = objects[c_block[l] - 1];

                        sf::Vector2f dir = object1->position - object2->position;
                        float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
                        if (dist < object1->radius + object2->radius)
                        {
                            float dist_back = (dist - (object1->radius + object2->radius)) / 2;
                            object1->position = object1->position - (dir / dist) * 0.2f * dist_back;
                            object2->position = object2->position + (dir / dist) * 0.2f * dist_back;
                        }
                    }
                }
            }
        }
    }
};
