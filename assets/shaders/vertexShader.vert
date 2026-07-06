#version 330 core
attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 pos;

uniform float millis;
uniform float xOffset;
uniform float yOffset;

void main() {
  pos = aTexCoord;

  vec4 position = vec4(aPosition, 1.0);
  position.y += sin(millis + position.x * 8.0) / 8.0;
  position.x += xOffset;
  position.y += yOffset;
  gl_Position = position;
}
