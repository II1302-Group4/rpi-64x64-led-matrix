#include "led-matrix.h"
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <exception>

#define GRID_WIDTH 64
#define GRID_HEIGHT 64
#define COLORS 3
#define BRIGHTNESS 0.5
#define MAX_VALUE 255
#define MIN_VALUE 0

using rgb_matrix::Canvas;
using rgb_matrix::FrameCanvas;
using rgb_matrix::RGBMatrix;
int ***theGrid = new int **[GRID_HEIGHT];
bool connection = false;

void createGrid()
{
  for (int i = 0; i < GRID_HEIGHT; i++)
  {
    theGrid[i] = new int *[GRID_WIDTH];
    for (int j = 0; j < GRID_WIDTH; j++)
    {
      theGrid[i][j] = new int[COLORS];
      for (int k = 0; k < COLORS; k++)
      {
        theGrid[i][j][k] = MIN_VALUE;
      }
    }
  }
}

// Make sure we can exit gracefully when Ctrl-C is pressed.
volatile bool interrupt_received = false;
static void InterruptHandler(int signo)
{
  interrupt_received = true;
}

int usage(const char *progname)
{
  fprintf(stderr, "Usage: %s [led-matrix-options]\n",
          progname);
  rgb_matrix::PrintMatrixFlags(stderr);
  return 1;
}

// Function for displaying a complete matrix on the Canvas
void updateCanvas(Canvas *canvas)
{

  printf("Updating canvas\n");

  // fetch all pixel data from firebase and convert it to suitable digits as well as setting the pixels on the canvas

  for (int y = 0; y < canvas->height(); ++y)
  {
    for (int x = 0; x < canvas->width(); ++x)
    {
      canvas->SetPixel(x, y, theGrid[y][x][0], theGrid[y][x][1], theGrid[y][x][2]);
    }
  }

  printf("Finished updating canvas\n");

  return;
}

bool checkForUpdatesToGrid()
{
  // call firebase and check if the update flag has been set to true
  // return true if there is new information, otherwise false
  return false;
}

// Callback function to receive the response data
size_t WriteCallback(char *data, size_t size, size_t nmemb, std::string *response)
{
  response->append(data, size * nmemb);
  return size * nmemb;
}

// Function to perform HTTP GET request and return response as a string
std::string HttpGet(const std::string &url)
{
  std::string response;
  CURL *curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    connection = true;
    if (res != CURLE_OK)
    {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
      connection = false;
    }
    curl_easy_cleanup(curl);
  }
  return response;
}

void updateGrid()
{
  printf("Starting update of grid\n");

  std::string url = "https://pixled-17de5-default-rtdb.europe-west1.firebasedatabase.app/ledMatrix.json";
  std::string hexString = HttpGet(url);
  // std::cout << response << std::endl;
  int i = 1;
  int count = 0;
  if (!connection)
  {
    printf("No connection established, aborting update of grid\n");
    return;
  }
  while (i < hexString.length() - 2)
  {
    int r, g, b;
    std::string hex = hexString.substr(i + 2, 6);
    sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
    theGrid[count / GRID_HEIGHT][count % GRID_WIDTH][0] = r * BRIGHTNESS;
    theGrid[count / GRID_HEIGHT][count % GRID_WIDTH][1] = g * BRIGHTNESS;
    theGrid[count / GRID_HEIGHT][count % GRID_WIDTH][2] = b * BRIGHTNESS;
    i += 10;
    count++;
  }

  printf("Finished update of grid\n");

  return;
}

