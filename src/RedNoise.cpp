#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <colour.h>
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

void draw(DrawingWindow &window) {
	window.clearPixels();
    std::vector <float> greyScales = interpolateSingleFloats(255,0,WIDTH);
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {

            /*
            float red = rand() % 256;
			float green = 0.0;
			float blue = 0.0;
            */

            float red = greyScales[x];
            float green = greyScales[x];
            float blue = greyScales[x];
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void drawColour(DrawingWindow &window) {
    window.clearPixels();

    glm::vec3 topLeft(255, 0, 0);        // red
    glm::vec3 topRight(0, 0, 255);       // blue
    glm::vec3 bottomRight(0, 255, 0);    // green
    glm::vec3 bottomLeft(255, 255, 0);   // yellow

    std::vector <glm::vec3> left = interpolateThreeElementValues(topLeft,bottomLeft,WIDTH);
    std::vector <glm::vec3> right = interpolateThreeElementValues(topRight,bottomRight,WIDTH);

    for (size_t y = 0; y < window.height; y++) {
        std::vector <glm::vec3> col = interpolateThreeElementValues(left[y], right[y], WIDTH);
        for (size_t x = 0; x < window.width; x++) {

            uint32_t colour = (255 << 24) + (int(col[x][0]) << 16) + (int(col[x][1]) << 8) + int(col[x][2]);
            window.setPixelColour(x, y, colour);
        }
    }
}


void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

    //test code for interpolateSingleElementValue
   /* std::vector<float> result;
    result = interpolateSingleFloats(2.2, 8.5, 7);
    for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
    std::cout << std::endl;
    */

    //test code for interpolateThreeElementValues
    std::vector<glm::vec3> result;
    glm::vec3 from(1.0, 4.0, 9.2);
    glm::vec3 to(4.0, 1.0, 9.8);
    result = interpolateThreeElementValues(from, to, 4);
    for(size_t i=0; i<result.size(); i++) {

        std::cout << "(" << result[i][0] << "," << result[i][1] << "," << result [i][2] << ")" << " ";
        std::cout << std::endl;
    }
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		drawColour(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}

}
