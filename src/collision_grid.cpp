

class Grid
{
public:
    Grid(int grid_w_, int grid_h_, int grid_depth_) : grid_h{grid_h_}, grid_w{grid_w_}, grid_depth{grid_depth_}
    {
        grid = new int[grid_w * grid_h * grid_depth];
        for (int i = 0; i < grid_w * grid_h * grid_depth; i++)
            grid[i] = -1;

        temp_cell = new int[grid_depth * 9];
        for (int i = 0; i < grid_depth; i++)
            temp_cell[i] = -1;
    }

    void move_object(int id, int old_grid_x, int old_grid_y, int grid_x, int grid_y)
    {
        int old_grid_idx = old_grid_x * grid_depth * grid_h + grid_depth * old_grid_y;
        int new_grid_idx = grid_x * grid_depth * grid_h + grid_depth * grid_y;

        //Remove old index from grid
        for (int idx = 0; idx < grid_depth; idx++)
        {
            if (grid[old_grid_idx + idx] == id)
            {
                grid[old_grid_idx + idx] = -1;
                break;
            }
        }

        //Add new index to grid
        for (int idx = 0; idx < grid_depth; idx++)
        {
            if (grid[new_grid_idx + idx] == -1)
            {
                grid[new_grid_idx + idx] = id;
                break;
            }
        }
    }

    void add_object(int id, int grid_x, int grid_y)
    {
        int grid_idx = grid_x * grid_depth * grid_h + grid_depth * grid_y;

        //Add new index to grid
        for (int idx = 0; idx < grid_depth; idx++)
        {
            if (grid[grid_idx + idx] == -1)
            {
                grid[grid_idx + idx] = id;
                break;
            }
        }
    }

    int *get_cell(int grid_x, int grid_y)
    {
        int n_grid_x = -1;
        int n_grid_y = -1;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                n_grid_x = std::min(std::max(0, grid_x + i - 1), grid_w - 1);
                n_grid_y = std::min(std::max(0, grid_y + j - 1), grid_h - 1);

                int grid_idx = n_grid_x * grid_depth * grid_h + grid_depth * n_grid_y;

                for (int k = 0; k < grid_depth; k++)
                {
                    temp_cell[(i * 3) * grid_depth + j * grid_depth + k] = grid[grid_idx + k];
                }
            }
        }
        return temp_cell;
    }

private:
    int grid_w;
    int grid_h;
    int grid_depth;
    int *grid;
    int *temp_cell;
};