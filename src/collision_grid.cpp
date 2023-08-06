class Cell
{
public:
    int depth = 0;
    int *cell;
    bool in_usage;
    int n_object;
    Cell(int depth_) : depth{depth_}
    {
        cell = new int[depth];
        for (int i = 0; i < depth; i++)
            cell[i] = -1;
        in_usage = false;
        n_object = 0;
    }
    Cell()
    {
        depth = 12;
        cell = new int[depth];
        for (int i = 0; i < depth; i++)
            cell[i] = -1;
        in_usage = false;
        n_object = 0;
    }
    void add_object(int id)
    {
        //Add new index to grid
        for (int idx = 0; idx < depth; idx++)
        {
            if (cell[idx] == -1)
            {
                cell[idx] = id;
                break;
            }
        }
        n_object++;
        in_usage = true;
    }
    void remove_object(int id)
    {
        //Remove old index from grid
        for (int idx = 0; idx < depth; idx++)
        {
            if (cell[idx] == id)
            {
                cell[idx] = -1;
                break;
            }
        }
        n_object--;
        in_usage = n_object == 0;
    }
    int *get_cell()
    {
        return cell;
    }
};

class Grid
{
public:
    Grid(int grid_w_, int grid_h_, int grid_depth_) : grid_h{grid_h_}, grid_w{grid_w_}, grid_depth{grid_depth_}
    {
        grid = new Cell[grid_w * grid_h];
        for (int i = 0; i < grid_w * grid_h; i++)
            grid[i] = Cell(grid_depth);

        cell_block = new int[grid_depth * 9];
        for (int i = 0; i < grid_depth; i++)
            cell_block[i] = -1;
    }

    void move_object(int id, int old_grid_x, int old_grid_y, int grid_x, int grid_y)
    {
        int old_grid_idx = old_grid_x * grid_depth * grid_h + grid_depth * old_grid_y;
        int new_grid_idx = grid_x * grid_depth * grid_h + grid_depth * grid_y;

        //Remove old index from grid
        grid[old_grid_x * grid_h + old_grid_y].remove_object(id);

        grid_x = std::min(std::max(grid_x, 0), grid_w);
        grid_y = std::min(std::max(grid_y, 0), grid_h);

        //Add new index to grid
        grid[grid_x * grid_h + grid_y]
            .add_object(id);
    }

    void add_object(int id, int grid_x, int grid_y)
    {
        int grid_idx = grid_x * grid_depth * grid_h + grid_depth * grid_y;

        //Add new index to grid
        grid[grid_x * grid_h + grid_y].add_object(id);
    }

    int *get_cell(int grid_x, int grid_y)
    {
        //std::cout << grid_x << "|" << grid_y << "|---|" << std::endl;
        return grid[grid_x * grid_h + grid_y].cell;
    }

    int *get_cell_block(int grid_x, int grid_y)
    {
        int i_start = (grid_x) == 0 ? 0 : -1;
        int i_end = (grid_w - grid_x) == 1 ? 1 : 2;
        int j_start = (grid_y) == 0 ? 0 : -1;
        int j_end = (grid_h - grid_y) == 1 ? 1 : 2;
        int block_idx = 0;
        //std::cout << grid_x << "|" << grid_y << "|---|" << i_start << "|" << i_end << std::endl;
        for (int i = i_start; i < i_end; i++)
        {
            for (int j = j_start; j < j_end; j++)
            {
                for (int k = 0; k < grid_depth; k++)
                {
                    if (grid[(grid_x + i) * grid_h + grid_y + j].cell[k] != -1)
                        cell_block[block_idx++] = grid[(grid_x + i) * grid_h + grid_y + j].cell[k];
                }
            }
        }
        cell_block[block_idx] = -1;
        return cell_block;
    }

private:
    int grid_w;
    int grid_h;
    int grid_depth;
    int *cell_block;
    Cell *grid;
};