void displayLogo()
{

  // P
  theGrid[10][10][0] = 255;
  theGrid[10][10][1] = 255;
  theGrid[10][10][2] = 255;
  theGrid[11][10][0] = 255;
  theGrid[11][10][1] = 255;
  theGrid[11][10][2] = 255;
  theGrid[12][10][0] = 255;
  theGrid[12][10][1] = 255;
  theGrid[12][10][2] = 255;
  theGrid[13][10][0] = 255;
  theGrid[13][10][1] = 255;
  theGrid[13][10][2] = 255;
  theGrid[14][10][0] = 255;
  theGrid[14][10][1] = 255;
  theGrid[14][10][2] = 255;

  theGrid[10][11][0] = 255;
  theGrid[10][11][1] = 255;
  theGrid[10][11][2] = 255;
  theGrid[11][11][0] = 0;
  theGrid[11][11][1] = 0;
  theGrid[11][11][2] = 0;
  theGrid[12][11][0] = 255;
  theGrid[12][11][1] = 255;
  theGrid[12][11][2] = 255;
  theGrid[13][11][0] = 0;
  theGrid[13][11][1] = 0;
  theGrid[13][11][2] = 0;
  theGrid[14][11][0] = 0;
  theGrid[14][11][1] = 0;
  theGrid[14][11][2] = 0;

  theGrid[10][12][0] = 255;
  theGrid[10][12][1] = 255;
  theGrid[10][12][2] = 255;
  theGrid[11][12][0] = 255;
  theGrid[11][12][1] = 255;
  theGrid[11][12][2] = 255;
  theGrid[12][12][0] = 255;
  theGrid[12][12][1] = 255;
  theGrid[12][12][2] = 255;
  theGrid[13][12][0] = 0;
  theGrid[13][12][1] = 0;
  theGrid[13][12][2] = 0;
  theGrid[14][12][0] = 0;
  theGrid[14][12][1] = 0;
  theGrid[14][12][2] = 0;

  // I
  theGrid[10][14][0] = 255;
  theGrid[10][14][1] = 255;
  theGrid[10][14][2] = 255;
  theGrid[11][14][0] = 255;
  theGrid[11][14][1] = 255;
  theGrid[11][14][2] = 255;
  theGrid[12][14][0] = 255;
  theGrid[12][14][1] = 255;
  theGrid[12][14][2] = 255;
  theGrid[13][14][0] = 255;
  theGrid[13][14][1] = 255;
  theGrid[13][14][2] = 255;
  theGrid[14][14][0] = 255;
  theGrid[14][14][1] = 255;
  theGrid[14][14][2] = 255;

  // X
  theGrid[10][16][0] = 255;
  theGrid[10][16][1] = 255;
  theGrid[10][16][2] = 255;
  theGrid[11][16][0] = 255;
  theGrid[11][16][1] = 255;
  theGrid[11][16][2] = 255;
  theGrid[12][16][0] = 0;
  theGrid[12][16][1] = 0;
  theGrid[12][16][2] = 0;
  theGrid[13][16][0] = 255;
  theGrid[13][16][1] = 255;
  theGrid[13][16][2] = 255;
  theGrid[14][16][0] = 255;
  theGrid[14][16][1] = 255;
  theGrid[14][16][2] = 255;

  theGrid[10][17][0] = 0;
  theGrid[10][17][1] = 0;
  theGrid[10][17][2] = 0;
  theGrid[11][17][0] = 0;
  theGrid[11][17][1] = 0;
  theGrid[11][17][2] = 0;
  theGrid[12][17][0] = 255;
  theGrid[12][17][1] = 255;
  theGrid[12][17][2] = 255;
  theGrid[13][17][0] = 0;
  theGrid[13][17][1] = 0;
  theGrid[13][17][2] = 0;
  theGrid[14][17][0] = 0;
  theGrid[14][17][1] = 0;
  theGrid[14][17][2] = 0;

  theGrid[10][18][0] = 255;
  theGrid[10][18][1] = 255;
  theGrid[10][18][2] = 255;
  theGrid[11][18][0] = 255;
  theGrid[11][18][1] = 255;
  theGrid[11][18][2] = 255;
  theGrid[12][18][0] = 0;
  theGrid[12][18][1] = 0;
  theGrid[12][18][2] = 0;
  theGrid[13][18][0] = 255;
  theGrid[13][18][1] = 255;
  theGrid[13][18][2] = 255;
  theGrid[14][18][0] = 255;
  theGrid[14][18][1] = 255;
  theGrid[14][18][2] = 255;

  // L
  theGrid[10][20][0] = 255;
  theGrid[10][20][1] = 0;
  theGrid[10][20][2] = 0;
  theGrid[11][20][0] = 255;
  theGrid[11][20][1] = 0;
  theGrid[11][20][2] = 0;
  theGrid[12][20][0] = 255;
  theGrid[12][20][1] = 0;
  theGrid[12][20][2] = 0;
  theGrid[13][20][0] = 255;
  theGrid[13][20][1] = 0;
  theGrid[13][20][2] = 0;
  theGrid[14][20][0] = 255;
  theGrid[14][20][1] = 0;
  theGrid[14][20][2] = 0;

  theGrid[10][21][0] = 0;
  theGrid[10][21][1] = 0;
  theGrid[10][21][2] = 0;
  theGrid[11][21][0] = 0;
  theGrid[11][21][1] = 0;
  theGrid[11][21][2] = 0;
  theGrid[12][21][0] = 0;
  theGrid[12][21][1] = 0;
  theGrid[12][21][2] = 0;
  theGrid[13][21][0] = 0;
  theGrid[13][21][1] = 0;
  theGrid[13][21][2] = 0;
  theGrid[14][21][0] = 255;
  theGrid[14][21][1] = 0;
  theGrid[14][21][2] = 0;

  theGrid[10][22][0] = 0;
  theGrid[10][22][1] = 0;
  theGrid[10][22][2] = 0;
  theGrid[11][22][0] = 0;
  theGrid[11][22][1] = 0;
  theGrid[11][22][2] = 0;
  theGrid[12][22][0] = 0;
  theGrid[12][22][1] = 0;
  theGrid[12][22][2] = 0;
  theGrid[13][22][0] = 0;
  theGrid[13][22][1] = 0;
  theGrid[13][22][2] = 0;
  theGrid[14][22][0] = 255;
  theGrid[14][22][1] = 0;
  theGrid[14][22][2] = 0;

  // E
  theGrid[10][24][0] = 0;
  theGrid[10][24][1] = 255;
  theGrid[10][24][2] = 0;
  theGrid[11][24][0] = 0;
  theGrid[11][24][1] = 255;
  theGrid[11][24][2] = 0;
  theGrid[12][24][0] = 0;
  theGrid[12][24][1] = 255;
  theGrid[12][24][2] = 0;
  theGrid[13][24][0] = 0;
  theGrid[13][24][1] = 255;
  theGrid[13][24][2] = 0;
  theGrid[14][24][0] = 0;
  theGrid[14][24][1] = 255;
  theGrid[14][24][2] = 0;

  theGrid[10][25][0] = 0;
  theGrid[10][25][1] = 255;
  theGrid[10][25][2] = 0;
  theGrid[11][25][0] = 0;
  theGrid[11][25][1] = 0;
  theGrid[11][25][2] = 0;
  theGrid[12][25][0] = 0;
  theGrid[12][25][1] = 255;
  theGrid[12][25][2] = 0;
  theGrid[13][25][0] = 0;
  theGrid[13][25][1] = 0;
  theGrid[13][25][2] = 0;
  theGrid[14][25][0] = 0;
  theGrid[14][25][1] = 255;
  theGrid[14][25][2] = 0;

  theGrid[10][26][0] = 0;
  theGrid[10][26][1] = 255;
  theGrid[10][26][2] = 0;
  theGrid[11][26][0] = 0;
  theGrid[11][26][1] = 0;
  theGrid[11][26][2] = 0;
  theGrid[12][26][0] = 0;
  theGrid[12][26][1] = 0;
  theGrid[12][26][2] = 0;
  theGrid[13][26][0] = 0;
  theGrid[13][26][1] = 0;
  theGrid[13][26][2] = 0;
  theGrid[14][26][0] = 0;
  theGrid[14][26][1] = 255;
  theGrid[14][26][2] = 0;

  // D
  theGrid[10][28][0] = 0;
  theGrid[10][28][1] = 0;
  theGrid[10][28][2] = 255;
  theGrid[11][28][0] = 0;
  theGrid[11][28][1] = 0;
  theGrid[11][28][2] = 255;
  theGrid[12][28][0] = 0;
  theGrid[12][28][1] = 0;
  theGrid[12][28][2] = 255;
  theGrid[13][28][0] = 0;
  theGrid[13][28][1] = 0;
  theGrid[13][28][2] = 255;
  theGrid[14][28][0] = 0;
  theGrid[14][28][1] = 0;
  theGrid[14][28][2] = 255;

  theGrid[10][29][0] = 0;
  theGrid[10][29][1] = 0;
  theGrid[10][29][2] = 255;
  theGrid[11][29][0] = 0;
  theGrid[11][29][1] = 0;
  theGrid[11][29][2] = 0;
  theGrid[12][29][0] = 0;
  theGrid[12][29][1] = 0;
  theGrid[12][29][2] = 0;
  theGrid[13][29][0] = 0;
  theGrid[13][29][1] = 0;
  theGrid[13][29][2] = 0;
  theGrid[14][29][0] = 0;
  theGrid[14][29][1] = 0;
  theGrid[14][29][2] = 255;

  theGrid[10][30][0] = 0;
  theGrid[10][30][1] = 0;
  theGrid[10][30][2] = 0;
  theGrid[11][30][0] = 0;
  theGrid[11][30][1] = 0;
  theGrid[11][30][2] = 255;
  theGrid[12][30][0] = 0;
  theGrid[12][30][1] = 0;
  theGrid[12][30][2] = 255;
  theGrid[13][30][0] = 0;
  theGrid[13][30][1] = 0;
  theGrid[13][30][2] = 255;
  theGrid[14][30][0] = 0;
  theGrid[14][30][1] = 0;
  theGrid[14][30][2] = 0;

  return;
}

int main(int argc, char *argv[])
{

  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;

  matrix_options.cols = GRID_WIDTH;
  matrix_options.rows = GRID_HEIGHT;
  matrix_options.disable_hardware_pulsing = true;
  matrix_options.show_refresh_rate = true;

  if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv,
                                         &matrix_options, &runtime_opt))
  {
    return usage(argv[0]);
  }

  if (argc != 1)
  {
    return usage(argv[0]);
  }

  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if (matrix == NULL)
  {
    return 1;
  }

  createGrid();
  displayLogo();
  updateCanvas(matrix);

  printf("\nStarting main loop\n\n");
  // Until Ctrl-C is pressed
  while (!interrupt_received)
  {

    try
    {

      updateGrid();
      updateCanvas(matrix);

      if (connection)
      {
        sleep(1); // sleep for 10 secs
      }
      else
      {
        sleep(2); // sleep for 2 secs
      }
      printf("I have slept\n");
    }
    catch (int e)
    {
      printf("An exception occured (nr %d), updating grid again soon!\n", e);
      sleep(2);
    }
  }

  matrix->Clear();
  delete matrix;

  printf("\n");

  return 0;
}