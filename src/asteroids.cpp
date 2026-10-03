#include "asteroids.h"
#include "helper.h"

// Define the static constants for the various ranges
const int Asteroids::TYPE_RANGE[2] = {0, Asteroids::NUM_VARIANTS - 1};
const float Asteroids::POS_RANGE_X[2] = {-1.0f, 1.0f};
const float Asteroids::POS_RANGE_Y[2] = {0.95f, 1.0f};
const float Asteroids::SCALE_RANGE[2] = {0.05f, 0.1f};
const float Asteroids::OMEGA_RANGE[2] = {10.0f, 50.0f};
const float Asteroids::SPEED_RANGE[2] = {0.1f, 0.3f};

// Colours for the different asteroid types
const sf::Color Asteroids::COLOR_LUT[Asteroids::NUM_VARIANTS] = {
    sf::Color(140, 150, 160), sf::Color(170, 110, 90), sf::Color(100, 160, 120),
    sf::Color(150, 100, 180)};

void Asteroids::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  
  // Build a completely flat, consecutive vertex buffer
  std::vector<AsteroidVertex> vertexBuffer;
  vertexBuffer.reserve(liveAsteroids.size() * 45);

  for (const auto &ast : liveAsteroids) {
    // Allocate exactly 45 vertices per asteroid instance
    for (int step = 0; step < 45; ++step) {
      AsteroidVertex v;
      v.r = ast.color.r / 255.f;
      v.g = ast.color.g / 255.f;
      v.b = ast.color.b / 255.f;
      v.a = ast.color.a / 255.f;

      v.posX = ast.pos.x;
      v.posY = ast.pos.y;
      v.rot = ast.rot;
      v.scale = ast.scale;

      v.type = static_cast<float>(ast.type);
      vertexBuffer.push_back(v);
    }
  }

  // Render the batch using simple, sequential draw arrays
  sf::Shader::bind(&shader);

  GLuint shaderProgID = shader.getNativeHandle();
  GLint locType = glGetAttribLocation(shaderProgID, "a_type");

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  if (locType != -1) {
    // std::cout << "Shader attribute 'a_type' found at location: " << locType
    // << std::endl;
    glEnableVertexAttribArray(locType);
  }

  GLsizei stride = sizeof(AsteroidVertex);
  const char *basePtr = reinterpret_cast<const char *>(vertexBuffer.data());
  glVertexPointer(4, GL_FLOAT, stride,
                  basePtr + offsetof(AsteroidVertex, posX));
  glColorPointer(4, GL_FLOAT, stride, basePtr + offsetof(AsteroidVertex, r));

  if (locType != -1) {
    glVertexAttribPointer(locType, 1, GL_FLOAT, GL_FALSE, stride,
                          basePtr + offsetof(AsteroidVertex, type));
  }

  // ONE SINGLE PURE STANDARD DRAW CALL FOR ALL INSTANCES
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexBuffer.size()));

  if (locType != -1)
    glDisableVertexAttribArray(locType);
  glDisableClientState(GL_COLOR_ARRAY);
  //glDisableClientState(GL_VERTEX_ARRAY);

  sf::Shader::bind(NULL);
}


// Constructor
Asteroids::Asteroids()
    : gen(std::random_device{}()), dist_type(TYPE_RANGE[0], TYPE_RANGE[1]),
      dist_pos_x(POS_RANGE_X[0], POS_RANGE_X[1]),
      dist_pos_y(POS_RANGE_Y[0], POS_RANGE_Y[1]),
      dist_scale(SCALE_RANGE[0], SCALE_RANGE[1]),
      dist_omega(OMEGA_RANGE[0], OMEGA_RANGE[1]),
      dist_speed(SPEED_RANGE[0], SPEED_RANGE[1]) {
  if (!shader.loadFromFile("shaders/asteroids.vert",
                           "shaders/asteroids.frag")) {
    std::cerr << "Failed to load shader for asteroids " << std::endl;
  }

  liveAsteroids.reserve(MAX_ASTEROIDS);

  // spwan a few asteroids at the start of the game
  for(int i = 0; i < MAX_ASTEROIDS; ++i) {
    spawnAsteroid();
  }
}

void Asteroids::spawnAsteroid(sf::Vector2f target) {
  if (liveAsteroids.size() >= MAX_ASTEROIDS) {
    //std::cerr << "Max asteroids reached, cannot spawn more!" << std::endl;
    return;
  }

  AsteroidEntity newAsteroid;
  newAsteroid.type = dist_type(gen);
  newAsteroid.pos = sf::Vector2f(dist_pos_x(gen), dist_pos_y(gen));
  // swap coordinates with 50% probability to ensure asteroids spawn in all quadrants
  if (std::uniform_int_distribution<int>(0, 1)(gen) == 0) {
    std::swap(newAsteroid.pos.x, newAsteroid.pos.y);
  }
  
  // flip the position based on a random draw
  if (std::uniform_int_distribution<int>(0, 1)(gen) == 0) {
    newAsteroid.pos.x *= -1.f;
    newAsteroid.pos.y *= -1.f;
  }

  newAsteroid.scale = dist_scale(gen);
  newAsteroid.rotSpeed = dist_omega(gen);
  // some asteroids will rotate clockwise, some counter-clockwise
  if (std::uniform_int_distribution<int>(0, 1)(gen) == 0) {
    newAsteroid.rotSpeed *= -1.f;
  }
  newAsteroid.rot = 0.f;
  newAsteroid.speed = dist_speed(gen);
  newAsteroid.color = COLOR_LUT[newAsteroid.type];

  // Calculate orientation based on the target position
  sf::Vector2f direction = target - newAsteroid.pos;
  direction /= std::sqrt(direction.x * direction.x + direction.y * direction.y); // Normalize
  newAsteroid.orientation = std::atan2(direction.y, direction.x);
  
  liveAsteroids.push_back(newAsteroid);
}

Asteroids& Asteroids::operator+=(float dt) {
  for (auto &ast : liveAsteroids) {
    ast.pos.x += ast.speed * std::cos(ast.orientation) * dt;
    ast.pos.y += ast.speed * std::sin(ast.orientation) * dt;
    ast.rot += sf::degrees(ast.rotSpeed).asRadians() * dt;
  }
  
  // check if any asteroids have moved out of bounds and wrap them around
  for (auto &ast : liveAsteroids) {  
    wrapAround(ast.pos);
  }

  return *this;
}
