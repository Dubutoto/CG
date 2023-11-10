#include <CanvasTriangle.h>
#include <CanvasPoint.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <fstream>
#include <Utils.h>
#include <glm/glm.hpp>
#include <vector>


#define WIDTH 320
#define HEIGHT 240



std::vector <float> interpolateSingleFloats(float from, float to, int numberOfValues){
    std::vector <float> result;
    float term = ((to-from)/ float (numberOfValues-1));
    float initialValue = from;
    for (int i = 0; i < numberOfValues; i++){
        result.push_back(initialValue);
        initialValue = initialValue + term;
    }
    return result;
}

std::vector <glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues){
    std::vector <glm::vec3> result;
    glm::vec3 term = (to - from);
    glm::vec3 unit = term / float(numberOfValues - 1);
    for (int i = 0; i < numberOfValues; i++){
        glm::vec3 a((float)i, (float)i, (float)i);
        result.push_back(from + unit * a);
    }
    return result;
}
/*
std::vector<CanvasPoint> interpolateCanvasPoint(CanvasPoint from, CanvasPoint to, int numberOfValues){
    std::vector<CanvasPoint> result;
    float xPoint =(to.x - from.x) / (float)(numberOfValues-1);
    float yPoint =(to.y - from.y) / (float)(numberOfValues-1);
    for (int i = 0; i < numberOfValues; i++) {
        result.push_back(CanvasPoint(from.x + xPoint*i, from.y + yPoint*i));
    }
    return result;
}
*/
void drawLine (CanvasPoint from, CanvasPoint to, DrawingWindow &window, Colour col) {

    float xDiff = round(to.x - from.x);
    float yDiff = round(to.y - from.y);
    float numberOfSteps = std::max(abs(xDiff) , abs(yDiff));
    float xStepSize = xDiff / numberOfSteps;
    float yStepSize = yDiff / numberOfSteps;

    uint32_t colour = (255 << 24) + (col.red << 16) + (col.green << 8) + col.blue;
    for (float i = 0.0; i <= numberOfSteps; i++) {
        float x = round(from.x + (xStepSize * i));
        float y = round(from.y + (yStepSize * i));
        window.setPixelColour(round(x), round(y), colour);
    }
}

Colour randomColour(){
    float r = rand()%256;
    float g = rand()%256;
    float b = rand()%256;
    return Colour(r, g, b);
}

CanvasTriangle randomCanvasPoint(){
    CanvasPoint v0 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);
    CanvasPoint v1 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);
    CanvasPoint v2 = CanvasPoint(rand()%WIDTH, rand()%HEIGHT);
    return CanvasTriangle(v0, v1, v2);
}

void drawTriangle(DrawingWindow &window, CanvasTriangle t, Colour col) {
    drawLine(t.v0(), t.v1(),window, col);
    drawLine(t.v1(), t.v2(),window, col);
    drawLine(t.v2(), t.v0(),window, col);
}

void unfilledTriangle(DrawingWindow &window) {
    drawTriangle(window,randomCanvasPoint(),randomColour());
}

    void draw(DrawingWindow &window) {
        window.clearPixels();

    //std::vector<float> greyScales = interpolateSingleFloats(255, 0, WIDTH);
    //for (size_t y = 0; y < window.height; y++) {
        //for (size_t x = 0; x < window.width; x++) {
          //  float red = rand() % 256;
          //  float green = 0.0;
          //  float blue = 0.0;
            // float red = greyScales[x];
            // float green = greyScales[x];
            // float blue = greyScales[x];

           // uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
          //  window.setPixelColour(x, y, colour);
       // }
   // }

//week3-ch2
//Colour col = Colour(255, 255, 255); // WHITE COLOR
//drawLine(CanvasPoint(0, 0), CanvasPoint(window.width * 1 / 2 , window.height * 1 / 2), window, col);
//drawLine(CanvasPoint(window.width - 1,1),CanvasPoint(window.width /2, window.height/2), window, col);
//drawLine(CanvasPoint(window.width/2, 0), CanvasPoint(window.width/2, window.height-1), window, col);
//drawLine(CanvasPoint(window.width/3, window.height/2), CanvasPoint(window.width * 2 / 3, window.height / 2), window, col);

    }

    void drawColour(DrawingWindow &window) {
        window.clearPixels();

        glm::vec3 topLeft(255, 0, 0);        // red
        glm::vec3 topRight(0, 0, 255);       // blue
        glm::vec3 bottomRight(0, 255, 0);    // green
        glm::vec3 bottomLeft(255, 255, 0);   // yellow

        std::vector<glm::vec3> left = interpolateThreeElementValues(topLeft, bottomLeft, WIDTH);
        std::vector<glm::vec3> right = interpolateThreeElementValues(topRight, bottomRight, WIDTH);

        for (size_t y = 0; y < window.height; y++) {
            std::vector<glm::vec3> col = interpolateThreeElementValues(left[y], right[y], WIDTH);
            for (size_t x = 0; x < window.width; x++) {

                uint32_t colour = (255 << 24) + (int(col[x][0]) << 16) + (int(col[x][1]) << 8) + int(col[x][2]);
                window.setPixelColour(x, y, colour);
            }
        }
    }




// add an event handling function for several keys

    void handleEvent(SDL_Event event, DrawingWindow &window) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
            else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
            else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
            else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
            else if (event.key.keysym.sym == SDLK_u) unfilledTriangle(window);
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            window.savePPM("output.ppm");
            window.saveBMP("output.bmp");
        }
    }

    int main(int argc, char *argv[]) {
        DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
        SDL_Event event;
        draw(window);
        //test code for interpolateSingleElementValue
        /*
        std::vector<float> result;
        result = interpolateSingleFloats(2.2, 8.5, 7);
        for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
        std::cout << std::endl;
        */

        //test code for interpolateThreeElementValues
        /*
        std::vector<glm::vec3> result;
        glm::vec3 from(1.0, 4.0, 9.2);
        glm::vec3 to(4.0, 1.0, 9.8);
        result = interpolateThreeElementValues(from, to, 4);
        for(size_t i=0; i<result.size(); i++) {
        std::cout << "(" << result[i][0] << "," << result[i][1] << "," << result [i][2] << ")" << " ";
        std::cout << std::endl;
        }
        */
        while (true) {
            // We MUST poll for events - otherwise the window will freeze !
            if (window.pollForInputEvents(event)) handleEvent(event, window);
            //draw(window);
            // Need to render the frame at the end, or nothing actually gets shown on the screen !
            window.renderFrame();
        }
    }
