
#include "led-matrix.h"

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <exception>

#define GRID_WIDTH  64
#define GRID_HEIGHT 64
#define COLORS       3

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;
int*** theGrid = new int**[GRID_HEIGHT];

void createGrid() {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        theGrid[i] = new int*[GRID_WIDTH];
        for (int j = 0; j < GRID_WIDTH; j++) {
            theGrid[i][j] = new int[COLORS];
        }
    }
}


// Make sure we can exit gracefully when Ctrl-C is pressed.
volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

int usage(const char *progname) {
  fprintf(stderr, "Usage: %s [led-matrix-options]\n",
          progname);
  rgb_matrix::PrintMatrixFlags(stderr);
  return 1;
}

// Function for displaying a complete matrix on the Canvas
void setupGrid(Canvas *canvas) {
  
  // fetch all pixel data from firebase and convert it to suitable digits as well as setting the pixels on the canvas
  
  for (int y = 0; y < canvas->height(); ++y) {
    for (int x = 0; x < canvas->width(); ++x) {
      canvas->SetPixel(x, y, theGrid[y][x][0], theGrid[y][x][1], theGrid[y][x][2]);
    }
  }

  return;
}

bool checkForUpdatesToGrid () {
  // call firebase and check if the update flag has been set to true
  // return true if there is new information, otherwise false
  return false;
}

void resetUpdateFlag () {
  // call firebase and set the update flag to false
  return;
}

void updateGrid () {
  // call firebase and fetch all pixels that have changed
  // reset the list of changed pixels
  // update all the changed pixels one by one for the local grid
  return;
}


// Callback function to receive the response data
size_t WriteCallback(char* data, size_t size, size_t nmemb, std::string* response) {
    response->append(data, size * nmemb);
    return size * nmemb;
}

// Function to perform HTTP GET request and return response as a string
std::string HttpGet(const std::string& url) {
    std::string response;
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return response;
}

void convertHexToRGB(std::string hexString) {
    int i = 1;
    int count = 0;
    while (i < hexString.length()-2) {
        int r, g, b;
        std::string hex = hexString.substr(i+2, 6);
        sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
        theGrid[count / GRID_HEIGHT][count % GRID_WIDTH][0] = r;
        theGrid[count / GRID_HEIGHT][count % GRID_WIDTH][1] = g;
        theGrid[count / GRID_HEIGHT][count % GRID_WIDTH][2] = b;
        i += 10;
        count++;
    }

    return;
}

int main(int argc, char *argv[]) {
  
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;
  
  
  matrix_options.cols=GRID_WIDTH;
  matrix_options.rows=GRID_HEIGHT;
  matrix_options.disable_hardware_pulsing=true;
  matrix_options.show_refresh_rate=true;
  
  
  if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv,
                                         &matrix_options, &runtime_opt)) {
    return usage(argv[0]);
  }
  
  
  if (argc != 1) {
    return usage(argv[0]);
  }

  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if (matrix == NULL) {
    return 1;
  }

    createGrid();
    std::string url = "https://pixled-17de5-default-rtdb.europe-west1.firebasedatabase.app/ledMatrix.json";
    std::string response = HttpGet(url);
    // std::cout << response << std::endl;
    convertHexToRGB(response);
    
  setupGrid(matrix);
  
  // Until Ctrl-C is pressed
  while (!interrupt_received) {
    sleep(10000); // 10 sec
    if (checkForUpdatesToGrid()) {
      resetUpdateFlag();
      updateGrid();
    }
  }


  matrix->Clear();
  delete matrix;
  
  printf("\n");
  
  return 0;
}
