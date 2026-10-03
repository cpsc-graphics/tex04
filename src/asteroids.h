#pragma once
#include <SFML/Graphics.hpp>
#include <glad/gl.h>
#include <iostream>
#include <random>
#include <vector>

class Asteroids : public sf::Drawable {

public:
  // Struct to hold the properties of an asteroid instance
  struct AsteroidEntity {
    int type;
    sf::Vector2f pos;
    float scale;
    float orientation; // in radians - directon of travel.
    float rot;         // current rotation angle in degrees
    float rotSpeed;
    float speed;
    sf::Color color;
  };

  // Ranges for random generation of asteroid properties
  // at the time of spawning. These are defined as static constants in the class.
  static const int TYPE_RANGE[2]; // type of asteroid
  static const float POS_RANGE_X[2]; // x position range
  static const float POS_RANGE_Y[2]; // y position range
  static const float SCALE_RANGE[2]; // scale range
  static const float OMEGA_RANGE[2]; // rotational speed range (degrees per second)
  static const float SPEED_RANGE[2]; // linear speed range

  static const size_t NUM_VARIANTS = 4; // number of different asteroid types
  static const sf::Color COLOR_LUT[NUM_VARIANTS]; // color lookup table for asteroid types

private:

  // maximum number of asteroids that can be alive at any given time
  static const size_t MAX_ASTEROIDS = 16;

  // Struct for transferring data to GPU for drawing
  struct AsteroidVertex {
    float type;                   // to be sent to the GPU as a float
    float posX, posY, rot, scale; // to be sent to the GPU as a float4 vertex
    float r, g, b, a;             // to be sent to the GPU as a float4 color
  };

  // vector that holds all live asteroid instances
  std::vector<AsteroidEntity> liveAsteroids; 

  // shader for rendering the asteroids.
  sf::Shader shader;
  
  // Random generator for spawning asteroids. We will use a single generator and multiple distributions for different properties.
  std::mt19937 gen;

  // Define five separate distribution states
  std::uniform_int_distribution<int> dist_type;
  std::uniform_real_distribution<float> dist_pos_x;
  std::uniform_real_distribution<float> dist_pos_y;
  std::uniform_real_distribution<float> dist_scale;
  std::uniform_real_distribution<float> dist_omega;
  std::uniform_real_distribution<float> dist_speed;

  // Draw function that will be called by the SFML rendering system. It will use the shader to draw all live asteroids.
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:

  // Constructor for the Asteroids class. It initializes the random generator and distributions, and loads the shader.
  Asteroids();

  // Spawn a new asteroid oriented to move towards the target position (default is the origin)
  void spawnAsteroid( sf::Vector2f target = sf::Vector2f(0.f, 0.f) );

  // update the position of all asteroids based on their speed and orientation
  Asteroids& operator+=(float dt);

};