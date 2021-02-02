#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel **createImage(int width, int height)
{
  cout << "Start createImage... " << endl;

  // Create a one dimensional array on the heap of pointers to Pixels
  //    that has width elements (i.e. the number of columns)
  Pixel **image = new Pixel *[width];

  bool fail = false;

  for (int i = 0; i < width; ++i)
  { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];

    if (image[i] == nullptr)
    { // failed to allocate
      fail = true;
    }
  }

  if (fail)
  { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i = 0; i < width; ++i)
    {
      delete[] image[i]; // deleting nullptr is not a problem
    }
    delete[] image; // dlete array of pointers
    return nullptr;
  }

  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = {0, 0, 0};
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel **image, int width)
{
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i = 0; i < width; ++i)
  {
    delete[] image[i]; // delete each individual array placed on the heap
  }
  delete[] image;
  image = nullptr;
}

// implement for part 1

int *createSeam(int length)
{
  if (length == 0)
    return nullptr;
  int *seam = new int[length];
  return seam;
}

void deleteSeam(int *seam)
{
  delete[] seam;
}

bool loadImage(string filename, Pixel **image, int width, int height)
{
  ifstream ifs(filename);
  // check that file opens properly
  if (!ifs.is_open())
  {
    cout << "Error: failed to open input file - " << filename;
    return false;
  }
  // get type from preamble
  char type[3];
  ifs >> type; // should be P3
  if ((toupper(type[0]) != 'P') || (type[1] != '3'))
  { // check that type is correct
    cout << "Error: type is " << type << " instead of P3" << endl;
    return false;
  }

  // get width (w) and height (h) from preamble
  int w = 0, h = 0;
  ifs >> w;
  if (ifs.fail())
  {
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if (w != width)
  { // check that width matches what was passed into the function
    cout << "Error: input width (" << width << ") does not match value in file (" << w << ")" << endl;
    return false;
  }
  ifs >> h;
  if (ifs.fail())
  {
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if (h != height)
  { // check that height matches what was passed into the function
    cout << "Error: input height (" << height << ") does not match value in file (" << h << ")" << endl;
    return false;
  }

  // get maximum value from preamble
  int colorMax = 0;
  ifs >> colorMax;
  // if (colorMax != 255) {
  //   cout << "Error: file is not using RGB color values." << endl;
  //   return false;
  // }

  // get RGB pixel values
  for (int j = 0; j < h; ++j)
  {
    for (int i = 0; i < w; ++i)
    {
      Pixel p;
      // rgb of Pixel p will get a value from the ifs
      // A check for a non-integer will occur
      // and a check for the blank space at eof will occur inside
      // for too few color values
      ifs >> p.r;
      if (ifs.fail())
      {
        if (ifs.eof())
        {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        cout << "Error: read non-integer value" << endl;
        return false;
      }
      if (p.r < 0 || p.r > 255)
      {
        cout << "Error: invalid color value " << p.r << endl;
        return false;
      }
      ifs >> p.g;
      if (ifs.fail())
      {
        if (ifs.eof())
        {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        cout << "Error: read non-integer value" << endl;
        return false;
      }
      if (p.g < 0 || p.g > 255)
      {
        cout << "Error: invalid color value " << p.g << endl;
        return false;
      }
      ifs >> p.b;
      if (ifs.fail())
      {
        if (ifs.eof())
        {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        cout << "Error: read non-integer value" << endl;
        return false;
      }
      if (p.b < 0 || p.b > 255)
      {
        cout << "Error: invalid color value " << p.b << endl;
        return false;
      }
      image[i][j] = p;
    }
  }
  // check for too many color values with testing for a valid integer after
  // the loop above has assigned all the pixels specified
  int test = 0;
  ifs >> test;
  if (!ifs.fail())
  {
    cout << "Error: too many color values" << endl;
    return false;
  }

  return true;
}

bool outputImage(string filename, Pixel **image, int width, int height)
{
  ofstream ofs(filename);
  // check for valid file opening
  if (!ofs.is_open())
  {
    cout << "Error: failed to open output file - " << filename;
    return false;
  }
  // output preamble
  ofs << "P3" << endl;
  ofs << width << " ";
  ofs << height << endl;
  ofs << "255" << endl;
  // output pixels
  for (int j = 0; j < height; ++j)
  {
    for (int i = 0; i < width; ++i)
    {
      ofs << image[i][j].r << " " << image[i][j].g << " " << image[i][j].b << endl;
    }
  }
  return true;
}

int energy(Pixel **image, int x, int y, int width, int height)
{
  // variable initialization
  int energy = 0;
  int xgrad = 0;
  int ygrad = 0;
  int Rx = 0;
  int Gx = 0;
  int Bx = 0;
  int Ry = 0;
  int Gy = 0;
  int By = 0;

  if (x == 0)
  {
    if (y == 0)
    { // top left corner check
      Rx = abs(image[x + 1][y].r - image[width - 1][y].r);
      Gx = abs(image[x + 1][y].g - image[width - 1][y].g);
      Bx = abs(image[x + 1][y].b - image[width - 1][y].b);
      Ry = abs(image[x][y + 1].r - image[x][height - 1].r);
      Gy = abs(image[x][y + 1].g - image[x][height - 1].g);
      By = abs(image[x][y + 1].b - image[x][height - 1].b);
    }
    else if (y == height - 1)
    { // bottom left corner check
      Rx = abs(image[x + 1][y].r - image[width - 1][y].r);
      Gx = abs(image[x + 1][y].g - image[width - 1][y].g);
      Bx = abs(image[x + 1][y].b - image[width - 1][y].b);
      Ry = abs(image[x][0].r - image[x][y - 1].r);
      Gy = abs(image[x][0].g - image[x][y - 1].g);
      By = abs(image[x][0].b - image[x][y - 1].b);
    }
    else
    { // left side check
      Rx = abs(image[x + 1][y].r - image[width - 1][y].r);
      Gx = abs(image[x + 1][y].g - image[width - 1][y].g);
      Bx = abs(image[x + 1][y].b - image[width - 1][y].b);
      Ry = abs(image[x][y + 1].r - image[x][y - 1].r);
      Gy = abs(image[x][y + 1].g - image[x][y - 1].g);
      By = abs(image[x][y + 1].b - image[x][y - 1].b);
    }
  }
  else if (x == width - 1)
  {
    if (y == 0)
    { // top right corner check
      Rx = abs(image[x - 1][y].r - image[0][y].r);
      Gx = abs(image[x - 1][y].g - image[0][y].g);
      Bx = abs(image[x - 1][y].b - image[0][y].b);
      Ry = abs(image[x][y + 1].r - image[x][height - 1].r);
      Gy = abs(image[x][y + 1].g - image[x][height - 1].g);
      By = abs(image[x][y + 1].b - image[x][height - 1].b);
    }
    else if (y == height - 1)
    { // bottom right corner check
      Rx = abs(image[x - 1][y].r - image[0][y].r);
      Gx = abs(image[x - 1][y].g - image[0][y].g);
      Bx = abs(image[x - 1][y].b - image[0][y].b);
      Ry = abs(image[x][0].r - image[x][y - 1].r);
      Gy = abs(image[x][0].g - image[x][y - 1].g);
      By = abs(image[x][0].b - image[x][y - 1].b);
    }
    else
    { // right side check
      Rx = abs(image[x - 1][y].r - image[0][y].r);
      Gx = abs(image[x - 1][y].g - image[0][y].g);
      Bx = abs(image[x - 1][y].b - image[0][y].b);
      Ry = abs(image[x][y + 1].r - image[x][y - 1].r);
      Gy = abs(image[x][y + 1].g - image[x][y - 1].g);
      By = abs(image[x][y + 1].b - image[x][y - 1].b);
    }
  }
  else if (y == 0)
  {
    if (x == 0)
    { // top left corner check
      Rx = abs(image[x + 1][y].r - image[width - 1][y].r);
      Gx = abs(image[x + 1][y].g - image[width - 1][y].g);
      Bx = abs(image[x + 1][y].b - image[width - 1][y].b);
      Ry = abs(image[x][y + 1].r - image[x][height - 1].r);
      Gy = abs(image[x][y + 1].g - image[x][height - 1].g);
      By = abs(image[x][y + 1].b - image[x][height - 1].b);
    }
    else if (x == width - 1)
    { // top right corner check
      Rx = abs(image[0][y].r - image[x - 1][y].r);
      Gx = abs(image[0][y].g - image[x - 1][y].g);
      Bx = abs(image[0][y].b - image[x - 1][y].b);
      Ry = abs(image[x][y + 1].r - image[x][height - 1].r);
      Gy = abs(image[x][y + 1].g - image[x][height - 1].g);
      By = abs(image[x][y + 1].b - image[x][height - 1].b);
    }
    else
    { // top side check
      Rx = abs(image[x + 1][y].r - image[x - 1][y].r);
      Gx = abs(image[x + 1][y].g - image[x - 1][y].g);
      Bx = abs(image[x + 1][y].b - image[x - 1][y].b);
      Ry = abs(image[x][y + 1].r - image[x][height - 1].r);
      Gy = abs(image[x][y + 1].g - image[x][height - 1].g);
      By = abs(image[x][y + 1].b - image[x][height - 1].b);
    }
  }
  else if (y == height - 1)
  {
    if (x == 0)
    { // bottom left corner check
      Rx = abs(image[x + 1][y].r - image[width - 1][y].r);
      Gx = abs(image[x + 1][y].g - image[width - 1][y].g);
      Bx = abs(image[x + 1][y].b - image[width - 1][y].b);
      Ry = abs(image[x][y - 1].r - image[x][0].r);
      Gy = abs(image[x][y - 1].g - image[x][0].g);
      By = abs(image[x][y - 1].b - image[x][0].b);
    }
    else if (x == width - 1)
    { // bottom right corner check
      Rx = abs(image[0][y].r - image[x - 1][y].r);
      Gx = abs(image[0][y].g - image[x - 1][y].g);
      Bx = abs(image[0][y].b - image[x - 1][y].b);
      Ry = abs(image[x][y - 1].r - image[x][0].r);
      Gy = abs(image[x][y - 1].g - image[x][0].g);
      By = abs(image[x][y - 1].b - image[x][0].b);
    }
    else
    { // bottom side check
      Rx = abs(image[x + 1][y].r - image[x - 1][y].r);
      Gx = abs(image[x + 1][y].g - image[x - 1][y].g);
      Bx = abs(image[x + 1][y].b - image[x - 1][y].b);
      Ry = abs(image[x][y - 1].r - image[x][0].r);
      Gy = abs(image[x][y - 1].g - image[x][0].g);
      By = abs(image[x][y - 1].b - image[x][0].b);
    }
  }
  else
  { // middle pixels check
    Rx = abs(image[x + 1][y].r - image[x - 1][y].r);
    Gx = abs(image[x + 1][y].g - image[x - 1][y].g);
    Bx = abs(image[x + 1][y].b - image[x - 1][y].b);
    Ry = abs(image[x][y + 1].r - image[x][y - 1].r);
    Gy = abs(image[x][y + 1].g - image[x][y - 1].g);
    By = abs(image[x][y + 1].b - image[x][y - 1].b);
  }
  // calculations for x and y gradients for energy
  xgrad = pow(Rx, 2) + pow(Gx, 2) + pow(Bx, 2);
  ygrad = pow(Ry, 2) + pow(Gy, 2) + pow(By, 2);
  energy = xgrad + ygrad;
  return energy;
}

// implement for part 2

int loadVerticalSeam(Pixel **image, int start_col, int width, int height, int *seam)
{
  int total = energy(image, start_col, 0, width, height);
  seam[0] = start_col;
  int column = start_col;
  for (int r = 1; r < height; r++)
  {
    if (column == 0)
    {
      if (energy(image, column, r, width, height) <= energy(image, column + 1, r, width, height))
      {
        total += energy(image, column, r, width, height);
        seam[r] = column;
      }
      else
      {
        total += energy(image, column + 1, r, width, height);
        seam[r] = column + 1;
        column += 1;
      }
    }
    else if (column == width - 1)
    {
      if (energy(image, column, r, width, height) <= energy(image, column - 1, r, width, height))
      {
        total += energy(image, column, r, width, height);
        seam[r] = column;
      }
      else
      {
        total += energy(image, column - 1, r, width, height);
        seam[r] = column - 1;
        column -= 1;
      }
    }
    else
    {
      if (energy(image, column, r, width, height) <= energy(image, column + 1, r, width, height) && energy(image, column, r, width, height) <= energy(image, column - 1, r, width, height))
      {
        total += energy(image, column, r, width, height);
        seam[r] = column;
      }
      else if (energy(image, column + 1, r, width, height) <= energy(image, column - 1, r, width, height))
      {
        total += energy(image, column + 1, r, width, height);
        seam[r] = column + 1;
        column += 1;
      }
      else
      {
        total += energy(image, column - 1, r, width, height);
        seam[r] = column - 1;
        column -= 1;
      }
    }
  }
  return total;
}

int loadHorizontalSeam(Pixel **image, int start_row, int width, int height, int *seam)
{
  int total = energy(image, 0, start_row, width, height);
  seam[0] = start_row;
  int row = start_row;
  for (int c = 1; c < width; c++)
  {
    if (row == 0)
    {
      if (energy(image, c, row, width, height) <= energy(image, c, row + 1, width, height))
      {
        total += energy(image, c, row, width, height);
        seam[c] = row;
      }
      else
      {
        total += energy(image, c, row + 1, width, height);
        seam[c] = row + 1;
        row += 1;
      }
    }
    else if (row == height - 1)
    {
      if (energy(image, c, row, width, height) <= energy(image, c, row - 1, width, height))
      {
        total += energy(image, c, row, width, height);
        seam[c] = row;
      }
      else
      {
        total += energy(image, c, row - 1, width, height);
        seam[c] = row - 1;
        row -= 1;
      }
    }
    else
    {
      if (energy(image, c, row, width, height) <= energy(image, c, row - 1, width, height) && energy(image, c, row, width, height) <= energy(image, c, row + 1, width, height))
      {
        total += energy(image, c, row, width, height);
        seam[c] = row;
      }
      else if (energy(image, c, row - 1, width, height) <= energy(image, c, row + 1, width, height))
      {
        total += energy(image, c, row - 1, width, height);
        seam[c] = row - 1;
        row -= 1;
      }
      else
      {
        total += energy(image, c, row + 1, width, height);
        seam[c] = row + 1;
        row += 1;
      }
    }
  }
  return total;
}

int *findMinVerticalSeam(Pixel **image, int width, int height)
{
  int *s = createSeam(height);
  int min = loadVerticalSeam(image, 0, width, height, s);
  int *index = s;
  for (int c = 1; c < width; c++)
  {
    s = createSeam(height);
    if (min > loadVerticalSeam(image, c, width, height, s))
    {
      // Delete old min and assign new min
      deleteSeam(index);
      index = s;
      min = loadVerticalSeam(image, c, width, height, s);
    }
    else
    { // Avoid memory leak
      deleteSeam(s);
    }
  }
  return index;
}

int *findMinHorizontalSeam(Pixel **image, int width, int height)
{
  int *s = createSeam(width);
  int min = loadHorizontalSeam(image, 0, width, height, s);
  int *index = s;
  for (int r = 1; r < height; r++)
  {
    s = createSeam(width);
    if (min > loadHorizontalSeam(image, r, width, height, s))
    {
      // Delete old min and assign new min
      deleteSeam(index);
      index = s;
      min = loadHorizontalSeam(image, r, width, height, s);
    }
    else
    { // Avoid memory leak
      deleteSeam(s);
    }
  }
  return index;
}

void removeVerticalSeam(Pixel **image, int width, int height, int *verticalSeam)
{
  for (int r = 0; r < height; ++r)
  {
    for (int c = verticalSeam[r]; c < width - 1; ++c)
    {
      image[c][r].r = image[c + 1][r].r;
      image[c][r].g = image[c + 1][r].g;
      image[c][r].b = image[c + 1][r].b;
    }
  }
}

void removeHorizontalSeam(Pixel **image, int width, int height, int *horizontalSeam)
{
  for (int c = 0; c < width; ++c)
  {
    for (int r = horizontalSeam[c]; r < height - 1; ++r)
    {
      image[c][r].r = image[c][r + 1].r;
      image[c][r].g = image[c][r + 1].g;
      image[c][r].b = image[c][r + 1].b;
    }
  }
}
