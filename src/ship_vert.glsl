#version 330

in vec2 position;

uniform mat4 projection;
uniform mat4 translation;

void main() {
  gl_Position = projection * (translation * vec4(position.x, position.y, 0, 1));
}
