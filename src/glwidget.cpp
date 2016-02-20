#include "glwidget.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QTextStream>
#include <QTimer>

#define PI 3.14159265

using namespace std;
using glm::mat4;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    baseWidth = 1600;
    baseHeight = 900;
    width = baseWidth;
    height = baseHeight;
    srand(time(NULL));
    vec2 center(320, 240);
    state = engine.createInitialState();
    QTimer *aTimer = new QTimer(this);
    connect(aTimer,SIGNAL(timeout()), this,SLOT(update()));
    aTimer->start(10);
    first = chrono::system_clock::now();
    last = chrono::system_clock::now();
}

GLWidget::~GLWidget() {
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_D:
            state->turningLeft = true;
            state->turningRight = false;
            break;
        case Qt::Key_F:
            state->turningLeft = false;
            state->turningRight = true;
            break;
        case Qt::Key_J:
            state->firing = true;
            break;
        case Qt::Key_K:
            state->thrusting = true;
            break;
        case Qt::Key_L:
            state->teleporting = true;
            break;
    }
    update();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_D:
            state->turningLeft = false;
            state->turningRight = false;
            break;
        case Qt::Key_F:
            state->turningLeft = false;
            state->turningRight = false;
            break;
        case Qt::Key_J:
            state->firing = false;
            break;
        case Qt::Key_K:
            state->thrusting = false;
            break;
        case Qt::Key_L:
            state->teleporting = false;
            break;
    }
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(2.0f);

    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a buffer on the GPU for position data
    glGenBuffers(1, &positionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    program = loadShaders(":/vert.glsl", ":/frag.glsl");

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    projectionLoc = glGetUniformLocation(program, "projection");

    initializeShip();
}

void GLWidget::initializeShip()
{
    glGenVertexArrays(1, &shipVao);
    glBindVertexArray(shipVao);

    GLuint shipPositionBuffer;
    glGenBuffers(1, &shipPositionBuffer);

    vec2 pts[8];
    pts[0] = vec2(0, -15);
    pts[1] = vec2(8, 15);
    pts[2] = vec2(4, 10);
    pts[3] = vec2(-4, 10);
    pts[4] = vec2(-8, 15);
    pts[5] = vec2(4, 10);
    pts[6] = vec2(0, 20);
    pts[7] = vec2(-4, 10);

    glBindBuffer(GL_ARRAY_BUFFER, shipPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    shipProg = loadShaders(":/ship_vert.glsl", ":/frag.glsl");
    glUseProgram(shipProg);
    
    glBindBuffer(GL_ARRAY_BUFFER, shipPositionBuffer);
    GLint positionIndex = glGetAttribLocation(shipProg, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0); 

    shipProjMatrixLoc = glGetUniformLocation(shipProg, "projection");
    shipTransMatrixLoc = glGetUniformLocation(shipProg, "translation");
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);
    width = w;
    height = h;

    if(((float)w / (float)h) < ((float)baseWidth / (float)baseHeight))
    {
        float newH = baseHeight * (1.f * w / baseWidth);
        float diff = ((float)h - newH) * (baseHeight / newH);
        projection = glm::ortho(0.f, (float)baseWidth, diff / 2 + baseHeight, diff / 2 * -1);
    }
    else
    {
        float newW = baseWidth * (1.f * h / baseHeight);
        float diff = ((float)w - newW) * (baseWidth / newW);
        projection = glm::ortho(diff / 2 * -1, diff / 2 + baseWidth, (float)baseHeight, 0.f);
    }

    glUseProgram(program);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    glUseProgram(shipProg);
    glUniformMatrix4fv(shipProjMatrixLoc, 1, GL_FALSE, &projection[0][0]);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    updatePositions();
    glUseProgram(program);
    glBindVertexArray(vao);
    int start = 0;
    for(unsigned int k = 0; k < state->asteroids->size(); k++)
    {
        glDrawArrays(GL_LINE_LOOP, start, state->asteroids->at(k).points->size());
        start += state->asteroids->at(k).points->size();
    }
    glDrawArrays(GL_POINTS, start, state->bullets->size());

    renderShip();
}

void GLWidget::updatePositions()
{
    current = chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = current - last;
    double elapsed = elapsed_seconds.count();
    last = current;
    engine.getNextState(state, elapsed);
    points = 0;
    vector<vec2> nonShip;
    for(unsigned int k = 0; k < state->asteroids->size(); k++)
    {
        Asteroid *asteroid = &(state->asteroids->at(k));
        for(unsigned int a = 0; a < asteroid->points->size(); a++)
        {
            nonShip.push_back(*(new vec2(asteroid->points->at(a).x + asteroid->position->x, asteroid->points->at(a).y + asteroid->position->y)));
            points++;
        }
    }
    for(unsigned int k = 0; k < state->bullets->size(); k++)
    {
        nonShip.push_back(*(new vec2(state->bullets->at(k).position->x, state->bullets->at(k).position->y)));
    }
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * nonShip.size(), nonShip.data(), GL_DYNAMIC_DRAW);
}

void GLWidget::renderShip()
{
    mat4 translate = glm::translate(mat4(1.0), vec3(state->shipLoc->x, state->shipLoc->y, 0));
    mat4 rotate = glm::rotate(mat4(1.0), (float)(state->shipAngle * PI / 180), vec3(0, 0, 1));
    shipTranslationMatrix = translate * rotate;
    glUseProgram(shipProg);
    glUniformMatrix4fv(shipTransMatrixLoc, 1, GL_FALSE, &shipTranslationMatrix[0][0]);
    glBindVertexArray(shipVao);
    glDrawArrays(GL_LINE_LOOP, 0, 5);
    std::chrono::duration<double> elapsed_seconds = current - first;
    if(state->thrusting && (int)(elapsed_seconds.count() * 100) % 16 < 8)
    {
        glDrawArrays(GL_LINE_STRIP, 5, 3);
    }
}

// Copied from LoadShaders.cpp in the the oglpg-8th-edition.zip
// file provided by the OpenGL Programming Guide, 8th edition.
const GLchar* GLWidget::readShader(const char* filename) {
#ifdef WIN32
        FILE* infile;
        fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen( filename, "rb" );
#endif // WIN32

    if ( !infile ) {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

GLuint GLWidget::loadShaders(const char* vertf, const char* fragf) {
    GLuint program = glCreateProgram();

    // read vertex shader from Qt resource file
    QFile vertFile(vertf);
    vertFile.open(QFile::ReadOnly | QFile::Text);
    QString vertString;
    QTextStream vertStream(&vertFile);
    vertString.append(vertStream.readAll());
    std::string vertSTLString = vertString.toStdString();

    const GLchar* vertSource = vertSTLString.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    {
        GLint compiled;
        glGetShaderiv( vertShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( vertShader, len, &len, log );
            std::cout << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, vertShader);

    // read fragment shader from Qt resource file
    QFile fragFile(fragf);
    fragFile.open(QFile::ReadOnly | QFile::Text);
    QString fragString;
    QTextStream fragStream(&fragFile);
    fragString.append(fragStream.readAll());
    std::string fragSTLString = fragString.toStdString();

    const GLchar* fragSource = fragSTLString.c_str();

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    {
        GLint compiled;
        glGetShaderiv( fragShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( fragShader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    {
        GLint linked;
        glGetProgramiv( program, GL_LINK_STATUS, &linked );
        if ( !linked ) {
            GLsizei len;
            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetProgramInfoLog( program, len, &len, log );
            std::cout << "Shader linker failed: " << log << std::endl;
            delete [] log;
        }
    }

    return program;
}

