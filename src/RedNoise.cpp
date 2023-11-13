#include <CanvasTriangle.h>
#include <CanvasPoint.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <fstream>
#include <TextureMap.h>
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

std::vector<CanvasPoint> interpolation(CanvasPoint from, CanvasPoint to, int numberOfValues){
    std::vector<CanvasPoint> result;
    float xPoint =(to.x - from.x) / (float)(numberOfValues-1);
    float yPoint =(to.y - from.y) / (float)(numberOfValues-1);
    for (int i = 0; i < numberOfValues; i++) {
        result.push_back(CanvasPoint(from.x + xPoint*i, from.y + yPoint*i));
    }
    return result;
}

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

void fillColour(bool flat, CanvasPoint left, CanvasPoint right, CanvasPoint c, DrawingWindow &window, Colour col) {
    int numberOfValue = abs(left.y - c.y);

    std::vector<float> v0, v1;

    if (flat) {
        v0 = interpolateSingleFloats(c.x, left.x, numberOfValue);
        v1 = interpolateSingleFloats(c.x, right.x, numberOfValue);
    } else {
        v0 = interpolateSingleFloats(left.x, c.x, numberOfValue);
        v1 = interpolateSingleFloats(right.x, c.x, numberOfValue);
    }
// not use static_cast, round, float (check for futher steps later)
    for (float y = flat ? c.y : left.y; y < (flat ? left.y : c.y); ++y) {
        drawLine(CanvasPoint(v0[y - (flat ? c.y : left.y)], y),
                 CanvasPoint(v1[y - (flat ? c.y : left.y)], y), window, col);
    }
}


void sortVertices(bool yOrX, CanvasTriangle &t) {
    int indices[3] = {0, 1, 2};

    auto compare = [&](int a, int b) {
        return yOrX ? t[a].y < t[b].y : t[a].x < t[b].x;
    };
    //좌표를 인덱스 배열을 기준으로 정렬
    std::sort(indices, indices + 3, compare);
    //정렬된 인덱스를 사용하여 좌표를 업데이트
    CanvasPoint temp[3];
    for (int i = 0; i < 3; ++i) {
        temp[i] = t[indices[i]];
    }
    //정렬된 좌표를 원래의 CanvasTriangle에 복사
    for (int i = 0; i < 3; ++i) {
        t[i] = temp[i];
    }
}

void leftToRight(CanvasPoint &left, CanvasPoint &right, CanvasTriangle &t) {
    sortVertices(true, t);

    float xDiff = t[2].x - t[0].x;
    float yDiff = t[2].y - t[0].y;
    float extraPointX = t[0].x + xDiff * (t[1].y - t[0].y) / yDiff;

    if (t[1].x < extraPointX) {
        left = t[1];
        right = CanvasPoint(extraPointX, t[1].y);
    } else {
        left = CanvasPoint(extraPointX, t[1].y);
        right = t[1];
    }
}


void filledTriangle(DrawingWindow &window){
    CanvasTriangle t = randomCanvasPoint();
    CanvasPoint left, right;
    leftToRight(left, right, t);
    Colour col = Colour(rand() % 256, rand() % 256, rand() % 256);
    fillColour(true, left, right, t[0], window, col);
    fillColour(false, left, right, t[2], window, col);
    drawTriangle(window,t,Colour(255, 255, 255));
}

std::vector<uint32_t> textureColour(TextureMap texturemap, CanvasPoint from, CanvasPoint to, int numberOfValues) {
    std::vector<CanvasPoint> texturePoints = interpolation(from, to, numberOfValues);
    std::vector<uint32_t> colours;

    for (int i = 0; i < numberOfValues; ++i) {
        colours.push_back(texturemap.pixels[round(texturePoints[i].x) + round(texturePoints[i].y) * texturemap.width]);
    }

    return colours;
}


void drawTexture(DrawingWindow &window, TextureMap textureMap, CanvasTriangle t, CanvasTriangle c){
    //CanvasTriangle always have 3 vertices.
    for (int i = 0; i < 3; ++i) {
        int startRow = round(c[i].y);
        int endRow = round(c[(i + 1) % 3].y);

        std::vector<CanvasPoint> canvasleft = interpolation(c[i], c[(i + 1) % 3], endRow - startRow + 1);
        std::vector<CanvasPoint> left = interpolation(t[i], t[(i + 1) % 3], endRow - startRow + 1);

        for (int j = startRow; j <= endRow; ++j) {
            int startCol = round(canvasleft[j - startRow].x);
            int endCol = round(c[(i + 2) % 3].x);
            std::vector<uint32_t> colours = textureColour(textureMap, left[j], left[0], endRow);
            for (int k = startCol; k <= endCol; ++k) {
                window.setPixelColour(k, j, colours[k - startCol]);
            }
        }
    }
}


void calculateTextureCoordinates(CanvasTriangle &t, CanvasTriangle &c, CanvasPoint &canvasLeft, CanvasPoint &canvasRight, CanvasPoint &left, CanvasPoint &right, TextureMap &textureMap){
    float lengthOfTri = (c[2].x - c[0].x) != 0 ? (canvasRight.x - c[0].x) / (c[2].x - c[0].x) : 0;
// texture left, right

   // leftToRight(left,right,t);
    if (c[1].x == canvasLeft.x) {
        left = t[1];                      // Xdiff                                           Ydiff
        right = CanvasPoint(t[0].x + (t[2].x - t[0].x) * lengthOfTri, t[0].y + (t[2].y - t[0].y) * lengthOfTri);
    } else {
        right = t[1];                    // Xdiff                                            Ydiff
        left = CanvasPoint(t[0].x + (t[2].x - t[0].x) * lengthOfTri, t[1].y + (t[2].y - t[1].y) * lengthOfTri);
    }

}
void mapTexture(CanvasTriangle t, CanvasTriangle c, DrawingWindow &window){
    std::string filename = "texture.ppm";
    TextureMap textureMap = TextureMap(filename);
    CanvasPoint canvasLeft,canvasRight,left,right;
    leftToRight(canvasLeft,canvasRight,c);
    leftToRight(left, right, t);
    calculateTextureCoordinates(t, c, canvasLeft, canvasRight, left, right, textureMap);


    drawTexture(window, textureMap, CanvasTriangle(t[0], left, right), CanvasTriangle(c[0], canvasLeft, canvasRight));
    drawTexture(window, textureMap, CanvasTriangle(t[2], left, right), CanvasTriangle(c[2], canvasLeft, canvasRight));

    CanvasTriangle calTriangle = CanvasTriangle(c[0],c[1],c[2]);
    drawTriangle(window,calTriangle, Colour(255,255,255));
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
        CanvasPoint t0 = CanvasPoint(195, 5);
        CanvasPoint t1 = CanvasPoint(395, 380);
        CanvasPoint t2 = CanvasPoint(65, 330);
        CanvasTriangle t = CanvasTriangle(t0, t1, t2);

        CanvasPoint c0 = CanvasPoint(160, 10);
        CanvasPoint c1 = CanvasPoint(300, 230);
        CanvasPoint c2 = CanvasPoint(10, 150);
        CanvasTriangle c = CanvasTriangle(c0, c1, c2);

        mapTexture(t, c, window);
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
            else if (event.key.keysym.sym == SDLK_u) unfilledTriangle(window),std::cout << "Create Unfilled Triangle" << std::endl;
            else if (event.key.keysym.sym == SDLK_f) filledTriangle(window),std::cout << "Create Filled Triangle" << std::endl;
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
