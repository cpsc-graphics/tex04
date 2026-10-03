#include <SFML/Graphics.hpp>
#include <vector>

// Class to represent the ship in the game. The ship is made up of triangles 
// and is able to move forward and turn. 

class Ship : public sf::Drawable, public sf::Transformable {
private:

    // Vertex array to hold base geometry centered at the origin 
    // (i.e. local object space coordinates)
    sf::VertexArray vertices;

    // Constant scale factor for the ship
    static constexpr float SIZE_SCALE = 0.05f;

    // variables to control the kinematics: acceleartion, max linear speed, angular speed
    
    static constexpr float MAX_SPEED = 0.3f; // ship moves at constant speed once it reaches this value
    static constexpr float OMEGA = 360.f; // constant angular speed in degrees per second
    static constexpr float ACC = 0.3f; // constant acceleration in units per second squared
    
    float a = 0.0f; // current acceleration
    float u = 0.0f; // current speed
    float w = 0.0f; // current angular speed
    
    // variables that hold the current position and orientation of the ship
    sf::Vector2f pos = {0.0f, 0.0f};
    float angle = 0.0f;

    // The ship has a weapon. The following members keep track of the weapon's state and behavior.
    // weapon control
    
    static constexpr float PROJECTILE_RADIUS = 0.025f; // radius of the fired projectile
    static constexpr float PROJECTILE_SCALE = 0.25f; // scale factor to be applied in the y-direction to make the projectile look elliptical
    
    static constexpr float PROJECTILE_SPEED = 1.f; // speed of the fired projectile
    static constexpr float FIRE_RATE = 5.f; // how many per second when the weapon is firing
    
    bool weapon_on = false; // whether the weapon is currently firing
    float ttnf = 0.f; // time to next fire

    // Struct to represent a projectile fired by the ship. Each projectile has a position and an angle.
    struct Projectile {
        sf::Vector2f pos;
        float angle;
    };

    // All active projectiles are stored in a vector. 
    std::vector< Ship::Projectile > projectiles;


    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    
    // Default constructor
    Ship();

    // Set the acceleration of the ship to the constant value ACC. 
    // This will cause the ship to accelerate in the direction it is currently facing.
    void accelerate() {
        this->a = ACC;
    }

    // Set the acceleration of the ship to the negative constant value -ACC.
    // This will cause the ship to decelerate in the direction it is currently facing.
    void deccelerate() {
        this->a = -1.0f * ACC;
    }

    // Toggle the turning (rotation) of the ship. If the ship is currently turning, it will stop turning.
    // If the ship is not currently turning, it will start turning in the direction specified by the ccw parameter. 
    // If ccw is true, the ship will turn counter-clockwise, otherwise it will turn clockwise.
    void toggleTurn( bool ccw = true ) {
        if( w != 0.0f ) {
            w = 0.0f;
        } else {
            w = ccw ? OMEGA : -1.0f * OMEGA;
        }
    }

    // Toggle the weapon state. If the weapon is currently firing, it will stop firing.
    // If the weapon is not currently firing, it will start firing.
    void toggleWeapon() {
        weapon_on = !weapon_on;
        ttnf = 0.0f;
    }

    // update the state of the ship based on the given incremental time-step.
    Ship& operator+=( float dt );
};


