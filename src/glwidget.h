#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <glm/glm.hpp>
#include "shape.h"
#include <QtGui/QImage>

// glm by default uses degrees, but that functionality
// is deprecated so GLM_FORCE_RADIANS turns off some 
// glm warnings
#define GLM_FORCE_RADIANS

using glm::vec2;
using glm::vec3;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    public:
        GLWidget(QWidget *parent=0);
        ~GLWidget();

    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

        void keyPressEvent(QKeyEvent *event);

    private:
        GLuint loadShaders(const char* vertf, const char* fragf);
        static const GLchar* readShader(const char* filename);

        GLuint vao;
        GLuint program;

        GLuint positionBuffer;
        GLuint colorBuffer;

        GLuint projectionBuffer;
        GLint projectionLoc;

        //the projection matrix
        glm::mat4 projection;

        //the width and height
        int baseWidth;
        int baseHeight;

        //the width and height of the window
        int width;
        int height;
};

#endif
